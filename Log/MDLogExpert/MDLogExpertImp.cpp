#include "MDLogExpertImp.h"
#include <QString>
#include "MDHistoryExpert.h"
#include "MDLogRecord.h"
#include "MDDataDefine.h"
#include "MDDatabaseRecordset.h"

#include "MDDataArchivesEngine.h"
#include "MDDataArchivesInterface/IMDDataGroupManager.h"
#include "MDDataArchivesInterface/IMDDataArchives.h"
#include "MDDataArchivesInterface/IMDDataItem.h"
#include "MDDataArchivesInterface/IMDDataGroup.h"

CMDLogExpertImp::CMDLogExpertImp()
{
    m_pid = "";
    m_contacter = NULL;
    m_logGroup = NULL;

    m_historyExpert = NULL;
    m_dataArchivesEngine = NULL;

    m_sArchiveLog = NULL;
}

CMDLogExpertImp::~CMDLogExpertImp()
{

}

/*!
\section 初始化与释放
*/
void CMDLogExpertImp::setParaBeforeLoad(const QString& pid, CMDHistoryExpert* historyExpert, CMDLogContacter* contacter)
{
    m_pid = pid;
    m_contacter = contacter;

    m_historyExpert = historyExpert;
    m_dataArchivesEngine = (historyExpert ? historyExpert->dataArchivesEngine() : NULL);

    IMDDataArchives* dataArchivesConfiger = (historyExpert ? historyExpert->dataArchivesConfiger() : NULL);
    m_logGroup = (dataArchivesConfiger ?
                                    dataArchivesConfiger->dataGroupManager()->getLogDataGroup() : NULL);
}

void CMDLogExpertImp::load()
{
    createSwitchs(true);
}

void CMDLogExpertImp::unload()
{
    m_recordMgr.release();

    createSwitchs(false);
}

void CMDLogExpertImp::createSwitchs(bool create)
{
    if(create)
    {
        m_sArchiveLog = CMDFuncSwitchFactory::createSwitch(m_pid, "LogExpert.ArchiveLog", "日志归档");

//        QString subFileDir = "ProjectExpert";
//        m_lsAlarmProcess = CMDLogSwitchFactory::createSwitchWithSubFileDir(m_pid, "AlarmExpert.AalarmProcess", "报警处理流程", subFileDir);
    }
    else
    {
        m_sArchiveLog = NULL;
    }
}

/*!
\section 缓存最近日志到内存；获取最近日志
*/
bool CMDLogExpertImp::log(CMDLogMessage& msg)
{
    CMDLogRecord* newRecord = m_recordMgr.insert(m_logGroup, msg, m_contacter);
    if(m_sArchiveLog->isOn())
        doArchive(newRecord);

    writeUpdateTime();

    return (newRecord != NULL);
}

bool CMDLogExpertImp::logRecordSet(CMDDatabaseRecordset* dataRecordset)
{
    if(!m_logGroup)
        return false;

    QStringList fieldList;
    for(int i = 0; i < m_logGroup->itemCount(); i++)
    {
        IMDDataItem* groupItem = m_logGroup->getItem(i);
        fieldList.append(groupItem->name());
    }
    dataRecordset->bindFileds(fieldList);
    return m_recordMgr.toDatabaseRecordset(dataRecordset);
}

QDateTime CMDLogExpertImp::readUpdateTime()
{
    QReadLocker wLocker(&m_rwLock);
    return m_updateTime;
}

void CMDLogExpertImp::writeUpdateTime()
{
    QWriteLocker wLocker(&m_rwLock);
    m_updateTime = QDateTime::currentDateTime();
}

/*!
\section 日志归档，通过历史专家类实现日志归档
*/
bool CMDLogExpertImp::doArchive(CMDLogRecord* record)
{
    if(!m_dataArchivesEngine)
        return false;

    return m_dataArchivesEngine->asyncCallDataArchives(DataArchives::DB_NOTIFY_ADD, &record->m_fields);
}
