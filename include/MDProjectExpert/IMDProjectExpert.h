/*!

\author dGFuZ3p3

\section 功能 - 项目专家纯接口类，通过此类可降低模块依赖性。

*/

#ifndef IMDPROJECTEXPERT_H
#define IMDPROJECTEXPERT_H

#include <QVariant>
#include <QString>
#include "../../include/MDVariableArchives/IMDVariableArchivesManager.h"
#include "../../include/MDSQLDataBaseProxy/MDDatabaseRecordset.h"
#include "../../include/MDSecurityPolicyHelper/MDActionSPSession.h"

class CMDObjectWrapper;
class IMDDBQueryParam;
class IMDVariableArchivesManager;
class CMDDatabaseRecordset;
class IMDDBQueryEngine;
class IMDProjectExpert
{
public:
    virtual ~IMDProjectExpert(){}
public:
    virtual QVariant readVariableValue(const QString &varName) = 0;
    virtual bool writeVariableValue(const QString &varName,QVariant varValue) = 0;
public:
    //! 获取项目全局脚本对象
    virtual void projectScriptWrappers(QList<CMDObjectWrapper*>& wrappersList) = 0;
public:
    virtual bool executeAction(const QString& actionName) = 0;
public:
    virtual bool queryArchiveData(IMDDBQueryParam* queryPara) = 0; //! 查询归档数据
    virtual IMDDBQueryEngine* queryEngine() = 0; //! 获取查询引擎
public:
    virtual IMDVariableArchivesManager* variableArchivesManager() = 0;
public:
    virtual bool alarmRecordSet(CMDDatabaseRecordset* dataRecordset) =0; //! 获取最近报警
public:
    virtual bool checkSP_WithUI(CMDActionSPSession* spSession) = 0; //! 检查安全策略（有界面）
public:
    virtual QDateTime alarmRecentUpdateTime() = 0; //! 获取报警最后更新时间
public:
    virtual bool logRecordSet(CMDDatabaseRecordset* dataRecordset) =0; //! 获取最近日志
    virtual QDateTime logRecentUpdateTime() = 0; //! 获取日志最后更新时间
public:
    virtual bool acknowledgeRequired(const QString& alarmID) =0;
    virtual bool confirmRequired(const QString& alarmID) =0;
    virtual bool acknowledgeAlarm(const QString& alarmID, const QString& comment = "", const int mode = 0) =0;
    virtual void acknowledgeAllAlarm(const int mode = 0) =0;
    virtual bool confirmAlarm(const QString& alarmID, const QString& comment = "", const int mode = 0) =0;
    virtual void confirmAllAlarm(const int mode = 0) =0;
    virtual bool suppressAlarm(const QString& alarmID) =0;
    virtual bool shelveAlarm(const QString& alarmID) =0;
    virtual bool addAlarmComment(const QString& alarmID, const QString& comment) =0;
public:
    virtual QString PID() =0;
};

#endif // IMDPROJECTEXPERT_H
