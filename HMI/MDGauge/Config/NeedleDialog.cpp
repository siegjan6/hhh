#include <QMessageBox>
#include "NeedleDialog.h"
#include "ui_NeedleDialog.h"
#include "ColorDialog.h"
#include "StyleDialog.h"
#include "Ccommon.h"
#include "DrawGauge.h"
#include "MDRTDBManagerUI/IMDRTDBManagerUI.h"
#include "MDMultiLanguage.h"
#include "CMessage.h"
#include "IStudioInterface.h"

CNeedleDialog::CNeedleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CNeedleDialog)
{
    ui->setupUi(this);
    m_environment = NULL;
    m_needleStyle = NULL;
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

CNeedleDialog::~CNeedleDialog()
{
    delete m_needleStyle;
    delete m_edgeStyle;
    delete m_edgeColor;
    delete m_gradient ;
    delete m_color1 ;
    delete m_color2;
    delete ui;
}

void CNeedleDialog::initDialog()
{
    this->move(190,10);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->widEdit->installEventFilter(this);
    ui->lenEdit->installEventFilter(this);
    ui->radiusEdit->installEventFilter(this);
    ui->stepEdit->installEventFilter(this);
    ui->edgeWidEdit->installEventFilter(this);
    QObject::connect(ui->widSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setNeedleWidth(qint32)));
    QObject::connect(ui->lenSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setLength(qint32)));
    QObject::connect(ui->radiusSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setRadius(qint32)));
    QObject::connect(ui->stepSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setStep(qint32)));
    QObject::connect(ui->edgeWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setEdgeWide(qint32)));

    QObject::connect(ui->widEdit,SIGNAL(editingFinished()),this,SLOT(widthWarning()));
    QObject::connect(ui->lenEdit,SIGNAL(editingFinished()),this,SLOT(lengthWarning()));
    QObject::connect(ui->radiusEdit,SIGNAL(editingFinished()),this,SLOT(radiusWarning()));
    QObject::connect(ui->stepEdit,SIGNAL(editingFinished()),this,SLOT(stepWarning()));
    QObject::connect(ui->edgeWidEdit,SIGNAL(editingFinished()),this,SLOT(edgeWarning()));
    QObject::connect(ui->expEdit,SIGNAL(textChanged(QString)),this,SLOT(setExpressionValue(QString)));

}
/*!
\section 绘制矩形框。
*/
void CNeedleDialog::paintEvent(QPaintEvent *)
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
/*!
\section 将各个属性的值显示在配置界面上。
*/
void CNeedleDialog::setData(CData *data)
{
    m_needle = (CDataNeedle*)data;
    m_needleStyle = new CPaintButton;
    m_needleStyle->m_button = ui->needleStyBtn;
    m_needleStyle->setButtonImage(ui->needleStyBtn,stNeedle,m_needle->m_style,ptImage);
     m_edgeStyle = new CPaintButton;
     m_edgeStyle->m_button = ui->edgeStyleBtn;
     m_edgeStyle->setButtonImage(ui->edgeStyleBtn,stLine,
                                 m_needle->m_edgeLine.m_lineStyle,ptImage);
     m_edgeColor = new CPaintButton;
     m_edgeColor->m_button = ui->edgeColorBtn;
     m_edgeColor->setButtonColor(ui->edgeColorBtn,m_needle->m_edgeLine.m_color,ptColor);
     m_gradient = new CPaintButton;
     m_gradient->m_button = ui->gradStyleBtn;
     m_gradient->setButtonImage(ui->gradStyleBtn,stGradient,
                                m_needle->m_backColor.m_gradientStyle,ptImage);
     m_color1 = new CPaintButton;
     m_color1->m_button = ui->color1Btn;
     m_color1->setButtonColor(ui->color1Btn,m_needle->m_backColor.m_color1,ptColor);
     m_color2 = new CPaintButton;
     m_color2->m_button = ui->color2Btn;
     m_color2->setButtonColor(ui->color2Btn,m_needle->m_backColor.m_color2,ptColor);
    Ccommon::setEdit(ui->widEdit,m_needle->getWidth());
    Ccommon::setEdit(ui->lenEdit,m_needle->getLen());
    Ccommon::setEdit(ui->radiusEdit,m_needle->getRadius());
    Ccommon::setEdit(ui->stepEdit,m_needle->getStep());
    Ccommon::setEdit(ui->edgeWidEdit,m_needle->m_edgeLine.getWidth());

    Ccommon::setSlider(ui->widSlider,m_needle->getWidth()*10);
    Ccommon::setSlider(ui->lenSlider,m_needle->getLen()*10);
    Ccommon::setSlider(ui->radiusSlider,m_needle->getRadius()*10);
    Ccommon::setSlider(ui->stepSlider,m_needle->getStep());
    Ccommon::setSlider(ui->edgeWidSlider,m_needle->m_edgeLine.getWidth());
    ui->expEdit->setText(m_needle->m_expression);
}

void CNeedleDialog::setEnvrionment(IDrawEnvrionment *value)
{
    m_environment = value;
}

