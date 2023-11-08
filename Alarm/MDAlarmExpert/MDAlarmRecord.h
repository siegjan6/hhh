/*!

\author dGFuZ3p3

\section 功能

运行时的报警记录类，实例都通过【报警名称】来标识。

*/

#ifndef MDALARMRECORD_H
#define MDALARMRECORD_H

#include <QList>
#include <QMap>
#include <QString>
#include <QDateTime>
#include "MDAlarmConditionRT.h"

#include "MDDataArchivesInterface/IMDDataGroup.h"
#include "MDDataArchivesInterface/IMDDataItem.h"
#include "MDDataArchivesNotifyParam.h"
#include "MDAlarmConstant.h"

class CMDDatabaseRecordset;
class CMDAlarmContacter;
class CMDAlarmRecord
{
public:
    CMDAlarmRecord(const QString& name);
    virtual ~CMDAlarmRecord();
    CMDAlarmRecord(const CMDAlarmRecord& obj);
    CMDAlarmRecord& operator = (const CMDAlarmRecord& obj);
    bool copy(const CMDAlarmRecord* source);
    QString toJsonString();

/*!
\section 初始化
*/
public:
    bool init(IMDDataGroup* alarmGroup, CMDAlarmConditionRT* cond, const QVariant& variableData, CMDAlarmContacter* contacter);
    QVariant initItemValue(IMDDataItem* groupItem, const QDateTime& dt, CMDAlarmConditionRT* cond, const QVariant& variableData);
public:
    CMDDataArchivesNotifyParam m_fields; //! 报警记录包含的字段
    CMDAlarmConditionRT* m_cond ;// NULL; //! 报警条件
    CMDAlarmContacter* m_contacter ;// NULL;

/*!
\section 报警ID（根据报警名称和发生时间构造）
*/
public:
    QString recordID();
    QString comment();
    static QString recordID(const QString& name, const QDateTime& firstDT);
    static QString recordIDToName(const QString& rid);
public:
    QString m_name;
    QDateTime m_firstDT;

/*!
\section 更新报警状态（更新相应的字段）
*/
public:
    void updateAcknowledgeStatus(MDAlarmAcknowledgeStatus cs);
    void updateConfirmStatus(MDAlarmConfirmStatus cs);
    void updateSuppressStatus(MDAlarmSuppressStatus cs);
    void updateShelveStatus(MDAlarmShelveStatus cs);
    void updateStatus(MDAlarmStatus cs);
    void updateComment(QString comment);
    void updateOnEnd(const QVariant& variableData, QDateTime& endTime);
    void updateEnd();
    bool acknowledgeStatusToColor(MDAlarmAcknowledgeStatus cs, CMDAlarmColor& ac);
    bool confirmStatusToColor(MDAlarmConfirmStatus cs, CMDAlarmColor& ac);
    bool suppressStatusToColor(MDAlarmSuppressStatus cs, CMDAlarmColor& ac);
    bool shelveStatusToColor(MDAlarmShelveStatus cs, CMDAlarmColor& ac);

    MDAlarmStatus status();
    bool isEnd();
public:
    MDAlarmAcknowledgeStatus m_acknowledgeStatus ;// MDAlarmAcknowledgeStatus::occur;
    MDAlarmConfirmStatus m_confirmStatus ;// MDAlarmConfirmStatus::occur;
    MDAlarmSuppressStatus m_suppressStatus ;// MDAlarmSuppressStatus::occur;
    MDAlarmShelveStatus m_shelveStatus ;// MDAlarmShelveStatus::occur;
    MDAlarmStatus m_status ;// MDAlarmStatus::occur;

/*!
\section 在 recordSet 中增加一行，并填充该行的内容
*/
public:
    void toDatabaseRecord(CMDDatabaseRecordset* recordSet);
};

/*!
 * \brief 报警记录状态类，其 m_recordStatus 成员记录了状态变化
 * 实例都通过【报警名称 + 发生时间】来标识。
 */
class CMDAlarmRecordStatus
{
public:
    CMDAlarmRecordStatus(const QString& rid);
    virtual ~CMDAlarmRecordStatus();
public:
    QString m_rid;
    CMDAlarmConditionRT* m_cond ;// NULL;
    MDAlarmAcknowledgeStatus m_acknowledgeStatus ;// MDAlarmAcknowledgeStatus::occur;
    MDAlarmConfirmStatus m_confirmStatus ;// MDAlarmConfirmStatus::occur;
    MDAlarmSuppressStatus m_suppressStatus ;// MDAlarmSuppressStatus::occur;
    MDAlarmShelveStatus m_shelveStatus ;// MDAlarmShelveStatus::occur;
    MDAlarmStatus m_status ;// MDAlarmStatus::occur;
public:
    CMDAlarmRecord* lastRecord();
    void appendRecord(CMDAlarmRecord* record);
    void toDatabaseRecordset(CMDDatabaseRecordset* dataRecordset);
    QString toJsonString();
protected:
    QList<CMDAlarmRecord*> m_recordStatus;
};

#endif // MDALARMRECORD_H
