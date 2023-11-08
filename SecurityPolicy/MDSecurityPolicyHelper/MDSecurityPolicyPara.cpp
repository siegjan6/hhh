#include "MDSecurityPolicyPara.h"
#include "MDSecurityPolicyParaData.h"

CMDSecurityPolicyPara::CMDSecurityPolicyPara()
{
    m_data = new CMDSecurityPolicyParaData;
}

CMDSecurityPolicyPara::~CMDSecurityPolicyPara()
{
    if(m_data)
        delete m_data;
}

CMDSecurityPolicyPara::CMDSecurityPolicyPara(const CMDSecurityPolicyPara& obj)
{
    m_data = new CMDSecurityPolicyParaData();

    copy(&obj);
}

CMDSecurityPolicyPara& CMDSecurityPolicyPara::operator = (const CMDSecurityPolicyPara& obj)
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
bool CMDSecurityPolicyPara::isEnableSP() const
{
    return (m_data ? m_data->m_enableSP : false);
}
void CMDSecurityPolicyPara::setEnableSP(bool enable)
{
    if(m_data)
        m_data->m_enableSP = enable;
}

bool CMDSecurityPolicyPara::isEnableLoginBeforeRun() const
{
    return (m_data ? m_data->m_enableLoginBeforeRun : false);
}
void CMDSecurityPolicyPara::setEnableLoginBeforeRun(bool enable)
{
    if(m_data)
        m_data->m_enableLoginBeforeRun = enable;
}

bool CMDSecurityPolicyPara::copy(const CMDSecurityPolicyPara* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

/*!
\section 序列化
*/
bool CMDSecurityPolicyPara::serialize(QDataStream& stream, bool useCurrentVersion)
{
    return (m_data ? m_data->serialize(stream, useCurrentVersion) : false);
}

void CMDSecurityPolicyPara::deserialize(QDataStream& stream)
{
    if(m_data)
        m_data->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CMDSecurityPolicyPara::serialize(QJsonObject& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CMDSecurityPolicyPara::deserialize(const QJsonObject& json)
{
    if(m_data)
        m_data->deserialize(json);
}
