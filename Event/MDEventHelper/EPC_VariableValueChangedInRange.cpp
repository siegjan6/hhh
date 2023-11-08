#include "EPC_VariableValueChangedInRange.h"
#include "MDEventPropertyData.h"

// 变量数值变化范围
CEPC_VariableValueChangedInRange::CEPC_VariableValueChangedInRange()
    : CMDEventProperty_Config(MDEventType::variable_ValueChangedInRange)
{
    m_data1 = new CEPCData_VariableValueChangedInRange;
}

CEPC_VariableValueChangedInRange::~CEPC_VariableValueChangedInRange()
{
    if(m_data1)
        delete m_data1;
}

CEPC_VariableValueChangedInRange::CEPC_VariableValueChangedInRange(const CEPC_VariableValueChangedInRange& obj)
    : CMDEventProperty_Config(obj)
{
    m_data1 = new CEPCData_VariableValueChangedInRange;

    copy(&obj); // 基类属性被重复拷贝1次
}

CEPC_VariableValueChangedInRange& CEPC_VariableValueChangedInRange::operator = (const CEPC_VariableValueChangedInRange& obj)
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
CValueRangeCondition& CEPC_VariableValueChangedInRange::rangeCondition() const
{
    return (m_data1 ? m_data1->m_rangeCondition : EMPTY_RANGECONDITION);
}
void CEPC_VariableValueChangedInRange::setRangeCondition(const CValueRangeCondition& rangeCondition)
{
    if(m_data1)
        m_data1->m_rangeCondition = rangeCondition;
}

QString CEPC_VariableValueChangedInRange::condition() const
{
    return rangeCondition().toExpression(objName());
}

bool CEPC_VariableValueChangedInRange::copy(const CMDEventProperty_Config* source)
{
    CMDEventProperty_Config::copy(source);

    return (m_data1 ? m_data1->copy(((CEPC_VariableValueChangedInRange*)source)->m_data1) : false);
}

/*!
\section 序列化
*/
bool CEPC_VariableValueChangedInRange::serialize(QDataStream& stream, bool useCurrentVersion)
{
    if(!CMDEventProperty_Config::serialize(stream, useCurrentVersion))
        return false;

    return (m_data1 ? m_data1->serialize(stream, useCurrentVersion) : false);
}

void CEPC_VariableValueChangedInRange::deserialize(QDataStream& stream)
{
    CMDEventProperty_Config::deserialize(stream);

    if(m_data1)
        m_data1->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CEPC_VariableValueChangedInRange::serialize(QJsonObject& json)
{
    CMDEventProperty_Config::serialize(json);

    if(m_data1)
        m_data1->serialize(json);
}

void CEPC_VariableValueChangedInRange::deserialize(const QJsonObject& json)
{
    CMDEventProperty_Config::deserialize(json);

    if(m_data1)
        m_data1->deserialize(json);
}
