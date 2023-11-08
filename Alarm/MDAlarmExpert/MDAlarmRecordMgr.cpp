#include "MDAlarmRecordMgr.h"
#include "MDAlarmExpertImp.h"
#include <QJsonDocument>

CMDAlarmRecordMgr::CMDAlarmRecordMgr()
{
    m_alarmExpert = NULL;
}

CMDAlarmRecordMgr::~CMDAlarmRecordMgr()
{

}

/*!
\section 初始化与释放
*/
void CMDAlarmRecordMgr::release()
{
    m_statusMgr.deleteAll();

    deleteAll();
}

void CMDAlarmRecordMgr::setAlarmExpert(CMDAlarmExpertImp* alarmExpert)
{
    m_alarmExpert = alarmExpert;
    m_statusMgr.m_alarmExpert = alarmExpert;
}

/*!
\section 管理报警记录
*/
CMDAlarmRecord* CMDAlarmRecordMgr::insert(IMDDataGroup* alarmGroup, CMDAlarmConditionRT* cond, const QVariant& variableData, CMDAlarmContacter* contacter)
{
    CMDAlarmRecord* newRecord = new CMDAlarmRecord(cond->m_condition->name());
    newRecord->init(alarmGroup, cond, variableData, contacter);

    QMutexLocker locker(&m_recordMapMutex);
    m_recordMap.insert(cond->m_condition->name().toUpper(), newRecord);
    locker.unlock();

    if(m_alarmExpert->m_alarmMode == MDAlarmMode::common)
        m_alarmExpert->doArchive(newRecord);
    else
    {
        m_statusMgr.insert(newRecord);
    }
    return newRecord;
}

bool CMDAlarmRecordMgr::end(CMDAlarmConditionRT* cond, const QVariant& variableData, QDateTime& startTime, QDateTime& endTime)
{
    CMDAlarmRecord* existRecord = find(cond->m_condition->name());
    if(existRecord)
    {
        startTime = existRecord->m_firstDT;
        existRecord->updateOnEnd(variableData, endTime);

        m_statusMgr.end(existRecord);

        if(m_alarmExpert->m_alarmMode == MDAlarmMode::common)
            m_alarmExpert->doArchive(existRecord, 1);
    }

    // 改为在调用end处自行移除
    //delete1(cond->m_condition->name());

    return (existRecord != NULL);
}

void CMDAlarmRecordMgr::enumAll(QStringList& alarmIDList)
{
    QMutexLocker locker(&m_recordMapMutex);

     QMap<QString, CMDAlarmRecord*>::const_iterator iPosition = m_recordMap.constBegin();
     while(m_recordMap.constEnd() != iPosition)
     {
         CMDAlarmRecord* record = iPosition.value();
         alarmIDList.append(record->m_name);
         ++ iPosition;
     }
}

CMDAlarmRecord* CMDAlarmRecordMgr::find(const QString& name)
{
    QMutexLocker locker(&m_recordMapMutex);
    CMDAlarmRecord* object = helper_find(name);
    return object;
}

CMDAlarmConditionRT* CMDAlarmRecordMgr::findCondition(const QString& name)
{
    QMutexLocker locker(&m_recordMapMutex);
    CMDAlarmRecord* object = helper_find(name);
    return (object ? object->m_cond : NULL);
}

CMDAlarmRecord* CMDAlarmRecordMgr::helper_find(const QString& name)
{
    CMDAlarmRecord* object = NULL;

    QMap<QString, CMDAlarmRecord*>::const_iterator iPosition = m_recordMap.find(name.toUpper());
    if(iPosition != m_recordMap.constEnd())
        object = iPosition.value();

    return object;
}

bool CMDAlarmRecordMgr::delete1(const QString& name)
{
    QMutexLocker locker(&m_recordMapMutex);

    bool result = false;

    QMap<QString, CMDAlarmRecord*>::iterator iPosition = m_recordMap.find(name.toUpper());
    if(iPosition != m_recordMap.constEnd())
   {
        CMDAlarmRecord* object = iPosition.value();
        delete object;

        m_recordMap.erase(iPosition);

        result = true;
    }
    return result;
}

