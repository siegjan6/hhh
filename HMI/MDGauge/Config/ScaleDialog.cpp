#include <QMessageBox>
#include "ScaleDialog.h"
#include "ui_ScaleDialog.h"
#include "StyleDialog.h"
#include "ColorDialog.h"
#include "Ccommon.h"
#include "MDMultiLanguage.h"
#include "CMessage.h"

CScaleDialog::CScaleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CScaleDialog)
{
    ui->setupUi(this);
    m_scaleStyle = NULL;
    m_scaleArrange = NULL;
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

CScaleDialog::~CScaleDialog()
{
    delete m_scaleStyle;
    delete m_scaleArrange;
    delete m_edgeStyle;
    delete m_edgeColor;
    delete m_gradient;
    delete m_color1;
    delete m_color2;
    delete ui;
}

void CScaleDialog::initDialog()
{
    this->move(190,0);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->checkBox->setChecked(false);
    ui->subScaleEdit->setDisabled(true);
    ui->subScaleSlider->setDisabled(true);

    ui->widEdit->installEventFilter(this);
    ui->lenEdit->installEventFilter(this);
    ui->radiusEdit->installEventFilter(this);
    ui->subScaleEdit->installEventFilter(this);
    ui->edgeWidEdit->installEventFilter(this);

    QObject::connect(ui->widSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setWid(qint32)));
    QObject::connect(ui->lenSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setLen(qint32)));
    QObject::connect(ui->radiusSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setRadius(qint32)));
    QObject::connect(ui->subScaleSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setSubScale(qint32)));
    QObject::connect(ui->edgeWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setEdgeWid(qint32)));
    QObject::connect(ui->checkBox,SIGNAL(stateChanged(qint32)),this,SLOT(setSub(qint32)));

    QObject::connect(ui->widEdit,SIGNAL(editingFinished()),this,SLOT(widWarning()));
    QObject::connect(ui->lenEdit,SIGNAL(editingFinished()),this,SLOT(lenWarning()));
    QObject::connect(ui->radiusEdit,SIGNAL(editingFinished()),this,SLOT(radiusWarning()));
    QObject::connect(ui->subScaleEdit,SIGNAL(editingFinished()),this,SLOT(subScaleWarning()));
    QObject::connect(ui->edgeWidEdit,SIGNAL(editingFinished()),this,SLOT(edgeWidWarning()));
}

void CScaleDialog::paintEvent(QPaintEvent *)
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
void CScaleDialog::setData(CData *data)
{
    m_scale = (CDataScale*)data;
    m_scaleStyle = new CPaintButton;
    m_scaleStyle->m_button = ui->scaleStyleBtn;
    m_scaleStyle->setButtonImage(ui->scaleStyleBtn,stScaleStyle,
                                 m_scale->m_style,ptImage);
    m_scaleArrange = new CPaintButton;
    m_scaleArrange->m_button = ui->layStyleBtn;
    m_scaleArrange->setButtonImage(ui->layStyleBtn,stScaleArrangement,
                                   m_scale->m_arrangement,ptImage);
    m_edgeStyle = new CPaintButton;
    m_edgeStyle->m_button = ui->edgeStyleBtn;
    m_edgeStyle->setButtonImage(ui->edgeStyleBtn,stLine,
                                m_scale->m_edgeLine.m_lineStyle,ptImage);
    m_edgeColor = new CPaintButton;
    m_edgeColor->m_button = ui->edgeColorBtn;
    m_edgeColor->setButtonColor(ui->edgeColorBtn,m_scale->m_edgeLine.m_color,ptColor);
    m_gradient = new CPaintButton;
    m_gradient->m_button = ui->gradStyleBtn;
    m_gradient->setButtonImage(ui->gradStyleBtn,stGradient,
                               m_scale->m_backColor.m_gradientStyle,ptImage);
    m_color1 = new CPaintButton;
    m_color1->m_button = ui->color1Btn;
    m_color1->setButtonColor(ui->color1Btn,m_scale->m_backColor.m_color1,ptColor);
    m_color2 = new CPaintButton;
    m_color2->m_button = ui->color2Btn;
    m_color2->setButtonColor(ui->color2Btn,m_scale->m_backColor.m_color2,ptColor);

    Ccommon::setEdit(ui->radiusEdit,m_scale->getWidth());
    Ccommon::setEdit(ui->lenEdit,m_scale->getLen());
    Ccommon::setEdit(ui->radiusEdit,m_scale->getRadius());
    Ccommon::setEdit(ui->edgeWidEdit,m_scale->m_edgeLine.getWidth());
    Ccommon::setSlider(ui->widSlider,m_scale->getWidth()*10);
    Ccommon::setSlider(ui->lenSlider,m_scale->getLen()*10);
    Ccommon::setSlider(ui->radiusSlider,m_scale->getRadius()*10);
    Ccommon::setSlider(ui->edgeWidSlider,m_scale->m_edgeLine.getWidth());
    QString str;
    if(m_scale->m_isSubScale ==  false)
    {
        ui->subScaleEdit->setDisabled(true);
        ui->subScaleSlider->setDisabled(true);
    }
    else
    {
        ui->checkBox->setChecked(true);
        ui->subScaleEdit->setDisabled(false);
        ui->subScaleSlider->setDisabled(false);
        ui->subScaleSlider->setValue(m_scale->getSubCount());
        str.setNum(m_scale->getSubCount());
        ui->subScaleEdit->setText(str);
    }
}



