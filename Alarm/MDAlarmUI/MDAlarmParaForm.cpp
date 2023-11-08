#include "MDAlarmParaForm.h"
#include "ui_MDAlarmParaForm.h"
#include "MDAlarmConfiger.h"
#include "MDAlarmPara.h"

CMDAlarmParaForm::CMDAlarmParaForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CMDAlarmParaForm)
{
    m_configer = NULL;
    ui->setupUi(this);
}

CMDAlarmParaForm::~CMDAlarmParaForm()
{
    delete ui;
}

bool CMDAlarmParaForm::initial(CMDAlarmConfiger* configer)
{
    m_configer = configer;
    CMDAlarmPara* para = configer->alarmPara();

    QStringList modeList;
    CMDAlarmPara::enumModes(modeList);
    foreach (const QString& modeID, modeList)
    {
        MDAlarmMode aMode = CMDAlarmPara::stringToMode(modeID);
        ui->modeComboBox->addItem(modeID, QVariant((int)aMode));
    }
    ui->modeComboBox->setCurrentText(CMDAlarmPara::modeToString(para->alarmMode()));

    return true;
}

bool CMDAlarmParaForm::accept()
{
    if(!m_configer)
        return false;

    CMDAlarmPara* para = m_configer->alarmPara();

    MDAlarmMode mode = (MDAlarmMode)ui->modeComboBox->currentData().toInt();
    para->setAlarmMode(mode);

     return m_configer->saveToFile();
}
