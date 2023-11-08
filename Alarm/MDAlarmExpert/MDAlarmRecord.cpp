#include "MDAlarmRecord.h"
#include "MDDatabaseRecordset.h"
#include "MDAlarmExpertImp.h"
#include "MDDataDefine.h"
#include "MDDataGroupFactory.h"
#include "MDAlarmContacter.h"
#include <QJsonArray>
#include <QJsonDocument>

CMDAlarmRecord::CMDAlarmRecord(const QString& name)
{
    m_name = name;

    m_cond = NULL; //! 报警条件
    m_contacter = NULL;
    m_confirmStatus = MDAlarmConfirmStatus::occur;
    m_acknowledgeStatus = MDAlarmAcknowledgeStatus::occur;
    m_suppressStatus = MDAlarmSuppressStatus::occur;
    m_shelveStatus = MDAlarmShelveStatus::occur;
    m_status = MDAlarmStatus::occur;
}

CMDAlarmRecord::~CMDAlarmRecord()
{

}

CMDAlarmRecord::CMDAlarmRecord(const CMDAlarmRecord& obj)
{
    copy(&obj);
}

CMDAlarmRecord& CMDAlarmRecord::operator = (const CMDAlarmRecord& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDAlarmRecord::copy(const CMDAlarmRecord* source)
{
    m_name = source->m_name;
    m_firstDT = source->m_firstDT;
    m_cond = source->m_cond;

    m_confirmStatus = source->m_confirmStatus;
    m_acknowledgeStatus = source->m_acknowledgeStatus;
    m_suppressStatus = source->m_suppressStatus;
    m_shelveStatus = source->m_shelveStatus;
    m_status = source->m_status;
    m_fields = source->m_fields;

    return true;
}

/*!
\section 初始化
*/
bool CMDAlarmRecord::init(IMDDataGroup* alarmGroup, CMDAlarmConditionRT* cond, const QVariant& variableData, CMDAlarmContacter* contacter)
{
    m_cond = cond;
    m_contacter = contacter;

    if(!alarmGroup)
        return false;

    QDateTime currentDT = QDateTime::currentDateTime();
    m_firstDT = currentDT;
    m_fields.setDataGroupName(alarmGroup->name());
    m_fields.setRecordDateTime(currentDT);
    for(int i = 0; i < alarmGroup->itemCount(); i++)
    {
        IMDDataItem* groupItem = alarmGroup->getItem(i);
        QString varName = groupItem->name();
        QVariant value = initItemValue(groupItem, currentDT, cond, variableData);

        IMDDataItem* varItem = CMDDataGroupFactory::createDataItem();
        varItem->setName(varName);
        varItem->setData(value);
        varItem->setDataType(groupItem->dataType());
        varItem->setDescription(varName);
        m_fields.addItem(varItem);
    }
    return true;
}

QVariant CMDAlarmRecord::initItemValue(IMDDataItem* groupItem, const QDateTime& dt, CMDAlarmConditionRT* cond, const QVariant& variableData)
{
    QVariant value = groupItem->data();
    QString itemName = groupItem->name();
    if(0 == itemName.compare(ALM_ID, Qt::CaseInsensitive))
    {
        value = QVariant(recordID());
    }
    else if(0 == itemName.compare(ALM_AID, Qt::CaseInsensitive))
    {
        value = QVariant(cond->m_condition->customId());
    }
    else if(0 == itemName.compare(ALM_NAME, Qt::CaseInsensitive))
    {
        value = QVariant(cond->m_condition->name());
    }
    else if(0 == itemName.compare(ALM_MESSAGE, Qt::CaseInsensitive))
    {
        value = QVariant(cond->m_condition->message());
    }
    else if(0 == itemName.compare(ALM_STARTTIME, Qt::CaseInsensitive))
    {
        value = QVariant(dt);
    }
    else if(0 == itemName.compare(ALM_STARTTIME_MS, Qt::CaseInsensitive))
    {
        value = QVariant(dt.time().msec());
    }
    else if(0 == itemName.compare(ALM_STARTVALUE, Qt::CaseInsensitive))
    {
        value = QVariant(variableData.toString());
    }
    else if(0 == itemName.compare(ALM_LEVEL, Qt::CaseInsensitive))
    {
        value = QVariant(cond->m_condition->levelName());
    }
    else if(0 == itemName.compare(ALM_SERVERRITY, Qt::CaseInsensitive))
    {
        value = QVariant(cond->m_condition->serverity());
    }
    else if(0 == itemName.compare(ALM_STATUS, Qt::CaseInsensitive))
    {
        if(cond->m_condition->acknowledgeRequired())
            value = QVariant((qint8)MDAlarmStatus::occur_need_ack);
        else
            value = QVariant((qint8)MDAlarmStatus::occur);
    }
    else if(0 == itemName.compare(ALM_USER, Qt::CaseInsensitive))
    {
        value = QVariant(m_contacter->currentAccount());
    }
    else if(0 == itemName.compare(ALM_COMMENT, Qt::CaseInsensitive))
    {
        value = QVariant(cond->m_condition->comment());
    }
    else if(0 == itemName.compare(ALM_FORECOLOR, Qt::CaseInsensitive))
    {
        CMDAlarmColor ac;
        if(cond->m_level)
            ac = cond->m_level->color_occur();
        value = QVariant(ac.f());
    }
    else if(0 == itemName.compare(ALM_BACKCOLOR, Qt::CaseInsensitive))
    {
        CMDAlarmColor ac;
        if(cond->m_level)
            ac = cond->m_level->color_occur();
        value = QVariant(ac.b());
    }
    else
    {
        QVariant varValue = m_contacter->variableValue(itemName);
        if(varValue.isValid())
            value = varValue;
    };

    return value;
}

/*!
\section 报警ID（根据报警名称和发生时间构造）
*/
QString CMDAlarmRecord::recordID()
{
    return recordID(m_name, m_firstDT);
}

QString CMDAlarmRecord::recordID(const QString& name, const QDateTime& firstDT)
{
    QLatin1String format = QLatin1String("yyyy-MM-dd hh:mm:ss.zzz");

    return name +  "_" + firstDT.toString(format);
}

QString CMDAlarmRecord::recordIDToName(const QString& rid)
{
    int index = rid.indexOf("_");
    if(-1 == index)
        return rid;

    return rid.left(index);
}

/*!
\section 更新报警确认状态（更新相应的字段）
*/
void CMDAlarmRecord::updateConfirmStatus(MDAlarmConfirmStatus cs)
{
    // confirm state
    IMDDataItem * item_confirm_state = m_fields.getItem(ALM_CONFIRMSTATE);
    item_confirm_state->setData(QVariant((qint8)cs));

    // 总状态 根据confirm和ack综合以后的总状态
    IMDDataItem * item = m_fields.getItem(ALM_STATUS);
    item->setData(QVariant((qint8)cs));

    // 颜色
    CMDAlarmColor ac;
    confirmStatusToColor(cs, ac);
    item = m_fields.getItem(ALM_FORECOLOR);
    item->setData(QVariant(ac.f()));
    item = m_fields.getItem(ALM_BACKCOLOR);
    item->setData(QVariant(ac.b()));

    // 时间
    if(MDAlarmConfirmStatus::confirmed_not_end == cs ||
       MDAlarmConfirmStatus::end_then_confirmed == cs)
    {
        QDateTime currentDT = QDateTime::currentDateTime();
        item = m_fields.getItem(ALM_CONFIRMTIME);
        item->setData(QVariant(currentDT));
        item = m_fields.getItem(ALM_CONFIRMTIME_MS);
        item->setData(QVariant(currentDT.time().msec()));

//        item = m_fields.getItem(ALM_STARTTIME);
//        item->setData(QVariant(currentDT));
//        item = m_fields.getItem(ALM_STARTTIME_MS);
//        item->setData(QVariant(currentDT.time().msec()));
    }
    else if(MDAlarmConfirmStatus::confirmed_then_end == cs ||
            MDAlarmConfirmStatus::end_not_confirmed == cs)
    {
//        IMDDataItem * endItem = m_fields.getItem(ALM_ENDTIME);
//        item = m_fields.getItem(ALM_STARTTIME);
//        item->setData(endItem->data());

//        endItem = m_fields.getItem(ALM_ENDTIME_MS);
//        item = m_fields.getItem(ALM_STARTTIME_MS);
//        item->setData(endItem->data());
    }
    else{}
}

/*!
\section 更新报警知悉状态（更新相应的字段）
*/
void CMDAlarmRecord::updateAcknowledgeStatus(MDAlarmAcknowledgeStatus cs)
{
    // ack state
    IMDDataItem * item_ack_state = m_fields.getItem(ALM_ACKNOWLEDGESTATE);
    item_ack_state->setData(QVariant((qint8)cs));

    // 总状态 根据confirm和ack综合以后的总状态
    IMDDataItem * item_state = m_fields.getItem(ALM_STATUS);
    item_state->setData(QVariant((qint8)cs));

    // 颜色
    CMDAlarmColor ac;
    acknowledgeStatusToColor(cs, ac);
    IMDDataItem * item = m_fields.getItem(ALM_FORECOLOR);
    item->setData(QVariant(ac.f()));
    item = m_fields.getItem(ALM_BACKCOLOR);
    item->setData(QVariant(ac.b()));

    // 时间
    if(MDAlarmAcknowledgeStatus::acknowledged_not_end == cs ||
       MDAlarmAcknowledgeStatus::end_then_acknowledged == cs)
    {
        QDateTime currentDT = QDateTime::currentDateTime();
        item = m_fields.getItem(ALM_ACKNOWLEDGETIME);
        item->setData(QVariant(currentDT));
        item = m_fields.getItem(ALM_ACKNOWLEDGETIME_MS);
        item->setData(QVariant(currentDT.time().msec()));

//        item = m_fields.getItem(ALM_STARTTIME);
//        item->setData(QVariant(currentDT));
//        item = m_fields.getItem(ALM_STARTTIME_MS);
//        item->setData(QVariant(currentDT.time().msec()));
    }
    else if(MDAlarmAcknowledgeStatus::acknowledged_then_end == cs ||
            MDAlarmAcknowledgeStatus::end_not_acknowledged == cs)
    {
//        IMDDataItem * endItem = m_fields.getItem(ALM_ENDTIME);
//        item = m_fields.getItem(ALM_STARTTIME);
//        item->setData(endItem->data());

//        endItem = m_fields.getItem(ALM_ENDTIME_MS);
//        item = m_fields.getItem(ALM_STARTTIME_MS);
//        item->setData(endItem->data());
    }
    else{}
}

/*!
\section 更新报警抑制状态（更新相应的字段）
*/
void CMDAlarmRecord::updateSuppressStatus(MDAlarmSuppressStatus cs)
{
    // suppress state
    IMDDataItem * item_sos_state = m_fields.getItem(ALM_SUPPRESSEDORSHELVED);
    item_sos_state->setData(QVariant((qint8)1));

    // suppress state
    IMDDataItem * item_ss_state = m_fields.getItem(ALM_ACKNOWLEDGESTATE);
    item_ss_state->setData(QVariant((qint8)cs));

    // 总状态 根据confirm和ack综合以后的总状态
    IMDDataItem * item_state = m_fields.getItem(ALM_STATUS);
    item_state->setData(QVariant((qint8)cs));

    // 颜色
    CMDAlarmColor ac;
    suppressStatusToColor(cs, ac);
    IMDDataItem * item = m_fields.getItem(ALM_FORECOLOR);
    item->setData(QVariant(ac.f()));
    item = m_fields.getItem(ALM_BACKCOLOR);
    item->setData(QVariant(ac.b()));

    // 时间
    if(MDAlarmSuppressStatus::suppressed_not_end == cs ||
       MDAlarmSuppressStatus::end_then_suppressed == cs)
    {
        QDateTime currentDT = QDateTime::currentDateTime();
        item = m_fields.getItem(ALM_SUPPRESSTIME);
        item->setData(QVariant(currentDT));
        item = m_fields.getItem(ALM_SUPPRESSTIME_MS);
        item->setData(QVariant(currentDT.time().msec()));

//        item = m_fields.getItem(ALM_STARTTIME);
//        item->setData(QVariant(currentDT));
//        item = m_fields.getItem(ALM_STARTTIME_MS);
//        item->setData(QVariant(currentDT.time().msec()));
    }
    else if(MDAlarmSuppressStatus::suppressed_then_end == cs ||
            MDAlarmSuppressStatus::end_not_suppressed == cs)
    {
//        IMDDataItem * endItem = m_fields.getItem(ALM_ENDTIME);
//        item = m_fields.getItem(ALM_STARTTIME);
//        item->setData(endItem->data());

//        endItem = m_fields.getItem(ALM_ENDTIME_MS);
//        item = m_fields.getItem(ALM_STARTTIME_MS);
//        item->setData(endItem->data());
    }
    else{}
}

/*!
\section 更新报警搁置状态（更新相应的字段）
*/
void CMDAlarmRecord::updateShelveStatus(MDAlarmShelveStatus cs)
{
    // shelve state
    IMDDataItem * item_sos_state = m_fields.getItem(ALM_SUPPRESSEDORSHELVED);
    item_sos_state->setData(QVariant((qint8)2));

    // suppress state
    IMDDataItem * item_ss_state = m_fields.getItem(ALM_SHELVESTATE);
    item_ss_state->setData(QVariant((qint8)cs));

    // 总状态 根据confirm和ack综合以后的总状态
    IMDDataItem * item_state = m_fields.getItem(ALM_STATUS);
    item_state->setData(QVariant((qint8)cs));

    // 颜色
    CMDAlarmColor ac;
    shelveStatusToColor(cs, ac);
    IMDDataItem * item = m_fields.getItem(ALM_FORECOLOR);
    item->setData(QVariant(ac.f()));
    item = m_fields.getItem(ALM_BACKCOLOR);
    item->setData(QVariant(ac.b()));

    // 时间
    if(MDAlarmShelveStatus::shelved_not_end == cs ||
       MDAlarmShelveStatus::end_then_shelved == cs)
    {
        QDateTime currentDT = QDateTime::currentDateTime();
        item = m_fields.getItem(ALM_SHELVETIME);
        item->setData(QVariant(currentDT));
        item = m_fields.getItem(ALM_SHELVETIME_MS);
        item->setData(QVariant(currentDT.time().msec()));

//        item = m_fields.getItem(ALM_STARTTIME);
//        item->setData(QVariant(currentDT));
//        item = m_fields.getItem(ALM_STARTTIME_MS);
//        item->setData(QVariant(currentDT.time().msec()));
    }
    else if(MDAlarmShelveStatus::shelved_then_end == cs ||
            MDAlarmShelveStatus::end_not_shelved == cs)
    {
//        IMDDataItem * endItem = m_fields.getItem(ALM_ENDTIME);
//        item = m_fields.getItem(ALM_STARTTIME);
//        item->setData(endItem->data());

//        endItem = m_fields.getItem(ALM_ENDTIME_MS);
//        item = m_fields.getItem(ALM_STARTTIME_MS);
//        item->setData(endItem->data());
    }
    else{}
}

MDAlarmStatus CMDAlarmRecord::status()
{
    IMDDataItem * item_state = m_fields.getItem(ALM_STATUS);
    bool ok = false;
    int state = item_state->data().toInt(&ok);
    if(ok)
        return (MDAlarmStatus)state;
    return MDAlarmStatus::null;
}

void CMDAlarmRecord::updateStatus(MDAlarmStatus cs)
{
    IMDDataItem * item_state = m_fields.getItem(ALM_STATUS);
    item_state->setData(QVariant((qint8)cs));
}

void CMDAlarmRecord::updateComment(QString comment)
{
    IMDDataItem * item_comment = m_fields.getItem(ALM_COMMENT);
    item_comment->setData(QVariant(comment));
}

QString CMDAlarmRecord::comment()
{
    IMDDataItem * item_comment = m_fields.getItem(ALM_COMMENT);
    return item_comment->data().toString();
}

bool CMDAlarmRecord::isEnd()
{
    IMDDataItem * endItem = m_fields.getItem(ALM_ENDTIME);
    if(!endItem->data().isNull())
        return true;
    return false;
}

/*!
\section 更新报警结束状态（更新相应的字段）
*/
void CMDAlarmRecord::updateEnd()
{
    QDateTime currentDT = QDateTime::currentDateTime();
    IMDDataItem * endItem = m_fields.getItem(ALM_ENDTIME);
    endItem->setData(QVariant(currentDT));

    endItem = m_fields.getItem(ALM_ENDTIME_MS);
    endItem->setData(QVariant(currentDT.time().msec()));
}

void CMDAlarmRecord::updateOnEnd(const QVariant& variableData, QDateTime& endTime)
{
    QDateTime currentDT = QDateTime::currentDateTime();
    IMDDataItem * item = m_fields.getItem(ALM_ENDTIME);
    item->setData(QVariant(currentDT));
    item = m_fields.getItem(ALM_ENDTIME_MS);
    item->setData(QVariant(currentDT.time().msec()));

    item = m_fields.getItem(ALM_ENDVALUE);
    item->setData(variableData.toString());

    endTime = currentDT;
}

bool CMDAlarmRecord::acknowledgeStatusToColor(MDAlarmAcknowledgeStatus cs, CMDAlarmColor& ac)
{
    CMDAlarmLevel* level = m_cond->m_level;
    if(!level)
        return false;

    switch (cs) {
    case MDAlarmAcknowledgeStatus::occur:
        ac = level->color_occur();
        break;
    case MDAlarmAcknowledgeStatus::acknowledged_not_end:   // 知悉
        ac = level->color_confirm_not_end();
        break;
    case MDAlarmAcknowledgeStatus::acknowledged_then_end:  // 结束
        ac = level->color_confirm_end();
        break;
    case MDAlarmAcknowledgeStatus::end_not_acknowledged:   // 结束
        ac = level->color_end_not_confirm();
        break;
    case MDAlarmAcknowledgeStatus::end_then_acknowledged:  // 知悉
        ac = level->color_end_confirm();
        break;
    default:;
        break;
    }
    return true;
}

bool CMDAlarmRecord::confirmStatusToColor(MDAlarmConfirmStatus cs, CMDAlarmColor& ac)
{
    CMDAlarmLevel* level = m_cond->m_level;
    if(!level)
        return false;

    switch (cs) {
    case MDAlarmConfirmStatus::occur:
        ac = level->color_occur();
        break;
    case MDAlarmConfirmStatus::confirmed_not_end:   // 确认
        ac = level->color_confirm_not_end();
        break;
    case MDAlarmConfirmStatus::confirmed_then_end:  // 结束
        ac = level->color_confirm_end();
        break;
    case MDAlarmConfirmStatus::end_not_confirmed:   // 结束
        ac = level->color_end_not_confirm();
        break;
    case MDAlarmConfirmStatus::end_then_confirmed:  // 确认
        ac = level->color_end_confirm();
        break;
    default:;
        break;
    }
    return true;
}

bool CMDAlarmRecord::suppressStatusToColor(MDAlarmSuppressStatus cs, CMDAlarmColor& ac)
{
    //TODO: suppressStatusToColor
    CMDAlarmLevel* level = m_cond->m_level;
    if(!level)
        return false;

    switch (cs) {
    case MDAlarmSuppressStatus::occur:
        ac = level->color_occur();
        break;
    case MDAlarmSuppressStatus::suppressed_not_end:   // 抑制
        ac = level->color_confirm_not_end();
        break;
    case MDAlarmSuppressStatus::suppressed_then_end:  // 结束
        ac = level->color_confirm_end();
        break;
    case MDAlarmSuppressStatus::end_not_suppressed:   // 结束
        ac = level->color_end_not_confirm();
        break;
    case MDAlarmSuppressStatus::end_then_suppressed:  // 抑制
        ac = level->color_end_confirm();
        break;
    default:;
        break;
    }
    return true;
}

bool CMDAlarmRecord::shelveStatusToColor(MDAlarmShelveStatus cs, CMDAlarmColor& ac)
{
    //TODO: shelveStatusToColor
    CMDAlarmLevel* level = m_cond->m_level;
    if(!level)
        return false;

    switch (cs) {
    case MDAlarmShelveStatus::occur:
        ac = level->color_occur();
        break;
    case MDAlarmShelveStatus::shelved_not_end:   // 搁置
        ac = level->color_confirm_not_end();
        break;
    case MDAlarmShelveStatus::shelved_then_end:  // 结束
        ac = level->color_confirm_end();
        break;
    case MDAlarmShelveStatus::end_not_shelved:   // 结束
        ac = level->color_end_not_confirm();
        break;
    case MDAlarmShelveStatus::end_then_shelved:  // 搁置
        ac = level->color_end_confirm();
        break;
    default:;
        break;
    }
    return true;
}

/*!
\section 在 recordSet 中增加一行，并填充该行的内容
*/
void CMDAlarmRecord::toDatabaseRecord(CMDDatabaseRecordset* recordSet)
{
    int row = recordSet->addRow();

    int colCount = m_fields.itemCount();
    for(int i = 0; i < colCount; i++)
    {
        QVariant data = m_fields.getItem(i)->data();
        recordSet->setValue(row, i, data);
    }
}

QString CMDAlarmRecord::toJsonString()
{
    QJsonObject alarm;
    int count = m_fields.itemCount();
    for (int i = 0; i< count;i++)
    {
        IMDDataItem* item = m_fields.getItem(i);
        alarm.insert(item->name(), QJsonValue::fromVariant(item->data()));
    }
    QJsonDocument doc;
    doc.setObject(alarm);
    return QString(doc.toJson().constData());
}







/*!
 * \brief 报警记录状态类，其 m_recordStatus 成员记录了状态变化
 * 实例都通过【报警名称 + 发生时间】来标识。
 */
CMDAlarmRecordStatus::CMDAlarmRecordStatus(const QString& rid)
{
    m_rid = rid;
    m_cond = NULL;
    m_status = MDAlarmStatus::occur;
}

CMDAlarmRecordStatus::~CMDAlarmRecordStatus()
{
    int count = m_recordStatus.count();
    for(int i = 0; i < count; i++)
        delete m_recordStatus.at(i);
}

CMDAlarmRecord* CMDAlarmRecordStatus::lastRecord()
{
    return m_recordStatus.isEmpty() ? NULL : m_recordStatus.last();
}

void CMDAlarmRecordStatus::appendRecord(CMDAlarmRecord* record)
{
    m_recordStatus.append(record);
}

void CMDAlarmRecordStatus::toDatabaseRecordset(CMDDatabaseRecordset* dataRecordset)
{
    int count = m_recordStatus.count();
    for(int i = 0; i < count; i++)
    {
        m_recordStatus.at(i)->toDatabaseRecord(dataRecordset);
    }
}

QString CMDAlarmRecordStatus::toJsonString()
{
    QJsonArray alarmArr;
    foreach (CMDAlarmRecord* record, m_recordStatus) {
        QJsonObject alarm;
        int count = record->m_fields.itemCount();
        for (int i = 0; i< count;i++)
        {
            IMDDataItem* item = record->m_fields.getItem(i);
            alarm.insert(item->name(), QJsonValue::fromVariant(item->data()));
        }
        alarmArr.append(alarm);
    }
    QJsonDocument doc;
    doc.setArray(alarmArr);
    return QString(doc.toJson().constData());
}

