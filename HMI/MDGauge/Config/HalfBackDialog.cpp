#include <QMessageBox>
#include "HalfBackDialog.h"
#include "ui_HalfBackDialog.h"
#include "StyleDialog.h"
#include "ColorDialog.h"
#include "Ccommon.h"
#include "MDMultiLanguage.h"
#include "CMessage.h"

CHalfBackDialog::CHalfBackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CHalfBackDialog)
{
    ui->setupUi(this);
    m_halfBack = NULL;
    m_halfBackStyle = NULL;
    m_halfArrange = NULL;
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

CHalfBackDialog::~CHalfBackDialog()
{
    delete m_halfBackStyle;
    delete m_halfArrange;
    delete m_edgeStyle ;
    delete m_edgeColor;
    delete m_gradient;
    delete m_color1;
    delete m_color2;
    delete m_frameGradient;
    delete m_frameColor1;
    delete m_frameColor2;
    delete ui;
}

void CHalfBackDialog::initDialog()
{
    this->move(190,0);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->edgeWidEdit->installEventFilter(this);
    ui->frameWidEdit->installEventFilter(this);
    QObject::connect(ui->edgeWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setEdgeWidth(qint32)));
    QObject::connect(ui->frameWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setFrameWidth(qint32)));
    QObject::connect(ui->edgeWidEdit,SIGNAL(editingFinished()),this,SLOT(edgeWarning()));
    QObject::connect(ui->frameWidEdit,SIGNAL(editingFinished()),this,SLOT(frameWarning()));

}

void CHalfBackDialog::paintEvent(QPaintEvent *)
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
    painter.drawRect(ui->bkColor1Btn->x()+7,ui->bkColor1Btn->y()+ui->groupBox_4->y()-2,
                     ui->bkColor1Btn->width()+3,ui->bkColor1Btn->height()+3);
    painter.drawRect(ui->bkColor2Btn->x()+7,ui->bkColor2Btn->y()+ui->groupBox_4->y()-2,
                     ui->bkColor2Btn->width()+3,ui->bkColor2Btn->height()+3);
}
 /*!
\section 将各个属性的值显示在配置界面上。
*/
void CHalfBackDialog::setData(CData *data)
{
    m_halfBack = (CDataHalfBack*)data;
    m_halfBackStyle = new CPaintButton;
    m_halfBackStyle->m_button = ui->halfBackStyBtn;
    m_halfBackStyle->setButtonImage(ui->halfBackStyBtn,stHalfStyle,
                                    m_halfBack->m_style,ptImage);
    m_halfArrange = new CPaintButton;
    m_halfArrange->m_button = ui->halfArrangeBtn;
    m_halfArrange->setButtonImage(ui->halfArrangeBtn,stHalfArrangement,
                                  m_halfBack->m_arrangement,ptImage);
     m_edgeStyle = new CPaintButton;
     m_edgeStyle->m_button = ui->edgeStyleBtn;
     m_edgeStyle->setButtonImage(ui->edgeStyleBtn,stLine,
                                 m_halfBack->m_edgeLine.m_lineStyle,ptImage);
     m_edgeColor = new CPaintButton;
     m_edgeColor->m_button = ui->edgeColorBtn;
     m_edgeColor->setButtonColor(ui->edgeColorBtn,m_halfBack->m_edgeLine.m_color,ptColor);
     m_gradient = new CPaintButton;
     m_gradient->m_button = ui->bkGraBtn;
     m_gradient->setButtonImage(ui->bkGraBtn,stGradient,
                                m_halfBack->m_backColor.m_gradientStyle,ptImage);
     m_color1 = new CPaintButton;
     m_color1->m_button = ui->bkColor1Btn;
     m_color1->setButtonColor(ui->bkColor1Btn,m_halfBack->m_backColor.m_color1,ptColor);
     m_color2 = new CPaintButton;
     m_color2->m_button = ui->bkColor2Btn;
     m_color2->setButtonColor(ui->bkColor2Btn,m_halfBack->m_backColor.m_color2,ptColor);
     m_frameGradient = new CPaintButton;
     m_frameGradient->m_button = ui->frameGraBtn;
     m_frameGradient->setButtonImage(ui->frameGraBtn,stGradient,
                                     m_halfBack->m_edgeColor.m_gradientStyle,ptImage);
     m_frameColor1 = new CPaintButton;
     m_frameColor1->m_button = ui->frameColor1Btn;
     m_frameColor1->setButtonColor(ui->frameColor1Btn,
                                   m_halfBack->m_edgeColor.m_color1,ptColor);
     m_frameColor2 = new CPaintButton;
     m_frameColor2->m_button = ui->frameColor2Btn;
     m_frameColor2->setButtonColor(ui->frameColor2Btn,
                                   m_halfBack->m_edgeColor.m_color2,ptColor);
    Ccommon::setSlider(ui->edgeWidSlider,m_halfBack->m_edgeLine.getWidth());
    Ccommon::setEdit(ui->edgeWidEdit,m_halfBack->m_edgeLine.getWidth());
    Ccommon::setSlider(ui->frameWidSlider,m_halfBack->getEdgeWidth()*10);
    Ccommon::setEdit(ui->frameWidEdit,m_halfBack->getEdgeWidth());
}

