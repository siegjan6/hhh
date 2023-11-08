/*!

\author dGFuZ3p3

\section 功能

一些辅助的数据类。

*/

#ifndef MDEVENTHELPER_H
#define MDEVENTHELPER_H

#include <QVariant>
#include "MDEventConstant.h"
#include "MDEventHelper_Global.h"

class CValueRangeConditionData;
/*!
 * \brief 数值范围条件类
 */
class MDEVENTHELPERSHARED_EXPORT CValueRangeCondition
{
public:
    CValueRangeCondition();
    virtual ~CValueRangeCondition();
    CValueRangeCondition(const CValueRangeCondition& obj);
    CValueRangeCondition& operator = (const CValueRangeCondition& obj);
public:
    void condition1(MDOperator& operator1, QVariant& operand1, quint8& bit1) const;
    void setCondition1(MDOperator operator1, QVariant operand1, quint8 bit1 = 0);
    void condition1(MDOperator& operator1, QVariant& operand1) const;
public:
    void condition2(MDOperator& operator2, QVariant& operand2, quint8& bit2) const;
    void setCondition2(MDOperator operator2, QVariant operand2, quint8 bit2 = 0);
    void condition2(MDOperator& operator2, QVariant& operand2) const;
public:
    MDRelationship relationship() const;
    void setRelationship(MDRelationship relationship);
public:
    QString toExpression(const QString& variableName) const;
public:
    static void enumOperators(QStringList& operatorList);
    static QString operatorToString(MDOperator nsOperator);
    static MDOperator stringToOperator(const QString& str);
public:
    static void enumRelationships(QStringList& relationshipList);
    static QString relationshipToString(MDRelationship relationship);
    static MDRelationship stringToRelationship(const QString& str);
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void deserialize(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
   virtual void serialize(QJsonObject& json);
   virtual void deserialize(const QJsonObject& json);
protected:
    CValueRangeConditionData* m_data;
};
static CValueRangeCondition EMPTY_RANGECONDITION;

#endif // MDEVENTHELPER_H
