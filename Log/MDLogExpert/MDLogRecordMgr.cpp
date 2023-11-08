#include "MDLogRecordMgr.h"

CMDLogRecordMgr::CMDLogRecordMgr()
{
    m_maxCount = 1024;
}

CMDLogRecordMgr::~CMDLogRecordMgr()
{
    deleteAll();
}

/*!
\section 初始化与释放
*/
void CMDLogRecordMgr::release()
{
    deleteAll();
}

/*!
\section 管理日志记录
*/
CMDLogRecord* CMDLogRecordMgr::insert(IMDDataGroup* logGroup, const CMDLogMessage& msg, CMDLogContacter* contacter)
{    
    QMutexLocker recordListLocker(&m_recordListMutex);

    CMDLogRecord* record =  NULL;
    if(m_recordList.size() >= m_maxCount)
    {
        record = m_recordList.takeLast();
        record->setData(msg);
    }
    else
    {
        record = new CMDLogRecord;
        record->init(msg, logGroup, contacter);
    }
    m_recordList.prepend(record);

    return record;
}

void CMDLogRecordMgr::deleteAll()
{
    QMutexLocker recordListLocker(&m_recordListMutex);

    qDeleteAll(m_recordList.begin(), m_recordList.end());
    m_recordList.clear();
}

int CMDLogRecordMgr::maxCount()
{
    return m_maxCount;
}

void CMDLogRecordMgr::setMaxCount(int maxCount)
{
    m_maxCount = maxCount;
}

/*!
\section 将管理的全部日志记录追加到 dataRecordset
*/
bool CMDLogRecordMgr::toDatabaseRecordset(CMDDatabaseRecordset* dataRecordset)
{
    QMutexLocker recordListLocker(&m_recordListMutex);

    int count = m_recordList.count();
    for(int i = 0; i < count; i++)
    {
         CMDLogRecord* record = m_recordList.at(i);
         record->toDatabaseRecord(dataRecordset);
    }
    return true;
}
