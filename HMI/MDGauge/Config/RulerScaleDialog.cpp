#include <QPainter>
#include <QMessageBox>
#include "RulerScaleDialog.h"
#include "ui_RulerScaleDialog.h"
#include "StyleDialog.h"
#include "ColorDialog.h"
#include "Ccommon.h"
#include "MDMultiLanguage.h"
#include "CMessage.h"

CRulerScaleDialog::CRulerScaleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRulerScaleDialog)
{
    ui->setupUi(this);
    m_rScale = NULL;
    m_rScaleStyle = NULL;
    m_rScaleArrange = NULL;
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

CRulerScaleDialog::~CRulerScaleDialog()
{
    delete m_rScaleStyle;
    delete m_rScaleArrange;
    delete m_edgeStyle;
    delete m_edgeColor;
    delete m_gradient;
    delete m_color1;
    delete m_color2 ;
    delete ui;
}

void CRulerScaleDialog::initDialog()
{
    this->move(190,10);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->rcolor2Btn->setChecked(false);
    ui->rSubScaleEdit->setDisabled(true);
    ui->rSubScaleSlider->setDisabled(true);
    ui->rWidEdit->installEventFilter(this);
    ui->rLenEdit->installEventFilter(this);
    ui->rOffsetEdit->installEventFilter(this);
    ui->rSubScaleEdit->installEventFilter(this);
    ui->redgeWidEdit->installEventFilter(this);

    QObject::connect(ui->rWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setScaleWid(qint32)));
    QObject::connect(ui->rLenSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setLen(qint32)));
    QObject::connect(ui->rOffsetSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setOffset(qint32)));
    QObject::connect(ui->rSubScaleSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setSubScale(qint32)));
    QObject::connect(ui->redgeWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setEdgeWide(qint32)));

    QObject::connect(ui->rWidEdit,SIGNAL(editingFinished()),this,SLOT(widWarning()));
    QObject::connect(ui->rLenEdit,SIGNAL(editingFinished()),this,SLOT(lenWarning()));
    QObject::connect(ui->rOffsetEdit,SIGNAL(editingFinished()),this,SLOT(offsetWarning()));
    QObject::connect(ui->rSubScaleEdit,SIGNAL(editingFinished()),this,SLOT(subWarning()));
    QObject::connect(ui->redgeWidEdit,SIGNAL(editingFinished()),this,SLOT(edgeWarning()));
    QObject::connect(ui->rCheckBox,SIGNAL(stateChanged(qint32)),this,SLOT(setSub(qint32)));
}

void CRulerScaleDialog::paintEvent(QPaintEvent *)
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

void CRulerScaleDialog::setData(CData *data)
{
    m_rScale = (CRulerScale*)data;
    m_rScaleStyle = new CPaintButton;
    m_rScaleStyle->m_button = ui->rScaleStyleBtn;
    m_rScaleStyle->setButtonImage(ui->rScaleStyleBtn,stRScaleStyle,
                                  m_rScale->m_style,ptImage);
    m_rScaleArrange = new CPaintButton;
    m_rScaleArrange->m_button = ui->rScaleArrangeBtn;
    m_rScaleArrange->setButtonImage(ui->rScaleArrangeBtn,stScaleArrangement,
                                    m_rScale->m_arrangement,ptImage);
     m_edgeStyle = new CPaintButton;
     m_edgeStyle->m_button = ui->redgeStyleBtn;
     m_edgeStyle->setButtonImage(ui->redgeStyleBtn,stLine,
                                 m_rScale->m_edgeLine.m_lineStyle,ptImage);
     m_edgeColor = new CPaintButton;
     m_edgeColor->m_button = ui->redgeColorBtn;
     m_edgeColor->setButtonColor(ui->redgeColorBtn,m_rScale->m_edgeLine.m_color,ptColor);
     m_gradient = new CPaintButton;
     m_gradient->m_button = ui->rgradStyleBtn;
     m_gradient->setButtonImage(ui->rgradStyleBtn,stGradient,
                                m_rScale->m_backColor.m_gradientStyle,ptImage);
     m_color1 = new CPaintButton;
     m_color1->m_button = ui->rcolor1Btn;
     m_color1->setButtonColor(ui->rcolor1Btn,m_rScale->m_backColor.m_color1,ptColor);
     m_color2 = new CPaintButton;
     m_color2->m_button = ui->rcolor2Btn;
     m_color2->setButtonColor(ui->rcolor2Btn,m_rScale->m_backColor.m_color2,ptColor);
    Ccommon::setEdit(ui->rWidEdit,m_rScale->getWidth());
    Ccommon::setEdit(ui->rLenEdit,m_rScale->getLen());
    Ccommon::setEdit(ui->rOffsetEdit,m_rScale->getOffset());
    Ccommon::setEdit(ui->rSubScaleEdit,m_rScale->getSubCount());
    Ccommon::setEdit(ui->redgeWidEdit,m_rScale->m_edgeLine.getWidth());
    Ccommon::setSlider(ui->rWidSlider,m_rScale->getWidth()*10);
    Ccommon::setSlider(ui->rLenSlider,m_rScale->getLen()*10);
    Ccommon::setSlider(ui->rOffsetSlider,m_rScale->getOffset()*10);
    Ccommon::setSlider(ui->rSubScaleSlider,m_rScale->getSubCount());
    Ccommon::setSlider(ui->redgeWidSlider,m_rScale->m_edgeLine.getWidth());
   QString str;
    if(m_rScale->m_isSubScale ==  false)
    {
        ui->rSubScaleEdit->setDisabled(true);
        ui->rSubScaleSlider->setDisabled(true);
    }
    else
    {
        ui->rCheckBox->setChecked(true);
        ui->rSubScaleEdit->setDisabled(false);
        ui->rSubScaleSlider->setDisabled(false);
        ui->rSubScaleSlider->setValue(m_rScale->getSubCount());
        str.setNum(m_rScale->getSubCount());
        ui->rSubScaleEdit->setText(str);
    }
}

