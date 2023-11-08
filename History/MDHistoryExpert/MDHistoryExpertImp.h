/*!

\author dGFuZ3p3

\section 功能

历史专家类的实现类，实现运行时的历史组存储、查询等处理。

*/

#ifndef MDHISTORYEXPERTIMP_H
#define MDHISTORYEXPERTIMP_H

#include <QString>
#include <QVariant>
#include "MDQueryTaskMgr.h"

class CMDHistoryContacter;
class CMDAction;
class CMDDataArchivesEngine;
class IMDDataArchives;
class IMDDBResultNotify;
class IMDDBQueryService;
class IMDDBQueryParam;
class IMDDataItem;
class IMDDataSource;
class IMDDBQueryEngine;
class CMDHistoryExpertImp
{
    friend class CMDHistoryExpert;
public:
    CMDHistoryExpertImp();
/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, IMDDataArchives *dataArchivesConfiger, CMDHistoryContacter* contacter);
    void load();
    void unload();
protected:
    CMDHistoryContacter* m_contacter ;// NULL;
    QString m_pid;

/*!
\section 执行保存历史组动作
*/
public:
    qint8 executeAction(CMDAction* action);
    bool saveHisGroup(const QString& name);
protected:
    QVariant initItemValue(IMDDataItem* groupItem, const QDateTime& dt);
protected:
    CMDDataArchivesEngine* m_dataArchivesEngine ;// NULL;
    IMDDataArchives* m_dataArchivesConfiger ;// NULL;

/*!
\section 查询归档数据
*/
public:
    IMDDBQueryEngine* queryEngine();
    bool queryArchiveData(IMDDBQueryParam* queryPara);
public:
    IMDDataSource* dataSource(const QString& dsName);
    CMDQueryTaskMgr m_qTaskMgr; //! 查询任务管理器
    void onQueryResult(const QString& queryID, int result);
protected:
    IMDDBQueryEngine* m_dbQueryEngine;
    IMDDBQueryService* m_dbQueryService ;// NULL;
};

#endif // MDHISTORYEXPERTIMP_H
