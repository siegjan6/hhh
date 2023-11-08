/*!

\author dGFuZ3p3

\section 功能

安全策略专家类，实现运行时的安全策略处理。

*/

#ifndef MDSECURITYPOLICYEXPERT_H
#define MDSECURITYPOLICYEXPERT_H

#include "MDSecurityPolicyConstant.h"
#include "MDSecurityPolicyExpert_Global.h"
#include <QStringList>

class CMDSecurityPolicyConfiger;
class CMDSecurityPolicyContacter;
class CMDSecurityPolicyExpertImp;
class CMDActionSPSession;
class MDSECURITYPOLICYEXPERTSHARED_EXPORT CMDSecurityPolicyExpert
{
public:
    CMDSecurityPolicyExpert();
    ~CMDSecurityPolicyExpert();
/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, CMDSecurityPolicyConfiger* configer, CMDSecurityPolicyContacter* contacter);
    void load();
    void unload();
public:
    CMDSecurityPolicyConfiger* configer();

/*!
\section 登录
*/
public:
    MDSecurityResultCode login(const QString& account, const QString& password, int checkGroup = 0);
    void loginByAccount(const QString& name);
    bool login1(const QString& account, const QString& password, int checkGroup = 0);
    void logout();
    QString currentAccount();
    MDSecurityResultCode currentLoginResult();
    int accountsCount();
    QStringList getAccounts();
    QStringList getAccountGroups(QString name);
    QStringList getDisabledAccounts();
    QStringList getLockedAccounts();
    bool enableAccount(QString name);
    bool disableAccount(QString name);
    int deleteAccount(QString name);
    bool lockAccount(QString name);
    bool unlockAccount(QString name);
    bool isAccountEnabled(QString name);
    bool isAccountLocked(QString name);
    bool isAccountLogin(QString name);
    bool upsertAccount(QString name, QString comment, QString password, bool enable, bool lock);
    QString getAccount(QString name);
    int modifyPassword(QString name, QString oldPassword, QString newPassword);
    bool resetAccount(QString name);
    bool resetAllAccount();
    QString getAccountOption();
    bool setAccountOption(QString option);


    int groupsCount();
    QString getGroup(QString name);
    QStringList getGroups();
    QStringList getAccountsInGroup(QString group);
    QStringList getDisabledGroups();
    bool enableGroup(QString name);
    bool disableGroup(QString name);
    bool deleteGroup(QString name);
    // modifyGroupByUI()
    bool addAccountToGroup(QString account,QString group);
    bool removeAccountFromGroup(QString account,QString group);
    bool isAccountInGroup(QString account,QString group);
    bool upsertGroup(QString name, QString comment, bool enable);

    bool commit();

    int accountSignature(QString actionInfo,QString sigConf);

    bool userValidation(const QString &name, const QString &password);

/*!
\section 安全验证
*/
public:
    bool isCurrentAccountHasPower(CMDActionSPSession* spSession);
public:
    bool checkSPSession_NSig(CMDActionSPSession* spSession);
public:
    bool checkSPSession_ESig_Operator(CMDActionSPSession* spSession);
    bool checkSPSession_ESig_Verifier(CMDActionSPSession* spSession);

    bool logSPSession(CMDActionSPSession* spSession);
protected:
    CMDSecurityPolicyExpertImp* m_imp;
};

#endif // MDSECURITYPOLICYEXPERT_H
