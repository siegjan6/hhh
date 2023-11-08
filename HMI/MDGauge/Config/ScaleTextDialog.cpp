#include <QMessageBox>
#include "ScaleTextDialog.h"
#include "ui_ScaleTextDialog.h"
#include "StyleDialog.h"
#include "ColorDialog.h"
#include "Ccommon.h"
#include "MDMultiLanguage.h"
#include "CMessage.h"

CScaleTextDialog::CScaleTextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CScaleTextDialog)
{
    ui->setupUi(this);
    m_text = NULL;
    m_textArrange = NULL;
    m_textColor = NULL;
    this->initDialog();
    QStringList types;
    types << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}

CScaleTextDialog::~CScaleTextDialog()
{
    delete m_textArrange;
    delete m_textColor;
    delete ui;
}

void CScaleTextDialog::initDialog()
{
    this->move(190,0);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    hide();
    ui->decimalsEdit->installEventFilter(this);
    ui->radiueEdit->installEventFilter(this);
    ui->sizeEdit->installEventFilter(this);
    ui->fontComboBox->setMaxVisibleItems(20);
    QObject::connect(ui->decimalsSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setDecimalsV(qint32)));
    QObject::connect(ui->radiusSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setRadiusV(qint32)));
    QObject::connect(ui->sizeSlider,SIGNAL(valueChanged(qint32)),this,SLOT(setSizeV(qint32)));

    QObject::connect(ui->decimalsEdit,SIGNAL(editingFinished()),this,SLOT(decimalsWarning()));
    QObject::connect(ui->radiueEdit,SIGNAL(editingFinished()),this,SLOT(radiusWarning()));
    QObject::connect(ui->sizeEdit,SIGNAL(editingFinished()),this,SLOT(sizeWarning()));

    QObject::connect(ui->boldCheckBox,SIGNAL(stateChanged(qint32)),this,SLOT(setTextBold(qint32)));
    QObject::connect(ui->fontComboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(setFontName(QString)));

}

void CScaleTextDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(0,0,0,100));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawRect(ui->colorBtn->x()+7,ui->colorBtn->y()+ui->groupBox_2->y()-2,
                     ui->colorBtn->width()+3,ui->colorBtn->height()+3);

}
void CScaleTextDialog::setData(CData *data)
{
    m_text = (CDataScaleText*)data;
    m_textArrange = new CPaintButton;
    m_textArrange->m_button = ui->textStyleBtn;
    m_textArrange->setButtonImage(ui->textStyleBtn,stScaleText,
                                  m_text->m_style,ptImage);
    m_textColor = new CPaintButton;
    m_textColor->m_button = ui->colorBtn;
    m_textColor->setButtonColor( ui->colorBtn,m_text->m_color,ptColor);
    Ccommon::setEdit(ui->decimalsEdit,m_text->getDecimals());
    Ccommon::setEdit(ui->radiueEdit,m_text->getRadius());
    Ccommon::setEdit(ui->sizeEdit,m_text->getSize());
    Ccommon::setSlider(ui->decimalsSlider,m_text->getDecimals());
    Ccommon::setSlider(ui->radiusSlider,m_text->getRadius()*10);
    Ccommon::setSlider(ui->sizeSlider,m_text->getSize()*10);
     if(m_text->m_isBold == false)
     {
         ui->boldCheckBox->setChecked(false);
     }
     else
         ui->boldCheckBox->setChecked(true);
     ui->fontComboBox->setCurrentText(m_text->m_name);
}

bool CScaleTextDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->decimalsEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->decimalsEdit->text();
                 qint32 decimals = str.toInt();
                 ui->decimalsSlider->setValue(decimals);
                 m_text->setDecimals(str.toInt());
             }
        }
    if (watched == ui->radiueEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->radiueEdit->text();
                 qint32 radius =qint32(str.toDouble()* 10);
                 ui->radiusSlider->setValue(radius);
                 m_text->setRadius(str.toDouble());
             }
        }
    if (watched == ui->sizeEdit)
        {
             if (event->type()== QEvent::FocusOut)
             {
                 QString str = ui->sizeEdit->text();
                 qint32 size =qint32(str.toDouble()* 10);
                 ui->sizeSlider->setValue(size);
                 m_text->setSize(str.toDouble());
             }
        }
     return QWidget::eventFilter(watched,event);
}

void CScaleTextDialog::on_textStyleBtn_clicked()
{
    m_textArrange->buttonClicked();
    m_text->m_style = m_textArrange->getIndex();
     ((QWidget*)parent())->update();
}

void CScaleTextDialog::setDecimalsV(qint32 value)
{
    QString ret = QString::number(value);
    ui->decimalsEdit->setText(ret);
    m_text->setDecimals(ui->decimalsEdit->text().toInt());
    ((QWidget*)parent())->update();
}

void CScaleTextDialog::setRadiusV(qint32 value)
{
    QString ret = QString("%1").arg(value / 10);
    ret.append(".");
    QString ret1 = QString("%1").arg(value % 10);
    ret.append(ret1);
    ui->radiueEdit->setText(ret);
    m_text->setRadius(ui->radiueEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CScaleTextDialog::setSizeV(qint32 value)
{
    QString ret = QString("%1").arg(value / 10);
    ret.append(".");
    QString ret1 = QString("%1").arg(value % 10);
    ret.append(ret1);
    ui->sizeEdit->setText(ret);
    m_text->setSize(ui->sizeEdit->text().toDouble());
    ((QWidget*)parent())->update();
}

void CScaleTextDialog::on_colorBtn_clicked()
{
    m_textColor->setParentDialog(this);
    m_textColor->buttonClicked();
    m_text->m_color = m_textColor->getColor();
    ((QWidget*)parent())->update();
}

void CScaleTextDialog::setTextBold(qint32 state)
{
    if(state == 0)
        m_text->m_isBold = false;
    else
        m_text->m_isBold = true;
    ((QWidget*)parent())->update();
}

void CScaleTextDialog::setFontName(QString str)
{
    m_text->m_name = str;
    ((QWidget*)parent())->update();
}

void CScaleTextDialog::decimalsWarning()
{
    Ccommon::editIntWarning(ui->decimalsEdit,WARN_0_5I,5,0);
}

void CScaleTextDialog::radiusWarning()
{
    Ccommon::editDoubleWarning(ui->radiueEdit,WARN_10_80F,80,10);
}

void CScaleTextDialog::sizeWarning()
{
    Ccommon::editDoubleWarning(ui->sizeEdit,WARN_1_20F,20,1);
}
