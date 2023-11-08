#include <QMessageBox>
#include <QLayout>
#include <QTranslator>
#include "BackDialog.h"
#include "ui_BackDialog.h"
#include "StyleDialog.h"
#include "ColorDialog.h"
#include "Ccommon.h"
#include "CMessage.h"
#include "MDMultiLanguage.h"

CBackDialog::CBackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBackDialog)
{
    ui->setupUi(this);
    m_back = NULL;
    m_backStyle = NULL;
    m_edgeStyle = NULL;
    m_edgeColor = NULL;
    m_gradient = NULL;
    m_color1 = NULL;
    m_color2 = NULL;
    m_frameGradient = NULL;
    m_frameColor1 = NULL;
    m_frameColor2 = NULL;
    this->initDialog();

    QStringList types;
    types << "QLineEdit";
     CMDMultiLanguage::instance()->translateWidget(this, &types);
}

CBackDialog::~CBackDialog()
{
    delete m_backStyle;
    delete m_edgeStyle ;
    delete m_edgeColor;
    delete m_gradient ;
    delete m_color1;
    delete m_color2 ;
    delete m_frameGradient ;
    delete m_frameColor1;
    delete m_frameColor2;
    delete ui;
}

void CBackDialog::initDialog()
{
    this->move(190,0);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->edgeWidEdit->installEventFilter(this);
    ui->frameWidEdit->installEventFilter(this);
    QObject::connect(ui->edgeWidSlider,SIGNAL(valueChanged(int)),this,SLOT(setEdgeWidth(int)));
    QObject::connect(ui->frameWidSlider,SIGNAL(valueChanged(int)),this,SLOT(setFrameWidth(int)));
    QObject::connect(ui->edgeWidEdit,SIGNAL(editingFinished()),this,SLOT(edgeWarning()));
    QObject::connect(ui->frameWidEdit,SIGNAL(editingFinished()),this,SLOT(frameWarning()));
}
/*!
\section 将各个图标放置在一个矩形框内
*/
void CBackDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(0,0,0,100));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(ui->edgeColorBtn->x()+7,ui->edgeColorBtn->y()+ui->groupBox_2->y()-2,
                     ui->edgeColorBtn->width()+3,ui->edgeColorBtn->height()+3);
    painter.drawRect(ui->frameColor1Btn->x()+7,ui->frameColor1Btn->y()+ui->groupBox_3->y()-2,
                     ui->frameColor1Btn->width()+3,ui->frameColor1Btn->height()+3);
    painter.drawRect(ui->frameColor2Btn->x()+7,ui->frameColor2Btn->y()+ui->groupBox_3->y()-2,
                     ui->frameColor2Btn->width()+3,ui->frameColor2Btn->height()+3);
    painter.drawRect(ui->color1Btn->x()+7,ui->color1Btn->y()+ui->groupBox_4->y()-2,
                     ui->color1Btn->width()+3,ui->color1Btn->height()+3);
    painter.drawRect(ui->color2Btn->x()+7,ui->color2Btn->y()+ui->groupBox_4->y()-2,
                     ui->color2Btn->width()+3,ui->color2Btn->height()+3);

}
/*!
\section 在对话框中输入背景的属性值
*/
bool CBackDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->edgeWidEdit)
    {
        if (event->type()==QEvent::FocusOut)
        {
            QString str = ui->edgeWidEdit->text();
            qint32 edge = str.toInt();
            ui->edgeWidSlider->setValue(edge);
            m_back->m_edgeLine.setWidth(edge);
        }
    }
    if (watched == ui->frameWidEdit)
    {
        if (event->type()==QEvent::FocusOut)
        {
            QString str = ui->frameWidEdit->text();
            qint32 frame =qint32(str.toDouble()* 10);
            ui->frameWidSlider->setValue(frame);
            m_back->setEdgeWidth(str.toDouble());
        }
    }

    return QWidget::eventFilter(watched,event);
}

