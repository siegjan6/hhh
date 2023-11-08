#include "MDAlarmRecordStatusMgr.h"
#include "MDAlarmExpertImp.h"

/*!
 * \brief 报警记录状态管理类
 */
CMDAlarmRecordMgr_OnConfirmStatus::CMDAlarmRecordMgr_OnConfirmStatus()
{
    m_alarmExpert = NULL;
}

CMDAlarmRecordMgr_OnConfirmStatus::~CMDAlarmRecordMgr_OnConfirmStatus()
{
    deleteAll();
}

QString CMDAlarmRecordMgr_OnConfirmStatus::recordID(CMDAlarmRecord* record)
{
    return CMDAlarmRecord::recordID(record->m_name, record->m_firstDT);
}

/*!
\section 管理报警状态记录
*/
CMDAlarmRecordStatus* CMDAlarmRecordMgr_OnConfirmStatus::insert(CMDAlarmRecord* record)
{
    QString rid = recordID(record);
    CMDAlarmRecordStatus* newObject = new CMDAlarmRecordStatus(rid);
    if(newObject)
    {
        newObject->m_rid = rid;
        newObject->m_cond = record->m_cond;
        newObject->m_confirmStatus = MDAlarmConfirmStatus::occur;

        CMDAlarmRecord* newRecord = new CMDAlarmRecord(*record);
        newRecord->updateConfirmStatus(newObject->m_confirmStatus);
        newObject->appendRecord(newRecord);

        QMutexLocker locker(&m_recordMapMutex);
        m_recordMap.insert(rid.toUpper(), newObject);
        locker.unlock();

        m_alarmExpert->doArchive(newRecord);
    }
    return newObject;
}

bool CMDAlarmRecordMgr_OnConfirmStatus::end(CMDAlarmRecord* record)
{
    QString rid = recordID(record);
    CMDAlarmRecordStatus* object = find(rid);
    if(!object)
        return false;

    if(object->m_confirmStatus == MDAlarmConfirmStatus::occur)
    {
        object->m_confirmStatus = MDAlarmConfirmStatus::end_not_confirmed;
    }
    else if(object->m_confirmStatus == MDAlarmConfirmStatus::confirmed_not_end)
        object->m_confirmStatus = MDAlarmConfirmStatus::confirmed_then_end;

    if(object->m_acknowledgeStatus == MDAlarmAcknowledgeStatus::occur)
    {
        object->m_acknowledgeStatus = MDAlarmAcknowledgeStatus::end_not_acknowledged;
    }
    else if(object->m_acknowledgeStatus == MDAlarmAcknowledgeStatus::acknowledged_not_end)
        object->m_acknowledgeStatus = MDAlarmAcknowledgeStatus::acknowledged_then_end;

    if(object->m_suppressStatus == MDAlarmSuppressStatus::occur)
    {
        object->m_suppressStatus = MDAlarmSuppressStatus::end_not_suppressed;
    }
    else if(object->m_suppressStatus == MDAlarmSuppressStatus::suppressed_not_end)
        object->m_suppressStatus = MDAlarmSuppressStatus::suppressed_then_end;

    if(object->m_shelveStatus == MDAlarmShelveStatus::occur)
    {
        object->m_shelveStatus = MDAlarmShelveStatus::end_not_shelved;
    }
    else if(object->m_shelveStatus == MDAlarmShelveStatus::shelved_not_end)
        object->m_shelveStatus = MDAlarmShelveStatus::shelved_then_end;

    CMDAlarmRecord* endRecord = new CMDAlarmRecord(*record);
    endRecord->updateConfirmStatus(object->m_confirmStatus);
    endRecord->updateAcknowledgeStatus(object->m_acknowledgeStatus);
    endRecord->updateSuppressStatus(object->m_suppressStatus);
    endRecord->updateShelveStatus(object->m_shelveStatus);
    object->appendRecord(endRecord);

    m_alarmExpert->doArchive(endRecord);

    if(object->m_confirmStatus == MDAlarmConfirmStatus::confirmed_then_end)
    {
        delete1(rid);
    }
    return true;
}

void CMDAlarmRecordMgr_OnConfirmStatus::enumAll(QStringList& alarmIDList)
{
    QMutexLocker locker(&m_recordMapMutex);

     QMap<QString, CMDAlarmRecordStatus*>::const_iterator iPosition = m_recordMap.constBegin();
     while(m_recordMap.constEnd() != iPosition)
     {
         CMDAlarmRecordStatus* alarmStatus = iPosition.value();
         alarmIDList.append(alarmStatus->m_rid);
         ++ iPosition;
     }
}

