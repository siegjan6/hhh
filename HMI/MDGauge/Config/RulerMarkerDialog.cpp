#include <QMessageBox>
#include "RulerMarkerDialog.h"
#include "ui_RulerMarkerDialog.h"
#include "StyleDialog.h"
#include "ColorDialog.h"
#include "Ccommon.h"
#include "MDRTDBManagerUI/IMDRTDBManagerUI.h"
#include "MDMultiLanguage.h"
#include "CMessage.h"
#include "IStudioInterface.h"

CRulerMarkerDialog::CRulerMarkerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRulerMarkerDialog)
{
    ui->setupUi(this);
    m_environment = NULL;
    m_marker = NULL;
    m_markStyle = NULL;
    m_markArrange = NULL;
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

CRulerMarkerDialog::~CRulerMarkerDialog()
{
    delete m_markStyle;
    delete m_markArrange;
    delete m_edgeStyle;
    delete m_edgeColor;
    delete m_gradient;
    delete m_color1;
    delete m_color2;
    delete ui;
}

void CRulerMarkerDialog::initDialog()
{
    this->move(190,10);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    QObject::connect(ui->rWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setMarkerWid(qint32)));
    QObject::connect(ui->rLenSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setLength(qint32)));
    QObject::connect(ui->rOffsetSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setOffset(qint32)));
    QObject::connect(ui->rStepSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setStep(qint32)));
    QObject::connect(ui->redgeWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setEdgeWide(qint32)));

    QObject::connect(ui->rWidEdit,SIGNAL(editingFinished()),this,SLOT(widWarning()));
    QObject::connect(ui->rLenEdit,SIGNAL(editingFinished()),this,SLOT(lenWarning()));
    QObject::connect(ui->rOffsetEdit,SIGNAL(editingFinished()),this,SLOT(offsetWarning()));
    QObject::connect(ui->rStepEdit,SIGNAL(editingFinished()),this,SLOT(stepWarning()));
    QObject::connect(ui->redgeWidEdit,SIGNAL(editingFinished()),this,SLOT(edgeWarning()));
     QObject::connect(ui->rExpEdit,SIGNAL(textChanged(QString)),this,SLOT(setExpValue(QString)));
}
/*!
\section 在各个按钮外绘制矩形框。
*/
void CRulerMarkerDialog::paintEvent(QPaintEvent *)
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
\section 向编辑框中输入数值对直表指示块属性进行配置。
*/
bool CRulerMarkerDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->rWidEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->rWidEdit->text();
                 qint32 wid =qint32(str.toDouble()* 10);
                 ui->rWidSlider->setValue(wid);
                 m_marker->setWidth(str.toDouble());
             }
        }
    if (watched == ui->rLenEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->rLenEdit->text();
                 qint32 len =qint32(str.toDouble()* 10);
                 ui->rLenSlider->setValue(len);
                 m_marker->setOffset(str.toDouble());
             }
        }
    if (watched == ui->rOffsetEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->rOffsetEdit->text();
                 qint32 offset =qint32(str.toDouble()* 10);
                 ui->rOffsetSlider->setValue(offset);
                 m_marker->setOffset(str.toDouble());
             }
        }
    if (watched == ui->rStepEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->rStepEdit->text();
                 qint32 step = str.toInt();
                 ui->rStepSlider->setValue(step);
                 m_marker->setStep(step);
             }
        }
    if (watched == ui->redgeWidEdit)
        {
             if (event->type()==QEvent::FocusOut)
             {
                 QString str = ui->redgeWidEdit->text();
                 qint32 edge = str.toInt();
                 ui->redgeWidSlider->setValue(edge);
                 m_marker->m_edgeLine.setWidth(edge);
             }
        }
     return QWidget::eventFilter(watched,event);
}

/*!
\section 将各个属性的值显示在配置界面上。
*/
void CRulerMarkerDialog::setData(CData *data)
{
    m_marker = (CRulerMarker*)data;
    m_markStyle = new CPaintButton;
    m_markStyle->m_button = ui->rMarkerStyBtn;
    m_markStyle->setButtonImage(ui->rMarkerStyBtn,stRMarkerStyle,
                                m_marker->m_style,ptImage);
    m_markArrange = new CPaintButton;
    m_markArrange->m_button = ui->rMarkerarrange;
    m_markArrange->setButtonImage(ui->rMarkerarrange,stRMarkerArrangement,
                                  m_marker->m_arrangement,ptImage);
     m_edgeStyle = new CPaintButton;
     m_edgeStyle->m_button = ui->redgeStyleBtn;
     m_edgeStyle->setButtonImage(ui->redgeStyleBtn,stLine,
                                 m_marker->m_edgeLine.m_lineStyle,ptImage);
     m_edgeColor = new CPaintButton;
     m_edgeColor->m_button = ui->redgeColorBtn;
     m_edgeColor->setButtonColor(ui->redgeColorBtn,m_marker->m_edgeLine.m_color,ptColor);
     m_gradient = new CPaintButton;
     m_gradient->m_button = ui->rgradStyleBtn;
     m_gradient->setButtonImage(ui->rgradStyleBtn,stGradient,
                                m_marker->m_backColor.m_gradientStyle,ptImage);
     m_color1 = new CPaintButton;
     m_color1->m_button = ui->rcolor1Btn;
     m_color1->setButtonColor(ui->rcolor1Btn,m_marker->m_backColor.m_color1,ptColor);
     m_color2 = new CPaintButton;
     m_color2->m_button = ui->rcolor2Btn;
     m_color2->setButtonColor(ui->rcolor2Btn,m_marker->m_backColor.m_color2,ptColor);
    Ccommon::setEdit(ui->rWidEdit,m_marker->getWidth());
    Ccommon::setEdit(ui->rLenEdit,m_marker->getLen());
    Ccommon::setEdit(ui->rOffsetEdit,m_marker->getOffset());
    Ccommon::setEdit(ui->rStepEdit,m_marker->getStep());
    Ccommon::setEdit(ui->redgeWidEdit,m_marker->m_edgeLine.getWidth());
    Ccommon::setSlider(ui->rWidSlider,m_marker->getWidth()*10);
    Ccommon::setSlider(ui->rLenSlider,m_marker->getLen()*10);
    Ccommon::setSlider(ui->rOffsetSlider,m_marker->getOffset()*10);
    Ccommon::setSlider(ui->rStepSlider,m_marker->getStep());
    Ccommon::setSlider(ui->redgeWidSlider,m_marker->m_edgeLine.getWidth());
     ui->rExpEdit->setText(m_marker->m_expression);
}