/*!
\section 向编辑框中输入数值对半圆背景进行配置。
*/
bool CHalfBackDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->edgeWidEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->edgeWidEdit->text();
                 qint32 edge = str.toInt();
                 ui->edgeWidSlider->setValue(edge);
                 m_halfBack->m_edgeLine.setWidth(edge);
             }
        }
    if (watched == ui->frameWidEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->frameWidEdit->text();
                 qint32 frame =qint32(str.toDouble()* 10);
                 ui->frameWidSlider->setValue(frame);
                 m_halfBack->setEdgeWidth(str.toDouble());
             }
        }

    return QWidget::eventFilter(watched,event);
}

void CHalfBackDialog::setEdgeWidth(qint32 value)
{
    QString ret = QString::number(value);
    ui->edgeWidEdit->setText(ret);
    m_halfBack->m_edgeLine.setWidth(ui->edgeWidEdit->text().toInt());
    ((QWidget*)parent())->update();
}

void CHalfBackDialog::setFrameWidth(qint32 value)
{
    QString ret = QString("%1").arg(value / 10);
    ret.append(".");
    QString ret1 = QString("%1").arg(value % 10);
    ret.append(ret1);
    ui->frameWidEdit->setText(ret);
    m_halfBack->setEdgeWidth(ui->frameWidEdit->text().toDouble());
    ((QWidget*)parent())->update();
}
/*!
\section 超出限定值给出警告。
*/
void CHalfBackDialog::edgeWarning()
{
    Ccommon::editIntWarning(ui->edgeWidEdit,WARN_0_10I,10,1);
}

void CHalfBackDialog::frameWarning()
{
    Ccommon::editDoubleWarning(ui->frameWidEdit,WARN_1_50F,50,1);
}
/*!
\section 设置半圆背景的风格。
*/
void CHalfBackDialog::on_halfBackStyBtn_clicked()
{
    m_halfBackStyle->buttonClicked();
    m_halfBack->m_style = m_halfBackStyle->getIndex();
     ((QWidget*)parent())->update();
}
/*!
\section 设置半圆背景的布局。
*/
void CHalfBackDialog::on_halfArrangeBtn_clicked()
{
    m_halfArrange->buttonClicked();
    m_halfBack->m_arrangement = m_halfArrange->getIndex();
     ((QWidget*)parent())->update();
}
/*!
\section 设置半圆背景边缘线的风格。
*/
void CHalfBackDialog::on_edgeStyleBtn_clicked()
{
    m_edgeStyle->buttonClicked();
    m_halfBack->m_edgeLine.m_lineStyle = (lineStyle)m_edgeStyle->getIndex();
     ((QWidget*)parent())->update();
}
/*!
\section 设置半圆背景的填充形式。
*/
void CHalfBackDialog::on_bkGraBtn_clicked()
{
    m_gradient->buttonClicked();
    m_halfBack->m_backColor.m_gradientStyle = (gradientStyle)m_gradient->getIndex();
     ((QWidget*)parent())->update();
}
/*!
\section 设置半圆背景的边框的填充形式。
*/
void CHalfBackDialog::on_frameGraBtn_clicked()
{
    m_frameGradient->buttonClicked();
    m_halfBack->m_edgeColor.m_gradientStyle = (gradientStyle)m_frameGradient->getIndex();
     ((QWidget*)parent())->update();
}
/*!
\section 设置半圆背景的边缘线的颜色。
*/
void CHalfBackDialog::on_edgeColorBtn_clicked()
{
    m_edgeColor->setParentDialog(this);
    m_edgeColor->buttonClicked();
    m_halfBack->m_edgeLine.m_color = m_edgeColor->m_backColor;
    ((QWidget*)parent())->update();
}
/*!
\section 设置半圆背景的填充颜色1。
*/

void CHalfBackDialog::on_bkColor1Btn_clicked()
{
    m_color1->setParentDialog(this);
    m_color1->buttonClicked();
    m_halfBack->m_backColor.m_color1 = m_color1->m_backColor;
    ((QWidget*)parent())->update();
}
/*!
\section 设置半圆背景的填充颜色2。
*/

void CHalfBackDialog::on_bkColor2Btn_clicked()
{
    m_color2->setParentDialog(this);
    m_color2->buttonClicked();
    m_halfBack->m_backColor.m_color2 = m_color2->m_backColor;
    ((QWidget*)parent())->update();
}
/*!
\section 设置半圆背景边框的填充颜色1。
*/
void CHalfBackDialog::on_frameColor1Btn_clicked()
{
    m_frameColor1->setParentDialog(this);
    m_frameColor1->buttonClicked();
    m_halfBack->m_edgeColor.m_color1 = m_frameColor1->m_backColor;
    ((QWidget*)parent())->update();
}

void CHalfBackDialog::on_frameColor2Btn_clicked()
{
    m_frameColor2->setParentDialog(this);
    m_frameColor2->buttonClicked();
    m_halfBack->m_edgeColor.m_color2 = m_frameColor2->m_backColor;
    ((QWidget*)parent())->update();
}