bool CRulerScaleDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->rWidEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->rWidEdit->text();
                 qint32 wid =qint32(str.toDouble()* 10);
                 ui->rWidSlider->setValue(wid);
                 m_rScale->setWidth(str.toDouble());
             }
        }
    if (watched == ui->rLenEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->rLenEdit->text();
                 qint32 len =qint32(str.toDouble()* 10);
                 ui->rLenSlider->setValue(len);
                 m_rScale->setOffset(str.toDouble());
             }
        }
    if (watched == ui->rOffsetEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->rOffsetEdit->text();
                 qint32 offset =qint32(str.toDouble()* 10);
                 ui->rOffsetSlider->setValue(offset);
                 m_rScale->setOffset(str.toDouble());
             }
        }
    if (watched == ui->rSubScaleEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->rSubScaleEdit->text();
                 qint32 sub = str.toInt();
                 ui->rSubScaleSlider->setValue(sub);
                 m_rScale->setSubCount(sub);
             }
        }
    if (watched == ui->redgeWidEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->redgeWidEdit->text();
                 qint32 edge = str.toInt();
                 ui->redgeWidSlider->setValue(edge);
                 m_rScale->m_edgeLine.setWidth(edge);
             }
        }
     return QWidget::eventFilter(watched,event);
}

void CRulerScaleDialog::setScaleWid(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->rWidEdit->setText(str);
    m_rScale->setWidth(ui->rWidEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerScaleDialog::setLen(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->rLenEdit->setText(str);
    m_rScale->setLen(ui->rLenEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerScaleDialog::setOffset(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(abs(value % 10));
    str.append(str1);
    ui->rOffsetEdit->setText(str);
    m_rScale->setOffset(ui->rOffsetEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerScaleDialog::setSubScale(qint32 value)
{
    QString str = QString::number(value);
    ui->rSubScaleEdit->setText(str);
    m_rScale->setSubCount(ui->rSubScaleEdit->text().toInt());
    ((QWidget*)parent())->update();
}

void CRulerScaleDialog::setEdgeWide(qint32 value)
{
    QString str = QString::number(value);
    ui->redgeWidEdit->setText(str);
    m_rScale->m_edgeLine.setWidth(ui->redgeWidEdit->text().toInt());
    ((QWidget*)parent())->update();
}

void CRulerScaleDialog::widWarning()
{
    Ccommon::editDoubleWarning(ui->rWidEdit,WARN_1_25F,25,1);
}

void CRulerScaleDialog::lenWarning()
{
    Ccommon::editDoubleWarning(ui->rLenEdit,WARN_1_36F,36,1);
}

void CRulerScaleDialog::offsetWarning()
{
    Ccommon::editDoubleWarning(ui->rOffsetEdit,WARN__55_55F,55,-55);
}

void CRulerScaleDialog::subWarning()
{
    Ccommon::editIntWarning(ui->rSubScaleEdit,WARN_1_10I,10,1);
}

void CRulerScaleDialog::edgeWarning()
{
    Ccommon::editIntWarning(ui->redgeWidEdit,WARN_1_10I,10,1);
}

void CRulerScaleDialog::setSub(qint32 state)
{
    if(state == 0)
    {
        m_rScale->m_isSubScale = false;
        ui->rSubScaleEdit->setDisabled(true);
        ui->rSubScaleSlider->setDisabled(true);
    }
    else
    {
        m_rScale->m_isSubScale = true;
        ui->rSubScaleEdit->setDisabled(false);
        ui->rSubScaleSlider->setDisabled(false);
    }
    ((QWidget*)parent())->update();
}

void CRulerScaleDialog::on_rScaleStyleBtn_clicked()
{
    m_rScaleStyle->buttonClicked();
    m_rScale->m_style = m_rScaleStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerScaleDialog::on_rScaleArrangeBtn_clicked()
{
    m_rScaleArrange->buttonClicked();
    m_rScale->m_arrangement = m_rScaleArrange->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerScaleDialog::on_redgeStyleBtn_clicked()
{
    m_edgeStyle->buttonClicked();
    m_rScale->m_edgeLine.m_lineStyle = (lineStyle)m_edgeStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerScaleDialog::on_rgradStyleBtn_clicked()
{
    m_gradient->buttonClicked();
    m_rScale->m_backColor.m_gradientStyle = (gradientStyle)m_gradient->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerScaleDialog::on_redgeColorBtn_clicked()
{
    m_edgeColor->setParentDialog(this);
    m_edgeColor->buttonClicked();
    m_rScale->m_edgeLine.m_color = m_edgeColor->m_backColor;
    ((QWidget*)parent())->update();
}

void CRulerScaleDialog::on_rcolor1Btn_clicked()
{
    m_color1->setParentDialog(this);
    m_color1->buttonClicked();
    m_rScale->m_backColor.m_color1 = m_color1->m_backColor;
    ((QWidget*)parent())->update();
}

void CRulerScaleDialog::on_rcolor2Btn_clicked()
{
    m_color2->setParentDialog(this);
    m_color2->buttonClicked();
    m_rScale->m_backColor.m_color2 = m_color2->m_backColor;
    ((QWidget*)parent())->update();
}



