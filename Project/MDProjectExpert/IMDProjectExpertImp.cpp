#include "IMDProjectExpertImp.h"
#include "MDProjectExpertImp.h"

IMDProjectExpertImp::IMDProjectExpertImp()
{
    m_projectExpert = NULL;
}

IMDProjectExpertImp::~IMDProjectExpertImp()
{

}

QVariant IMDProjectExpertImp::readVariableValue(const QString &varName)
{
    return (m_projectExpert ? m_projectExpert->m_variableExpert.readVariableValue(varName) : QVariant());
}

bool IMDProjectExpertImp::writeVariableValue(const QString &varName,QVariant varValue)
{
    return (m_projectExpert ? m_projectExpert->m_variableExpert.writeVariableValue(varName, varValue) : false);
}

void IMDProjectExpertImp::projectScriptWrappers(QList<CMDObjectWrapper*>& wrappersList)
{
    if(m_projectExpert)
        m_projectExpert->m_wrappersCollecter.m_wrappersMgr.Wrappers(wrappersList);
}

bool IMDProjectExpertImp::executeAction(const QString &actionName)
{
    return (m_projectExpert ? m_projectExpert->m_actionExpert.asyncProcessAction(actionName) : false);
}

bool IMDProjectExpertImp::queryArchiveData(IMDDBQueryParam* queryPara)
{
    return (m_projectExpert ? m_projectExpert->m_historyExpert.queryArchiveData(queryPara) : false);
}

IMDDBQueryEngine* IMDProjectExpertImp::queryEngine()
{
    return (m_projectExpert ? m_projectExpert->m_historyExpert.queryEngine() : NULL);
}

IMDVariableArchivesManager* IMDProjectExpertImp::variableArchivesManager()
{
    return (m_projectExpert ? m_projectExpert->m_variableExpert.variableArchivesManager() : NULL);
}

bool IMDProjectExpertImp::alarmRecordSet(CMDDatabaseRecordset* dataRecordset)
{
    return (m_projectExpert ? m_projectExpert->m_alarmExpert.alarmRecordSet(dataRecordset) : false);
}

bool IMDProjectExpertImp::checkSP_WithUI(CMDActionSPSession* spSession)
{
    return (m_projectExpert ? m_projectExpert->checkSP_WithUI(spSession) : false);
}

QDateTime IMDProjectExpertImp::alarmRecentUpdateTime()
{
    return (m_projectExpert ? m_projectExpert->m_alarmExpert.recentUpdateTime() : QDateTime());
}

bool IMDProjectExpertImp::logRecordSet(CMDDatabaseRecordset* dataRecordset)
{
    return (m_projectExpert ? m_projectExpert->m_logExpert.logRecordSet(dataRecordset) : false);
}

QDateTime IMDProjectExpertImp::logRecentUpdateTime()
{
    return (m_projectExpert ? m_projectExpert->m_logExpert.recentUpdateTime() : QDateTime());
}

bool IMDProjectExpertImp::acknowledgeRequired(const QString& alarmName)
{
    return (m_projectExpert ? m_projectExpert->m_alarmExpert.acknowledgeRequired(alarmName) : false);
}

bool IMDProjectExpertImp::acknowledgeAlarm(const QString& alarmID, const QString& comment/* = ""*/, const int mode/* = 0*/)
{
    return (m_projectExpert ? m_projectExpert->m_alarmExpert.acknowledge(alarmID, comment, mode) : false);
}
void IMDProjectExpertImp::acknowledgeAllAlarm(const int mode/* = 0*/)
{
    if(m_projectExpert)
        m_projectExpert->m_alarmExpert.acknowledgeAll(mode);
}

bool IMDProjectExpertImp::confirmRequired(const QString& alarmName)
{
    return (m_projectExpert ? m_projectExpert->m_alarmExpert.confirmRequired(alarmName) : false);
}

bool IMDProjectExpertImp::confirmAlarm(const QString& alarmID, const QString& comment/* = ""*/, const int mode/* = 0*/)
{
    return (m_projectExpert ? m_projectExpert->m_alarmExpert.confirm(alarmID, comment, mode) : false);
}

void IMDProjectExpertImp::confirmAllAlarm(const int mode/* = 0*/)
{
    if(m_projectExpert)
        m_projectExpert->m_alarmExpert.confirmAll(mode);
}

bool IMDProjectExpertImp::suppressAlarm(const QString& alarmID)
{
    return (m_projectExpert ? m_projectExpert->m_alarmExpert.suppress(alarmID) : false);
}
bool IMDProjectExpertImp::shelveAlarm(const QString& alarmID)
{
    return (m_projectExpert ? m_projectExpert->m_alarmExpert.shelve(alarmID) : false);
}

bool IMDProjectExpertImp::addAlarmComment(const QString& alarmID, const QString& comment)
{
    return (m_projectExpert ? m_projectExpert->m_alarmExpert.addComment(alarmID, comment) : false);
}

QString IMDProjectExpertImp::PID()
{
    return (m_projectExpert ? m_projectExpert->PID() : "");
}
