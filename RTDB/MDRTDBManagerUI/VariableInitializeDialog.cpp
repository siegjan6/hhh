#include "VariableInitializeDialog.h"
#include "ui_VariableInitializeDialog.h"
#include "MDMultiLanguage.h"
CVariableInitializeDialog::CVariableInitializeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CVariableInitializeDialog)
{
    m_initialParameter.m_initType = MDRTInitMode::None;
    m_initialParameter.m_value = "";
    m_LastVal = "";
    m_FixedValue = "";
    ui->setupUi(this);

    connect(ui->noneradio,SIGNAL(clicked()),this,SLOT(onClickNoneRadio()));
    //connect(ui->eventcheckbox,SIGNAL(clicked()),this,SLOT(onClickEventEnable()));
    connect(ui->lastvalradio,SIGNAL(clicked()),this,SLOT(onClickLastValueRadio()));
    connect(ui->fixedvalradio,SIGNAL(clicked()),this,SLOT(onClickFixedValueRadio()));
    connect(ui->fixedvaledit,SIGNAL(textChanged(const QString &)),this,SLOT(onFixedValueChanged()));
    //多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

CVariableInitializeDialog::~CVariableInitializeDialog()
{
    delete ui;
}

void CVariableInitializeDialog::setRunModel(bool isRunning)
{
    m_isRunning = isRunning;
}

void CVariableInitializeDialog::initial(IMDRTDBVariable *curRTDBVariable)
{
    if(curRTDBVariable!= NULL)
    {
        m_initialParameter = curRTDBVariable->initializeParameter();

        if(m_initialParameter.m_initType == MDRTInitMode::LastValue)
            m_LastVal = m_initialParameter.m_value;
        else if(m_initialParameter.m_initType == MDRTInitMode::FixedValue)
            m_FixedValue = m_initialParameter.m_value;
    }
    updateContols();
}
void CVariableInitializeDialog::updateContols()
{
    ui->lastvaledit->setEnabled(false);
    //!刷新控件值   
    if(m_initialParameter.m_initType == MDRTInitMode::None)
    {
        ui->noneradio->setChecked(true);
        ui->lastvalradio->setChecked(false);
        ui->fixedvalradio->setChecked(false);
        ui->fixedvaledit->hide();
        ui->lastvaledit->hide();
    }
    else if(m_initialParameter.m_initType == MDRTInitMode::LastValue)
    {
        ui->noneradio->setChecked(false);
        ui->lastvalradio->setChecked(true);
        ui->fixedvalradio->setChecked(false);
        ui->lastvaledit->setText(m_LastVal);
        ui->lastvaledit->show();
        ui->fixedvaledit->hide();
    }
    else
    {
        ui->noneradio->setChecked(false);
        ui->lastvalradio->setChecked(false);
        ui->fixedvalradio->setChecked(true);
        ui->lastvaledit->hide();
        ui->fixedvaledit->show();
        ui->fixedvaledit->setText(m_FixedValue);

    }
    //!正在运行时，全部控件不使能
    if(m_isRunning)
    {
        ui->noneradio->setEnabled(false);
        ui->lastvalradio->setEnabled(false);
        ui->fixedvalradio->setEnabled(false);
        ui->fixedvaledit->setEnabled(false);        
    }
    else
    {
        ui->noneradio->setEnabled(true);
        ui->lastvalradio->setEnabled(true);
        ui->fixedvalradio->setEnabled(true);
        ui->fixedvaledit->setEnabled(true);
    }
}
//!使能初始化
void CVariableInitializeDialog::onClickNoneRadio()
{
    if(ui->noneradio->isChecked())
    {
        m_initialParameter.m_initType = MDRTInitMode::None;
        updateContols();
    }
}

//!事件使能
void CVariableInitializeDialog::onClickEventEnable()
{
    if(ui->eventcheckbox->checkState() == Qt::CheckState::Checked)
    {
        m_initialParameter.m_enableEvent = true;
    }
    else
    {
        m_initialParameter.m_enableEvent = false;
    }
    updateContols();
}
//!上次值Radio
void CVariableInitializeDialog::onClickLastValueRadio()
{
    if(ui->lastvalradio->isChecked())
    {
        m_initialParameter.m_initType = MDRTInitMode::LastValue;
        m_initialParameter.m_value = m_LastVal;
        updateContols();
    }
}
//!固定值Radio
void CVariableInitializeDialog::onClickFixedValueRadio()
{
    if(ui->fixedvalradio->isChecked())
    {
        m_initialParameter.m_initType = MDRTInitMode::FixedValue;
        m_initialParameter.m_value = m_FixedValue;
        updateContols();
    }
}
//!改变固定设置值
void CVariableInitializeDialog::onFixedValueChanged()
{
    m_FixedValue = ui->fixedvaledit->text();
    m_initialParameter.m_value = m_FixedValue;
}
