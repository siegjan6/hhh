#include <QMessageBox>
#include <QPainter>
#include "RulerScaleTextDialog.h"
#include "ui_RulerScaleTextDialog.h"
#include "StyleDialog.h"
#include "ColorDialog.h"
#include "Ccommon.h"
#include "MDMultiLanguage.h"
#include "CMessage.h"

CRulerScaleTextDialog::CRulerScaleTextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRulerScaleTextDialog)
{
    ui->setupUi(this);
    m_rText = NULL;
    m_rTextArrange = NULL;
    m_rTextColor = NULL;
    this->initDialog();
    QStringList types;
    types << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}

CRulerScaleTextDialog::~CRulerScaleTextDialog()
{
    delete m_rTextArrange;
    delete m_rTextColor;
    delete ui;
}

void CRulerScaleTextDialog::initDialog()
{
    this->move(190,10);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->rFontComboBox->setMaxVisibleItems(20);
    ui->rDecimalsEdit->installEventFilter(this);
    ui->rOffsetEdit->installEventFilter(this);
    ui->rSizeEdit->installEventFilter(this);

    QObject::connect(ui->rDecimalsSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setrDecimals(qint32)));
    QObject::connect(ui->rOffsetSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setrOffset(qint32)));
    QObject::connect(ui->rSizeSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setrSize(qint32)));
    QObject::connect(ui->rDecimalsEdit,SIGNAL(editingFinished()),this,SLOT(decimalsWarning()));
    QObject::connect(ui->rOffsetEdit,SIGNAL(editingFinished()),this,SLOT(offsetWarning()));
    QObject::connect(ui->rSizeEdit,SIGNAL(editingFinished()),this,SLOT(sizeWarning()));
    QObject::connect(ui->rFontComboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(setFontName(QString)));
    QObject::connect(ui->rBoldCheckBox,SIGNAL(stateChanged(qint32)),this,SLOT(setTextBold(qint32)));
}

void CRulerScaleTextDialog::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(0,0,0,100));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(ui->rColorBtn->x()+7,ui->rColorBtn->y()+ui->groupBox_2->y()-2,
                     ui->rColorBtn->width()+3,ui->rColorBtn->height()+3);

}

bool CRulerScaleTextDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->rDecimalsEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->rDecimalsEdit->text();
                 qint32 dec = str.toInt();
                 ui->rDecimalsSlider->setValue(dec);
                 m_rText->setDecimals(dec);
             }
        }
    if (watched == ui->rOffsetEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->rOffsetEdit->text();
                 qint32 offset =qint32(str.toDouble()* 10);
                 ui->rOffsetSlider->setValue(offset);
                 m_rText->setOffset(str.toDouble());
             }
        }
    if (watched == ui->rSizeEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->rSizeEdit->text();
                 qint32 size =qint32(str.toDouble()* 10);
                 ui->rSizeSlider->setValue(size);
                 m_rText->setSize(str.toDouble());
             }
        }

    return QWidget::eventFilter(watched,event);
}

void CRulerScaleTextDialog::setData(CData *data)
{
    m_rText = (CRulerScaleText*)data;
    m_rTextArrange = new CPaintButton;
    m_rTextArrange->m_button = ui->rTextStyleBtn;
    m_rTextArrange->setButtonImage(ui->rTextStyleBtn,stRScaleText,
                                   m_rText->m_arrangement,ptImage);
    m_rTextColor = new CPaintButton;
    m_rTextColor->m_button = ui->rColorBtn;
    m_rTextColor->setButtonColor(ui->rColorBtn,m_rText->m_color,ptColor);
    Ccommon::setEdit(ui->rDecimalsEdit,m_rText->getDecimals());
    Ccommon::setEdit(ui->rOffsetEdit,m_rText->getOffset());
    Ccommon::setEdit(ui->rSizeEdit,m_rText->getSize());
    Ccommon::setSlider(ui->rDecimalsSlider,m_rText->getDecimals());
    Ccommon::setSlider(ui->rOffsetSlider,m_rText->getOffset()*10);
    Ccommon::setSlider(ui->rSizeSlider,m_rText->getSize()*10);
     if(m_rText->m_isBold == false)
     {
         ui->rBoldCheckBox->setChecked(false);
     }
     else
         ui->rBoldCheckBox->setChecked(true);
     ui->rFontComboBox->setCurrentText(m_rText->m_name);
}

void CRulerScaleTextDialog::setrDecimals(qint32 value)
{
    QString str = QString::number(value);
    ui->rDecimalsEdit->setText(str);
    m_rText->setDecimals(ui->rDecimalsEdit->text().toInt());
    ((QWidget*)parent())->update();
}

void CRulerScaleTextDialog::setrOffset(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(abs(value % 10));
    str.append(str1);
    ui->rOffsetEdit->setText(str);
    m_rText->setOffset(ui->rOffsetEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerScaleTextDialog::setrSize(qint32 value)
{
    QString str = QString("%1").arg(value / 10);
    str.append(".");
    QString str1 = QString("%1").arg(value % 10);
    str.append(str1);
    ui->rSizeEdit->setText(str);
    m_rText->setSize(ui->rSizeEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CRulerScaleTextDialog::decimalsWarning()
{
    Ccommon::editIntWarning(ui->rDecimalsEdit,WARN_0_5I,5,0);
}

void CRulerScaleTextDialog::offsetWarning()
{
    Ccommon::editDoubleWarning(ui->rOffsetEdit,WARN__55_55F,55,-55);
}

void CRulerScaleTextDialog::sizeWarning()
{
    Ccommon::editDoubleWarning(ui->rSizeEdit,WARN_1_20F,20,1);
}

void CRulerScaleTextDialog::on_rTextStyleBtn_clicked()
{
    m_rTextArrange->buttonClicked();
    m_rText->m_arrangement = m_rTextArrange->getIndex();
     ((QWidget*)parent())->update();
}

void CRulerScaleTextDialog::setFontName(QString str)
{
    m_rText->m_name = str;
    ((QWidget*)parent())->update();
}

void CRulerScaleTextDialog::setTextBold(qint32 state)
{
    if(state == 0)
        m_rText->m_isBold = false;
    else
        m_rText->m_isBold = true;
    ((QWidget*)parent())->update();
}

void CRulerScaleTextDialog::on_rColorBtn_clicked()
{
    m_rTextColor->setParentDialog(this);
    m_rTextColor->buttonClicked();
    m_rText->m_color = m_rTextColor->m_backColor;
    ((QWidget*)parent())->update();
}


