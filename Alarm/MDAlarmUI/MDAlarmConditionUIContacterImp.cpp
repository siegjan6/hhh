#include "MDAlarmConditionUIContacterImp.h"
#include "MDAlarmUIImp.h"
#include "../../include/MDRTDBManagerUI/IMDRTDBManagerUI.h"

CMDAlarmConditionUIContacterImp::CMDAlarmConditionUIContacterImp()
{
    m_uiImp = NULL;
}

bool CMDAlarmConditionUIContacterImp::onSelectAlarmLevel(const QString& source, QString& dest)
{
    return (m_uiImp ? m_uiImp->m_levelUI.select(source, dest) : false);
}

bool CMDAlarmConditionUIContacterImp::onSelectVariable(const QString& source, QString& dest)
{
    if(!m_uiImp)
        return false;

    bool result = false;
    QStringList destList;
    m_uiImp->m_studioInterface->rtdbManagerUI()->selectVariables(destList);
    if(!destList.isEmpty())
    {
        dest = destList.first();
        result = true;
    }
    return result;
}
