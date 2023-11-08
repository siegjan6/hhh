#include "CYPropertySettingDialog.h"
#include "ui_CYPropertySettingDialog.h"
#include "CBisectrixLineSettingDialog.h"

#include "CYAxis.h"
#include <QColorDialog>
#include <QFontDialog>
#include "CTranslator.h"
#include "CFormToolFunc.h"

#include <QDebug>
#include <QButtonGroup>
#include <QRegExpValidator>

using namespace Translator;
using namespace FormToolFunc;

CYPropertySettingDialog::CYPropertySettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CYPropertySettingDialog),
    m_yAxis(0),
    m_translatTool(0)
{
    ui->setupUi(this);    
    installDialogTranslator(m_translatTool,this);
}

CYPropertySettingDialog::~CYPropertySettingDialog()
{
    delete ui;
}

CYAxis *CYPropertySettingDialog::getYAxis(bool *ok, CYAxis *initial, QWidget *parent)
{
    if(initial)
    {
        CYPropertySettingDialog dlg(parent);
        dlg.setData(initial);  
        dlg.setWindowTitle(QString("%1-%2").arg(initial->name(),initial->comment()));
        int ret = dlg.exec();
        if(ok)
        {
            *ok = !!ret;
            return initial;
        }
    }
    return NULL;
}
void CYPropertySettingDialog::setData(CYAxis *y)
{
    if(y)
    {
        m_yAxis = y;
        initialize();
    }
}
/*!
\section
*/
void CYPropertySettingDialog::initialize()
{
    initializeData();
    initializeUI();
    createConnections();
}

void CYPropertySettingDialog::initializeData()
{
    if(m_yAxis)
    {
        m_font = m_yAxis->font();
        m_fontColor = m_yAxis->fontColor();
    }
}

void CYPropertySettingDialog::initializeUI()
{
    QButtonGroup *locationButtonGroup = new QButtonGroup(this);
    locationButtonGroup->addButton(ui->leftRadioButton);
    locationButtonGroup->addButton(ui->rigthRadioButton);

    QButtonGroup *valueButtonGroup = new QButtonGroup(this);
    valueButtonGroup->addButton(ui->valueRadioButton);
    valueButtonGroup->addButton(ui->percentRadioButton);
    ui->commentLineEdit->setMaxLength(100);

    ui->precisionLineEdit->setValidator(new QIntValidator(0,5,this));

    QRegExp rx("^-?\\d+$");
    QRegExpValidator *pReg = new QRegExpValidator(rx, this);
    ui->maxValueLineEdit->setValidator(pReg);
    ui->minValueLineEdit->setValidator(pReg);

    setButtonColor(m_fontColor,*ui->fontColorButton);//
    if(m_yAxis)
    {
        ui->nameLineEdit->setText(m_yAxis->name());
        ui->commentLineEdit->setText(m_yAxis->comment());
//        ui->maxValueLineEdit->setText(QString::number(m_yAxis->maxValue()));
//        ui->minValueLineEdit->setText(QString::number(m_yAxis->minValue()));
        ui->widthLineEdit->setText(QString::number(m_yAxis->yWidth()));
        ui->precisionLineEdit->setText(QString::number(m_yAxis->valuePrecision()));
        ui->axisVisibleCheckBox->setChecked(m_yAxis->axisVisible());
        ui->axisLineVisibleCheckBox->setChecked(m_yAxis->axisLineVisible());
        ui->leftRadioButton->setChecked(m_yAxis->isLeft());
        ui->rigthRadioButton->setChecked(!m_yAxis->isLeft());
        ui->valueRadioButton->setChecked(m_yAxis->isNumeric());
        ui->percentRadioButton->setChecked(!m_yAxis->isNumeric());
        if(m_yAxis->isNumeric())
        {
            ui->maxValueLineEdit->setText(QString::number(m_yAxis->maxValue()));
            ui->minValueLineEdit->setText(QString::number(m_yAxis->minValue()));
        }
        else
        {
            ui->maxValueLineEdit->setText(QString::number(m_yAxis->maxPercent()));
            ui->minValueLineEdit->setText(QString::number(m_yAxis->minPercent()));
        }
    }
}

/*!
\section
*/
void CYPropertySettingDialog::applyChanges()
{
    m_yAxis->setFont(m_font);
    m_yAxis->setFontColor(m_fontColor);
    QString comment = ui->commentLineEdit->text().trimmed();
    m_yAxis->setComment(comment);
    m_yAxis->setYWidth(ui->widthLineEdit->text().toInt());
    m_yAxis->setValuePrecision(ui->precisionLineEdit->text().toInt());
    m_yAxis->setAxisLineVisible(ui->axisLineVisibleCheckBox->isChecked());
    m_yAxis->setAxisVisible(ui->axisVisibleCheckBox->isChecked());
    m_yAxis->setIsLeft(ui->leftRadioButton->isChecked());
    m_yAxis->setIsNumeric(ui->valueRadioButton->isChecked());
    m_yAxis->setOriginMax(ui->maxValueLineEdit->text().toDouble());
    m_yAxis->setOriginMin(ui->minValueLineEdit->text().toDouble());

    if(ui->valueRadioButton->isChecked())
    {
        m_yAxis->setMaxValue(ui->maxValueLineEdit->text().toDouble());
        m_yAxis->setMinValue(ui->minValueLineEdit->text().toDouble());
    }
    else
    {
        m_yAxis->setMaxValue(100);
        m_yAxis->setMinValue(0);

        m_yAxis->setMaxPercent(ui->maxValueLineEdit->text().toDouble());
        m_yAxis->setMinPercent(ui->minValueLineEdit->text().toDouble());
    }
}
/*!
\section
*/
void CYPropertySettingDialog::createConnections()
{
    connect(ui->fontColorButton,&QPushButton::clicked,this,&CYPropertySettingDialog::slotSetFontColor);
    connect(ui->axisLineVisibleCheckBox,&QCheckBox::clicked,this,&CYPropertySettingDialog::slotSetAxisLineVisible);
    connect(ui->axisLineSettingButton,&QPushButton::clicked,this,&CYPropertySettingDialog::slotSetAxisLineProperty);
    connect(ui->okButton,&QPushButton::clicked,this,&CYPropertySettingDialog::slotOkButtonClicked);
    connect(ui->cancelButton,&QPushButton::clicked,this,&CYPropertySettingDialog::slotCancelButtonClicked);
    connect(ui->fontButton,&QPushButton::clicked,this,&CYPropertySettingDialog::slotSetFont);
}

void CYPropertySettingDialog::slotOkButtonClicked()
{
    applyChanges();
    accept();
}

void CYPropertySettingDialog::slotCancelButtonClicked()
{
    reject();
}

void CYPropertySettingDialog::slotSetAxisLineVisible(bool visible)
{
    ui->axisLineSettingButton->setEnabled(visible);
}

void CYPropertySettingDialog::slotSetAxisLineProperty()
{
    CBisectrixLine *initialLine = m_yAxis->axisLine();
    bool ok = false;

    CBisectrixLineSettingDialog::getLine(&ok,initialLine,this);
}

void CYPropertySettingDialog::slotSetFont()
{
    selectFont(m_font,this);

}

void CYPropertySettingDialog::slotSetFontColor()
{
    setColor(m_fontColor,*ui->fontColorButton);
}

/*!
\section
*/
void CYPropertySettingDialog::setColor( QColor &initialColor, QPushButton &colorButton)
{
    selectColor(initialColor,colorButton,this);
}
