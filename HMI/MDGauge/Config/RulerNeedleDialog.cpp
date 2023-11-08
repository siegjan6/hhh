#include <QPainter>
#include <QMessageBox>
#include "RulerNeedleDialog.h"
#include "ui_RulerNeedleDialog.h"
#include "StyleDialog.h"
#include "ColorDialog.h"
#include "Ccommon.h"
#include "MDRTDBManagerUI/IMDRTDBManagerUI.h"
#include "MDMultiLanguage.h"
#include "CMessage.h"
#include "IStudioInterface.h"

CRulerNeedleDialog::CRulerNeedleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRulerNeedleDialog)
{
    ui->setupUi(this);
    m_environment = NULL;
    m_rNeedle = NULL;
    m_rneedleStyle = NULL;
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

CRulerNeedleDialog::~CRulerNeedleDialog()
{
    delete m_rneedleStyle;
    delete m_edgeStyle;
    delete m_edgeColor;
    delete m_gradient;
    delete m_color1;
    delete m_color2;
    delete ui;
}

void CRulerNeedleDialog::initDialog()
{
    this->move(190,10);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->rWidEdit->installEventFilter(this);
    ui->rOffsetEdit->installEventFilter(this);
    ui->rstepEdit->installEventFilter(this);
    ui->redgeWidEdit->installEventFilter(this);
    QObject::connect(ui->rWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setRNeedleWid(qint32)));
    QObject::connect(ui->rOffsetSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setOffset(qint32)));
    QObject::connect(ui->rstepSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setStep(qint32)));
    QObject::connect(ui->redgeWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setEdgeWide(qint32)));

    QObject::connect(ui->rWidEdit,SIGNAL(editingFinished()),this,SLOT(widWarning()));
    QObject::connect(ui->rOffsetEdit,SIGNAL(editingFinished()),this,SLOT(offsetWarning()));
    QObject::connect(ui->rstepEdit,SIGNAL(editingFinished()),this,SLOT(stepWarning()));
    QObject::connect(ui->redgeWidEdit,SIGNAL(editingFinished()),this,SLOT(edgeWarning()));
     QObject::connect(ui->rExpEdit,SIGNAL(textChanged(QString)),this,SLOT(setExpValue(QString)));
}
/*!
\section 在各个按钮外绘制矩形框。
*/
void CRulerNeedleDialog::paintEvent(QPaintEvent *)
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
/*!
\section 将各个属性的值显示在配置界面上。
*/
void CRulerNeedleDialog::setData(CData *data)
{
    m_rNeedle = (CRulerNeedle*)data;
    m_rneedleStyle = new CPaintButton;
    m_rneedleStyle->m_button = ui->rNeedleArrangeBtn;
    m_rneedleStyle->setButtonImage(ui->rNeedleArrangeBtn,stRNeedle,
                                   m_rNeedle->m_arrangement,ptImage);
     m_edgeStyle = new CPaintButton;
     m_edgeStyle->m_button = ui->redgeStyleBtn;
     m_edgeStyle->setButtonImage(ui->redgeStyleBtn,stLine,
                                 m_rNeedle->m_edgeLine.m_lineStyle,ptImage);
     m_edgeColor = new CPaintButton;
     m_edgeColor->m_button = ui->redgeColorBtn;
     m_edgeColor->setButtonColor(ui->redgeColorBtn,m_rNeedle->m_edgeLine.m_color,ptColor);
     m_gradient = new CPaintButton;
     m_gradient->m_button = ui->rgradStyleBtn;
     m_gradient->setButtonImage(ui->rgradStyleBtn,stGradient,
                                m_rNeedle->m_backColor.m_gradientStyle,ptImage);
     m_color1 = new CPaintButton;
     m_color1->m_button = ui->rcolor1Btn;
     m_color1->setButtonColor(ui->rcolor1Btn,m_rNeedle->m_backColor.m_color1,ptColor);
     m_color2 = new CPaintButton;
     m_color2->m_button = ui->rcolor2Btn;
     m_color2->setButtonColor(ui->rcolor2Btn,m_rNeedle->m_backColor.m_color2,ptColor);
    Ccommon::setEdit(ui->rWidEdit,m_rNeedle->getWidth());
    Ccommon::setEdit(ui->rOffsetEdit,m_rNeedle->getOffset());
    Ccommon::setEdit(ui->rstepEdit,m_rNeedle->getStep());
    Ccommon::setEdit(ui->redgeWidEdit,m_rNeedle->m_edgeLine.getWidth());
    Ccommon::setSlider(ui->rWidSlider,m_rNeedle->getWidth()*10);
    Ccommon::setSlider(ui->rOffsetSlider,m_rNeedle->getOffset()*10);
    Ccommon::setSlider(ui->rstepSlider,m_rNeedle->getStep());
    Ccommon::setSlider(ui->redgeWidSlider,m_rNeedle->m_edgeLine.getWidth());
    ui->rExpEdit->setText(m_rNeedle->m_expression);
}

