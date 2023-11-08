#ifndef MDACCOUNTSTRATEGYDATA_H
#define MDACCOUNTSTRATEGYDATA_H

#include <QString>
#include <QDateTime>
#include <QVariant>
#include <QDataStream>

class CMDAccountStrategyData
{
public:
    CMDAccountStrategyData();
    ~CMDAccountStrategyData();
    CMDAccountStrategyData(const CMDAccountStrategyData& obj);
    CMDAccountStrategyData& operator = (const CMDAccountStrategyData& obj);
public:
    // 账户最小长度
    quint8 m_minAccountID;
    // 密码最小长度
    quint8 m_minPassword;
    // 密码包含字母
    bool m_containAlphabet;
    // 密码包含数字
    bool m_containNumber;
    // 密码包含大小写
    bool m_containUppercaseLowercase;
    // 密码包含特殊字符
    bool m_containExtraAlphabet;
    // 可使用曾经使用过的密码
    bool m_cantUseOldPsw;
    // 连续N次输入错误密码后，自动锁定[0=不自动锁定]
    int m_passwordInvalidContinueInputCount;
    // 密码有效期
    int m_passwordValidPeriod;
    // 0-minite 1-hour 2-day 3-month
    int m_passwordValidPeriodUnit;

    // 锁定经过以下时间自动解锁[0=必须手动解锁]
    int m_autoUnlockPeriod;
    // 0-minite 1-hour 2-day 3-month
    int m_autoUnlockPeriodUnit;

    // 锁屏时间，目前由gd处理
    int m_lockScreenPeriod;
    // 0-minite 1-hour 2-day 3-month
    int m_lockScreenPeriodUnit;
    // 允许删除账户
    bool m_enableDeleteAccount;
    // 首次登陆必须修改密码
    bool m_enableModifyPasswordFirstLogin;
    // 密码不可包含账户名
    bool m_pswCannotContainAccout;
public:
    bool copy(const CMDAccountStrategyData* source);
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void serializeOnVersion_1(QDataStream& stream);
public:
    void deserialize(QDataStream& stream);
    void deserializeOnVersion_1(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    quint8 m_version;
};

#endif // MDACCOUNTSTRATEGYDATA_H