void CRulerMarkerDialog::setEnvrionment(IDrawEnvrionment *value)
{
    m_environment = value;
}

void CRulerMarkerDialog::setMarkerWid(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->rWidEdit->setText(str);
    m_marker->setWidth(ui->rWidEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerMarkerDialog::setLength(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->rLenEdit->setText(str);
    m_marker->setLen(ui->rLenEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerMarkerDialog::setOffset(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(abs(value % 10));
    str.append(str1);
    ui->rOffsetEdit->setText(str);
    m_marker->setOffset(ui->rOffsetEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerMarkerDialog::setStep(qint32 value)
{
    QString str = QString::number(value);
    ui->rStepEdit->setText(str);
    m_marker->setStep(ui->rStepEdit->text().toInt());
    ((QWidget*)parent())->update();
}

void CRulerMarkerDialog::setEdgeWide(qint32 value)
{
    QString str = QString::number(value);
    ui->redgeWidEdit->setText(str);
    m_marker->m_edgeLine.setWidth(ui->redgeWidEdit->text().toInt());
    ((QWidget*)parent())->update();
}
/*!
\section 输入数值超限给出警告信息。
*/
void CRulerMarkerDialog::widWarning()
{
    Ccommon::editDoubleWarning(ui->rWidEdit,WARN_1_25F,25,1);
}

void CRulerMarkerDialog::lenWarning()
{
    Ccommon::editDoubleWarning(ui->rLenEdit,WARN_1_100F,100,1);
}

void CRulerMarkerDialog::offsetWarning()
{
    Ccommon::editDoubleWarning(ui->rOffsetEdit,WARN__55_55F,55,-55);
}

void CRulerMarkerDialog::stepWarning()
{
    Ccommon::editIntWarning(ui->rStepEdit,WARN_0_10I,10,0);
}

void CRulerMarkerDialog::edgeWarning()
{
    Ccommon::editIntWarning(ui->redgeWidEdit,WARN_1_10I,10,1);
}

void CRulerMarkerDialog::on_rMarkerStyBtn_clicked()
{
    m_markStyle->buttonClicked();
    m_marker->m_style = m_markStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerMarkerDialog::on_rMarkerarrange_clicked()
{
    m_markArrange->buttonClicked();
    m_marker->m_arrangement = m_markArrange->getIndex();
     ((QWidget*)parent())->update();

}

void CRulerMarkerDialog::on_redgeStyleBtn_clicked()
{
    m_edgeStyle->buttonClicked();
    m_marker->m_edgeLine.m_lineStyle = (lineStyle)m_edgeStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerMarkerDialog::on_rgradStyleBtn_clicked()
{
    m_gradient->buttonClicked();
    m_marker->m_backColor.m_gradientStyle = (gradientStyle)m_gradient->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerMarkerDialog::on_redgeColorBtn_clicked()
{
    m_edgeColor->setParentDialog(this);
    m_edgeColor->buttonClicked();
    m_marker->m_edgeLine.m_color = m_edgeColor->m_backColor;
    ((QWidget*)parent())->update();
}

void CRulerMarkerDialog::on_rcolor1Btn_clicked()
{
    m_color1->setParentDialog(this);
    m_color1->buttonClicked();
    m_marker->m_backColor.m_color1 = m_color1->m_backColor;
    ((QWidget*)parent())->update();
}

void CRulerMarkerDialog::on_rcolor2Btn_clicked()
{
    m_color2->setParentDialog(this);
    m_color2->buttonClicked();
    m_marker->m_backColor.m_color2 = m_color2->m_backColor;
    ((QWidget*)parent())->update();
}
//!  绑定的变量表达式
void CRulerMarkerDialog::setExpValue(QString str)
{
    ui->rExpEdit->setText(str);
    m_marker->m_expression = str;
}

void CRulerMarkerDialog::on_rExpBtn_clicked()
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
          m_marker->m_expression = var;
      }
}
