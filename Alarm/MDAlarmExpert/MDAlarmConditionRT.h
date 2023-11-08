/*!

\author dGFuZ3p3

\section 功能

运行时的报警条件类。

*/

#ifndef MDALARMCONDITIONRT_H
#define MDALARMCONDITIONRT_H

#include "MDExpression.h"
#include "MDAlarmCondition.h"

class CMDAlarmCondition;
class CMDAlarmContacter;
class IMDVariable;
class CMDAlarmConditionRT
{
public:
    CMDAlarmConditionRT(CMDAlarmCondition* condition, CMDAlarmLevel* level,
                        CMDAlarmContacter* contacter, CMDActionSP* acknowledgeSP, CMDActionSP* confirmSP);
    ~CMDAlarmConditionRT();
public:
    CMDAlarmCondition* m_condition ;// NULL;
    CMDAlarmLevel* m_level ;// NULL;
    CMDActionSP* m_acknowledgeSP ;// NULL;
    CMDActionSP* m_confirmSP ;// NULL;

    CMDAlarmCondition::LimitAlarmMode m_limitAlarmMode; // CMDAlarmCondition::LimitAlarmMode::NONE;

/*!
\section 判断报警条件是否满足
*/
public:
    bool handle();
    bool isGoodExpressionVar(const QString& varName, QVariant& curValue);
protected:
    CMDExpression m_eventExpression; //! 报警条件表达式
    QList<QString> m_refVarList;
    CMDAlarmContacter* m_contacter ;//  NULL;
};

#endif // MDALARMCONDITIONRT_H
