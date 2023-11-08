/*!

\author dGFuZ3p3

\section 功能

一些辅助的数据类。

*/

#ifndef MDEVENTHELPERIMP_H
#define MDEVENTHELPERIMP_H

#include <QVariant>
#include <QDateTime>
#include "MDEventConstant.h"
#include "../../include/MDRTDBInterfaces/IMDChangeVariable.h"

/*!
 * \brief 数值范围条件类的实现类
 */
class CValueRangeConditionData
{
    friend class CValueRangeCondition;
public:
    CValueRangeConditionData();
    virtual ~CValueRangeConditionData();
    CValueRangeConditionData(const CValueRangeConditionData& obj);
    CValueRangeConditionData& operator = (const CValueRangeConditionData& obj);
protected:
    MDOperator m_operator1;
    QVariant m_operand1;
    quint8 m_bit1;
protected:
    MDOperator m_operator2;
    QVariant m_operand2;
    quint8 m_bit2;
protected:
    MDRelationship m_relationshipBetween1and2;

    QString toExpression(const QString& variableName);
    static QString operatorToString(MDOperator theOperator);

public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void serializeOnVersion_1(QDataStream& stream);
public:
    void deserialize(QDataStream& stream);
    void deserializeOnVersion_1(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    quint8 m_version;
};

class CMDObject;
/*!
 * \brief 原始事件数据类的实现类
 */
class CMDLogSwitch;
class CMDRawEventData
{
public:
    CMDRawEventData();
    CMDRawEventData(const QString& rawObjName, MDEventType eventType);
    virtual ~CMDRawEventData();
    CMDRawEventData(const CMDRawEventData& obj);
    CMDRawEventData& operator = (const CMDRawEventData& obj);

public:
    QString m_rawObjName;
public:
    MDEventType m_eventType;
    QDateTime m_timeStamp;
    QString m_comment;
    CMDLogSwitch* m_logSwitch;
};

/*!
 * \brief 原始变量事件数据类的实现类
 */
class CMDRawVariableEventData
{
public:
    CMDRawVariableEventData();
    CMDRawVariableEventData(IMDChangeVariable* changedVariable);
    ~CMDRawVariableEventData();
   CMDRawVariableEventData(const CMDRawVariableEventData& obj);
   CMDRawVariableEventData& operator = (const CMDRawVariableEventData& obj);
public:
    IMDChangeVariable* m_changedVariable;
};

#endif // MDEVENTHELPERIMP_H