void CBackDialog::setData(CData *data)
{
    m_back = (CDataBack*)data;
    m_backStyle = new CPaintButton;
    m_backStyle->m_button = ui->backStyleBtn;
    m_backStyle->setButtonImage(ui->backStyleBtn,stBack,m_back->m_style,ptImage);
     m_edgeStyle = new CPaintButton;
     m_edgeStyle->m_button = ui->edgeStyleBtn;
     m_edgeStyle->setButtonImage(ui->edgeStyleBtn,stLine,
                                 m_back->m_edgeLine.m_lineStyle,ptImage);
     m_edgeColor = new CPaintButton;
     m_edgeColor->m_button = ui->edgeColorBtn;
     m_edgeColor->setButtonColor(ui->edgeColorBtn,m_back->m_edgeLine.m_color,ptColor);
     m_gradient = new CPaintButton;
     m_gradient->m_button = ui->gradStyleBtn;
     m_gradient->setButtonImage(ui->gradStyleBtn,stGradient,
                                m_back->m_backColor.m_gradientStyle,ptImage);
     m_color1 = new CPaintButton;
     m_color1->m_button = ui->color1Btn;
     m_color1->setButtonColor(ui->color1Btn,m_back->m_backColor.m_color1,ptColor);
     m_color2 = new CPaintButton;
     m_color2->m_button = ui->color2Btn;
     m_color2->setButtonColor(ui->color2Btn,m_back->m_backColor.m_color2,ptColor);
     m_frameGradient = new CPaintButton;
     m_frameGradient->m_button = ui->frameGraBtn;
     m_frameGradient->setButtonImage(ui->frameGraBtn,stGradient,m_back->m_edgeColor.m_gradientStyle,ptImage);
     m_frameColor1 = new CPaintButton;
     m_frameColor1->m_button = ui->frameColor1Btn;
     m_frameColor1->setButtonColor(ui->frameColor1Btn,m_back->m_edgeColor.m_color1,ptColor);
     m_frameColor2 = new CPaintButton;
     m_frameColor2->m_button = ui->frameColor2Btn;
     m_frameColor2->setButtonColor(ui->frameColor2Btn,m_back->m_edgeColor.m_color2,ptColor);
    Ccommon::setSlider(ui->edgeWidSlider,m_back->m_edgeLine.getWidth());
    Ccommon::setSlider(ui->frameWidSlider,m_back->getEdgeWidth()*10);
    Ccommon::setEdit(ui->edgeWidEdit,m_back->m_edgeLine.getWidth());
    Ccommon::setEdit(ui->frameWidEdit,m_back->getEdgeWidth());
}

void CBackDialog::on_backStyleBtn_clicked()
{
    m_backStyle->buttonClicked();
    m_back->m_style = m_backStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CBackDialog::setEdgeWidth(qint32 value)
{
    QString ret = QString::number(value);
    ui->edgeWidEdit->setText(ret);
    m_back->m_edgeLine.setWidth(ui->edgeWidEdit->text().toInt());
    ((QWidget*)parent())->update();
}

void CBackDialog::setFrameWidth(qint32 value)
{
    QString ret = QString("%1").arg(value / 10);
    ret.append(".");
    QString ret1 = QString("%1").arg(value % 10);
    ret.append(ret1);
    ui->frameWidEdit->setText(ret);
    m_back->setEdgeWidth(ui->frameWidEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CBackDialog::on_edgeColorBtn_clicked()
{
    m_edgeColor->setParentDialog(this);
    m_edgeColor->buttonClicked();
    m_back->m_edgeLine.m_color = m_edgeColor->m_backColor;
    ((QWidget*)parent())->update();
}

void CBackDialog::on_edgeStyleBtn_clicked()
{
    m_edgeStyle->buttonClicked();
    m_back->m_edgeLine.m_lineStyle = (lineStyle)m_edgeStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CBackDialog::on_color1Btn_clicked()
{
    m_color1->setParentDialog(this);
    m_color1->buttonClicked();
    m_back->m_backColor.m_color1 = m_color1->m_backColor;
    ((QWidget*)parent())->update();
}

void CBackDialog::on_color2Btn_clicked()
{
    m_color2->setParentDialog(this);
    m_color2->buttonClicked();
    m_back->m_backColor.m_color2 = m_color2->m_backColor;
    ((QWidget*)parent())->update();
}

void CBackDialog::on_gradStyleBtn_clicked()
{
    m_gradient->buttonClicked();
    m_back->m_backColor.m_gradientStyle = (gradientStyle)m_gradient->getIndex();
     ((QWidget*)parent())->update();
}

void CBackDialog::on_frameColor1Btn_clicked()
{
    m_frameColor1->setParentDialog(this);
    m_frameColor1->buttonClicked();
    m_back->m_edgeColor.m_color1 = m_frameColor1->m_backColor;
    ((QWidget*)parent())->update();
}

void CBackDialog::on_frameColor2Btn_clicked()
{
    m_frameColor2->setParentDialog(this);
    m_frameColor2->buttonClicked();
    m_back->m_edgeColor.m_color2 = m_frameColor2->m_backColor;
    ((QWidget*)parent())->update();
}

void CBackDialog::on_frameGraBtn_clicked()
{
    m_frameGradient->buttonClicked();
    m_back->m_edgeColor.m_gradientStyle = (gradientStyle)m_frameGradient->getIndex();
     ((QWidget*)parent())->update();
}

void CBackDialog::edgeWarning()
{
    Ccommon::editIntWarning(ui->edgeWidEdit,WARN_1_10I,10,1);
}

void CBackDialog::frameWarning()
{
    Ccommon::editDoubleWarning(ui->frameWidEdit,WARN_1_50F,50,1);
}

