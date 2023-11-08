#include "MDEventHelperImp.h"
#include "../../include/MDRTDBManager/MDRTDBManagerFactory.h"
#include <QJsonArray>
#include <QJsonObject>

/*!
 * \brief 数值范围条件类的实现类
 */
CValueRangeConditionData::CValueRangeConditionData()
{
    m_version = 0;

    m_operator1                             = MDOperator::equal_to;
    m_operand1                              = 0;
    m_bit1                                          = 0;
    m_operator2                             = MDOperator::equal_to;
    m_operand2                              = 0;
    m_bit2                                          = 0;
    m_relationshipBetween1and2 = MDRelationship::relationship_no;
}

CValueRangeConditionData::~CValueRangeConditionData()
{
}

CValueRangeConditionData::CValueRangeConditionData(const CValueRangeConditionData& obj)
{
    m_version = obj.m_version;

    m_operator1                             = obj.m_operator1;
    m_operand1                              = obj.m_operand1;
    m_bit1                                          = obj.m_bit1;
    m_operator2                             = obj.m_operator2;
    m_operand2                              = obj.m_operand2;
    m_bit2                                          = obj.m_bit2;
    m_relationshipBetween1and2 = obj.m_relationshipBetween1and2;
}

CValueRangeConditionData& CValueRangeConditionData::operator = (const CValueRangeConditionData& obj)
{
    if( this != &obj )
    {
        m_version = obj.m_version;
        m_operator1                             = obj.m_operator1;
        m_operand1                              = obj.m_operand1;
        m_bit1                                          = obj.m_bit1;
        m_operator2                             = obj.m_operator2;
        m_operand2                              = obj.m_operand2;
        m_bit2                                          = obj.m_bit2;
        m_relationshipBetween1and2 = obj.m_relationshipBetween1and2;
    }
    return *this;
}

QString CValueRangeConditionData::toExpression(const QString& variableName)
{
    QString expression = "(";
    expression += variableName;
    expression += operatorToString(m_operator1);
    expression += m_operand1.toString();
    expression += ")";

    if(MDRelationship::null_relationship == m_relationshipBetween1and2 ||
        MDRelationship::relationship_no == m_relationshipBetween1and2)
    {
        return expression;
    }

    if(MDRelationship::relationship_and == m_relationshipBetween1and2)
    {
        expression += " && ";
    }
    else if(MDRelationship::relationship_or == m_relationshipBetween1and2)
    {
        expression += " || ";
    }
    else{}

    expression += "(";
    expression += variableName;
    expression += operatorToString(m_operator2);
    expression += m_operand2.toString();
    expression += ")";

    return expression;
}

QString CValueRangeConditionData::operatorToString(MDOperator theOperator)
{
    QString operatorString;
    switch (theOperator) {
    case MDOperator::greater_than:
        operatorString = ">";
        break;
    case MDOperator::less_than:
        operatorString = "<";
        break;
    case MDOperator::equal_to:
        operatorString = "==";
        break;
    case MDOperator::greater_than_or_equal_to:
        operatorString = ">=";
        break;
    case MDOperator::less_than_or_equal_to:
        operatorString = "<=";
        break;
    case MDOperator::not_equal_to:
        operatorString = "!=";
        break;
    default:
        break;
    }
    return operatorString;
}

const quint8 CURRENT_VERSION = 1;
bool CValueRangeConditionData::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream);
    else
        result = false;
    return result;
}

void CValueRangeConditionData::serializeOnVersion_1(QDataStream& stream)
{
    quint8 operator1 = (quint8)m_operator1;
    quint8 operator2 = (quint8)m_operator2;
    quint8 relationship = (quint8)m_relationshipBetween1and2;

    stream << operator1 << m_operand1 << m_bit1;
    stream << operator2 << m_operand2 << m_bit2;
    stream << relationship;
}

void CValueRangeConditionData::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

