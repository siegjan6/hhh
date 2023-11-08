#include "MDAccountConfiger.h"
#include "MDAccountConfigerImp.h"

CMDAccountConfiger::CMDAccountConfiger()
{
    m_imp = new CMDAccountConfigerImp;
}

CMDAccountConfiger::~CMDAccountConfiger()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 文件操作：加载；卸载；保存
*/
bool CMDAccountConfiger::loadConfig(const QString& projectPath)
{
    return (m_imp ? m_imp->loadConfig(projectPath) : false);
}

void CMDAccountConfiger::unloadConfig()
{
    if(m_imp)
        m_imp->unloadConfig();
}

void CMDAccountConfiger::configedList(QList<CMDAccountProperty_Config*>& accountList)
{
    if(m_imp)
        m_imp->configedList(accountList);
}

QString CMDAccountConfiger::configFileName()
{
    return (m_imp ? m_imp->configFileName() : "");
}

bool CMDAccountConfiger::saveToFile()
{
    return (m_imp ? m_imp->saveToFile() : false);
}

/*!
\section 管理帐户
*/
bool CMDAccountConfiger::add(const CMDAccountProperty_Config* configProperty)
{
    return (m_imp ? m_imp->add(configProperty) : false);
}

bool CMDAccountConfiger::delete1(const QString& name)
{
    return (m_imp ? m_imp->delete1(name) : false);
}

CMDAccountProperty_Config* CMDAccountConfiger::find(const QString& name)
{
    return (m_imp ? m_imp->find(name) : NULL);
}

CMDAccountProperty_Config* CMDAccountConfiger::find(int index)
{
    return (m_imp ? m_imp->find(index) : NULL);
}

bool CMDAccountConfiger::modify(const CMDAccountProperty_Config* sourceConfig,
                                const CMDAccountProperty_Config* destConfig)
{
    return (m_imp ? m_imp->modify(sourceConfig, destConfig) : false);
}

int CMDAccountConfiger::count()
{
    return (m_imp ? m_imp->count() : 0);
}

QStringList CMDAccountConfiger::getAccounts()
{
    return (m_imp ? m_imp->getAccounts() : QStringList());
}
QStringList CMDAccountConfiger::getDisabledAccounts()
{
    return (m_imp ? m_imp->getDisabledAccounts() : QStringList());
}
QStringList CMDAccountConfiger::getLockedAccounts()
{
    return (m_imp ? m_imp->getLockedAccounts() : QStringList());
}
bool CMDAccountConfiger::enableAccount(QString name)
{
    return (m_imp ? m_imp->enableAccount(name) : 0);
}
bool CMDAccountConfiger::disableAccount(QString name)
{
    return (m_imp ? m_imp->disableAccount(name) : 0);
}
bool CMDAccountConfiger::lockAccount(QString name)
{
    return (m_imp ? m_imp->lockAccount(name) : 0);
}
bool CMDAccountConfiger::unlockAccount(QString name)
{
    return (m_imp ? m_imp->unlockAccount(name) : 0);
}
bool CMDAccountConfiger::isAccountEnabled(QString name)
{
    return (m_imp ? m_imp->isAccountEnabled(name) : 0);
}
bool CMDAccountConfiger::isAccountLocked(QString name)
{
    return (m_imp ? m_imp->isAccountLocked(name) : 0);
}
bool CMDAccountConfiger::isAccountLogin(QString name)
{
    return (m_imp ? m_imp->isAccountLogin(name) : 0);
}
void CMDAccountConfiger::setAccountLogin(QString name, bool logined)
{
    if(m_imp)
        return m_imp->setAccountLogin(name, logined);
}
bool CMDAccountConfiger::upsertAccount(QString name, QString comment, QString password, bool enable, bool lock)
{
    return (m_imp ? m_imp->upsert(name, comment, password, enable, lock) : 0);
}
QString CMDAccountConfiger::getAccount(QString name)
{
    return (m_imp ? m_imp->getAccount(name) : "");
}
int CMDAccountConfiger::modifyPassword(QString name, QString oldPassword, QString newPassword)
{
    return (m_imp ? m_imp->modifyPassword(name, oldPassword, newPassword) : -1);
}