bool CScaleDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->widEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->widEdit->text();
                 qint32 wid = (qint32)(str.toDouble()*10);
                 ui->widSlider->setValue(wid);
                 m_scale->setWidth(str.toDouble());
             }
        }
    if (watched == ui->lenEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->lenEdit->text();
                 qint32 len = (qint32)(str.toDouble()*10);
                 ui->lenSlider->setValue(len);
                 m_scale->setLen(str.toDouble());
             }
        }
    if (watched == ui->radiusEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->radiusEdit->text();
                 qint32 radius = (qint32)(str.toDouble()*10);
                 ui->radiusSlider->setValue(radius);
                 m_scale->setRadius(str.toDouble());
             }
        }
    if (watched == ui->subScaleEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->subScaleEdit->text();
                 qint32 sub = str.toInt();
                 ui->subScaleSlider->setValue(sub);
                 m_scale->setSubCount(sub);
             }
        }
    if (watched == ui->edgeWidEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->edgeWidEdit->text();
                 qint32 edge = str.toInt();
                 ui->edgeWidSlider->setValue(edge);
                 m_scale->m_edgeLine.setWidth(edge);
             }
        }
     return QWidget::eventFilter(watched,event);
}

void CScaleDialog::on_scaleStyleBtn_clicked()
{
    m_scaleStyle->buttonClicked();
    m_scale->m_style = m_scaleStyle->m_index;
     ((QWidget*)parent())->update();
}

void CScaleDialog::on_layStyleBtn_clicked()
{
    m_scaleArrange->buttonClicked();
    m_scale->m_arrangement = m_scaleArrange->m_index;
     ((QWidget*)parent())->update();
}

void CScaleDialog::setWid(qint32 value)
{
    QString ret = QString("%1").arg(value / 10);
    ret.append(".");
    QString ret1 = QString("%1").arg(value % 10);
    ret.append(ret1);
    ui->widEdit->setText(ret);
    m_scale->setWidth(ui->widEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CScaleDialog::setLen(qint32 value)
{
    QString ret = QString("%1").arg(value / 10);
    ret.append(".");
    QString ret1 = QString("%1").arg(value % 10);
    ret.append(ret1);
    ui->lenEdit->setText(ret);
    m_scale->setLen(ui->lenEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CScaleDialog::setRadius(qint32 value)
{
    QString ret = QString("%1").arg(value / 10);
    ret.append(".");
    QString ret1 = QString("%1").arg(value % 10);
    ret.append(ret1);
    ui->radiusEdit->setText(ret);
    m_scale->setRadius(ui->radiusEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CScaleDialog::setSubScale(qint32 value)
{
    QString ret = QString::number(value);
    ui->subScaleEdit->setText(ret);
    m_scale->setSubCount(ui->subScaleEdit->text().toInt());
    ((QWidget*)parent())->update();
}

void CScaleDialog::setEdgeWid(qint32 value)
{
    QString ret = QString::number(value);
    ui->edgeWidEdit->setText(ret);
    m_scale->m_edgeLine.setWidth(ui->edgeWidEdit->text().toInt());
    ((QWidget*)parent())->update();
}

void CScaleDialog::setSub(qint32 state)
{
    if(state == 0)
    {
        m_scale->m_isSubScale = false;
        ui->subScaleEdit->setDisabled(true);
        ui->subScaleSlider->setDisabled(true);
    }
    else
    {
        m_scale->m_isSubScale = true;
        ui->subScaleEdit->setDisabled(false);
        ui->subScaleSlider->setDisabled(false);
    }
    ((QWidget*)parent())->update();
}

void CScaleDialog::widWarning()
{
    Ccommon::editDoubleWarning(ui->widEdit,WARN_1_25F,25,1);
}

void CScaleDialog::lenWarning()
{
    Ccommon::editDoubleWarning(ui->lenEdit,WARN_1_36F,36,1);
}

void CScaleDialog::radiusWarning()
{
    Ccommon::editDoubleWarning(ui->radiusEdit,WARN_10_80F,80,10);
}

void CScaleDialog::subScaleWarning()
{
    Ccommon::editIntWarning(ui->subScaleEdit,WARN_1_10I,10,1);
}

void CScaleDialog::edgeWidWarning()
{
    Ccommon::editIntWarning(ui->edgeWidEdit,WARN_1_10I,10,1);
}

void CScaleDialog::on_edgeColorBtn_clicked()
{
    m_edgeColor->setParentDialog(this);
    m_edgeColor->buttonClicked();
    m_scale->m_edgeLine.m_color = m_edgeColor->m_backColor;
    ((QWidget*)parent())->update();
}

void CScaleDialog::on_edgeStyleBtn_clicked()
{
    m_edgeStyle->buttonClicked();
    m_scale->m_edgeLine.m_lineStyle = (lineStyle)m_edgeStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CScaleDialog::on_color1Btn_clicked()
{
    m_color1->setParentDialog(this);
    m_color1->buttonClicked();
    m_scale->m_backColor.m_color1 = m_color1->m_backColor;
    ((QWidget*)parent())->update();
}

void CScaleDialog::on_color2Btn_clicked()
{
    m_color2->setParentDialog(this);
    m_color2->buttonClicked();
    m_scale->m_backColor.m_color2 = m_color2->m_backColor;
    ((QWidget*)parent())->update();
}

void CScaleDialog::on_gradStyleBtn_clicked()
{
    m_gradient->buttonClicked();
    m_scale->m_backColor.m_gradientStyle = (gradientStyle)m_gradient->getIndex();
     ((QWidget*)parent())->update();
}
