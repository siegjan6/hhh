/*!

\author dGFuZ3p3

\section 功能

帐户策略数据类，包含如最小帐户长度、最小密码长度等 。

*/

#ifndef MDACCOUNTSTRATEGY_H
#define MDACCOUNTSTRATEGY_H

#include <QString>
#include <QDateTime>
#include <QVariant>
#include "MDSecurityPolicyHelper_Global.h"

class CMDAccountStrategyData;
class MDSECURITYPOLICYHELPERSHARED_EXPORT CMDAccountStrategy
{
public:
    CMDAccountStrategy();
    virtual ~CMDAccountStrategy();
   CMDAccountStrategy(const CMDAccountStrategy& obj);
   CMDAccountStrategy& operator = (const CMDAccountStrategy& obj);
/*!
\section 属性
*/
public:
    quint8 minAccountID() const;
    void setMinAccountID(quint8 min);

    quint8 minPassword() const;
    void setMinPassword(quint8 min);
public:
    bool isEnableDeleteAccount() const;
    void setEnableDeleteAccount(bool enable);

    bool isEnableModifyPasswordFirstLogin() const;
    void setEnableModifyPasswordFirstLogin(bool enable);

public:
    bool containAlphabet() const;
    void setContainAlphabet(bool enable);

    bool containNumber() const;
    void setContainNumber(bool enable);

    bool containUppercaseLowercase() const;
    void setContainUppercaseLowercase(bool enable);

    bool containExtraAlphabet() const;
    void setContainExtraAlphabet(bool enable);

    bool cantUseOldPsw() const;
    void setCantUseOldPsw(bool enable);

    int passwordInvalidContinueInputCount() const;
    void setPasswordInvalidContinueInputCount(int value);

    int passwordValidPeriod() const;
    void setPasswordValidPeriod(int value);

    int passwordValidPeriodUnit() const;
    void setPasswordValidPeriodUnit(int value);

    int autoUnlockPeriod() const;
    void setAutoUnlockPeriod(int value);

    int autoUnlockPeriodUnit() const;
    void setAutoUnlockPeriodUnit(int value);

    int lockScreenPeriod() const;
    void setLockScreenPeriod(int value);

    int lockScreenPeriodUnit() const;
    void setLockScreenPeriodUnit(int value);

    bool pswCannotContainAccout() const;
    void setPswCannotContainAccout(bool enable);

public:
    virtual bool copy(const CMDAccountStrategy* source);

/*!
\section 序列化
*/
public:
    virtual bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    virtual void deserialize(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
protected:
    CMDAccountStrategyData* m_data;
};

#endif // MDACCOUNTSTRATEGY_H
