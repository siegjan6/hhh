#include "EPC_VariableQualityChanged.h"
#include "MDEventPropertyData.h"

// 变量品质变化
CEPC_VariableQualityChanged::CEPC_VariableQualityChanged()
    : CMDEventProperty_Config(MDEventType::variable_QualityChanged)
{
    m_data1 = new CEPCData_VariableQualityChanged;
}

CEPC_VariableQualityChanged::~CEPC_VariableQualityChanged()
{
    if(m_data1)
        delete m_data1;
}

CEPC_VariableQualityChanged::CEPC_VariableQualityChanged(const CEPC_VariableQualityChanged& obj)
    : CMDEventProperty_Config(obj)
{
    m_data1 = new CEPCData_VariableQualityChanged;

    copy(&obj); // 基类属性被重复拷贝1次
}

CEPC_VariableQualityChanged& CEPC_VariableQualityChanged::operator = (const CEPC_VariableQualityChanged& obj)
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
CValueRangeCondition& CEPC_VariableQualityChanged::rangeCondition() const
{
    return (m_data1 ? m_data1->m_rangeCondition : EMPTY_RANGECONDITION);
}
void CEPC_VariableQualityChanged::setRangeCondition(const CValueRangeCondition& rangeCondition)
{
    if(m_data1)
        m_data1->m_rangeCondition = rangeCondition;
}

QString CEPC_VariableQualityChanged::condition() const
{
    return rangeCondition().toExpression(objName());
}

bool CEPC_VariableQualityChanged::copy(const CMDEventProperty_Config* source)
{
    CMDEventProperty_Config::copy(source);

    return (m_data1 ? m_data1->copy(((CEPC_VariableQualityChanged*)source)->m_data1) : false);
}

/*!
\section 序列化
*/
bool CEPC_VariableQualityChanged::serialize(QDataStream& stream, bool useCurrentVersion)
{
    if(!CMDEventProperty_Config::serialize(stream, useCurrentVersion))
        return false;

    return (m_data1 ? m_data1->serialize(stream, useCurrentVersion) : false);
}

void CEPC_VariableQualityChanged::deserialize(QDataStream& stream)
{
    CMDEventProperty_Config::deserialize(stream);

    if(m_data1)
        m_data1->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CEPC_VariableQualityChanged::serialize(QJsonObject& json)
{
    CMDEventProperty_Config::serialize(json);

    if(m_data1)
        m_data1->serialize(json);
}

void CEPC_VariableQualityChanged::deserialize(const QJsonObject& json)
{
    CMDEventProperty_Config::deserialize(json);

    if(m_data1)
        m_data1->deserialize(json);
}
