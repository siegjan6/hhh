#include "FrameDialog.h"
#include "ui_FrameDialog.h"
#include <QColorDialog>
#include <QPainter>
#include "ShareTool.h"

CFrameDialog::CFrameDialog(const frameData &data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CFrameDialog)
{
    ui->setupUi(this);

    m_frameData = data;
    //! 子部件初始化
    //! 边框颜色
    ui->toolbuttonColor->setPalette(QPalette(m_frameData.pen.color()));
    ui->toolbuttonColor->setAutoFillBackground(true);
    //! 边框类型
    initFrameTypeWidgets();
    //! 边框的画笔样式将在showEvent中初始化
    //! 绘制边框类型
    ui->frameShow->m_typeShowed = data.type;
    ui->frameShow->update();
    //! 画笔宽度
    ui->lineeditWidth->setText(QString::number(data.pen.width()));
    ui->lineeditWidth->setValidator(new QIntValidator(1,100,this));

    CShareTool::installDialogTranslator(this);
}

CFrameDialog::~CFrameDialog()
{
    delete ui;
}

void CFrameDialog::showEvent(QShowEvent *event)
{
    /*!
    由于使用了布局,导致子控件在showEvent和构造函数之间的时间里大小有变化
    所有个别控件需要做一些单独设置
    */
    //! 边框的画笔样式
    CShareTool::initPenStyleComboBox(ui->comboboxStyle, m_frameData.pen.style());

    QDialog::showEvent(event);
}

void CFrameDialog::on_toolbuttonColor_clicked()
{
    QColor color = QColorDialog::getColor(m_frameData.pen.color(), this);
    if(!color.isValid())
        return;

    m_frameData.pen.setColor(color);
    ui->toolbuttonColor->setPalette(QPalette(color));
}

void CFrameDialog::buttonCheckedHandler(int type, bool checked)
{
    qint32& frametype = ui->frameShow->m_typeShowed;

    checked ?   frametype |= type : frametype &= ~type;

    ui->frameShow->update();
}

void CFrameDialog::buttonFrameAllClicked()
{
    foreach(QAbstractButton *button, m_typeButtons->buttons())
        button->setChecked(true);
}

void CFrameDialog::accept()
{
    //! 边框类型
    qint32 type = (qint32)m_frameData.type;
    type = ui->pushbuttonTop->isChecked() ? type | frameType::topframe : type & ~frameType::topframe;
    type = ui->pushbuttonLeft->isChecked() ? type | frameType::leftframe : type & ~frameType::leftframe;
    type = ui->pushbuttonBottom->isChecked() ? type | frameType::bottomframe : type & ~frameType::bottomframe;
    type = ui->pushbuttonRight->isChecked() ? type | frameType::rightframe : type & ~frameType::rightframe;
    m_frameData.type = (frameType)type;
    //! 画笔风格
    m_frameData.pen.setStyle((Qt::PenStyle)(ui->comboboxStyle->currentIndex() + Qt::SolidLine));
    //! 画笔颜色直接在on_toolbuttonColor_clicked()中赋值
    //! 画笔宽度
    bool ok=false;
    qint32 width = m_frameData.pen.width();
    width = ui->lineeditWidth->text().toInt(&ok);
    if(ok)
        m_frameData.pen.setWidth(width);

    QDialog::accept();
}

void CFrameDialog::initFrameTypeWidgets()
{
    qint32 type = m_frameData.type;

    if( type & frameType::topframe )
        ui->pushbuttonTop->setChecked(true);
    if( type & frameType::leftframe )
        ui->pushbuttonLeft->setChecked(true);
    if( type & frameType::bottomframe )
        ui->pushbuttonBottom->setChecked(true);
    if( type & frameType::rightframe )
        ui->pushbuttonRight->setChecked(true);

    m_typeButtons = new QButtonGroup(this);
    m_typeButtons->setExclusive(false);
    m_typeButtons->addButton(ui->pushbuttonTop, frameType::topframe);
    m_typeButtons->addButton(ui->pushbuttonLeft, frameType::leftframe);
    m_typeButtons->addButton(ui->pushbuttonBottom, frameType::bottomframe);
    m_typeButtons->addButton(ui->pushbuttonRight, frameType::rightframe);

    QObject::connect(m_typeButtons, static_cast<void (QButtonGroup:: *)(int,bool)>(&QButtonGroup::buttonToggled),
                     this, &CFrameDialog::buttonCheckedHandler);
    QObject::connect(ui->pushbuttonAll, &QPushButton::clicked,
                     this, &CFrameDialog::buttonFrameAllClicked);
}

CFrameTypeWidget::CFrameTypeWidget(QWidget *parent) :
    QFrame(parent)
{
    m_typeShowed = frameType::noframe;
    m_cornerWidth = 16;
}

void CFrameTypeWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.save();

    QRect rect = this->rect();
    QRect innerRect = rect;
    innerRect.adjust(m_cornerWidth,m_cornerWidth,-m_cornerWidth,-m_cornerWidth);
    //! 绘制中间的"文本"
    QFont font = painter.font();
    font.setBold(true);
    font.setPointSize(12);
    painter.setFont(font);
    painter.drawText(rect, Qt::AlignCenter, tr("文本"));
    //! 绘制四个角落的拐角
    QPoint point1 = innerRect.topLeft();
    painter.drawLine(point1, point1 + QPoint(-m_cornerWidth/2,0));
    painter.drawLine(point1, point1 + QPoint(0,-m_cornerWidth/2));
    QPoint point2 = innerRect.topRight();
    painter.drawLine(point2, point2 + QPoint(m_cornerWidth/2,0));
    painter.drawLine(point2, point2 + QPoint(0,-m_cornerWidth/2));
    QPoint point3 = innerRect.bottomRight();
    painter.drawLine(point3, point3 + QPoint(m_cornerWidth/2,0));
    painter.drawLine(point3, point3 + QPoint(0,m_cornerWidth/2));
    QPoint point4 = innerRect.bottomLeft();
    painter.drawLine(point4, point4 + QPoint(-m_cornerWidth/2,0));
    painter.drawLine(point4, point4 + QPoint(0,m_cornerWidth/2));

    //! 绘制边框演示
    if( m_typeShowed & frameType::topframe )
        painter.drawLine(point1, point2);
    if( m_typeShowed & frameType::rightframe )
        painter.drawLine(point2, point3);
    if( m_typeShowed & frameType::bottomframe )
        painter.drawLine(point3, point4);
    if( m_typeShowed & frameType::leftframe )
        painter.drawLine(point4, point1);

    painter.restore();

    QFrame::paintEvent(event);
}
