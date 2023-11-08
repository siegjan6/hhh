#include "MDAlarmConfiger.h"
#include "MDAlarmConfigerImp.h"

CMDAlarmConfiger::CMDAlarmConfiger()
{
    m_imp = new CMDAlarmConfigerImp;
}

CMDAlarmConfiger::~CMDAlarmConfiger()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDAlarmConfiger::loadConfig(const QString& projectPath)
{
    return (m_imp ? m_imp->loadConfig(projectPath) : false);
}

void CMDAlarmConfiger::unloadConfig()
{
    if(m_imp)
        m_imp->unloadConfig();
}

QString CMDAlarmConfiger::configFileName()
{
    return (m_imp ? m_imp->configFileName() : "");
}

bool CMDAlarmConfiger::saveToFile()
{
    return (m_imp ? m_imp->saveToFile() : false);
}

/*!
\section 管理报警参数
*/
void CMDAlarmConfiger::setAlarmPara(const CMDAlarmPara* destConfig)
{
    if(m_imp)
        m_imp->setAlarmPara(destConfig);
}

CMDAlarmPara* CMDAlarmConfiger::alarmPara()
{
    return (m_imp ? m_imp->alarmPara() : NULL);
}

/*!
\section 获取【报警条件配置类】和【报警等级配置类】的对象实例
*/
CMDAlarmLevelConfiger* CMDAlarmConfiger::levelConfiger()
{
    return (m_imp ? &m_imp->m_levelConfiger : NULL);
}

CMDAlarmConditionConfiger* CMDAlarmConfiger::conditionConfiger()
{
    return (m_imp ? &m_imp->m_conditionConfiger : NULL);
}

