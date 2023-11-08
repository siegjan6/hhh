/*!

\author dGFuZ3p3

\section 功能

报警变量及管理类；
一个报警变量可以关联多个报警条件。

*/

#ifndef MDALARMVARIABLE_H
#define MDALARMVARIABLE_H

#include <QList>
#include <QMap>
#include <QString>
#include "MDAlarmConditionRT.h"
#include "MDRawEvent.h"

/*!
 *\class 报警变量类，一个报警变量可以关联多个报警条件。
 */
class CMDRawVariableEvent;
class CMDAlarmExpertImp;
class CMDAlarmVariable
{
public:
    CMDAlarmVariable();
    CMDAlarmVariable(const QString& name);
public:
    void setAlarmExpert(CMDAlarmExpertImp* alarmExpert);
public:
    CMDAlarmExpertImp* m_alarmExpert;
    QString m_name;
public:
    bool initAddAlarmCondition(CMDAlarmConditionRT* condition);
    void ProcessRawEvent(CMDRawVariableEvent* rawEvent);
protected:
    QList<CMDAlarmConditionRT*> m_conditionList; //! 关联的报警条件列表
};

/*!
 *\class 报警变量管理类
 */
class CMDAlarmVariableMgr
{
public:
    CMDAlarmVariableMgr();
    virtual ~CMDAlarmVariableMgr();
public:
    CMDAlarmVariable* add(const QString& name, qint8& result);
    CMDAlarmVariable* find(const QString& name);
    void deleteAll();
public:
    void deletePrefixMatched(const QString& prefix);
public:
    bool ProcessRawEvent(CMDRawVariableEvent* rawEvent);
protected:
    QMap<QString, CMDAlarmVariable*> m_variableMap;
};

#endif // MDALARMVARIABLE_H