void CMDAlarmRecordMgr::deleteAll()
{
    QMutexLocker locker(&m_recordMapMutex);

    QMap<QString, CMDAlarmRecord*>::const_iterator iPosition = m_recordMap.constBegin();
    while(m_recordMap.constEnd() != iPosition)
    {
        delete iPosition.value();
        ++ iPosition;
    }
    m_recordMap.clear();
}

bool CMDAlarmRecordMgr::confirm(const QString& alarmID, const QString& comment/* = ""*/, const int mode/* = 0*/)
{
    CMDAlarmConditionRT* cond = NULL;
    if(MDAlarmMode::common == m_alarmExpert->m_alarmMode)
        cond = findCondition(CMDAlarmRecord::recordIDToName(alarmID));
    else
        cond = m_statusMgr.findCondition(alarmID);
    if(!cond)
        return false;

    if(mode == 1){
        // from mindscada call
        if(!m_alarmExpert->checkSP_WithUI(cond,1))
            return false;
    }

    return m_alarmExpert->submitConfirm(alarmID, comment);
}

void CMDAlarmRecordMgr::confirmAll(const int mode/* = 0*/)
{
    QStringList alarmIDList;

    if(MDAlarmMode::common == m_alarmExpert->m_alarmMode)
        enumAll(alarmIDList);
    else
        m_statusMgr.enumAll(alarmIDList);

    int count = alarmIDList.count();
    for(int i = 0; i < count; i++)
        confirm(alarmIDList.at(i), 0);
}

//! 报警确认
bool CMDAlarmRecordMgr::confirmDirect(const QString& alarmID, const QString& comment/* = ""*/)
{
    bool result = false;
    if(MDAlarmMode::common == m_alarmExpert->m_alarmMode)
    {
        QString name = CMDAlarmRecord::recordIDToName(alarmID);
        CMDAlarmRecord* existRecord = find(name);
        if(existRecord)
        {
            confirmHelper(existRecord, comment);
            result = true;
        }
    }
    else
    {
        result = m_statusMgr.confirmDirect(alarmID, comment);
    }
    return result;
}

void CMDAlarmRecordMgr::confirmHelper(CMDAlarmRecord* record, const QString& comment/* = ""*/)
{
    if(record->m_confirmStatus == MDAlarmConfirmStatus::occur)
    {
        record->m_confirmStatus = MDAlarmConfirmStatus::confirmed_not_end;
        record->updateComment(comment);
        record->updateConfirmStatus(record->m_confirmStatus);

        // 如果已经结束
        if(record->isEnd()) {
            record->updateStatus(MDAlarmStatus::end_acked_confirmed);
        }
        else
            record->updateStatus(MDAlarmStatus::occur_acked_confirmed);

        m_alarmExpert->updataUaConfirm(record);

        m_alarmExpert->doArchive(record, 1);
    }
}

bool CMDAlarmRecordMgr::acknowledge(const QString& alarmID, const QString& comment/* = ""*/, const int mode/* = 0*/)
{
    CMDAlarmConditionRT* cond = NULL;
    if(MDAlarmMode::common == m_alarmExpert->m_alarmMode)
        cond = findCondition(CMDAlarmRecord::recordIDToName(alarmID));
    else
        cond = m_statusMgr.findCondition(alarmID);
    if(!cond)
        return false;

    if(mode == 1){
        // from mindscada call
        if(!m_alarmExpert->checkSP_WithUI(cond,0))
            return false;
    }

    return m_alarmExpert->submitAcknowledge(alarmID, comment);
}

void CMDAlarmRecordMgr::acknowledgeAll(const int mode/* = 0*/)
{
    QStringList alarmIDList;

    if(MDAlarmMode::common == m_alarmExpert->m_alarmMode)
        enumAll(alarmIDList);
    else
        m_statusMgr.enumAll(alarmIDList);

    int count = alarmIDList.count();
    for(int i = 0; i < count; i++)
        acknowledge(alarmIDList.at(i), 0);
}

