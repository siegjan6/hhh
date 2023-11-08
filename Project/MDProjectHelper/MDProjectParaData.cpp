#include "MDProjectParaData.h"
#include <QDebug>

CMDProjectParaData::CMDProjectParaData()
{
    m_version = 0;
    m_enableDeviceCloudClient = false;
    m_cloudClientID = "";
    m_port = 4840;
    /// ConnectAuthMode 组合
    m_caMode = 1;
    m_securityPolicy = SecurityPolicy::NONE;
    m_messageSecurityMode = MessageSecurityMode::NONE;
    m_user = "";
    m_password = "";
    m_certificatePath = "";
    m_privateKeyPath = "";
}

CMDProjectParaData::~CMDProjectParaData()
{
    m_spList.deleteAll();
}

CMDProjectParaData::CMDProjectParaData(const CMDProjectParaData& obj)
{
    copy(&obj);
}

CMDProjectParaData& CMDProjectParaData::operator = (const CMDProjectParaData& obj)
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
bool CMDProjectParaData::copy(const CMDProjectParaData* source)
{
    m_version = source->m_version;

    m_spList = source->m_spList;
    m_enableDeviceCloudClient = source->m_enableDeviceCloudClient;
    m_cloudClientID = source->m_cloudClientID;
    m_port = source->m_port;
    /// ConnectAuthMode 组合
    m_caMode = source->m_caMode;
    m_securityPolicy = source->m_securityPolicy;
    m_messageSecurityMode = source->m_messageSecurityMode;
    m_user = source->m_user;
    m_password = source->m_password;
    m_certificatePath = source->m_certificatePath;
    m_privateKeyPath = source->m_privateKeyPath;

    return true;
}

/*!
\section 序列化
*/

// 按照版本序列化/反序列化
const quint8 CURRENT_VERSION_PPD = 3;
bool CMDProjectParaData::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_PPD : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream, useCurrentVersion);
    else if(actualVersion == 2)
        serializeOnVersion_2(stream, useCurrentVersion);
    else if(actualVersion == 3) {
        serializeOnVersion_2(stream, useCurrentVersion);
        stream << m_port;
        /// ConnectAuthMode 组合
        stream << m_caMode;
        stream << (quint16)m_securityPolicy;
        stream << (quint16)m_messageSecurityMode;
        stream << m_user;
        stream << m_password;
        stream << m_certificatePath;
        stream << m_privateKeyPath;
        qDebug() << "CMDProjectParaData::serialize";
        qDebug() << m_port << m_caMode << (quint16)m_securityPolicy << (quint16)m_messageSecurityMode << m_user << m_password << m_certificatePath << m_privateKeyPath;
    }
    else
        result = false;
    return result;
}
void CMDProjectParaData::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else if(m_version == 2)
        deserializeOnVersion_2(stream);
    else if(m_version == 3) {
        deserializeOnVersion_2(stream);
        stream >> m_port;
        /// ConnectAuthMode 组合
        stream >> m_caMode;
        quint16 securityPolicy = 0;
        stream >> securityPolicy;
        quint16 messageSecurityMode = 1;
        stream >> messageSecurityMode;
        stream >> m_user;
        stream >> m_password;
        stream >> m_certificatePath;
        stream >> m_privateKeyPath;

        m_securityPolicy = (SecurityPolicy)securityPolicy;
        m_messageSecurityMode = (MessageSecurityMode)messageSecurityMode;

        qDebug() << "CMDProjectParaData::deserialize";
        qDebug() << m_port << m_caMode << (quint16)m_securityPolicy << (quint16)m_messageSecurityMode << m_user << m_password << m_certificatePath << m_privateKeyPath;
    }
    else{}
}

// 版本 1
void CMDProjectParaData::serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion)
{
    m_spList.serialize(stream, useCurrentVersion);
}
void CMDProjectParaData::deserializeOnVersion_1(QDataStream& stream)
{
    m_spList.deserialize(stream);
}

// 版本 2
void CMDProjectParaData::serializeOnVersion_2(QDataStream& stream, bool useCurrentVersion)
{
    serializeOnVersion_1(stream, useCurrentVersion);

    stream << m_enableDeviceCloudClient;
    stream << m_cloudClientID;
}
void CMDProjectParaData::deserializeOnVersion_2(QDataStream& stream)
{
    deserializeOnVersion_1(stream);

    stream >> m_enableDeviceCloudClient;
    stream >> m_cloudClientID;
}
