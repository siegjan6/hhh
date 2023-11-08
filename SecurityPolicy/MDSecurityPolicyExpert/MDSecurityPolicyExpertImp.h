/*!

\author dGFuZ3p3

\section 功能

安全策略专家类的实现类，实现运行时的安全策略处理。

*/

#ifndef MDSECURITYPOLICYEXPERTIMP_H
#define MDSECURITYPOLICYEXPERTIMP_H

#include <QString>
#include "MDSecurityPolicyConstant.h"
#include "MDActionSPSession.h"

class CMDSecurityPolicyConfiger;
class CMDSecurityPolicyContacter;
class CMDActionSPSession;
class CMDPowerGroup;
class MMKV;
class CMDSecurityPolicyExpertImp
{
public:
    CMDSecurityPolicyExpertImp();
    ~CMDSecurityPolicyExpertImp();
/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, CMDSecurityPolicyConfiger* configer, CMDSecurityPolicyContacter* contacter);
    void load();
    void unload();
public:
    CMDSecurityPolicyConfiger* m_configer ;// NULL;
protected:
    CMDSecurityPolicyContacter* m_contacter ;// NULL;
    QString m_pid;

/*!
\section 登录
*/
public:
    // accounts
    MDSecurityResultCode login(const QString& account, const QString& password, int checkGroup = 0);
    void loginByAccount(const QString& name);
    void logout();
    int accountsCount();
    QString currentAccount();
    MDSecurityResultCode currentLoginResult();
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
    QString getAccount(QString account);
    int modifyPassword(QString account, QString oldPassword, QString newPassword);
    bool resetAccount(QString name);
    bool resetAllAccount();
    QString getAccountOption();
    bool setAccountOption(QString option);
    QString getPasswordOption();
    bool setPasswordOption(QString option);
    MDSecurityResultCode userValidation(const QString &name, const QString &password);

    // groups
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

protected:
    CMDSigInput m_currentAccount;
    bool log_login(const QString& account, MDSecurityResultCode result, QString& retMsg);

/*!
\section 安全验证
*/
public:
    bool checkSPSession_NSig(CMDActionSPSession* spSession);
    bool log_NSig(CMDActionSPSession* spSession, bool result);
    bool logSPSession(CMDActionSPSession* spSession);
public:
    bool isCurrentAccountHasPower(CMDActionSPSession* spSession);
protected:
    bool helper_CheckSPSession_Operator(CMDActionSPSession* spSession);
    bool helper_CheckSPSession_Verifier(CMDActionSPSession* spSession);
public:
    bool checkSPSession_ESig_Operator(CMDActionSPSession* spSession);
    bool checkSPSession_ESig_Verifier(CMDActionSPSession* spSession);
    bool log_ESig_Operator(CMDActionSPSession* spSession, bool result);
    bool log_ESig_Verifier(CMDActionSPSession* spSession, bool result);
public:
    MDSecurityResultCode checkAccount(const QString& account, const QString& password, bool isNoNeedPassword = false);
    MDSecurityResultCode checkPower(const QString& account, const QStringList& powerGroupList);
    MDSecurityResultCode checkSig(const CMDSigInput& input, const QStringList& powerGroupList,
                                                            CMDSigOutput& output);
private:
    QString getAuthErrorMsg(MDSecurityResultCode errcode);

    CMDActionSPSession lastSPSession_Operator;

    MDSecurityResultCode m_currentLoginResult;

    MMKV* m_mmkv;
    MMKV* m_mmkv_by_prj;
    bool loadKV();
};

#endif // MDSECURITYPOLICYEXPERTIMP_H
