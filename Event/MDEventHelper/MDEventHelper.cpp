#include "MDEventHelper.h"
#include "MDEventHelperImp.h"

CValueRangeCondition::CValueRangeCondition()
{
    m_data = new CValueRangeConditionData;
}

CValueRangeCondition::~CValueRangeCondition()
{
    if(m_data)
        delete m_data;
}

CValueRangeCondition::CValueRangeCondition(const CValueRangeCondition& obj)
{
    if(obj.m_data)
        m_data = new CValueRangeConditionData(*obj.m_data);
}

CValueRangeCondition& CValueRangeCondition::operator = (const CValueRangeCondition& obj)
{
    if( this != &obj )
    {
        if(m_data && obj.m_data)
            *m_data = *obj.m_data;
    }
    return *this;
}

void CValueRangeCondition::condition1(MDOperator& operator1, QVariant& operand1, quint8& bit1) const
{
    if(m_data)
    {
        operator1 = m_data->m_operator1;
        operand1 = m_data->m_operand1;
        bit1 = m_data->m_bit1;
    }
}
void CValueRangeCondition::setCondition1(MDOperator operator1, QVariant operand1, quint8 bit1)
{
    if(m_data)
    {
        m_data->m_operator1 = operator1;
        m_data->m_operand1 = operand1;
        m_data->m_bit1 = bit1;
    }
}

void CValueRangeCondition::condition1(MDOperator& operator1, QVariant& operand1) const
{
    quint8 bit1;
    condition1(operator1, operand1, bit1);
}

void CValueRangeCondition::condition2(MDOperator& operator2, QVariant& operand2, quint8& bit2) const
{
    if(m_data)
    {
        operator2 = m_data->m_operator2;
        operand2 = m_data->m_operand2;
        bit2 = m_data->m_bit2;
    }
}
void CValueRangeCondition::setCondition2(MDOperator operator2, QVariant operand2, quint8 bit2)
{
    if(m_data)
    {
        m_data->m_operator2 = operator2;
        m_data->m_operand2 = operand2;
        m_data->m_bit2 = bit2;
    }
}

void CValueRangeCondition::condition2(MDOperator& operator2, QVariant& operand2) const
{
    quint8 bit2;
    condition2(operator2, operand2, bit2);
}

MDRelationship CValueRangeCondition::relationship() const
{
    return (m_data ? m_data->m_relationshipBetween1and2 : MDRelationship::relationship_no);
}
void CValueRangeCondition::setRelationship(MDRelationship relationship)
{
    if(m_data)
        m_data->m_relationshipBetween1and2 = relationship;
}

QString CValueRangeCondition::toExpression(const QString& variableName) const
{
    return (m_data ? m_data->toExpression(variableName) : "");
}

bool CValueRangeCondition::serialize(QDataStream& stream, bool useCurrentVersion)
{
    return (m_data ? m_data->serialize(stream, useCurrentVersion) : false);
}

void CValueRangeCondition::deserialize(QDataStream& stream)
{
    if(m_data)
        m_data->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CValueRangeCondition::serialize(QJsonObject& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CValueRangeCondition::deserialize(const QJsonObject& json)
{
    if(m_data)
        m_data->deserialize(json);
}

void CValueRangeCondition::enumOperators(QStringList& operatorList)
{
    operatorList.append(operatorToString(MDOperator::greater_than));
    operatorList.append(operatorToString(MDOperator::less_than));
    operatorList.append(operatorToString(MDOperator::equal_to));
    operatorList.append(operatorToString(MDOperator::greater_than_or_equal_to));
    operatorList.append(operatorToString(MDOperator::less_than_or_equal_to));
    operatorList.append(operatorToString(MDOperator::not_equal_to));
    operatorList.append(operatorToString(MDOperator::take_a_bit));
}

void CValueRangeCondition::enumRelationships(QStringList& relationshipList)
{
    relationshipList.append(relationshipToString(MDRelationship::relationship_no));
    relationshipList.append(relationshipToString(MDRelationship::relationship_or));
    relationshipList.append(relationshipToString(MDRelationship::relationship_and));
}

QString CValueRangeCondition::operatorToString(MDOperator nsOperator)
{
    QString str;
    switch (nsOperator) {
    case MDOperator::null_operator: str = "";
        break;
    case MDOperator::greater_than: str = ">";
        break;
    case MDOperator::less_than: str = "<";
        break;
    case MDOperator::equal_to: str = "==";
        break;
    case MDOperator::greater_than_or_equal_to: str = ">=";
        break;
    case MDOperator::less_than_or_equal_to: str = "<=";
        break;
    case MDOperator::not_equal_to: str = "!=";
        break;
    case MDOperator::take_a_bit: str = "bit[]";
        break;
    default:
        break;
    }
    return str;
}

MDOperator CValueRangeCondition::stringToOperator(const QString& str)
{
    MDOperator op = MDOperator::null_operator;
    if(0 == str.compare(""))
        op = MDOperator::null_operator;
    else if(0 == str.compare(">"))
        op = MDOperator::greater_than;
    else if(0 == str.compare("<"))
        op = MDOperator::less_than;
    else if(0 == str.compare("=="))
        op = MDOperator::equal_to;
    else if(0 == str.compare(">="))
        op = MDOperator::greater_than_or_equal_to;
    else if(0 == str.compare("<="))
        op = MDOperator::less_than_or_equal_to;
    else if(0 == str.compare("!="))
        op = MDOperator::not_equal_to;
    else if(0 == str.compare("bit[]"))
        op = MDOperator::take_a_bit;
    else{};
    return op;
}

QString CValueRangeCondition::relationshipToString(MDRelationship relationship)
{
    QString str;
    switch (relationship) {
    case MDRelationship::null_relationship: str = "";
        break;
    case MDRelationship::relationship_no: str = "no";
        break;
    case MDRelationship::relationship_or: str = "or";
        break;
    case MDRelationship::relationship_and: str = "and";
        break;
    default:
        break;
    }
    return str;
}

MDRelationship CValueRangeCondition::stringToRelationship(const QString& str)
{
    MDRelationship re = MDRelationship::null_relationship;
    if(0 == str.compare(""))
        re = MDRelationship::null_relationship;
    else if(0 == str.compare("no"))
        re = MDRelationship::relationship_no;
    else if(0 == str.compare("or"))
        re = MDRelationship::relationship_or;
    else if(0 == str.compare("and"))
        re = MDRelationship::relationship_and;
    else{};
    return re;
}
