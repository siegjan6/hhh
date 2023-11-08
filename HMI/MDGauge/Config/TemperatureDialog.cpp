#include <QMessageBox>
#include "TemperatureDialog.h"
#include "ui_TemperatureDialog.h"
#include "StyleDialog.h"
#include "ColorDialog.h"
#include "Ccommon.h"
#include "MDRTDBManagerUI/IMDRTDBManagerUI.h"
#include "MDMultiLanguage.h"
#include "CMessage.h"
#include "IStudioInterface.h"

CTemperatureDialog::CTemperatureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CTemperatureDialog)
{
    ui->setupUi(this);
    m_temp = NULL;
    m_tempStyle = NULL;
    m_edgeStyle = NULL;
    m_edgeColor = NULL;
    m_gradient = NULL;
    m_color1 = NULL;
    m_color2 = NULL;
    m_tempGradient = NULL;
    m_tempColor1 = NULL;
    m_tempColor2 = NULL;
    m_tempHelp = NULL;
    m_environment = NULL;
    this->initDialog();
    QStringList types;
    types << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}

CTemperatureDialog::~CTemperatureDialog()
{
    delete m_tempStyle;
    delete m_edgeStyle;
    delete m_edgeColor;
    delete m_gradient;
    delete m_color1;
    delete m_color2;
    delete m_tempGradient;
    delete m_tempColor1;
    delete m_tempColor2;
    delete ui;
}

void CTemperatureDialog::initDialog()
{
    this->move(190,0);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->W1_Edit->installEventFilter(this);
    ui->H1_Edit->installEventFilter(this);
    ui->W2_Edit->installEventFilter(this);
    ui->H2_Edit->installEventFilter(this);
    ui->offsetEdit->installEventFilter(this);
    ui->stepEdit->installEventFilter(this);
    ui->edgeWidEdit->installEventFilter(this);

    QObject::connect(ui->W1_Slider,SIGNAL(valueChanged(qint32)),this,SLOT(setW1(qint32)));
    QObject::connect(ui->H1_Slider,SIGNAL(valueChanged(qint32)),this,SLOT(setH1(qint32)));
    QObject::connect(ui->W2_Slider,SIGNAL(valueChanged(qint32)),this,SLOT(setW2(qint32)));
    QObject::connect(ui->H2_Slider,SIGNAL(valueChanged(qint32)),this,SLOT(setH2(qint32)));
    QObject::connect(ui->offsetSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setOffset(qint32)));
    QObject::connect(ui->stepSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setStep(qint32)));
    QObject::connect(ui->edgeWidSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setEdgeWid(qint32)));

    QObject::connect(ui->W1_Edit,SIGNAL(editingFinished()),this,SLOT(w1Warning()));
    QObject::connect(ui->H1_Edit,SIGNAL(editingFinished()),this,SLOT(h1Warning()));
    QObject::connect(ui->W2_Edit,SIGNAL(editingFinished()),this,SLOT(w2Warning()));
    QObject::connect(ui->H2_Edit,SIGNAL(editingFinished()),this,SLOT(h2Warning()));
    QObject::connect(ui->offsetEdit,SIGNAL(editingFinished()),this,SLOT(offsetWarning()));
    QObject::connect(ui->stepEdit,SIGNAL(editingFinished()),this,SLOT(stepWarning()));
    QObject::connect(ui->edgeWidEdit,SIGNAL(editingFinished()),this,SLOT(edgeWidWarning()));
    QObject::connect(ui->expEdit,SIGNAL(textChanged(QString)),this,SLOT(setExpValue(QString)));
}

void CTemperatureDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(0,0,0,100));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(ui->edgeColorBtn->x()+7,ui->edgeColorBtn->y()+ui->groupBox_2->y()-2,
                     ui->edgeColorBtn->width()+3,ui->edgeColorBtn->height()+3);
    painter.drawRect(ui->tempColor1Btn->x()+7,ui->tempColor1Btn->y()+ui->groupBox_3->y()-2,
                     ui->tempColor1Btn->width()+3,ui->tempColor1Btn->height()+3);
    painter.drawRect(ui->tempColor2Btn->x()+7,ui->tempColor2Btn->y()+ui->groupBox_3->y()-2,
                     ui->tempColor2Btn->width()+3,ui->tempColor2Btn->height()+3);
    painter.drawRect(ui->bkColor1Btn->x()+7,ui->bkColor1Btn->y()+ui->groupBox_4->y()-2,
                     ui->bkColor1Btn->width()+3,ui->bkColor1Btn->height()+3);
    painter.drawRect(ui->bkColor2Btn->x()+7,ui->bkColor2Btn->y()+ui->groupBox_4->y()-2,
                     ui->bkColor2Btn->width()+3,ui->bkColor2Btn->height()+3);
}