bool CMDAlarmRecordMgr::acknowledgeDirect(const QString& alarmID, bool autoAck/* = false*/, const QString& comment/* = ""*/)
{
    bool result = false;
    if(MDAlarmMode::common == m_alarmExpert->m_alarmMode)
    {
        QString name = CMDAlarmRecord::recordIDToName(alarmID);
        CMDAlarmRecord* existRecord = find(name);
        if(existRecord)
        {
            //CMDAlarmConditionRT* cond = findCondition(CMDAlarmRecord::recordIDToName(alarmID));
            acknowledgeHelper(existRecord, autoAck, comment);
            result = true;
        }
    }
    else
    {
        // todo acknowledgeDirect
        result = m_statusMgr.acknowledgeDirect(alarmID);
    }
    return result;
}

void CMDAlarmRecordMgr::acknowledgeHelper(CMDAlarmRecord* record, bool autoAck/* = false*/, const QString& comment/* = ""*/)
{
    if(record->m_acknowledgeStatus == MDAlarmAcknowledgeStatus::occur)
    {
        record->m_acknowledgeStatus = MDAlarmAcknowledgeStatus::acknowledged_not_end;
        if(autoAck)
            record->m_acknowledgeStatus = MDAlarmAcknowledgeStatus::end_then_acknowledged;

        record->updateComment(comment);
        // updateAcknowledgeStatus
        record->updateAcknowledgeStatus(record->m_acknowledgeStatus);
        if(autoAck)
            record->updateStatus(MDAlarmStatus::end);
        else {
            // 如果已经结束
            bool latched = record->m_cond->m_condition->latched();
            if(record->isEnd()) {
                if(latched) {
                    // need confirmed
                    record->updateStatus(MDAlarmStatus::end_acked_need_confirm);
                }
                else
                    record->updateStatus(MDAlarmStatus::end_acked);
            }
            else {
                // 未结束
                if(latched){
                    // need confirmed
                    record->updateStatus(MDAlarmStatus::occur_acked_need_confirm);
                }
                else {
                    record->updateStatus(MDAlarmStatus::occur_acked);
                }
            }
        }


        m_alarmExpert->updataUaAcknowledge(record);

        m_alarmExpert->doArchive(record, 1);
    }
}

