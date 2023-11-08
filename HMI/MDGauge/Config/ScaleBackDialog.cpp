#include <QMessageBox>
#include "ScaleBackDialog.h"
#include "ui_ScaleBackDialog.h"
#include "StyleDialog.h"
#include "ColorDialog.h"
#include "Ccommon.h"
#include "MDMultiLanguage.h"
#include "CMessage.h"

CScaleBackDialog::CScaleBackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CScaleBackDialog)
{
    ui->setupUi(this);
    m_scaleBackStyle = NULL;
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

CScaleBackDialog::~CScaleBackDialog()
{
    delete m_scaleBackStyle;
    delete m_edgeStyle;
    delete m_edgeColor;
    delete m_gradient;
    delete m_color1;
    delete m_color2;
    delete ui;
}

void CScaleBackDialog::initDialog()
{
    this->move(190,0);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->widEdit->installEventFilter(this);
    ui->radiusEdit->installEventFilter(this);
    ui->edgeWidEdit->installEventFilter(this);
    QObject::connect(ui->widSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setWid(qint32)));
    QObject::connect(ui->radiusSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setRadius(qint32)));
    QObject::connect(ui->edgeWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setEdgeWid(qint32)));

    QObject::connect(ui->widEdit,SIGNAL(editingFinished()),this,SLOT(widWarning()));
    QObject::connect(ui->radiusEdit,SIGNAL(editingFinished()),this,SLOT(radiusWarning()));
    QObject::connect(ui->edgeWidEdit,SIGNAL(editingFinished()),this,SLOT(edgeWarning()));
}

void CScaleBackDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(0,0,0,100));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(ui->edgeColorBtn->x()+7,ui->edgeColorBtn->y()+ui->groupBox_2->y()-2,
                     ui->edgeColorBtn->width()+3,ui->edgeColorBtn->height()+3);
    painter.drawRect(ui->color1Btn->x()+7,ui->color1Btn->y()+ui->groupBox_3->y()-2,
                     ui->color1Btn->width()+3,ui->color1Btn->height()+3);
    painter.drawRect(ui->color2Btn->x()+7,ui->color2Btn->y()+ui->groupBox_3->y()-2,
                     ui->color2Btn->width()+3,ui->color2Btn->height()+3);
}

void CScaleBackDialog::setData(CData *data)
{
    m_scaleBack = (CDataScaleBack*)data;
    m_scaleBackStyle = new CPaintButton;
    m_scaleBackStyle->m_button = ui->scaleBackBtn;
    m_scaleBackStyle->setButtonImage(ui->scaleBackBtn,stScaleBack,
                                   m_scaleBack->m_arrangement,ptImage );
     m_edgeStyle = new CPaintButton;
     m_edgeStyle->m_button = ui->edgeStyleBtn;
     m_edgeStyle->setButtonImage(ui->edgeStyleBtn,stLine,
                                 m_scaleBack->m_edgeLine.m_lineStyle,ptImage);
     m_edgeColor = new CPaintButton;
     m_edgeColor->m_button = ui->edgeColorBtn;
     m_edgeColor->setButtonColor(ui->edgeColorBtn,m_scaleBack->m_edgeLine.m_color,ptColor);
     m_gradient = new CPaintButton;
     m_gradient->m_button = ui->gradStyleBtn;
     m_gradient->setButtonImage(ui->gradStyleBtn,stGradient,
                                m_scaleBack->m_backColor.m_gradientStyle,ptImage);
     m_color1 = new CPaintButton;
     m_color1->m_button = ui->color1Btn;
     m_color1->setButtonColor(ui->color1Btn,m_scaleBack->m_backColor.m_color1,ptColor);
     m_color2 = new CPaintButton;
     m_color2->m_button = ui->color2Btn;
     m_color2->setButtonColor(ui->color2Btn,m_scaleBack->m_backColor.m_color2,ptColor);
    Ccommon::setEdit(ui->widEdit,m_scaleBack->getWidth());
    Ccommon::setEdit(ui->radiusEdit,m_scaleBack->getRadius());
    Ccommon::setEdit(ui->edgeWidEdit,m_scaleBack->m_edgeLine.getWidth());
    Ccommon::setSlider(ui->widSlider,m_scaleBack->getWidth()*10);
    Ccommon::setSlider(ui->radiusSlider,m_scaleBack->getRadius()*10);
    Ccommon::setSlider(ui->edgeWidSlider,m_scaleBack->m_edgeLine.getWidth());
}

