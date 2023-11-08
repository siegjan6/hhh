/*!

\author dGFuZ3p3

\section 功能

日志专家类的实现类，实现运行时的日志归档及最近日志查询。

*/

#ifndef MDLOGEXPERTIMP_H
#define MDLOGEXPERTIMP_H

#include <QDateTime>
#include <QReadWriteLock>
#include "MDLogMessage.h"
#include "MDLogRecordMgr.h"
#include "MDSwitchFactory/MDFuncSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"

class CMDHistoryExpert;
class CMDLogRecord;
class IMDDataItem;
class IMDDataGroup;
class CMDDatabaseRecordset;
class CMDLogContacter;
class CMDDataArchivesEngine;
class CMDLogExpertImp
{
public:
    CMDLogExpertImp();
    ~CMDLogExpertImp();
/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, CMDHistoryExpert* historyExpert, CMDLogContacter* contacter);
    void load();
    void unload();
protected:
    CMDLogContacter* m_contacter ;// NULL;
    QString m_pid;

/*!
\section 缓存最近日志到内存；获取最近日志
*/
public:
    bool log(CMDLogMessage& msg);
    CMDLogRecordMgr m_recordMgr;
public:
    bool logRecordSet(CMDDatabaseRecordset* dataRecordset);
protected:
    IMDDataGroup* m_logGroup;
public:
    QDateTime readUpdateTime();
    void writeUpdateTime();
protected:
    QDateTime m_updateTime; //! 日志队列最近更新时间，外部可据此判断是否有新日志产生
    QReadWriteLock m_rwLock; //! 线程安全地访问 m_updateTime（使用读写锁，提高并发性）

/*!
\section 日志归档，通过历史专家类实现日志归档
*/
public:
    bool doArchive(CMDLogRecord* record);
protected:
    CMDHistoryExpert* m_historyExpert;
    CMDDataArchivesEngine* m_dataArchivesEngine;
protected:
    void createSwitchs(bool create);
    CMDSwitch* m_sArchiveLog;
};

#endif // MDLOGEXPERTIMP_H
