#include "MDEventConfiger.h"
#include "MDEventConfigerImp.h"

CMDEventConfiger::CMDEventConfiger()
{
    m_imp = new CMDEventConfigerImp;
}

CMDEventConfiger::~CMDEventConfiger()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDEventConfiger::loadConfig(const QString& projectPath)
{
    return (m_imp ? m_imp->loadConfig(projectPath) : false);
}

void CMDEventConfiger::unloadConfig()
{
    if(m_imp)
        m_imp->unloadConfig();
}

void CMDEventConfiger::configedEventList(QList<CMDEventProperty_Config*>& eventList)
{
    if(m_imp)
        m_imp->configedEventList(eventList);
}

int CMDEventConfiger::count()
{
    return (m_imp ? m_imp->count() : 0);
}

QString CMDEventConfiger::configFileName()
{
    return (m_imp ? m_imp->configFileName() : "");
}

bool CMDEventConfiger::saveToFile()
{
    return (m_imp ? m_imp->saveToFile() : false);
}

/*!
\section 管理事件
*/
bool CMDEventConfiger::addEvent(const CMDEventProperty_Config* configProperty)
{
    return (m_imp ? m_imp->addEvent(configProperty) : false);
}

bool CMDEventConfiger::deleteEvent(const QString& name)
{
    return (m_imp ? m_imp->deleteEvent(name) : false);
}

CMDEventProperty_Config* CMDEventConfiger::findEvent(const QString& name)
{
    return (m_imp ? m_imp->findEvent(name) : NULL);
}

CMDEventProperty_Config* CMDEventConfiger::findEvent(int index)
{
    return (m_imp ? m_imp->findEvent(index) : NULL);
}

bool CMDEventConfiger::modifyEvent(const CMDEventProperty_Config* sourceConfig,
                                const CMDEventProperty_Config* destConfig)
{
    return (m_imp ? m_imp->modifyEvent(sourceConfig, destConfig) : false);
}