void CTemperatureDialog::setData(CData *data)
{
    m_temp = (CDataTemperature*)data;
    m_tempStyle = new CPaintButton;
    m_tempStyle->m_button = ui->temArrangeBtn;
    m_tempStyle->setButtonImage(ui->temArrangeBtn,stTemperature,
                                m_temp->m_arrangement,ptImage);
     m_edgeStyle = new CPaintButton;
     m_edgeStyle->m_button = ui->edgeStyleBtn;
     m_edgeStyle->setButtonImage(ui->edgeStyleBtn,stLine,
                                 m_temp->m_edgeLine.m_lineStyle,ptImage);
     m_edgeColor = new CPaintButton;
     m_edgeColor->m_button = ui->edgeColorBtn;
     m_edgeColor->setButtonColor(ui->edgeColorBtn,m_temp->m_edgeLine.m_color,ptColor);
     m_gradient = new CPaintButton;
     m_gradient->m_button = ui->bkGradBtn;
     m_gradient->setButtonImage(ui->bkGradBtn,stGradient,
                                m_temp->m_backColor.m_gradientStyle,ptImage);
     m_color1 = new CPaintButton;
     m_color1->m_button = ui->bkColor1Btn;
     m_color1->setButtonColor(ui->bkColor1Btn,m_temp->m_temColor.m_color1,ptColor);
     m_color2 = new CPaintButton;
     m_color2->m_button = ui->bkColor2Btn;
     m_color2->setButtonColor(ui->bkColor2Btn,m_temp->m_temColor.m_color2,ptColor);
     m_tempGradient = new CPaintButton;
     m_tempGradient->m_button = ui->tempGradBtn;
     m_tempGradient->setButtonImage(ui->tempGradBtn,stGradient,
                                    m_temp->m_temColor.m_gradientStyle,ptImage);
     m_tempColor1 = new CPaintButton;
     m_tempColor1->m_button = ui->tempColor1Btn;
     m_tempColor1->setButtonColor(ui->tempColor1Btn,m_temp->m_backColor.m_color1,ptColor);
     m_tempColor2 = new CPaintButton;
     m_tempColor2->m_button = ui->tempColor2Btn;
     m_tempColor2->setButtonColor(ui->tempColor2Btn,m_temp->m_backColor.m_color2,ptColor);
    m_tempHelp = new CPaintButton;
    m_tempHelp->setButtonImage(ui->temHelpBtn,stTemHelp,0,ptImage);
     Ccommon::setEdit(ui->W1_Edit,m_temp->getWidth1());
    Ccommon::setEdit(ui->H1_Edit,m_temp->getHeight1());
    Ccommon::setEdit(ui->W2_Edit,m_temp->getWidth2());
    Ccommon::setEdit(ui->H2_Edit,m_temp->getHeight2());
    Ccommon::setEdit(ui->offsetEdit,m_temp->getOffset());
    Ccommon::setEdit(ui->stepEdit,m_temp->getStep());
    Ccommon::setEdit(ui->edgeWidEdit,m_temp->m_edgeLine.getWidth());
    Ccommon::setSlider(ui->W1_Slider,m_temp->getWidth1()*10);
    Ccommon::setSlider(ui->H1_Slider,m_temp->getHeight1()*10);
    Ccommon::setSlider(ui->W2_Slider,m_temp->getWidth2()*10);
    Ccommon::setSlider(ui->H2_Slider,m_temp->getHeight2()*10);
    Ccommon::setSlider(ui->offsetSlider,m_temp->getOffset()*10);
    Ccommon::setSlider(ui->stepSlider,m_temp->getStep()*10);
    Ccommon::setSlider(ui->edgeWidSlider,m_temp->m_edgeLine.getWidth());
    ui->expEdit->setText(m_temp->m_expression);

}

void CTemperatureDialog::setEnvrionment(IDrawEnvrionment *value)
{
    m_environment = value;
}

void CTemperatureDialog::on_temArrangeBtn_clicked()
{
    m_tempStyle->buttonClicked();
    m_temp->m_arrangement = m_tempStyle->m_index;
     ((QWidget*)parent())->update();
}

