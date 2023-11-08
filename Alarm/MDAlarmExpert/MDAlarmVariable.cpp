#include "MDAlarmVariable.h"
#include "MDAlarmExpertImp.h"

/*!
 *\class 报警变量类，一个报警变量可以关联多个报警条件。
 */
CMDAlarmVariable::CMDAlarmVariable()
{
    m_alarmExpert = NULL;
}

CMDAlarmVariable::CMDAlarmVariable(const QString& name)
{
    m_name = name;
}

void CMDAlarmVariable::setAlarmExpert(CMDAlarmExpertImp* alarmExpert)
{
    m_alarmExpert = alarmExpert;
}

bool CMDAlarmVariable::initAddAlarmCondition(CMDAlarmConditionRT* condition)
{
    m_conditionList.append(condition);
    return true;
}

void CMDAlarmVariable::ProcessRawEvent(CMDRawVariableEvent* rawEvent)
{
    if(rawEvent->logSwitch()->isOn())
    {
        QString para = QString("%1").arg(m_conditionList.size());
        rawEvent->logSwitch()->LOG_INFO("报警变量关联了报警条件，数量=" + para);
    }

    foreach (CMDAlarmConditionRT* cond, m_conditionList)
    {
        if(rawEvent->logSwitch()->isOn())
            rawEvent->logSwitch()->LOG_INFO("评估报警条件：" + cond->m_condition->name());

        m_alarmExpert->handleCondition(cond, rawEvent);
    }
}

/*!
 *\class 报警变量管理类
 */
CMDAlarmVariableMgr::CMDAlarmVariableMgr()
{
}

CMDAlarmVariableMgr::~CMDAlarmVariableMgr()
{
    deleteAll();
}

CMDAlarmVariable* CMDAlarmVariableMgr::add(const QString& name, qint8& result)
{
    CMDAlarmVariable* existObject = find(name);
    if(existObject)
    {
        result = 0;
        return existObject;
    }

    CMDAlarmVariable* newObject = new CMDAlarmVariable(name);
    if(newObject)
    {
        m_variableMap.insert(name.toUpper(), newObject);
    }
    result = 1;
    return newObject;
}

CMDAlarmVariable* CMDAlarmVariableMgr::find(const QString& name)
{
    CMDAlarmVariable* object = NULL;

    QMap<QString, CMDAlarmVariable*>::const_iterator iPosition = m_variableMap.find(name.toUpper());
    if(iPosition != m_variableMap.constEnd())
        object = iPosition.value();

    return object;
}

void CMDAlarmVariableMgr::deleteAll()
{
    QMap<QString, CMDAlarmVariable*>::const_iterator iPosition = m_variableMap.constBegin();
    while(m_variableMap.constEnd() != iPosition)
    {
        delete iPosition.value();
        ++ iPosition;
    }
    m_variableMap.clear();
}

void CMDAlarmVariableMgr::deletePrefixMatched(const QString& prefix)
{
    QMap<QString, CMDAlarmVariable*> ::iterator iPosition;
    QMap<QString, CMDAlarmVariable*> ::iterator iPosition1;
    for (iPosition = m_variableMap.begin(); iPosition != m_variableMap.end();)
    {
        iPosition1 = iPosition;
        iPosition++;

        QString namePrefix = iPosition1.value()->m_name.left(prefix.length());
        if(0 == namePrefix.compare(prefix, Qt::CaseInsensitive))
        {
            delete iPosition1.value();
            m_variableMap.erase(iPosition1);
        }
    }
}

bool CMDAlarmVariableMgr::ProcessRawEvent(CMDRawVariableEvent* rawEvent)
{
    CMDAlarmVariable* av = find(rawEvent->rawObjName());
    if(!av)
        return false;

    av->ProcessRawEvent(rawEvent);
    return true;
}
