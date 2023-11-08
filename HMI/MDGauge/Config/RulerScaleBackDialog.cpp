#include <QPainter>
#include <QMessageBox>
#include "RulerScaleBackDialog.h"
#include "ui_RulerScaleBackDialog.h"
#include "StyleDialog.h"
#include "ColorDialog.h"
#include "Ccommon.h"
#include "MDMultiLanguage.h"
#include "CMessage.h"

CRulerScaleBackDialog::CRulerScaleBackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRulerScaleBackDialog)
{
    ui->setupUi(this);
    m_rScaleBack = NULL;
    m_rScaleBackStyle = NULL;
    m_edgeStyle = NULL;
    m_edgeColor = NULL;
    m_gradient = NULL;
    m_color1 = NULL;
    m_color2 = NULL;
    this->initDialog();
    QStringList types;
    types << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}

CRulerScaleBackDialog::~CRulerScaleBackDialog()
{
    delete m_rScaleBackStyle;
    delete m_edgeStyle;
    delete m_edgeColor;
    delete m_gradient;
    delete m_color1;
    delete m_color2;
    delete ui;
}

void CRulerScaleBackDialog::initDialog()
{
    this->move(190,10);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->rWidEdit->installEventFilter(this);
    ui->rOffsetEdit->installEventFilter(this);
    ui->redgeWidEdit->installEventFilter(this);
    QObject::connect(ui->rWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setBackWid(qint32)));
    QObject::connect(ui->rOffsetSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setOffset(qint32)));
    QObject::connect(ui->redgeWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setEdgeWide(qint32)));

    QObject::connect(ui->rWidEdit,SIGNAL(editingFinished()),this,SLOT(widWarning()));
    QObject::connect(ui->rOffsetEdit,SIGNAL(editingFinished()),this,SLOT(offsetWarning()));
    QObject::connect(ui->redgeWidEdit,SIGNAL(editingFinished()),this,SLOT(edgeWarning()));
}

void CRulerScaleBackDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(0,0,0,100));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(ui->redgeColorBtn->x()+7,ui->redgeColorBtn->y()+ui->groupBox_2->y()-2,
                     ui->redgeColorBtn->width()+3,ui->redgeColorBtn->height()+3);
    painter.drawRect(ui->rcolor1Btn->x()+7,ui->rcolor1Btn->y()+ui->groupBox_3->y()-2,
                     ui->rcolor1Btn->width()+3,ui->rcolor1Btn->height()+3);
    painter.drawRect(ui->rcolor2Btn->x()+7,ui->rcolor2Btn->y()+ui->groupBox_3->y()-2,
                     ui->rcolor2Btn->width()+3,ui->rcolor2Btn->height()+3);
}

void CRulerScaleBackDialog::setData(CData *data)
{
    m_rScaleBack = (CRulerScaleBack*)data;
    m_rScaleBackStyle = new CPaintButton;
    m_rScaleBackStyle->m_button = ui->rBackArrangeBtn;
    m_rScaleBackStyle->setButtonImage(ui->rBackArrangeBtn,stRScaleBack,
                                      m_rScaleBack->m_arrangement,ptImage);
     m_edgeStyle = new CPaintButton;
     m_edgeStyle->m_button = ui->redgeStyleBtn;
     m_edgeStyle->setButtonImage(ui->redgeStyleBtn,stLine,
                                m_rScaleBack->m_edgeLine.m_lineStyle, ptImage);
     m_edgeColor = new CPaintButton;
     m_edgeColor->m_button = ui->redgeColorBtn;
     m_edgeColor->setButtonColor(ui->redgeColorBtn,m_rScaleBack->m_edgeLine.m_color,ptColor);
     m_gradient = new CPaintButton;
     m_gradient->m_button = ui->rgradStyleBtn;
     m_gradient->setButtonImage(ui->rgradStyleBtn,stGradient,
                                m_rScaleBack->m_backColor.m_gradientStyle,ptImage);
     m_color1 = new CPaintButton;
     m_color1->m_button = ui->rcolor1Btn;
     m_color1->setButtonColor(ui->rcolor1Btn,m_rScaleBack->m_backColor.m_color1,ptColor);
     m_color2 = new CPaintButton;
     m_color2->m_button = ui->rcolor2Btn;
     m_color2->setButtonColor(ui->rcolor2Btn,m_rScaleBack->m_backColor.m_color2,ptColor);
    Ccommon::setEdit(ui->rWidEdit,m_rScaleBack->getWidth());
    Ccommon::setEdit(ui->rOffsetEdit,m_rScaleBack->getOffset());
    Ccommon::setEdit(ui->redgeWidEdit,m_rScaleBack->m_edgeLine.getWidth());
    Ccommon::setSlider(ui->rWidSlider,m_rScaleBack->getWidth()*10);
    Ccommon::setSlider(ui->rOffsetSlider,m_rScaleBack->getOffset()*10);
    Ccommon::setSlider(ui->redgeWidSlider,m_rScaleBack->m_edgeLine.getWidth());
}
//!  通过编辑框输入属性的值
bool CRulerScaleBackDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->rWidEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->rWidEdit->text();
                 qint32 wid =qint32(str.toDouble()* 10);
                 ui->rWidSlider->setValue(wid);
                 m_rScaleBack->setWidth(str.toDouble());
             }
        }

    if (watched == ui->rOffsetEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->rOffsetEdit->text();
                 qint32 offset =qint32(str.toDouble()* 10);
                 ui->rOffsetSlider->setValue(offset);
                 m_rScaleBack->setOffset(str.toDouble());
             }
        }

    if (watched == ui->redgeWidEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->redgeWidEdit->text();
                 qint32 edge = str.toInt();
                 ui->redgeWidSlider->setValue(edge);
                 m_rScaleBack->m_edgeLine.setWidth(edge);
             }
        }
     return QWidget::eventFilter(watched,event);
}

