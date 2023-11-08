#include "MDPowerGroupConfiger.h"
#include "MDPowerGroupConfigerImp.h"

CMDPowerGroupConfiger::CMDPowerGroupConfiger()
{
    m_imp = new CMDPowerGroupConfigerImp;
}

CMDPowerGroupConfiger::~CMDPowerGroupConfiger()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDPowerGroupConfiger::loadConfig(const QString& projectPath)
{
    return (m_imp ? m_imp->loadConfig(projectPath) : false);
}

void CMDPowerGroupConfiger::unloadConfig()
{
    if(m_imp)
        m_imp->unloadConfig();
}

void CMDPowerGroupConfiger::configedList(QList<CMDPowerGroup*>& powerGroupList)
{
    if(m_imp)
        m_imp->configedList(powerGroupList);
}

QString CMDPowerGroupConfiger::configFileName()
{
    return (m_imp ? m_imp->configFileName() : "");
}

bool CMDPowerGroupConfiger::saveToFile()
{
    return (m_imp ? m_imp->saveToFile() : false);
}

/*!
\section 管理权限组
*/
bool CMDPowerGroupConfiger::add(const CMDPowerGroup* configProperty)
{
    return (m_imp ? m_imp->add(configProperty) : false);
}

bool CMDPowerGroupConfiger::delete1(const QString& name)
{
    return (m_imp ? m_imp->delete1(name) : false);
}

CMDPowerGroup* CMDPowerGroupConfiger::find(const QString& name)
{
    return (m_imp ? m_imp->find(name) : NULL);
}

CMDPowerGroup* CMDPowerGroupConfiger::find(int index)
{
    return (m_imp ? m_imp->find(index) : NULL);
}

bool CMDPowerGroupConfiger::modify(const CMDPowerGroup* sourceConfig,
                                const CMDPowerGroup* destConfig)
{
    return (m_imp ? m_imp->modify(sourceConfig, destConfig) : false);
}

int CMDPowerGroupConfiger::count()
{
    return (m_imp ? m_imp->count() : 0);
}


QString CMDPowerGroupConfiger::getGroup(QString name)
{
    return (m_imp ? m_imp->getGroup(name) : "");
}
QStringList CMDPowerGroupConfiger::getGroups()
{
    return (m_imp ? m_imp->getGroups() : QStringList());
}
QStringList CMDPowerGroupConfiger::getAccountsInGroup(QString group)
{
    return (m_imp ? m_imp->getAccountsInGroup(group) : QStringList());
}
QStringList CMDPowerGroupConfiger::getDisabledGroups()
{
    return (m_imp ? m_imp->getDisabledGroups() : QStringList());
}
bool CMDPowerGroupConfiger::enableGroup(QString name)
{
    return (m_imp ? m_imp->enableGroup(name) : 0);
}
bool CMDPowerGroupConfiger::disableGroup(QString name)
{
    return (m_imp ? m_imp->disableGroup(name) : 0);
}
bool CMDPowerGroupConfiger::deleteGroup(QString name)
{
    return (m_imp ? m_imp->deleteGroup(name) : 0);
}
// modifyGroupByUI()
bool CMDPowerGroupConfiger::addAccountToGroup(QString account,QString group)
{
    return (m_imp ? m_imp->addAccountToGroup(account, group) : 0);
}
bool CMDPowerGroupConfiger::removeAccountFromGroup(QString account,QString group)
{
    return (m_imp ? m_imp->removeAccountFromGroup(account, group) : 0);
}
bool CMDPowerGroupConfiger::isAccountInGroup(QString account,QString group)
{
    return (m_imp ? m_imp->isAccountInGroup(account, group) : 0);
}
bool CMDPowerGroupConfiger::upsertGroup(QString name, QString comment, bool enable)
{
    return (m_imp ? m_imp->upsert(name, comment, enable) : 0);
}
