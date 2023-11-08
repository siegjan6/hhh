/*!

\author dGFuZ3p3

\section 功能

历史专家类，实现运行时的历史组存储、查询等处理。

*/

#ifndef MDHISTORYEXPERT_H
#define MDHISTORYEXPERT_H

#include "MDHistoryExpert_Global.h"
#include "MDQueryTaskMgr.h"
#include <QStringList>

class CMDHistoryContacter;
class CMDAction;
class CMDHistoryExpertImp;
class IMDDataArchives;
class IMDDBResultNotify;
class CMDDataArchivesEngine;
class IMDDBQueryParam;
class CMDQueryTaskMgr;
class IMDDBQueryEngine;
class MDHISTORYEXPERTSHARED_EXPORT CMDHistoryExpert
{
public:
    CMDHistoryExpert();
    ~CMDHistoryExpert();
/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, IMDDataArchives *dataArchivesConfiger, CMDHistoryContacter* contacter);
    void load();
    void unload();

/*!
\section 执行保存历史组动作
*/
public:
    qint8 executeAction(CMDAction* action);
    bool saveHisGroup(const QString& name);

/*!
\section 查询归档数据
*/
public:
    IMDDBQueryEngine* queryEngine();
    bool queryArchiveData(IMDDBQueryParam* queryPara);
public:
    IMDDataArchives* dataArchivesConfiger();
    CMDDataArchivesEngine* dataArchivesEngine();
public:
    CMDQueryTaskMgr* qTaskMgr();

/*!
\section 设置数据源参数
*/
public:
    void startSetting();
    void endSetting();
    bool setDatabaseParam(const QString &datasourceName, int paraType, const QString &para);
    int excuteSQLCommand(const QString& datasourceName, const QString& sqlCmd);
    QString getLastError(const QString& datasourceName);
protected:
    CMDHistoryExpertImp* m_imp;
};

#endif // MDHISTORYEXPERT_H
