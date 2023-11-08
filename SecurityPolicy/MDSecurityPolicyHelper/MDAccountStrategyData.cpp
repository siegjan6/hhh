#include "MDAccountStrategyData.h"
#include <QJsonArray>
#include <QJsonObject>

CMDAccountStrategyData::CMDAccountStrategyData()
{
    m_version = 0;

    m_minAccountID = 1;
    m_minPassword = 0;
    m_enableDeleteAccount = true;
    m_containAlphabet = false;
    m_containNumber = false;
    m_containUppercaseLowercase = false;
    m_containExtraAlphabet = false;
    m_cantUseOldPsw = true;
    m_passwordInvalidContinueInputCount = 0;
    m_passwordValidPeriod = 0;
    m_passwordValidPeriodUnit = 2;
    m_autoUnlockPeriod = 30;
    m_autoUnlockPeriodUnit = 0;
    m_lockScreenPeriod = 30;
    m_lockScreenPeriodUnit = 0;
    m_enableModifyPasswordFirstLogin = false;
    m_pswCannotContainAccout = false;
}

CMDAccountStrategyData::~CMDAccountStrategyData()
{

}

CMDAccountStrategyData::CMDAccountStrategyData(const CMDAccountStrategyData& obj)
{
    copy(&obj);
}

CMDAccountStrategyData& CMDAccountStrategyData::operator = (const CMDAccountStrategyData& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDAccountStrategyData::copy(const CMDAccountStrategyData* source)
{
    m_version = source->m_version;

    m_minAccountID = source->m_minAccountID;
    m_minPassword = source->m_minPassword;
    m_enableDeleteAccount = source->m_enableDeleteAccount;
    m_containAlphabet = source->m_containAlphabet;
    m_containNumber = source->m_containNumber;
    m_containUppercaseLowercase = source->m_containUppercaseLowercase;
    m_containExtraAlphabet = source->m_containExtraAlphabet;
    m_cantUseOldPsw = source->m_cantUseOldPsw;
    m_passwordInvalidContinueInputCount = source->m_passwordInvalidContinueInputCount;
    m_passwordValidPeriod = source->m_passwordValidPeriod;
    m_passwordValidPeriodUnit = source->m_passwordValidPeriodUnit;
    m_autoUnlockPeriod = source->m_autoUnlockPeriod;
    m_autoUnlockPeriodUnit = source->m_autoUnlockPeriodUnit;
    m_lockScreenPeriod = source->m_lockScreenPeriod;
    m_lockScreenPeriodUnit = source->m_lockScreenPeriodUnit;
    m_enableModifyPasswordFirstLogin = source->m_enableModifyPasswordFirstLogin;
    m_pswCannotContainAccout = source->m_pswCannotContainAccout;

    return true;
}

// 按照版本序列化/反序列化
const quint8 CURRENT_VERSION_ASD = 1;
bool CMDAccountStrategyData::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_ASD : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream);
    else
        result = false;
    return result;
}
void CMDAccountStrategyData::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

// 版本 1
void CMDAccountStrategyData::serializeOnVersion_1(QDataStream& stream)
{
    stream << m_minAccountID << m_minPassword << m_enableDeleteAccount;
}
void CMDAccountStrategyData::deserializeOnVersion_1(QDataStream& stream)
{
    stream >> m_minAccountID >> m_minPassword >> m_enableDeleteAccount;
}

void CMDAccountStrategyData::serialize(QJsonObject& json)
{
    json.insert("MinAccountIDLength",m_minAccountID);
    json.insert("MinPasswordLength", m_minPassword);
    json.insert("EnableDeleteAccount", m_enableDeleteAccount);
    json.insert("ContainAlphabet",m_containAlphabet);
    json.insert("ContainNumber", m_containNumber);
    json.insert("ContainUppercaseLowercase", m_containUppercaseLowercase);
    json.insert("ContainExtraAlphabet", m_containExtraAlphabet);
    json.insert("CantUseOldPsw", m_cantUseOldPsw);
    json.insert("PasswordInvalidContinueInputCount", m_passwordInvalidContinueInputCount);
    json.insert("PasswordValidPeriod", m_passwordValidPeriod);
    json.insert("PasswordValidPeriodUnit", m_passwordValidPeriodUnit);
    json.insert("AutoUnlockPeriod", m_autoUnlockPeriod);
    json.insert("AutoUnlockPeriodUnit", m_autoUnlockPeriodUnit);
    json.insert("LockScreenPeriod", m_lockScreenPeriod);
    json.insert("LockScreenPeriodUnit", m_lockScreenPeriodUnit);
    json.insert("EnableModifyPasswordFirstLogin", m_enableModifyPasswordFirstLogin);
    json.insert("PswCannotContainAccout", m_pswCannotContainAccout);

}

void CMDAccountStrategyData::deserialize(const QJsonObject& json)
{
    if(json.contains("MinAccountIDLength"))
        m_minAccountID = json.value("MinAccountIDLength").toInt();
    if(json.contains("MinPasswordLength"))
        m_minPassword = json.value("MinPasswordLength").toInt();
    if(json.contains("EnableDeleteAccount"))
        m_enableDeleteAccount = json.value("EnableDeleteAccount").toBool();
    if(json.contains("ContainAlphabet"))
        m_containAlphabet = json.value("ContainAlphabet").toBool();
    if(json.contains("ContainNumber"))
        m_containNumber = json.value("ContainNumber").toBool();
    if(json.contains("ContainUppercaseLowercase"))
        m_containUppercaseLowercase = json.value("ContainUppercaseLowercase").toBool();
    if(json.contains("ContainExtraAlphabet"))
        m_containExtraAlphabet = json.value("ContainExtraAlphabet").toBool();
    if(json.contains("CantUseOldPsw"))
        m_cantUseOldPsw = json.value("CantUseOldPsw").toBool();
    if(json.contains("PasswordInvalidContinueInputCount"))
        m_passwordInvalidContinueInputCount = json.value("PasswordInvalidContinueInputCount").toInt();
    if(json.contains("PasswordValidPeriod"))
        m_passwordValidPeriod = json.value("PasswordValidPeriod").toInt();
    if(json.contains("PasswordValidPeriodUnit"))
        m_passwordValidPeriodUnit = json.value("PasswordValidPeriodUnit").toInt();
    if(json.contains("AutoUnlockPeriod"))
        m_autoUnlockPeriod = json.value("AutoUnlockPeriod").toInt();
    if(json.contains("AutoUnlockPeriodUnit"))
        m_autoUnlockPeriodUnit = json.value("AutoUnlockPeriodUnit").toInt();
    if(json.contains("LockScreenPeriod"))
        m_lockScreenPeriod = json.value("LockScreenPeriod").toInt();
    if(json.contains("LockScreenPeriodUnit"))
        m_lockScreenPeriodUnit = json.value("LockScreenPeriodUnit").toInt();
    if(json.contains("EnableModifyPasswordFirstLogin"))
        m_enableModifyPasswordFirstLogin = json.value("EnableModifyPasswordFirstLogin").toBool();
    if(json.contains("PswCannotContainAccout"))
        m_pswCannotContainAccout = json.value("PswCannotContainAccout").toBool();
}
