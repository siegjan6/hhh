/*!

\author dGFuZ3p3

\section 功能

帐户配置类，负责存储到文件及从文件加载。

*/

#ifndef MDACCOUNTCONFIGER_H
#define MDACCOUNTCONFIGER_H

#include <QString>
#include "MDAccountConfiger_Global.h"

class CMDAccountProperty_Config;
class CMDAccountConfigerImp;
class MDACCOUNTCONFIGERSHARED_EXPORT CMDAccountConfiger
{
public:
    CMDAccountConfiger();
    ~CMDAccountConfiger();

/*!
\section 文件操作：加载；卸载；保存
*/
public:
    bool loadConfig(const QString& projectPath);
    void unloadConfig();
    void configedList(QList<CMDAccountProperty_Config*>& accountList);
    QString configFileName();
    bool saveToFile();

/*!
\section 管理帐户
*/
public:
    bool add(const CMDAccountProperty_Config* configProperty);
    bool delete1(const QString& name);
    CMDAccountProperty_Config* find(const QString& name);
    CMDAccountProperty_Config* find(int index);
    bool modify(const CMDAccountProperty_Config* sourceConfig,
                                    const CMDAccountProperty_Config* destConfig);
    int count();
    QStringList getAccounts();
    QStringList getDisabledAccounts();
    QStringList getLockedAccounts();
    bool enableAccount(QString name);
    bool disableAccount(QString name);
    bool lockAccount(QString name);
    bool unlockAccount(QString name);
    bool isAccountEnabled(QString name);
    bool isAccountLocked(QString name);
    bool isAccountLogin(QString name);
    void setAccountLogin(QString name, bool logined);
    bool upsertAccount(QString name, QString comment, QString password, bool enable, bool lock);
    QString getAccount(QString name);
    int modifyPassword(QString name, QString oldPassword, QString newPassword);
protected:
    CMDAccountConfigerImp* m_imp;
};

#endif // MDACCOUNTCONFIGER_H