CMDAlarmRecordStatus* CMDAlarmRecordMgr_OnConfirmStatus::find(const QString& rid)
{
    QMutexLocker locker(&m_recordMapMutex);
    CMDAlarmRecordStatus* object = helper_find(rid);
    return object;
}

CMDAlarmConditionRT* CMDAlarmRecordMgr_OnConfirmStatus::findCondition(const QString& rid)
{
    QMutexLocker locker(&m_recordMapMutex);
    CMDAlarmRecordStatus* object = helper_find(rid);
    return (object ? object->m_cond : NULL);
}

CMDAlarmRecordStatus* CMDAlarmRecordMgr_OnConfirmStatus::helper_find(const QString& rid)
{
    CMDAlarmRecordStatus* object = NULL;

    QMap<QString, CMDAlarmRecordStatus*>::const_iterator iPosition = m_recordMap.find(rid.toUpper());
    if(iPosition != m_recordMap.constEnd())
        object = iPosition.value();

    return object;
}

bool CMDAlarmRecordMgr_OnConfirmStatus::delete1(const QString& rid)
{
    QMutexLocker locker(&m_recordMapMutex);

    bool result = false;

    QMap<QString, CMDAlarmRecordStatus*>::iterator iPosition = m_recordMap.find(rid.toUpper());
    if(iPosition != m_recordMap.constEnd())
   {
        CMDAlarmRecordStatus* object = iPosition.value();
        delete object;
        m_recordMap.erase(iPosition);

        result = true;
    }
    return result;
}

void CMDAlarmRecordMgr_OnConfirmStatus::deleteAll()
{
    QMutexLocker locker(&m_recordMapMutex);

    QMap<QString, CMDAlarmRecordStatus*>::const_iterator iPosition = m_recordMap.constBegin();
    while(m_recordMap.constEnd() != iPosition)
    {
        delete iPosition.value();
        ++ iPosition;
    }
    m_recordMap.clear();
}

bool CMDAlarmRecordMgr_OnConfirmStatus::confirmDirect(const QString& alarmID, const QString& comment/* = ""*/)
{
    CMDAlarmRecordStatus* status = find(alarmID);
    if(!status)
        return false;

    // 已经确认过
    if(status->m_confirmStatus == MDAlarmConfirmStatus::confirmed_not_end ||
           status->m_confirmStatus == MDAlarmConfirmStatus::confirmed_then_end)
    {
        return false;
    }

    CMDAlarmRecord* record = status->lastRecord();
    if(!record)
        return false;

    if(status->m_confirmStatus == MDAlarmConfirmStatus::occur)
        status->m_confirmStatus = MDAlarmConfirmStatus::confirmed_not_end;
    else if(status->m_confirmStatus == MDAlarmConfirmStatus::end_not_confirmed)
          status->m_confirmStatus = MDAlarmConfirmStatus::end_then_confirmed;

    CMDAlarmRecord* confirmedRecord = new CMDAlarmRecord(*record);
    confirmedRecord->updateConfirmStatus(status->m_confirmStatus);
    confirmedRecord->updateComment(comment);
    status->appendRecord(confirmedRecord);

    m_alarmExpert->doArchive(confirmedRecord);

    if(status->m_confirmStatus == MDAlarmConfirmStatus::end_then_confirmed)
    {
        delete1(status->m_rid);
    }
    return true;
}

bool CMDAlarmRecordMgr_OnConfirmStatus::acknowledgeDirect(const QString& alarmID, const QString& comment/* = ""*/)
{
    CMDAlarmRecordStatus* status = find(alarmID);
    if(!status)
        return false;

    // 已经知悉过
    if(status->m_acknowledgeStatus == MDAlarmAcknowledgeStatus::acknowledged_not_end ||
           status->m_acknowledgeStatus == MDAlarmAcknowledgeStatus::acknowledged_then_end)
    {
        return false;
    }

    CMDAlarmRecord* record = status->lastRecord();
    if(!record)
        return false;

    if(status->m_acknowledgeStatus == MDAlarmAcknowledgeStatus::occur)
        status->m_acknowledgeStatus = MDAlarmAcknowledgeStatus::acknowledged_not_end;
    else if(status->m_acknowledgeStatus == MDAlarmAcknowledgeStatus::end_not_acknowledged)
          status->m_acknowledgeStatus = MDAlarmAcknowledgeStatus::end_then_acknowledged;

    CMDAlarmRecord* acknowledgedRecord = new CMDAlarmRecord(*record);
    acknowledgedRecord->updateAcknowledgeStatus(status->m_acknowledgeStatus);
    acknowledgedRecord->updateComment(comment);
    status->appendRecord(acknowledgedRecord);

    m_alarmExpert->doArchive(acknowledgedRecord);

    if(status->m_acknowledgeStatus == MDAlarmAcknowledgeStatus::end_then_acknowledged)
    {
        delete1(status->m_rid);
    }
    return true;
}

