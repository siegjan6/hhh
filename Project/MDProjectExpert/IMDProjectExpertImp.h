/*!

\author dGFuZ3p3

\section 功能 - 项目专家纯接口类的实现类。

*/

#ifndef IMDPROJECTEXPERTIMP_H
#define IMDPROJECTEXPERTIMP_H

#include "IMDProjectExpert.h"

class CMDProjectExpertImp;
class IMDProjectExpertImp : public IMDProjectExpert
{
public:
    IMDProjectExpertImp();
    ~IMDProjectExpertImp();
public:
    CMDProjectExpertImp* m_projectExpert ;// NULL;
public:
    virtual QVariant readVariableValue(const QString &varName);
    virtual bool writeVariableValue(const QString &varName,QVariant varValue);
public:
    virtual void projectScriptWrappers(QList<CMDObjectWrapper*>& wrappersList);
public:
    virtual bool executeAction(const QString& actionName);
public:
    virtual bool queryArchiveData(IMDDBQueryParam* queryPara);
    virtual IMDDBQueryEngine* queryEngine(); //! 获取查询引擎
public:
    virtual IMDVariableArchivesManager* variableArchivesManager();
public:
    virtual bool alarmRecordSet(CMDDatabaseRecordset* dataRecordset);
public:
    virtual bool checkSP_WithUI(CMDActionSPSession* spSession);
public:
    virtual QDateTime alarmRecentUpdateTime();
public:
    virtual bool logRecordSet(CMDDatabaseRecordset* dataRecordset);
    virtual QDateTime logRecentUpdateTime();
public:
    virtual bool acknowledgeRequired(const QString& alarmName);
    virtual bool acknowledgeAlarm(const QString& alarmID, const QString& comment = "", const int mode = 0);
    virtual void acknowledgeAllAlarm(const int mode = 0);
    virtual bool confirmRequired(const QString& alarmID);
    virtual bool confirmAlarm(const QString& alarmID, const QString& comment = "", const int mode = 0);
    virtual void confirmAllAlarm(const int mode = 0);
    virtual bool suppressAlarm(const QString& alarmID);
    virtual bool shelveAlarm(const QString& alarmID);
    virtual bool addAlarmComment(const QString& alarmID, const QString& comment);
public:
    virtual QString PID();
};

#endif // IMDPROJECTEXPERTIMP_H