/*!
\section 向编辑框中输入数值对指针属性进行配置。
*/
bool CNeedleDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->widEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->widEdit->text();
                 qint32 wid = (qint32)(str.toDouble()*10);
                 ui->widSlider->setValue(wid);
                 m_needle->setWidth(str.toDouble());
             }
        }
    if (watched == ui->lenEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->lenEdit->text();
                 qint32 len = (qint32)(str.toDouble()*10);
                 ui->lenSlider->setValue(len);
                 m_needle->setLen(str.toDouble());
             }
        }

    if (watched == ui->radiusEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->radiusEdit->text();
                 qint32 radius =qint32(str.toDouble()* 10);
                 ui->radiusSlider->setValue(radius);
                 m_needle->setRadius(str.toDouble());
             }
        }
    if (watched == ui->stepEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->stepEdit->text();
                 qint32 step = str.toInt();
                 ui->stepSlider->setValue(step);
                 m_needle->setStep(str.toInt());
             }
        }
    if (watched == ui->edgeWidEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->edgeWidEdit->text();
                 qint32 edge = str.toInt();
                 ui->edgeWidSlider->setValue(edge);
                 m_needle->m_edgeLine.setWidth(edge);
             }
        }

    return QWidget::eventFilter(watched,event);
}

void CNeedleDialog::setNeedleWidth(qint32 value)
{
    QString ret = QString("%1").arg(value / 10);
    ret.append(".");
    QString ret1 = QString("%1").arg(value % 10);
    ret.append(ret1);
    ui->widEdit->setText(ret);
    m_needle->setWidth(ui->widEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CNeedleDialog::setLength(qint32 value)
{
    QString ret = QString("%1").arg(value / 10);
    ret.append(".");
    QString ret1 = QString("%1").arg(value % 10);
    ret.append(ret1);
    ui->lenEdit->setText(ret);
    m_needle->setLen(ui->lenEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CNeedleDialog::setRadius(qint32 value)
{
    QString ret = QString("%1").arg(value / 10);
    ret.append(".");
    QString ret1 = QString("%1").arg(abs(value % 10));
    ret.append(ret1);
    ui->radiusEdit->setText(ret);
    m_needle->setRadius(ui->radiusEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CNeedleDialog::setStep(qint32 value)
{
    QString ret = QString::number(value);
    ui->stepEdit->setText(ret);
    m_needle->setStep(ui->stepEdit->text().toInt());
    ((QWidget*)parent())->update();
}

void CNeedleDialog::setEdgeWide(qint32 value)
{
    QString ret = QString::number(value);
    ui->edgeWidEdit->setText(ret);
    m_needle->m_edgeLine.setWidth(ui->edgeWidEdit->text().toInt());
    ((QWidget*)parent())->update();
}
/*!
\section 超出限定值给出警告。
*/
void CNeedleDialog::widthWarning()
{
    Ccommon::editDoubleWarning(ui->widEdit,WARN_1_25F,25,1);
}

void CNeedleDialog::lengthWarning()
{
    Ccommon::editDoubleWarning(ui->lenEdit,WARN_1_100F,100,1);
}

void CNeedleDialog::radiusWarning()
{
    Ccommon::editDoubleWarning(ui->radiusEdit,WARN__55_55F,55,-55);
}

void CNeedleDialog::stepWarning()
{
    Ccommon::editIntWarning(ui->stepEdit,WARN_0_10I,10,0);
}

void CNeedleDialog::edgeWarning()
{
    Ccommon::editIntWarning(ui->edgeWidEdit,WARN_1_10I,10,1);
}

void CNeedleDialog::on_edgeColorBtn_clicked()
{
    m_edgeColor->setParentDialog(this);
    m_edgeColor->buttonClicked();
    m_needle->m_edgeLine.m_color = m_edgeColor->m_backColor;
    ((QWidget*)parent())->update();
}

void CNeedleDialog::on_edgeStyleBtn_clicked()
{
    m_edgeStyle->buttonClicked();
    m_needle->m_edgeLine.m_lineStyle = (lineStyle)m_edgeStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CNeedleDialog::on_gradStyleBtn_clicked()
{
    m_gradient->buttonClicked();
    m_needle->m_backColor.m_gradientStyle = (gradientStyle)m_gradient->getIndex();
     ((QWidget*)parent())->update();
}

void CNeedleDialog::on_needleStyBtn_clicked()
{
    m_needleStyle->buttonClicked();
    m_needle->m_style = m_needleStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CNeedleDialog::on_color1Btn_clicked()
{
    m_color1->setParentDialog(this);
    m_color1->buttonClicked();
    m_needle->m_backColor.m_color1 = m_color1->m_backColor;
    ((QWidget*)parent())->update();
}

void CNeedleDialog::on_color2Btn_clicked()
{
    m_color2->setParentDialog(this);
    m_color2->buttonClicked();
    m_needle->m_backColor.m_color2 = m_color2->m_backColor;
    ((QWidget*)parent())->update();
}
/*!
\section 指针绑定的变量表达式。
*/
void CNeedleDialog::setExpressionValue(QString str)
{
    ui->expEdit->setText(str);
    m_needle->m_expression = str;
}

void CNeedleDialog::on_expBtn_clicked()
{
    IMDRTDBManagerUI* varUI = m_environment->studioProject()->rtdbManagerUI();
    QStringList varNames;
    bool result = varUI->selectVariables(varNames);

    if (result)
    {
        QString var;
        if (varNames.count() > 0)
            var = varNames.at(0);
        ui->expEdit->setText(var);
        m_needle->m_expression = var;
    }
}
