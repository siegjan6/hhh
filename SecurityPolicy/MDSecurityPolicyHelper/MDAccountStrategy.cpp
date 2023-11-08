#include "MDAccountStrategy.h"
#include "MDAccountStrategyData.h"

CMDAccountStrategy::CMDAccountStrategy()
{
    m_data = new CMDAccountStrategyData;
}

CMDAccountStrategy::~CMDAccountStrategy()
{
    if(m_data)
        delete m_data;
}

CMDAccountStrategy::CMDAccountStrategy(const CMDAccountStrategy& obj)
{
    m_data = new CMDAccountStrategyData();

    copy(&obj);
}

CMDAccountStrategy& CMDAccountStrategy::operator = (const CMDAccountStrategy& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

/*!
\section 属性
*/
quint8 CMDAccountStrategy::minAccountID() const
{
    return (m_data ? m_data->m_minAccountID : 1);
}
void CMDAccountStrategy::setMinAccountID(quint8 min)
{
    if(m_data)
        m_data->m_minAccountID = min;
}

quint8 CMDAccountStrategy::minPassword() const
{
    return (m_data ? m_data->m_minPassword : 0);
}
void CMDAccountStrategy::setMinPassword(quint8 min)
{
    if(m_data)
        m_data->m_minPassword = min;
}

bool CMDAccountStrategy::isEnableDeleteAccount() const
{
    return (m_data ? m_data->m_enableDeleteAccount : false);
}
void CMDAccountStrategy::setEnableDeleteAccount(bool enable)
{
    if(m_data)
        m_data->m_enableDeleteAccount = enable;
}

bool CMDAccountStrategy::isEnableModifyPasswordFirstLogin() const
{
    return (m_data ? m_data->m_enableModifyPasswordFirstLogin : false);
}
void CMDAccountStrategy::setEnableModifyPasswordFirstLogin(bool enable)
{
    if(m_data)
        m_data->m_enableModifyPasswordFirstLogin = enable;
}


bool CMDAccountStrategy::containAlphabet() const
{
    return (m_data ? m_data->m_containAlphabet : false);
}
void CMDAccountStrategy::setContainAlphabet(bool enable)
{
    if(m_data)
        m_data->m_containAlphabet = enable;
}

bool CMDAccountStrategy::containNumber() const
{
    return (m_data ? m_data->m_containNumber : false);
}
void CMDAccountStrategy::setContainNumber(bool enable)
{
    if(m_data)
        m_data->m_containNumber = enable;
}

bool CMDAccountStrategy::containUppercaseLowercase() const
{
    return (m_data ? m_data->m_containUppercaseLowercase : false);
}
void CMDAccountStrategy::setContainUppercaseLowercase(bool enable)
{
    if(m_data)
        m_data->m_containUppercaseLowercase = enable;
}

bool CMDAccountStrategy::containExtraAlphabet() const
{
    return (m_data ? m_data->m_containExtraAlphabet : false);
}
void CMDAccountStrategy::setContainExtraAlphabet(bool enable)
{
    if(m_data)
        m_data->m_containExtraAlphabet = enable;
}

bool CMDAccountStrategy::cantUseOldPsw() const
{
    return (m_data ? m_data->m_cantUseOldPsw : false);
}
void CMDAccountStrategy::setCantUseOldPsw(bool enable)
{
    if(m_data)
        m_data->m_cantUseOldPsw = enable;
}

int CMDAccountStrategy::passwordInvalidContinueInputCount() const
{
    return (m_data ? m_data->m_passwordInvalidContinueInputCount : 0);
}
void CMDAccountStrategy::setPasswordInvalidContinueInputCount(int value)
{
    if(m_data)
        m_data->m_passwordInvalidContinueInputCount = value;
}

int CMDAccountStrategy::passwordValidPeriod() const
{
    return (m_data ? m_data->m_passwordValidPeriod : 0);
}
void CMDAccountStrategy::setPasswordValidPeriod(int value)
{
    if(m_data)
        m_data->m_passwordValidPeriod = value;
}

int CMDAccountStrategy::passwordValidPeriodUnit() const
{
    return (m_data ? m_data->m_passwordValidPeriodUnit : 0);
}
void CMDAccountStrategy::setPasswordValidPeriodUnit(int value)
{
    if(m_data)
        m_data->m_passwordValidPeriodUnit = value;
}

int CMDAccountStrategy::autoUnlockPeriod() const
{
    return (m_data ? m_data->m_autoUnlockPeriod : 0);
}
void CMDAccountStrategy::setAutoUnlockPeriod(int value)
{
    if(m_data)
        m_data->m_autoUnlockPeriod = value;
}

int CMDAccountStrategy::autoUnlockPeriodUnit() const
{
    return (m_data ? m_data->m_autoUnlockPeriodUnit : 0);
}
void CMDAccountStrategy::setAutoUnlockPeriodUnit(int value)
{
    if(m_data)
        m_data->m_autoUnlockPeriodUnit = value;
}

int CMDAccountStrategy::lockScreenPeriod() const
{
    return (m_data ? m_data->m_lockScreenPeriod : 0);
}
void CMDAccountStrategy::setLockScreenPeriod(int value)
{
    if(m_data)
        m_data->m_lockScreenPeriod = value;
}

int CMDAccountStrategy::lockScreenPeriodUnit() const
{
    return (m_data ? m_data->m_lockScreenPeriodUnit : 0);
}
void CMDAccountStrategy::setLockScreenPeriodUnit(int value)
{
    if(m_data)
        m_data->m_lockScreenPeriodUnit = value;
}

bool CMDAccountStrategy::pswCannotContainAccout() const
{
    return (m_data ? m_data->m_pswCannotContainAccout : 0);
}
void CMDAccountStrategy::setPswCannotContainAccout(bool enable)
{
    if(m_data)
        m_data->m_pswCannotContainAccout = enable;
}


bool CMDAccountStrategy::copy(const CMDAccountStrategy* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

/*!
\section 序列化
*/
bool CMDAccountStrategy::serialize(QDataStream& stream, bool useCurrentVersion)
{
    return (m_data ? m_data->serialize(stream, useCurrentVersion) : false);
}

void CMDAccountStrategy::deserialize(QDataStream& stream)
{
    if(m_data)
        m_data->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CMDAccountStrategy::serialize(QJsonObject& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CMDAccountStrategy::deserialize(const QJsonObject& json)
{
    if(m_data)
        m_data->deserialize(json);
}