bool CMDAlarmRecordMgr_OnConfirmStatus::suppressDirect(const QString& alarmID, const QString& comment/* = ""*/)
{
    CMDAlarmRecordStatus* status = find(alarmID);
    if(!status)
        return false;

    // 已经抑制过
    if(status->m_suppressStatus == MDAlarmSuppressStatus::suppressed_not_end ||
           status->m_suppressStatus == MDAlarmSuppressStatus::suppressed_then_end)
    {
        return false;
    }

    CMDAlarmRecord* record = status->lastRecord();
    if(!record)
        return false;

    if(status->m_suppressStatus == MDAlarmSuppressStatus::occur)
        status->m_suppressStatus = MDAlarmSuppressStatus::suppressed_not_end;
    else if(status->m_suppressStatus == MDAlarmSuppressStatus::end_not_suppressed)
          status->m_suppressStatus = MDAlarmSuppressStatus::end_then_suppressed;

    CMDAlarmRecord* suppressedRecord = new CMDAlarmRecord(*record);
    suppressedRecord->updateSuppressStatus(status->m_suppressStatus);
    suppressedRecord->updateComment(comment);
    status->appendRecord(suppressedRecord);

    m_alarmExpert->doArchive(suppressedRecord);

    if(status->m_suppressStatus == MDAlarmSuppressStatus::end_then_suppressed)
    {
        delete1(status->m_rid);
    }
    return true;
}

bool CMDAlarmRecordMgr_OnConfirmStatus::shelveDirect(const QString& alarmID, const QString& comment/* = ""*/)
{
    CMDAlarmRecordStatus* status = find(alarmID);
    if(!status)
        return false;

    // 已经抑制过
    if(status->m_shelveStatus == MDAlarmShelveStatus::shelved_not_end ||
           status->m_shelveStatus == MDAlarmShelveStatus::shelved_then_end)
    {
        return false;
    }

    CMDAlarmRecord* record = status->lastRecord();
    if(!record)
        return false;

    if(status->m_shelveStatus == MDAlarmShelveStatus::occur)
        status->m_shelveStatus = MDAlarmShelveStatus::shelved_not_end;
    else if(status->m_shelveStatus == MDAlarmShelveStatus::end_not_shelved)
          status->m_shelveStatus = MDAlarmShelveStatus::end_then_shelved;

    CMDAlarmRecord* shelvedRecord = new CMDAlarmRecord(*record);
    shelvedRecord->updateShelveStatus(status->m_shelveStatus);
    shelvedRecord->updateComment(comment);
    status->appendRecord(shelvedRecord);

    m_alarmExpert->doArchive(shelvedRecord);

    if(status->m_shelveStatus == MDAlarmShelveStatus::end_then_shelved)
    {
        delete1(status->m_rid);
    }
    return true;
}

bool CMDAlarmRecordMgr_OnConfirmStatus::addCommentDirect(const QString& alarmID, const QString& comment/* = ""*/)
{
    //TODO: addCommentDirect 需要增加报警备注副表
    return false;
}

/*!
\section 将管理的全部报警状态记录追加到 dataRecordset
*/
bool CMDAlarmRecordMgr_OnConfirmStatus::toDatabaseRecordset(CMDDatabaseRecordset* dataRecordset)
{
    QMutexLocker locker(&m_recordMapMutex);

    QMap<QString, CMDAlarmRecordStatus*>::const_iterator iPosition = m_recordMap.constBegin();
    while(m_recordMap.constEnd() != iPosition)
    {
        CMDAlarmRecordStatus* alarmStatus = iPosition.value();
        alarmStatus->toDatabaseRecordset(dataRecordset);
        ++ iPosition;
    }
    return true;
}