bool CMDAlarmRecordMgr::suppress(const QString& alarmID, const QString& comment/* = ""*/)
{
    return m_alarmExpert->submitSuppress(alarmID, comment);
}
bool CMDAlarmRecordMgr::suppressDirect(const QString& alarmID, const QString& comment/* = ""*/)
{
    bool result = false;
    if(MDAlarmMode::common == m_alarmExpert->m_alarmMode)
    {
        QString name = CMDAlarmRecord::recordIDToName(alarmID);
        CMDAlarmRecord* existRecord = find(name);
        if(existRecord)
        {
            suppressHelper(existRecord, comment);
            result = true;
        }
    }
    else
    {
        // todo suppressDirect
        result = m_statusMgr.suppressDirect(alarmID, comment);
    }
    return result;
}
void CMDAlarmRecordMgr::suppressHelper(CMDAlarmRecord* record, const QString& comment/* = ""*/)
{
    if(record->m_suppressStatus == MDAlarmSuppressStatus::occur)
    {
        record->m_suppressStatus = MDAlarmSuppressStatus::suppressed_not_end;
        record->updateComment(comment);
        // updateSuppressStatus
        record->updateSuppressStatus(record->m_suppressStatus);

        m_alarmExpert->updataUaSuppress(record);

        m_alarmExpert->doArchive(record, 1);
    }
}
bool CMDAlarmRecordMgr::shelve(const QString& alarmID, const QString& comment/* = ""*/)
{
    return m_alarmExpert->submitShelve(alarmID, comment);
}
bool CMDAlarmRecordMgr::shelveDirect(const QString& alarmID, const QString& comment/* = ""*/)
{
    bool result = false;
    if(MDAlarmMode::common == m_alarmExpert->m_alarmMode)
    {
        QString name = CMDAlarmRecord::recordIDToName(alarmID);
        CMDAlarmRecord* existRecord = find(name);
        if(existRecord)
        {
            shelveHelper(existRecord, comment);
            result = true;
        }
    }
    else
    {
        // todo shelveDirect
        result = m_statusMgr.shelveDirect(alarmID, comment);
    }
    return result;
}
void CMDAlarmRecordMgr::shelveHelper(CMDAlarmRecord* record, const QString& comment/* = ""*/)
{
    if(record->m_shelveStatus == MDAlarmShelveStatus::occur)
    {
        record->m_shelveStatus = MDAlarmShelveStatus::shelved_not_end;
        record->updateComment(comment);
        // updateShelveStatus
        record->updateShelveStatus(record->m_shelveStatus);

        m_alarmExpert->updataUaShelve(record);

        m_alarmExpert->doArchive(record, 1);
    }
}
bool CMDAlarmRecordMgr::addComment(const QString& alarmID, const QString& comment/* = ""*/)
{
    return m_alarmExpert->submitAddComment(alarmID, comment);
}
bool CMDAlarmRecordMgr::addCommentDirect(const QString& alarmID, const QString& comment/* = ""*/)
{
    bool result = false;
    if(MDAlarmMode::common == m_alarmExpert->m_alarmMode)
    {
        QString name = CMDAlarmRecord::recordIDToName(alarmID);
        CMDAlarmRecord* existRecord = find(name);
        if(existRecord)
        {
            addCommentHelper(existRecord, comment);
            result = true;
        }
    }
    else
    {
        // todo addCommentDirect
        result = m_statusMgr.addCommentDirect(alarmID, comment);
    }
    return result;
}
void CMDAlarmRecordMgr::addCommentHelper(CMDAlarmRecord* record, const QString& comment/* = ""*/)
{
    // todo update some Status
    record->updateComment(comment);
    m_alarmExpert->updataUaComment(record);

    m_alarmExpert->doArchive(record, 1);
}

int CMDAlarmRecordMgr::getAlarmCount()
{
    QStringList alarmIDList;

    if(MDAlarmMode::common == m_alarmExpert->m_alarmMode)
        enumAll(alarmIDList);
    else
        m_statusMgr.enumAll(alarmIDList);

    return alarmIDList.count();
}

QString CMDAlarmRecordMgr::getAlarmByIndex(int index)
{
    QStringList aidlist;
    enumAll(aidlist);
    if(index >= aidlist.count())
        return "";
    QString aid = aidlist.at(index);

    CMDAlarmRecord* record = NULL;
    if(MDAlarmMode::common == m_alarmExpert->m_alarmMode)
    {
        QString aname = CMDAlarmRecord::recordIDToName(aid);
        record = find(aname);
        if(record)
            return record->toJsonString();
    }
    else
    {
        CMDAlarmRecordStatus* status =  m_statusMgr.find(aid);
        if(status)
        {
            return status->toJsonString();
        }
    }

    return "";
}

QString CMDAlarmRecordMgr::getAlarmById(QString id)
{
    CMDAlarmRecord* record = NULL;
    if(MDAlarmMode::common == m_alarmExpert->m_alarmMode)
    {
        QString aname = CMDAlarmRecord::recordIDToName(id);
        record = find(aname);
        if(record)
            return record->toJsonString();
    }
    else
    {
        CMDAlarmRecordStatus* status =  m_statusMgr.find(id);
        if(status)
        {
            return status->toJsonString();
        }
    }

    return "";
}

/*!
\section 将管理的全部报警记录追加到 dataRecordset
*/
bool CMDAlarmRecordMgr::toDatabaseRecordset(CMDDatabaseRecordset* dataRecordset)
{
    QMutexLocker locker(&m_recordMapMutex);

    QMap<QString, CMDAlarmRecord*>::const_iterator iPosition = m_recordMap.constBegin();
    while(m_recordMap.constEnd() != iPosition)
    {
        CMDAlarmRecord* alarmRecord = iPosition.value();

        alarmRecord->toDatabaseRecord(dataRecordset);

        ++ iPosition;
    }
    return true;
}