void CValueRangeConditionData::deserializeOnVersion_1(QDataStream& stream)
{
    quint8 operator1;
    quint8 operator2;
    quint8 relationship;

    stream >> operator1 >> m_operand1 >> m_bit1;
    stream >> operator2 >> m_operand2 >> m_bit2;
    stream >> relationship;

    m_operator1 = (MDOperator)operator1;
    m_operator2 = (MDOperator)operator2;
    m_relationshipBetween1and2 = (MDRelationship)relationship;
}

void CValueRangeConditionData::serialize(QJsonObject& json)
{
    json.insert("Operator1",(int)m_operator1);
    json.insert("Operand1", m_operand1.toString());
    json.insert("Bit1", (int)m_bit1);
    json.insert("Operator2", (int)m_operator2);
    json.insert("Operand2", m_operand2.toString());
    json.insert("Bit2", (int)m_bit2);
    json.insert("RelationshipBetween1and2", (int)m_relationshipBetween1and2);
}

void CValueRangeConditionData::deserialize(const QJsonObject& json)
{
    m_operator1 = (MDOperator)json.value("Operator1").toInt();
    m_operand1 = json.value("Operand1").toVariant();
    m_bit1 = json.value("Bit1").toInt();
    m_operator2 = (MDOperator)json.value("Operator2").toInt();
    m_operand2 = json.value("Operand2").toVariant();
    m_bit2 = json.value("Bit2").toInt();
    m_relationshipBetween1and2 = (MDRelationship)json.value("RelationshipBetween1and2").toInt();
}

/*!
 * \brief 原始事件数据类的实现类
 */
CMDRawEventData::CMDRawEventData()
{
    m_rawObjName = "";
    m_eventType = MDEventType::null_event;
    m_logSwitch = NULL;
}

CMDRawEventData::~CMDRawEventData()
{

}

CMDRawEventData::CMDRawEventData(const QString& rawObjName, MDEventType eventType)
{
    m_rawObjName = rawObjName;
    m_eventType = eventType;
    m_logSwitch = NULL;
}

CMDRawEventData::CMDRawEventData(const CMDRawEventData& obj)
{
    m_rawObjName = obj.m_rawObjName;
    m_eventType = obj.m_eventType;
    m_timeStamp = obj.m_timeStamp;
    m_logSwitch = obj.m_logSwitch;
}

CMDRawEventData& CMDRawEventData::operator = (const CMDRawEventData& obj)
{
    if( this != &obj )
    {
        m_rawObjName = obj.m_rawObjName;
        m_eventType = obj.m_eventType;
        m_timeStamp = obj.m_timeStamp;
        m_logSwitch = obj.m_logSwitch;
    }
    return *this;
}

/*!
 * \brief 原始变量事件数据类的实现类
 */
CMDRawVariableEventData::CMDRawVariableEventData()
{
    m_changedVariable = CMDRTDBManagerFactory::createChangeVariable();
}

CMDRawVariableEventData::CMDRawVariableEventData(IMDChangeVariable* changedVariable)
{
    m_changedVariable = CMDRTDBManagerFactory::createChangeVariable();
    if(changedVariable && m_changedVariable)
        m_changedVariable->copy(changedVariable);
}

CMDRawVariableEventData::~CMDRawVariableEventData()
{
    if(m_changedVariable)
        delete m_changedVariable;
}

CMDRawVariableEventData::CMDRawVariableEventData(const CMDRawVariableEventData& obj)
{
    m_changedVariable = CMDRTDBManagerFactory::createChangeVariable();
    if(obj.m_changedVariable && m_changedVariable)
        m_changedVariable->copy(obj.m_changedVariable);
}

CMDRawVariableEventData& CMDRawVariableEventData::operator = (const CMDRawVariableEventData& obj)
{
    if( this != &obj )
    {
        if(obj.m_changedVariable && m_changedVariable)
            m_changedVariable->copy(obj.m_changedVariable);
    }
    return *this;
}