void CTemperatureDialog::setW1(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->W1_Edit->setText(str);
    m_temp->setWidth1(ui->W1_Edit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CTemperatureDialog::setH1(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->H1_Edit->setText(str);
    m_temp->setHeight1(ui->H1_Edit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CTemperatureDialog::setW2(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->W2_Edit->setText(str);
    m_temp->setWidth2(ui->W2_Edit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CTemperatureDialog::setH2(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->H2_Edit->setText(str);
    m_temp->setHeight2(ui->H2_Edit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CTemperatureDialog::setOffset(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(abs(value % 10));
    str.append(str1);
    ui->offsetEdit->setText(str);
    m_temp->setOffset(ui->offsetEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CTemperatureDialog::setStep(qint32 value)
{
    QString str = QString::number(value);
    ui->stepEdit->setText(str);
    m_temp->setStep(ui->stepEdit->text().toLongLong());
    ((QWidget*)parent())->update();
}

void CTemperatureDialog::setEdgeWid(qint32 value)
{
    QString str = QString::number(value);
    ui->edgeWidEdit->setText(str);
    m_temp->m_edgeLine.setWidth(ui->edgeWidEdit->text().toLongLong());
    ((QWidget*)parent())->update();
}

void CTemperatureDialog::w1Warning()
{
    Ccommon::editDoubleWarning(ui->W1_Edit,WARN_1_50F,50,1);
}

void CTemperatureDialog::h1Warning()
{
    Ccommon::editDoubleWarning(ui->H1_Edit,WARN_0_30F,30,0);
}

void CTemperatureDialog::w2Warning()
{
    Ccommon::editDoubleWarning(ui->W2_Edit,WARN_1_50F,50,1);
}

void CTemperatureDialog::h2Warning()
{
    Ccommon::editDoubleWarning(ui->H2_Edit,WARN_0_30F,30,0);
}

void CTemperatureDialog::offsetWarning()
{
    Ccommon::editDoubleWarning(ui->offsetEdit,WARN__55_55F,55,-55);
}

void CTemperatureDialog::stepWarning()
{
    Ccommon::editIntWarning(ui->stepEdit,WARN_0_10I,10,0);
}

void CTemperatureDialog::edgeWidWarning()
{
    Ccommon::editIntWarning(ui->edgeWidEdit,WARN_1_10I,10,1);
}

bool CTemperatureDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->W1_Edit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->W1_Edit->text();
                 qint32 w1 =qint32(str.toDouble()* 10);
                 ui->W1_Slider->setValue(w1);
                 m_temp->setWidth1(str.toDouble());
             }
        }
    if (watched == ui->H1_Edit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->H1_Edit->text();
                 qint32 h1 =qint32(str.toDouble()* 10);
                 ui->H1_Slider->setValue(h1);
                 m_temp->setHeight1(str.toDouble());
             }
        }
    if (watched == ui->W2_Edit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->W2_Edit->text();
                 qint32 w2 =qint32(str.toDouble()* 10);
                 ui->W2_Slider->setValue(w2);
                 m_temp->setWidth2(str.toDouble());
             }
        }
    if (watched == ui->H2_Edit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->H2_Edit->text();
                 qint32 h2 =qint32(str.toDouble()* 10);
                 ui->H2_Slider->setValue(h2);
                 m_temp->setHeight2(str.toDouble());
             }
        }
    if (watched == ui->offsetEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->offsetEdit->text();
                 qint32 off =qint32(str.toDouble()* 10);
                 ui->offsetSlider->setValue(off);
                 m_temp->setOffset(str.toDouble());
             }
        }
    if (watched == ui->stepEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->stepEdit->text();
                 qint32 step = str.toLongLong();
                 ui->stepSlider->setValue(step);
                 m_temp->setStep(step);
             }
        }
    if (watched == ui->edgeWidEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->edgeWidEdit->text();
                 qint32 edge = str.toInt();
                 ui->edgeWidSlider->setValue(edge);
                 m_temp->m_edgeLine.setWidth(edge);
             }
        }
    return QWidget::eventFilter(watched,event);
}

void CTemperatureDialog::on_edgeStyleBtn_clicked()
{
    m_edgeStyle->buttonClicked();
    m_temp->m_edgeLine.m_lineStyle = (lineStyle)m_edgeStyle->getIndex();
     ((QWidget*)parent())->update();
}

void CTemperatureDialog::on_tempGradBtn_clicked()
{
    m_tempGradient->buttonClicked();
    m_temp->m_temColor.m_gradientStyle = (gradientStyle)m_tempGradient->getIndex();
     ((QWidget*)parent())->update();
}

void CTemperatureDialog::on_bkGradBtn_clicked()
{
    m_gradient->buttonClicked();
    m_temp->m_backColor.m_gradientStyle = (gradientStyle)m_gradient->getIndex();
     ((QWidget*)parent())->update();
}

void CTemperatureDialog::on_edgeColorBtn_clicked()
{
    m_edgeColor->setParentDialog(this);
    m_edgeColor->buttonClicked();
    m_temp->m_edgeLine.m_color = m_edgeColor->m_backColor;
    ((QWidget*)parent())->update();
}

void CTemperatureDialog::on_tempColor1Btn_clicked()
{
    m_tempColor1->setParentDialog(this);
    m_tempColor1->buttonClicked();
    m_temp->m_temColor.m_color1 = m_tempColor1->m_backColor;
    ((QWidget*)parent())->update();
}

void CTemperatureDialog::on_tempColor2Btn_clicked()
{
    m_tempColor2->setParentDialog(this);
    m_tempColor2->buttonClicked();
    m_temp->m_temColor.m_color2 = m_tempColor2->m_backColor;
    ((QWidget*)parent())->update();
}

void CTemperatureDialog::on_bkColor1Btn_clicked()
{
    m_color1->setParentDialog(this);
    m_color1->buttonClicked();
    m_temp->m_backColor.m_color1 = m_color1->m_backColor;
    ((QWidget*)parent())->update();

}

void CTemperatureDialog::on_bkColor2Btn_clicked()
{
    m_color2->setParentDialog(this);
    m_color2->buttonClicked();
    m_temp->m_backColor.m_color2 = m_color2->m_backColor;
    ((QWidget*)parent())->update();
}

void CTemperatureDialog::setExpValue(QString str)
{
    ui->expEdit->setText(str);
    m_temp->m_expression = str;
}

void CTemperatureDialog::on_expBtn_clicked()
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
          m_temp->m_expression = var;
      }

}
