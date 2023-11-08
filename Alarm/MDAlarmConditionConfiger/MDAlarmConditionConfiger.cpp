#include "MDAlarmConditionConfiger.h"
#include "MDAlarmConditionConfigerImp.h"

CMDAlarmConditionConfiger::CMDAlarmConditionConfiger()
{
    m_imp = new CMDAlarmConditionConfigerImp;
}

CMDAlarmConditionConfiger::~CMDAlarmConditionConfiger()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDAlarmConditionConfiger::loadConfig(const QString& projectPath)
{
    return (m_imp ? m_imp->loadConfig(projectPath) : false);
}

void CMDAlarmConditionConfiger::unloadConfig()
{
    if(m_imp)
        m_imp->unloadConfig();
}

QString CMDAlarmConditionConfiger::configFileName()
{
    return (m_imp ? m_imp->configFileName() : "");
}

bool CMDAlarmConditionConfiger::saveToFile()
{
    return (m_imp ? m_imp->saveToFile() : false);
}

/*!
\section 管理报警条件
*/
void CMDAlarmConditionConfiger::configedList(QList<CMDAlarmCondition*>& acList)
{
    if(m_imp)
        m_imp->configedList(acList);
}

int CMDAlarmConditionConfiger::count()
{
    return (m_imp ? m_imp->count() : 0);
}

bool CMDAlarmConditionConfiger::add(const CMDAlarmCondition* configProperty)
{
    return (m_imp ? m_imp->add(configProperty) : false);
}

bool CMDAlarmConditionConfiger::addNew(CMDAlarmCondition* new_configProperty)
{
    return (m_imp ? m_imp->addNew(new_configProperty) : false);
}

bool CMDAlarmConditionConfiger::delete1(const QString& name)
{
    return (m_imp ? m_imp->delete1(name) : false);
}

void CMDAlarmConditionConfiger::deleteAll()
{
    if(m_imp)
        m_imp->deleteAll();
}

CMDAlarmCondition* CMDAlarmConditionConfiger::find(const QString& name)
{
    return (m_imp ? m_imp->find(name) : NULL);
}

CMDAlarmCondition* CMDAlarmConditionConfiger::find(int index)
{
    return (m_imp ? m_imp->find(index) : NULL);
}

bool CMDAlarmConditionConfiger::modify(const CMDAlarmCondition* sourceConfig,
                                const CMDAlarmCondition* destConfig)
{
    return (m_imp ? m_imp->modify(sourceConfig, destConfig) : false);
}

void CMDAlarmConditionConfiger::deletePrefixMatched(const QString& prefix)
{
    if(m_imp)
        m_imp->deletePrefixMatched(prefix);
}