void CRulerScaleBackDialog::setBackWid(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->rWidEdit->setText(str);
    m_rScaleBack->setWidth(ui->rWidEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerScaleBackDialog::setOffset(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(abs(value % 10));
    str.append(str1);
    ui->rOffsetEdit->setText(str);
    m_rScaleBack->setOffset(ui->rOffsetEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerScaleBackDialog::setEdgeWide(qint32 value)
{
    QString str = QString::number(value);
    ui->redgeWidEdit->setText(str);
    m_rScaleBack->m_edgeLine.setWidth(ui->redgeWidEdit->text().toInt());
    ((QWidget*)parent())->update();
}
/*!
\section 设置输入数值超限给出警告信息。
*/
void CRulerScaleBackDialog::widWarning()
{
    Ccommon::editDoubleWarning(ui->rWidEdit,WARN_1_25F,25,1);
}

void CRulerScaleBackDialog::offsetWarning()
{
    Ccommon::editDoubleWarning(ui->rOffsetEdit,WARN__55_55F,55,-55);
}

void CRulerScaleBackDialog::edgeWarning()
{
    Ccommon::editIntWarning(ui->redgeWidEdit,WARN_1_10I,10,1);
}

void CRulerScaleBackDialog::on_rBackArrangeBtn_clicked()
{
    m_rScaleBackStyle->buttonClicked();
    m_rScaleBack->m_arrangement = m_rScaleBackStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerScaleBackDialog::on_redgeStyleBtn_clicked()
{
    m_edgeStyle->buttonClicked();
    m_rScaleBack->m_edgeLine.m_lineStyle = (lineStyle)m_edgeStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerScaleBackDialog::on_rgradStyleBtn_clicked()
{
    m_gradient->buttonClicked();
    m_rScaleBack->m_backColor.m_gradientStyle = (gradientStyle)m_gradient->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerScaleBackDialog::on_redgeColorBtn_clicked()
{
    m_edgeColor->setParentDialog(this);
    m_edgeColor->buttonClicked();
    m_rScaleBack->m_edgeLine.m_color = m_edgeColor->m_backColor;
    ((QWidget*)parent())->update();
}

void CRulerScaleBackDialog::on_rcolor1Btn_clicked()
{
    m_color1->setParentDialog(this);
    m_color1->buttonClicked();
    m_rScaleBack->m_backColor.m_color1 = m_color1->m_backColor;
    ((QWidget*)parent())->update();
}

void CRulerScaleBackDialog::on_rcolor2Btn_clicked()
{
    m_color2->setParentDialog(this);
    m_color2->buttonClicked();
    m_rScaleBack->m_backColor.m_color2 = m_color2->m_backColor;
    ((QWidget*)parent())->update();
}

