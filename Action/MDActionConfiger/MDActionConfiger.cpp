#include "MDActionConfiger.h"
#include "MDActionConfigerImp.h"

CMDActionConfiger::CMDActionConfiger()
{
    m_imp = new CMDActionConfigerImp;
}

CMDActionConfiger::~CMDActionConfiger()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDActionConfiger::loadConfig(const QString& projectPath)
{
    return (m_imp ? m_imp->loadConfig(projectPath) : false);
}

void CMDActionConfiger::unloadConfig()
{
    if(m_imp)
        m_imp->unloadConfig();
}

QString CMDActionConfiger::configFileName()
{
    return (m_imp ? m_imp->configFileName() : "");
}

bool CMDActionConfiger::saveToFile()
{
    return (m_imp ? m_imp->saveToFile() : false);
}

/*!
\section 管理动作
*/
bool CMDActionConfiger::addAction(const CMDActionProperty_Config* configProperty)
{
    return (m_imp ? m_imp->addAction(configProperty) : false);
}

bool CMDActionConfiger::deleteAction(const QString& name)
{
    return (m_imp ? m_imp->deleteAction(name) : false);
}

CMDActionProperty_Config* CMDActionConfiger::findAction(const QString& name)
{
    return (m_imp ? m_imp->findAction(name) : NULL);
}

CMDActionProperty_Config* CMDActionConfiger::findAction(int index)
{
    return (m_imp ? m_imp->findAction(index) : NULL);
}

bool CMDActionConfiger::modifyAction(const CMDActionProperty_Config* sourceConfig,
                                const CMDActionProperty_Config* destConfig)
{
    return (m_imp ? m_imp->modifyAction(sourceConfig, destConfig) : false);
}

void CMDActionConfiger::configedActionList(QList<CMDActionProperty_Config*>& actionList)
{
    if(m_imp)
        m_imp->configedActionList(actionList);
}

int CMDActionConfiger::count()
{
    return (m_imp ? m_imp->count() : 0);
}
