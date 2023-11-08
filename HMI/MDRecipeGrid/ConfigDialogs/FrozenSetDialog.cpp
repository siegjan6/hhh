#include "FrozenSetDialog.h"
#include "ui_FrozenSetDialog.h"
#include <QPainter>
#include <QColorDialog>
#include "ShareTool.h"

CFrozenSetDialog::CFrozenSetDialog(qint32 rowFrozen, qint32 columnFrozen, const frameData &data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CFrozenSetDialog)
{
    ui->setupUi(this);

    m_rowFrozen = rowFrozen;
    m_columnFrozen = columnFrozen;
    m_frameData = data;
    //! 子部件初始化
    QIntValidator* validator = new QIntValidator(0,1000,this);
    //! 固定行数
    ui->lineeditRowNum->setText(QString::number(m_rowFrozen));
    ui->lineeditRowNum->setValidator(validator);
    //! 固定列数
    ui->lineeditColumnNum->setText(QString::number(m_columnFrozen));
    ui->lineeditColumnNum->setValidator(validator);
    //! 边框颜色
    ui->toolbuttonColor->setPalette(QPalette(m_frameData.pen.color()));
    ui->toolbuttonColor->setAutoFillBackground(true);
    //! 画笔宽度
    ui->lineeditWidth->setText(QString::number(data.pen.width()));
    ui->lineeditWidth->setValidator(new QIntValidator(1,100,this));
    //! 边框的画笔样式将在showEvent中初始化,具体原因参考CFrameDialog

    CShareTool::installDialogTranslator(this);
}

CFrozenSetDialog::~CFrozenSetDialog()
{
    delete ui;
}

void CFrozenSetDialog::showEvent(QShowEvent *event)
{
    //! 边框的画笔样式
    CShareTool::initPenStyleComboBox(ui->comboboxStyle, m_frameData.pen.style());

    QDialog::showEvent(event);
}

void CFrozenSetDialog::on_toolbuttonColor_clicked()
{
    QColor color = QColorDialog::getColor(m_frameData.pen.color(), this);
    if(!color.isValid())
        return;

    m_frameData.pen.setColor(color);
    ui->toolbuttonColor->setPalette(QPalette(color));
}

void CFrozenSetDialog::accept()
{
    bool ok=false;
    qint32 intValue;

    //! 固定行数量
    if(ui->lineeditRowNum->text().isEmpty())
        m_rowFrozen = 0;
    else
    {
        intValue = ui->lineeditRowNum->text().toInt(&ok);
        if(ok)
            m_rowFrozen = intValue;
    }
    //! 固定列数量
    if(ui->lineeditColumnNum->text().isEmpty())
        m_columnFrozen = 0;
    else
    {
        intValue = ui->lineeditColumnNum->text().toInt(&ok);
        if(ok)
            m_columnFrozen = intValue;
    }
    //! 边框类型:目前仅支持四边都有的全边框
    m_frameData.type = frameType::allframe;
    //! 画笔风格
    m_frameData.pen.setStyle((Qt::PenStyle)(ui->comboboxStyle->currentIndex() + Qt::SolidLine));
    //! 画笔颜色直接在on_toolbuttonColor_clicked()中赋值
    //! 画笔宽度
    ok=false;
    intValue = m_frameData.pen.width();
    intValue = ui->lineeditWidth->text().toInt(&ok);
    if(ok)
        m_frameData.pen.setWidth(intValue);

    QDialog::accept();
}
