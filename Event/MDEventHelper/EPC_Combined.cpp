#include "EPC_Combined.h"
#include "MDEventPropertyData.h"

CEPC_Combined::CEPC_Combined()
    : CMDEventProperty_Config(MDEventType::combined)
{
    m_data1 = new CEPCData_Combined;
}

CEPC_Combined::~CEPC_Combined()
{
    if(m_data1)
        delete m_data1;
}

CEPC_Combined::CEPC_Combined(const CEPC_Combined& obj)
    : CMDEventProperty_Config(obj)
{
    m_data1 = new CEPCData_Combined;

    copy(&obj); // 基类属性被重复拷贝1次
}

CEPC_Combined& CEPC_Combined::operator = (const CEPC_Combined& obj)
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
QString CEPC_Combined::condition() const
{
    return expressionString();
}

bool CEPC_Combined::copy(const CMDEventProperty_Config* source)
{
    CMDEventProperty_Config::copy(source);

    return (m_data1 ? m_data1->copy(((CEPC_Combined*)source)->m_data1) : false);
}

/*!
\section 序列化
*/
bool CEPC_Combined::serialize(QDataStream& stream, bool useCurrentVersion)
{
    if(!CMDEventProperty_Config::serialize(stream, useCurrentVersion))
        return false;

    return (m_data1 ? m_data1->serialize(stream, useCurrentVersion) : false);
}

void CEPC_Combined::deserialize(QDataStream& stream)
{
    CMDEventProperty_Config::deserialize(stream);

    if(m_data1)
        m_data1->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CEPC_Combined::serialize(QJsonObject& json)
{
    CMDEventProperty_Config::serialize(json);

    if(m_data1)
        m_data1->serialize(json);
}

void CEPC_Combined::deserialize(const QJsonObject& json)
{
    CMDEventProperty_Config::deserialize(json);

    if(m_data1)
        m_data1->deserialize(json);
}
