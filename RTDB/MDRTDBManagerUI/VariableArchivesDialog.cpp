#include "VariableArchivesDialog.h"
#include "ui_VariableArchivesDialog.h"
#include "MDMultiLanguage.h"
CVariableArchivesDialog::CVariableArchivesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CVariableArchivesDialog)
{
    m_isRunning = false;
    m_dataSaveRulerParameter.m_saveRuler = MDRTDataSaveRuler::None;
    m_dataSaveRulerParameter.m_saveMode = MDRTDataSaveMode::Memory;
    m_dataSaveRulerParameter.m_saveRuler = MDRTDataSaveRuler::None;
    m_dataSaveRulerParameter.m_saveCycle = 1;
    m_dataSaveRulerParameter.m_timeRange = 360;
    ui->setupUi(this);

    m_timeRangeValidator=new QIntValidator(1,2147483647,this);
    m_cycleValidator=new QIntValidator(1,2147483647,this);

    ui->timeRangeEdit->setValidator(m_timeRangeValidator);
    ui->cycleEdit->setValidator(m_cycleValidator);
    connect(ui->timeRangeEdit,SIGNAL(textChanged(const QString &)),this,SLOT(onTimeRangeEditTextChanged(const QString &)));
    connect(ui->cycleEdit,SIGNAL(textChanged(const QString &)),this,SLOT(onCycleEditTextChanged(const QString &)));
    connect(ui->rulerRadioButton0,SIGNAL(clicked()),this,SLOT(onRulerRadioButtonClicked()));
    connect(ui->rulerRadioButton1,SIGNAL(clicked()),this,SLOT(onRulerRadioButtonClicked()));
    connect(ui->rulerRadioButton2,SIGNAL(clicked()),this,SLOT(onRulerRadioButtonClicked()));
    connect(ui->modeRadioButton0,SIGNAL(clicked()),this,SLOT(onModeRadioButtonClicked()));
    connect(ui->modeRadioButton1,SIGNAL(clicked()),this,SLOT(onModeRadioButtonClicked()));
    connect(ui->modeRadioButton2,SIGNAL(clicked()),this,SLOT(onModeRadioButtonClicked()));

    //多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

CVariableArchivesDialog::~CVariableArchivesDialog()
{
    delete ui;
    if(m_timeRangeValidator)
    {
        delete m_timeRangeValidator;
        m_timeRangeValidator = NULL;
    }
    if(m_cycleValidator)
    {
        delete m_cycleValidator;
        m_cycleValidator = NULL;
    }
}

void CVariableArchivesDialog::setRunModel(bool isRunning)
{
    m_isRunning = isRunning;
}
//!设置历史归档时间，默认360秒
void CVariableArchivesDialog::onTimeRangeEditTextChanged(const QString & arg1)
{
    int timeRange = ui->timeRangeEdit->text().toInt();
    if(timeRange<=0)
    {
        m_dataSaveRulerParameter.m_timeRange = 360;
    }
    else
    {
        m_dataSaveRulerParameter.m_timeRange = ui->timeRangeEdit->text().toInt();
    }
     updateContols();
}
//!设置周期存储时间默认1秒
void CVariableArchivesDialog::onCycleEditTextChanged(const QString & arg1)
{
    int saveCycle = ui->cycleEdit->text().toInt();
    if(saveCycle<=0)
    {
        m_dataSaveRulerParameter.m_saveCycle = 1;
    }
    else
    {
        m_dataSaveRulerParameter.m_saveCycle = ui->cycleEdit->text().toInt();
    }
    updateContols();

}
void CVariableArchivesDialog::onRulerRadioButtonClicked()
{
    m_dataSaveRulerParameter.m_saveRuler = MDRTDataSaveRuler::None;
    if(ui->rulerRadioButton1->isChecked())
    {
        m_dataSaveRulerParameter.m_saveRuler = MDRTDataSaveRuler::DataChanged;
    }
    else  if(ui->rulerRadioButton2->isChecked())
    {
        m_dataSaveRulerParameter.m_saveRuler = MDRTDataSaveRuler::Cycle;
    }
    updateContols();
}
void CVariableArchivesDialog::onModeRadioButtonClicked()
{
    m_dataSaveRulerParameter.m_saveMode = MDRTDataSaveMode::Memory;
    if(ui->modeRadioButton1->isChecked())
    {
        m_dataSaveRulerParameter.m_saveMode = MDRTDataSaveMode::DiskFile;
    }
    else  if(ui->modeRadioButton2->isChecked())
    {
        m_dataSaveRulerParameter.m_saveMode = MDRTDataSaveMode::Both;
    }
    updateContols();
}

void CVariableArchivesDialog::initial(IMDRTDBVariable *curRTDBVariable)
{
    if(curRTDBVariable!= NULL)
    {
        m_dataSaveRulerParameter = curRTDBVariable->dataSaveRulerParameter();
    }
    updateContols();
}
void CVariableArchivesDialog::updateContols()
{
    ui->cycleEdit->setText(QString("%1").arg(m_dataSaveRulerParameter.m_saveCycle));
    ui->timeRangeEdit->setText(QString("%1").arg(m_dataSaveRulerParameter.m_timeRange));

    switch (m_dataSaveRulerParameter.m_saveRuler) {
    case MDRTDataSaveRuler::None:
        ui->rulerRadioButton0->setChecked(true);
        ui->rulerRadioButton1->setChecked(false);
        ui->rulerRadioButton2->setChecked(false);
        ui->cycleEdit->setEnabled(false);
        ui->modeRadioButton0->setEnabled(false);
        ui->modeRadioButton1->setEnabled(false);
        ui->modeRadioButton2->setEnabled(false);
        break;
    case MDRTDataSaveRuler::DataChanged:
        ui->rulerRadioButton0->setChecked(false);
        ui->rulerRadioButton1->setChecked(true);
        ui->rulerRadioButton2->setChecked(false);
        ui->cycleEdit->setEnabled(false);
        ui->modeRadioButton0->setEnabled(true);
        ui->modeRadioButton1->setEnabled(false);
        ui->modeRadioButton2->setEnabled(false);
        break;
    case MDRTDataSaveRuler::Cycle:
        ui->rulerRadioButton0->setChecked(false);
        ui->rulerRadioButton1->setChecked(false);
        ui->rulerRadioButton2->setChecked(true);
        ui->cycleEdit->setEnabled(true);
        ui->modeRadioButton0->setEnabled(true);
        ui->modeRadioButton1->setEnabled(false);
        ui->modeRadioButton2->setEnabled(false);
        break;
    default:
        break;
    }
  ui->timeRangeEdit->setEnabled(false);
    switch (m_dataSaveRulerParameter.m_saveMode)
    {
    case MDRTDataSaveMode::Memory:
        {
            ui->modeRadioButton0->setChecked(true);
            ui->modeRadioButton1->setChecked(false);
            ui->modeRadioButton2->setChecked(false);
            if(m_dataSaveRulerParameter.m_saveRuler!=MDRTDataSaveRuler::None)
            {
                ui->timeRangeEdit->setEnabled(true);
            }
        }
        break;
    case MDRTDataSaveMode::DiskFile:
        {
            ui->modeRadioButton0->setChecked(false);
            ui->modeRadioButton1->setChecked(true);
            ui->modeRadioButton2->setChecked(false);

        }
        break;
    case MDRTDataSaveMode::Both:
        {
            ui->modeRadioButton0->setChecked(false);
            ui->modeRadioButton1->setChecked(true);
            ui->modeRadioButton2->setChecked(false);
            if(m_dataSaveRulerParameter.m_saveRuler!=MDRTDataSaveRuler::None)
            {
                ui->timeRangeEdit->setEnabled(true);
            }
        }
        break;
    default:
        break;
    }
    /*/
    if(m_isRunning)
    {
        ui->cycleEdit->setEnabled(false);
        ui->timeRangeEdit->setEnabled(false);
        ui->modeRadioButton0->setEnabled(false);
        ui->modeRadioButton1->setEnabled(false);
        ui->modeRadioButton2->setEnabled(false);
        ui->rulerRadioButton0->setEnabled(false);
        ui->rulerRadioButton1->setEnabled(false);
        ui->rulerRadioButton2->setEnabled(false);
        ui->cycleEdit->setEnabled(false);
    }
    /*/
}
