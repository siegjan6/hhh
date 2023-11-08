#include "EPC_VariableValueWrite.h"
#include "MDEventPropertyData.h"

// 变量写
CEPC_VariableValueWrite::CEPC_VariableValueWrite()
    : CMDEventProperty_Config(MDEventType::variable_QualityChanged)
{
    m_data1 = new CEPCData_VariableValueWrite;
}

CEPC_VariableValueWrite::~CEPC_VariableValueWrite()
{
    if(m_data1)
        delete m_data1;
}

CEPC_VariableValueWrite::CEPC_VariableValueWrite(const CEPC_VariableValueWrite& obj)
    : CMDEventProperty_Config(obj)
{
    m_data1 = new CEPCData_VariableValueWrite;

    copy(&obj); // 基类属性被重复拷贝1次
}

CEPC_VariableValueWrite& CEPC_VariableValueWrite::operator = (const CEPC_VariableValueWrite& obj)
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
quint8 CEPC_VariableValueWrite::checkConditionBeforeOrAfterWrite() const
{
    return (m_data1 ? m_data1->m_checkConditionBeforeOrAfterWrite : 0);
}
void CEPC_VariableValueWrite::setCheckConditionBeforeOrAfterWrite(quint8 flag)
{
    if(m_data1)
        m_data1->m_checkConditionBeforeOrAfterWrite = flag;
}

CValueRangeCondition& CEPC_VariableValueWrite::rangeCondition() const
{
    return (m_data1 ? m_data1->m_rangeCondition : EMPTY_RANGECONDITION);
}
void CEPC_VariableValueWrite::setRangeCondition(const CValueRangeCondition& rangeCondition)
{
    if(m_data1)
        m_data1->m_rangeCondition = rangeCondition;
}

QString CEPC_VariableValueWrite::condition() const
{
    return rangeCondition().toExpression(objName());
}

bool CEPC_VariableValueWrite::copy(const CMDEventProperty_Config* source)
{
    CMDEventProperty_Config::copy(source);

    return (m_data1 ? m_data1->copy(((CEPC_VariableValueWrite*)source)->m_data1) : false);
}

/*!
\section 序列化
*/
bool CEPC_VariableValueWrite::serialize(QDataStream& stream, bool useCurrentVersion)
{
    if(!CMDEventProperty_Config::serialize(stream, useCurrentVersion))
        return false;

    return (m_data1 ? m_data1->serialize(stream, useCurrentVersion) : false);
}

void CEPC_VariableValueWrite::deserialize(QDataStream& stream)
{
    CMDEventProperty_Config::deserialize(stream);

    if(m_data1)
        m_data1->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CEPC_VariableValueWrite::serialize(QJsonObject& json)
{
    CMDEventProperty_Config::serialize(json);

    if(m_data1)
        m_data1->serialize(json);
}

void CEPC_VariableValueWrite::deserialize(const QJsonObject& json)
{
    CMDEventProperty_Config::deserialize(json);

    if(m_data1)
        m_data1->deserialize(json);
}
