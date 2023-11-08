/*!

\author dGFuZ3p3

\section 功能

报警记录管理类。

*/

#ifndef MDALARMRECORDMGR_H
#define MDALARMRECORDMGR_H

#include "MDAlarmRecord.h"
#include "MDAlarmRecordStatusMgr.h"
#include <QMutex>

class CMDAlarmExpertImp;
class CMDDatabaseRecordset;
class CMDAlarmRecordMgr
{
    friend class CMDAlarmExpertImp;
public:
    CMDAlarmRecordMgr();
    virtual ~CMDAlarmRecordMgr();
/*!
\section 初始化与释放
*/
public:
    void release();
    void setAlarmExpert(CMDAlarmExpertImp* alarmExpert);
protected:
    CMDAlarmExpertImp* m_alarmExpert ;// NULL;
    CMDAlarmRecordMgr_OnConfirmStatus m_statusMgr; //! 报警状态管理器

/*!
\section 管理报警记录：触发；消除；确认
*/
public:
    CMDAlarmRecord* insert(IMDDataGroup* alarmGroup, CMDAlarmConditionRT* cond, const QVariant& variableData, CMDAlarmContacter* contacter);
    bool end(CMDAlarmConditionRT* cond, const QVariant& variableData, QDateTime& startTime, QDateTime& endTime);
    bool delete1(const QString& name);
    void enumAll(QStringList& alarmIDList);
protected:
    CMDAlarmRecord* find(const QString& name);
    CMDAlarmRecord* helper_find(const QString& name);
    CMDAlarmConditionRT* findCondition(const QString& name);
protected:
    void deleteAll();

    //! 报警确认
public:
    bool acknowledge(const QString& alarmID, const QString& comment = "", const int mode = 0);
    void acknowledgeAll(const int mode = 0);
    bool confirm(const QString& alarmID, const QString& comment = "", const int mode = 0);
    void confirmAll(const int mode = 0);
    bool suppress(const QString& alarmID, const QString& comment = "");
    bool shelve(const QString& alarmID, const QString& comment = "");
    bool addComment(const QString& alarmID, const QString& comment = "");
    int getAlarmCount();
    QString getAlarmByIndex(int index);
    QString getAlarmById(QString id);
protected:
    bool confirmDirect(const QString& alarmID, const QString& comment = "");
    void confirmHelper(CMDAlarmRecord* record, const QString& comment = "");
    bool acknowledgeDirect(const QString& alarmID, bool autoAck = false, const QString& comment = "");
    void acknowledgeHelper(CMDAlarmRecord* record, bool autoAck = false, const QString& comment = "");
    bool suppressDirect(const QString& alarmID, const QString& comment = "");
    void suppressHelper(CMDAlarmRecord* record, const QString& comment = "");
    bool shelveDirect(const QString& alarmID, const QString& comment = "");
    void shelveHelper(CMDAlarmRecord* record, const QString& comment = "");
    bool addCommentDirect(const QString& alarmID, const QString& comment = "");
    void addCommentHelper(CMDAlarmRecord* record, const QString& comment = "");
protected:
    QMap<QString, CMDAlarmRecord*> m_recordMap;
    QMutex m_recordMapMutex;

/*!
\section 将管理的全部报警记录追加到 dataRecordset
*/
public:
    bool toDatabaseRecordset(CMDDatabaseRecordset* dataRecordset);
};

#endif // MDALARMRECORDMGR_H
