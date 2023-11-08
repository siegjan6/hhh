#include <QMessageBox>
#include "LedDialog.h"
#include "ui_LedDialog.h"
#include "ColorDialog.h"
#include "Ccommon.h"
#include "MDRTDBManagerUI/IMDRTDBManagerUI.h"
#include "MDMultiLanguage.h"
#include "CMessage.h"
#include "IStudioInterface.h"

CLedDialog::CLedDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLedDialog)
{
    ui->setupUi(this);
    m_environment = NULL;
    m_led = NULL;
    m_color = NULL;
    m_x = 50;
    m_y = 75;
    this->initDialog();
    QStringList types;
    types << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}

CLedDialog::~CLedDialog()
{
    delete m_color;
    delete ui;
}

void CLedDialog::initDialog()
{
    this->move(190,0);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->leftEdit->installEventFilter(this);
    ui->topEdit->installEventFilter(this);
    ui->decimalsEdit->installEventFilter(this);
    ui->sizeEdit->installEventFilter(this);
    QObject::connect(ui->leftSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setLeft(qint32)));
    QObject::connect(ui->topSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setTop(qint32)));
    QObject::connect(ui->decimalsSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setDecimals(qint32)));
    QObject::connect(ui->sizeSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setSize(qint32)));

    QObject::connect(ui->leftEdit,SIGNAL(editingFinished()),this,SLOT(leftWarning()));
    QObject::connect(ui->topEdit,SIGNAL(editingFinished()),this,SLOT(topWarning()));
    QObject::connect(ui->decimalsEdit,SIGNAL(editingFinished()),this,SLOT(decimalsWarning()));
    QObject::connect(ui->sizeEdit,SIGNAL(editingFinished()),this,SLOT(sizeWarning()));
    QObject::connect(ui->expEdit,SIGNAL(textChanged(QString)),this,SLOT(setExpressionValue(QString)));
}
/*!
\section 将各个属性的值显示在配置界面上。
*/
void CLedDialog::setData(CData *data)
{
    m_led = (CDataLed*)data;
    m_color = new CPaintButton;
    m_color->m_button = ui->colorBtn;
    m_color->setButtonColor(ui->colorBtn,m_led->m_color,ptColor);
    QPointF point = m_led->m_pos;
    Ccommon::setEdit(ui->leftEdit,point.x());
    Ccommon::setEdit(ui->topEdit,point.y());
    Ccommon::setEdit(ui->decimalsEdit,m_led->getDecimals());
    Ccommon::setEdit(ui->sizeEdit,m_led->getSize());
    Ccommon::setSlider(ui->leftSlider,point.x()*10);
    Ccommon::setSlider(ui->topSlider,point.y()*10);
    Ccommon::setSlider(ui->decimalsSlider,m_led->getDecimals());
    Ccommon::setSlider(ui->sizeSlider,m_led->getSize()*10);
    ui->expEdit->setText(m_led->m_expression);
}

void CLedDialog::setEnvironment(IDrawEnvrionment *value)
{
    m_environment = value;
}

void CLedDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(0,0,0,100));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(ui->colorBtn->x()+7,ui->colorBtn->y()+ui->groupBox_2->y()-2,
                     ui->colorBtn->width()+3,ui->colorBtn->height()+3);
}
/*!
\section 向编辑框中输入数值对LED数字进行配置。
*/
bool CLedDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->leftEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->leftEdit->text();
                 qint32 left = qint32(str.toDouble()*10);
                 ui->leftSlider->setValue(left);
                 m_led->m_pos = QPointF(str.toDouble(),m_y);
             }
        }
    if (watched == ui->topEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->topEdit->text();
                 qint32 top = qint32(str.toDouble()* 10);
                 ui->topSlider->setValue(top);
                 m_led->m_pos = QPointF(m_x,str.toDouble());
             }
        }
    if (watched == ui->decimalsEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->decimalsEdit->text();
                 qint32 decimals =str.toLongLong();
                 ui->decimalsSlider->setValue(decimals);
                 m_led->setDecimals(decimals);
             }
        }

    if (watched == ui->sizeEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->sizeEdit->text();
                 qint32 size = qint32(str.toDouble()* 10);
                 ui->sizeSlider->setValue(size);
                 m_led->setSize(str.toDouble());
             }
        }
     return QWidget::eventFilter(watched,event);

}
/*!
\section 设置LED数字的位置。
*/
void CLedDialog::setLeft(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->leftEdit->setText(str);
    m_x = ui->leftEdit->text().toDouble();
    m_led->m_pos = QPointF(m_x,m_y);
    ((QWidget*)parent())->update();
}

void CLedDialog::setTop(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->topEdit->setText(str);
    m_y = ui->topEdit->text().toDouble();
    m_led->m_pos = QPointF(m_x,m_y);
    ((QWidget*)parent())->update();
}
/*!
\section 设置LED数字的显示精度。
*/
void CLedDialog::setDecimals(qint32 value)
{
    QString str = QString::number(value);
    ui->decimalsEdit->setText(str);
    m_led->setDecimals(ui->decimalsEdit->text().toLongLong());
    ((QWidget*)parent())->update();
}
/*!
\section 设置LED数字的显示大小。
*/
void CLedDialog::setSize(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->sizeEdit->setText(str);
    m_led->setSize(ui->sizeEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CLedDialog::on_colorBtn_clicked()
{
    m_color->setParentDialog(this);
    m_color->buttonClicked();
    m_led->m_color = m_color->getColor();
    ((QWidget*)parent())->update();
}

/*!
\section 设置LED数字的显示所绑定的表达式。
*/
void CLedDialog::setExpressionValue(QString str)
{
    ui->expEdit->setText(str);
    m_led->m_expression = str;
}

void CLedDialog::leftWarning()
{
    Ccommon::editDoubleWarning(ui->leftEdit,WARN_0_100F,100,0);
}

void CLedDialog::topWarning()
{
    Ccommon::editDoubleWarning(ui->topEdit,WARN_0_100F,100,0);
}

void CLedDialog::decimalsWarning()
{
     Ccommon::editIntWarning(ui->decimalsEdit,WARN_0_6I,6,0);
}

void CLedDialog::sizeWarning()
{
    Ccommon::editDoubleWarning(ui->sizeEdit,WARN_5_20F,20,5);
}

void CLedDialog::on_expBtn_clicked()
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
          m_led->m_expression = var;
      }
}
