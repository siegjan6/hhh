/*!

\author dGFuZ3p3

\section 功能

日志记录管理类。

*/

#ifndef MDLOGRECORDMGR_H
#define MDLOGRECORDMGR_H

#include <QList>
#include <QMutexLocker>
#include "MDLogRecord.h"

class CMDLogContacter;
class CMDLogRecordMgr
{
public:
    CMDLogRecordMgr();
    ~CMDLogRecordMgr();
/*!
\section 初始化与释放
*/
public:
    void release();

/*!
\section 管理日志记录
*/
public:
    CMDLogRecord* insert(IMDDataGroup* logGroup, const CMDLogMessage& msg, CMDLogContacter* contacter);
    void deleteAll();
    int maxCount();
    void setMaxCount(int maxCount);
protected:
    QList<CMDLogRecord*> m_recordList;
    int m_maxCount;
    QMutex m_recordListMutex;

/*!
\section 将管理的全部日志记录追加到 dataRecordset
*/
public:
    bool toDatabaseRecordset(CMDDatabaseRecordset* dataRecordset);
};

#endif // MDLOGRECORDMGR_H
