#include "MDSecurityPolicyParaData.h"
#include <QJsonArray>
#include <QJsonObject>

CMDSecurityPolicyParaData::CMDSecurityPolicyParaData()
{
    m_version = 0;

    m_enableSP = true;
    m_enableLoginBeforeRun = false;
}

CMDSecurityPolicyParaData::~CMDSecurityPolicyParaData()
{

}

CMDSecurityPolicyParaData::CMDSecurityPolicyParaData(const CMDSecurityPolicyParaData& obj)
{
    copy(&obj);
}

CMDSecurityPolicyParaData& CMDSecurityPolicyParaData::operator = (const CMDSecurityPolicyParaData& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDSecurityPolicyParaData::copy(const CMDSecurityPolicyParaData* source)
{
    m_version = source->m_version;

    m_enableSP = source->m_enableSP;
    m_enableLoginBeforeRun = source->m_enableLoginBeforeRun;

    return true;
}

// 按照版本序列化/反序列化
const quint8 CURRENT_VERSION_SPPD = 1;
bool CMDSecurityPolicyParaData::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_SPPD : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream, useCurrentVersion);
    else
        result = false;
    return result;
}
void CMDSecurityPolicyParaData::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

// 版本 1
void CMDSecurityPolicyParaData::serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion)
{
    stream << m_enableSP << m_enableLoginBeforeRun;
}
void CMDSecurityPolicyParaData::deserializeOnVersion_1(QDataStream& stream)
{
    stream >> m_enableSP >> m_enableLoginBeforeRun;
}

void CMDSecurityPolicyParaData::serialize(QJsonObject& json)
{
    json.insert("EnableSP", m_enableSP);
    json.insert("EnableLoginBeforeRun", m_enableLoginBeforeRun);
}

void CMDSecurityPolicyParaData::deserialize(const QJsonObject& json)
{
    m_enableSP = json.value("EnableSP").toBool();
    m_enableLoginBeforeRun = json.value("EnableLoginBeforeRun").toBool();
}