bool CScaleBackDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->widEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->widEdit->text();
                 qint32 wid = (qint32)(str.toDouble()*10);
                 ui->widSlider->setValue(wid);
                 m_scaleBack->setWidth(str.toDouble());
             }
        }
    if (watched == ui->radiusEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->radiusEdit->text();
                 qint32 radius = qint32(str.toDouble()* 10);
                 ui->radiusSlider->setValue(radius);
                 m_scaleBack->setRadius(str.toDouble());
             }
        }
    if (watched == ui->edgeWidEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->edgeWidEdit->text();
                 qint32 edge = str.toInt();
                 ui->edgeWidSlider->setValue(edge);
                 m_scaleBack->m_edgeLine.setWidth(edge);
             }
        }
    return QWidget::eventFilter(watched,event);
    ((QWidget*)parent())->update();
}

void CScaleBackDialog::on_scaleBackBtn_clicked()
{
    m_scaleBackStyle->buttonClicked();
    m_scaleBack->m_arrangement = m_scaleBackStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CScaleBackDialog::setWid(qint32 value)
{
    QString ret = QString("%1").arg(value / 10);
    ret.append(".");
    QString ret1 = QString("%1").arg(value % 10);
    ret.append(ret1);
    ui->widEdit->setText(ret);
    m_scaleBack->setWidth(ui->widEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CScaleBackDialog::setRadius(qint32 value)
{
    QString ret = QString("%1").arg(value / 10);
    ret.append(".");
    QString ret1 = QString("%1").arg(value % 10);
    ret.append(ret1);
    ui->radiusEdit->setText(ret);
    m_scaleBack->setRadius(ui->radiusEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CScaleBackDialog::setEdgeWid(qint32 value)
{
    QString ret = QString::number(value);
    ui->edgeWidEdit->setText(ret);
    m_scaleBack->m_edgeLine.setWidth(ui->edgeWidEdit->text().toInt());
    ((QWidget*)parent())->update();
}

void CScaleBackDialog::on_edgeColorBtn_clicked()
{
    m_edgeColor->setParentDialog(this);
    m_edgeColor->buttonClicked();
    m_scaleBack->m_edgeLine.m_color = m_edgeColor->m_backColor;
    ((QWidget*)parent())->update();
}

void CScaleBackDialog::on_edgeStyleBtn_clicked()
{
    m_edgeStyle->buttonClicked();
    m_scaleBack->m_edgeLine.m_lineStyle = (lineStyle)m_edgeStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CScaleBackDialog::on_color1Btn_clicked()
{
    m_color1->setParentDialog(this);
    m_color1->buttonClicked();
    m_scaleBack->m_backColor.m_color1 = m_color1->m_backColor;
    ((QWidget*)parent())->update();
}

void CScaleBackDialog::on_color2Btn_clicked()
{
    m_color2->setParentDialog(this);
    m_color2->buttonClicked();
    m_scaleBack->m_backColor.m_color2 = m_color2->m_backColor;
    ((QWidget*)parent())->update();
}

void CScaleBackDialog::on_gradStyleBtn_clicked()
{
    m_gradient->buttonClicked();
    m_scaleBack->m_backColor.m_gradientStyle = (gradientStyle)m_gradient->getIndex();
     ((QWidget*)parent())->update();
}

void CScaleBackDialog::widWarning()
{
    Ccommon::editDoubleWarning(ui->widEdit,WARN_1_25F,25,1);
}

void CScaleBackDialog::radiusWarning()
{
    Ccommon::editDoubleWarning(ui->radiusEdit,WARN_1_50F,50,1);
}

void CScaleBackDialog::edgeWarning()
{
    Ccommon::editIntWarning(ui->edgeWidEdit,WARN_1_10I,10,1);
}