void CRulerNeedleDialog::setEnvrionment(IDrawEnvrionment *value)
{
    m_environment  = value;
}
/*!
\section 向编辑框中输入数值对直表指针属性进行配置。
*/
bool CRulerNeedleDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->rWidEdit)
        {
             if (event->type() == QEvent::FocusOut)
             {
                 QString str = ui->rWidEdit->text();
                 qint32 wid =qint32(str.toDouble()* 10);
                 ui->rWidSlider->setValue(wid);
                 m_rNeedle->setWidth(str.toDouble());
             }
        }

    if (watched == ui->rOffsetEdit)
        {
             if (event->type() == QEvent::FocusOut)
             {
                 QString str = ui->rOffsetEdit->text();
                 qint32 offset =qint32(str.toDouble()* 10);
                 ui->rOffsetSlider->setValue(offset);
                 m_rNeedle->setOffset(str.toDouble());
             }
        }
    if (watched == ui->rstepEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->rstepEdit->text();
                 qint32 step = str.toInt();
                 ui->rstepSlider->setValue(step);
                 m_rNeedle->setStep(step);
             }
        }
    if (watched == ui->redgeWidEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->redgeWidEdit->text();
                 qint32 edge = str.toInt();
                 ui->redgeWidSlider->setValue(edge);
                 m_rNeedle->m_edgeLine.setWidth(edge);
             }
        }
     return QWidget::eventFilter(watched,event);
}

void CRulerNeedleDialog::setRNeedleWid(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->rWidEdit->setText(str);
    m_rNeedle->setWidth(ui->rWidEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerNeedleDialog::setOffset(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(abs(value % 10));
    str.append(str1);
    ui->rOffsetEdit->setText(str);
    m_rNeedle->setOffset(ui->rOffsetEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerNeedleDialog::setStep(qint32 value)
{
    QString str = QString::number(value);
    ui->rstepEdit->setText(str);
    m_rNeedle->setStep(ui->rstepEdit->text().toInt());
    ((QWidget*)parent())->update();
}

void CRulerNeedleDialog::setEdgeWide(qint32 value)
{
    QString str = QString::number(value);
    ui->redgeWidEdit->setText(str);
    m_rNeedle->m_edgeLine.setWidth(ui->redgeWidEdit->text().toInt());
    ((QWidget*)parent())->update();
}
/*!
\section 输入数值超限给出警告信息。
*/
void CRulerNeedleDialog::widWarning()
{
    Ccommon::editDoubleWarning(ui->rWidEdit,WARN_1_25F,25,1);
}

void CRulerNeedleDialog::offsetWarning()
{
    Ccommon::editDoubleWarning(ui->rOffsetEdit,WARN__55_55F,55,-55);
}

void CRulerNeedleDialog::stepWarning()
{
    Ccommon::editIntWarning(ui->rstepEdit,WARN_0_10I,10,0);
}

void CRulerNeedleDialog::edgeWarning()
{
     Ccommon::editIntWarning(ui->redgeWidEdit,WARN_1_10I,10,1);
}

void CRulerNeedleDialog::on_rNeedleArrangeBtn_clicked()
{
    m_rneedleStyle->buttonClicked();
    m_rNeedle->m_arrangement = m_rneedleStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerNeedleDialog::on_redgeStyleBtn_clicked()
{
    m_edgeStyle->buttonClicked();
    m_rNeedle->m_edgeLine.m_lineStyle = (lineStyle)m_edgeStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerNeedleDialog::on_rgradStyleBtn_clicked()
{
    m_gradient->buttonClicked();
    m_rNeedle->m_backColor.m_gradientStyle = (gradientStyle)m_gradient->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerNeedleDialog::on_redgeColorBtn_clicked()
{
    m_edgeColor->setParentDialog(this);
    m_edgeColor->buttonClicked();
    m_rNeedle->m_edgeLine.m_color = m_edgeColor->m_backColor;
    ((QWidget*)parent())->update();
}

void CRulerNeedleDialog::on_rcolor1Btn_clicked()
{
    m_color1->setParentDialog(this);
    m_color1->buttonClicked();
    m_rNeedle->m_backColor.m_color1 = m_color1->m_backColor;
    ((QWidget*)parent())->update();
}

void CRulerNeedleDialog::on_rcolor2Btn_clicked()
{
    m_color2->setParentDialog(this);
    m_color2->buttonClicked();
    m_rNeedle->m_backColor.m_color2 = m_color2->m_backColor;
    ((QWidget*)parent())->update();
}

//!  直表指针绑定的变量表达式
void CRulerNeedleDialog::setExpValue(QString str)
{
    ui->rExpEdit->setText(str);
    m_rNeedle->m_expression = str;
}

void CRulerNeedleDialog::on_rExpBtn_clicked()
{
    IMDRTDBManagerUI* varUI = m_environment->studioProject()->rtdbManagerUI();
      QStringList varNames;
      bool result = varUI->selectVariables(varNames);

      if (result)
      {
          QString var;
          if (varNames.count() > 0)
              var = varNames.at(0);
          ui->rExpEdit->setText(var);
          m_rNeedle->m_expression = var;
      }
}
