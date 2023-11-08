#include "MDAlarmLevelConfiger.h"
#include "MDAlarmLevelConfigerImp.h"

CMDAlarmLevelConfiger::CMDAlarmLevelConfiger()
{
    m_imp = new CMDAlarmLevelConfigerImp;
}

CMDAlarmLevelConfiger::~CMDAlarmLevelConfiger()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDAlarmLevelConfiger::loadConfig(const QString& projectPath)
{
    return (m_imp ? m_imp->loadConfig(projectPath) : false);
}

void CMDAlarmLevelConfiger::unloadConfig()
{
    if(m_imp)
        m_imp->unloadConfig();
}

QString CMDAlarmLevelConfiger::configFileName()
{
    return (m_imp ? m_imp->configFileName() : "");
}

bool CMDAlarmLevelConfiger::saveToFile()
{
    return (m_imp ? m_imp->saveToFile() : false);
}

/*!
\section 管理报警等级
*/
void CMDAlarmLevelConfiger::configedList(QList<CMDAlarmLevel*>& accountList)
{
    if(m_imp)
        m_imp->configedList(accountList);
}

int CMDAlarmLevelConfiger::count()
{
    return (m_imp ? m_imp->count() : 0);
}

bool CMDAlarmLevelConfiger::add(const CMDAlarmLevel* configProperty)
{
    return (m_imp ? m_imp->add(configProperty) : false);
}

bool CMDAlarmLevelConfiger::delete1(const QString& name)
{
    return (m_imp ? m_imp->delete1(name) : false);
}

CMDAlarmLevel* CMDAlarmLevelConfiger::find(const QString& name)
{
    return (m_imp ? m_imp->find(name) : NULL);
}

CMDAlarmLevel* CMDAlarmLevelConfiger::find(int index)
{
    return (m_imp ? m_imp->find(index) : NULL);
}

bool CMDAlarmLevelConfiger::modify(const CMDAlarmLevel* sourceConfig,
                                const CMDAlarmLevel* destConfig)
{
    return (m_imp ? m_imp->modify(sourceConfig, destConfig) : false);
}
