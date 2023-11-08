#include "VariableDataConversionDialog.h"
#include "ui_VariableDataConversionDialog.h"
#include "MDMultiLanguage.h"

CVariableDataConversionDialog::CVariableDataConversionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CVariableDataConversionDialog)
{
    m_isRunning = false;
    m_dataConversionParameter.m_enableLinearConversion = false;
    m_dataConversionParameter.m_maxValue=0;
    m_dataConversionParameter.m_minValue=0;
    m_dataConversionParameter.m_originalMaxValue=0;
    m_dataConversionParameter.m_originalMinValue=0;
    m_dataConversionParameter.m_conversionType = MDVariableDataConversionType::None;

    ui->setupUi(this);
    connect(ui->maxValueEdit,SIGNAL(textChanged(const QString &)),this,SLOT(onMaxValueEditTextChanged(const QString &)));
    connect(ui->minValueEdit,SIGNAL(textChanged(const QString &)),this,SLOT(onMinValueEditTextChanged(const QString &)));
    connect(ui->originalMaxValueEdit,SIGNAL(textChanged(const QString &)),this,SLOT(onOriginalMaxValueEditTextChanged(const QString &)));
    connect(ui->originalMinValueEdit,SIGNAL(textChanged(const QString &)),this,SLOT(onOriginalMinValueEditTextChanged(const QString &)));
    connect(ui->conversionTypeBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onConversionTypeBoxCurrentIndexChanged(int)));
    connect(ui->enableLinearCheckBox,SIGNAL(clicked()),this,SLOT(onEnableLinearConvertCheckBoxClicked()));

    m_convertionTypeList.append(tr("无"));
    m_convertionTypeList.append(tr("3位BCD码"));
    m_convertionTypeList.append(tr("4位BCD码"));
    m_convertionTypeList.append(tr("8位BCD码"));
    m_convertionTypeList.append(tr("OMR有符号4位BCD码"));
    m_convertionTypeList.append(tr("8位二进制无报警"));
    m_convertionTypeList.append(tr("8位二进制有报警"));
    m_convertionTypeList.append(tr("12位二进制无报警"));
    m_convertionTypeList.append(tr("12位二进制有报警"));
    m_convertionTypeList.append(tr("13位二进制无报警"));
    m_convertionTypeList.append(tr("13位二进制有报警"));
    m_convertionTypeList.append(tr("15位二进制无报警"));
    m_convertionTypeList.append(tr("15位二进制有报警"));
    m_convertionTypeList.append(tr("数据字节顺序转换"));
    m_convertionTypeList.append(tr("西门子有符号4位BCD码"));
    ui->conversionTypeBox->addItems(m_convertionTypeList);

    //多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

CVariableDataConversionDialog::~CVariableDataConversionDialog()
{
    m_convertionTypeList.clear();
    delete ui;
}
void CVariableDataConversionDialog::setRunModel(bool isRunning)
{
    m_isRunning = isRunning;
}
/*!
\section 响应变化
*/
void CVariableDataConversionDialog::onMaxValueEditTextChanged(const QString & arg1)
{
    m_dataConversionParameter.m_maxValue  = ui->maxValueEdit->text().toDouble();
}
void CVariableDataConversionDialog::onMinValueEditTextChanged(const QString & arg1)
{
    m_dataConversionParameter.m_minValue  = ui->minValueEdit->text().toDouble();
}
void CVariableDataConversionDialog::onOriginalMaxValueEditTextChanged(const QString & arg1)
{
    m_dataConversionParameter.m_originalMaxValue  = ui->originalMaxValueEdit->text().toDouble();
}
void CVariableDataConversionDialog::onOriginalMinValueEditTextChanged(const QString & arg1)
{
    m_dataConversionParameter.m_originalMinValue  = ui->originalMinValueEdit->text().toDouble();
}

void CVariableDataConversionDialog::onConversionTypeBoxCurrentIndexChanged(int index)
{
     m_dataConversionParameter.m_conversionType =  (MDVariableDataConversionType)index;
     updateContols();
}
void CVariableDataConversionDialog::initial(IMDRTDBVariable *curRTDBVariable)
{
    if(curRTDBVariable!= NULL)
    {
        m_dataConversionParameter = curRTDBVariable->dataConversionParameter();
    }
    updateContols();
}
void CVariableDataConversionDialog::updateContols()
{
    /*
    if(m_dataConversionParameter.m_enable)
    {
        ui->enableCheckBox->setChecked(true);
        ui->conversionTypeBox->setEnabled(true);
        ui->maxValueEdit->setEnabled(true);
        ui->minValueEdit->setEnabled(true);
        ui->originalMaxValueEdit->setEnabled(true);
        ui->originalMinValueEdit->setEnabled(true);
    }
    else
    {
        ui->enableCheckBox->setChecked(false);
        ui->conversionTypeBox->setEnabled(false);
        ui->maxValueEdit->setEnabled(false);
        ui->minValueEdit->setEnabled(false);
        ui->originalMaxValueEdit->setEnabled(false);
        ui->originalMinValueEdit->setEnabled(false);
    }*/
    if(m_dataConversionParameter.m_enableLinearConversion)
    {
        ui->maxValueEdit->setEnabled(true);
        ui->minValueEdit->setEnabled(true);
        ui->originalMaxValueEdit->setEnabled(true);
        ui->originalMinValueEdit->setEnabled(true);
    }
    else
    {
        ui->maxValueEdit->setEnabled(false);
        ui->minValueEdit->setEnabled(false);
        ui->originalMaxValueEdit->setEnabled(false);
        ui->originalMinValueEdit->setEnabled(false);
    }
    ui->conversionTypeBox->setCurrentIndex(int(m_dataConversionParameter.m_conversionType));
    ui->maxValueEdit->setText(QString("%1").arg(m_dataConversionParameter.m_maxValue));
    ui->minValueEdit->setText(QString("%1").arg(m_dataConversionParameter.m_minValue));
    ui->originalMaxValueEdit->setText(QString("%1").arg(m_dataConversionParameter.m_originalMaxValue));
    ui->originalMinValueEdit->setText(QString("%1").arg(m_dataConversionParameter.m_originalMinValue));
    /*/
    if(m_isRunning)
    {
        ui->conversionTypeBox->setEnabled(false);
        ui->maxValueEdit->setEnabled(false);
        ui->minValueEdit->setEnabled(false);
        ui->originalMaxValueEdit->setEnabled(false);
        ui->originalMinValueEdit->setEnabled(false);
        ui->enableCheckBox->setEnabled(false);
    }/*/
}

void CVariableDataConversionDialog::onEnableLinearConvertCheckBoxClicked()
{
    if(ui->enableLinearCheckBox->checkState() == Qt::CheckState::Checked)
    {
        m_dataConversionParameter.m_enableLinearConversion  = true;
    }
    else
    {
        m_dataConversionParameter.m_enableLinearConversion  = false;
    }
    updateContols();
}
