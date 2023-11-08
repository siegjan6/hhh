/*!

\author dGFuZ3p3

\section 功能

报警状态及报警状态管理类。

\section  扩展模式如何报警？

当一条基本报警记录产生时，一个 CMDAlarmRecordStatus 对象被创建，
并由 CMDAlarmRecordMgr 的成员 m_statusMgr 插入到管理队列中；

代码参考 CMDAlarmRecordMgr::insert 和 CMDAlarmRecordMgr_OnConfirmStatus::insert
\code
CMDAlarmRecord* CMDAlarmRecordMgr::insert(...)
{
    CMDAlarmRecord* newRecord = add(cond->m_condition->name());
    ...
        m_statusMgr.insert(newRecord);
    ...
}
\endcode

当一条基本报警记录消除时，对应的 CMDAlarmRecordStatus 对象依然存在，
且该对象的 m_recordStatus 成员的队列长度为 2；

代码参考 CMDAlarmRecordMgr::end 和 CMDAlarmRecordMgr_OnConfirmStatus::end
\code
bool CMDAlarmRecordMgr::end(...)
{
    ...
        m_statusMgr.end(existRecord);
    ...
}
\endcode

CMDAlarmRecordStatus 对象消除的条件是报警结束且已经被确认。
一个 CMDAlarmRecordStatus 对象将产生 3 条归档记录（发生时；确认时；结束时）。

\section  理解报警状态管理
假设只定义了一个报警，且该报警周期性地产生和消除，则：
基本报警记录队列中，始终只有 1 条记录；
报警状态队列的长度则在一直增加，除非进行了“确认”操作。

*/

#ifndef MDALARMRECORDSTATUSMGR_H
#define MDALARMRECORDSTATUSMGR_H

#include "MDAlarmRecord.h"
#include <QMutex>

class CMDAlarmExpertImp;
/*!
 * \brief 报警记录状态管理类
 */
class CMDAlarmRecordMgr_OnConfirmStatus
{    
    friend class CMDAlarmRecordMgr;
protected: //! 只能依赖 CMDAlarmRecordMgr 的实例存在，不能单独实例化
    CMDAlarmRecordMgr_OnConfirmStatus();
    virtual ~CMDAlarmRecordMgr_OnConfirmStatus();

    CMDAlarmExpertImp* m_alarmExpert ;// NULL;
    static QString recordID(CMDAlarmRecord* record);

/*!
\section 管理报警状态记录：触发；确认
*/
protected:
    CMDAlarmRecordStatus* insert(CMDAlarmRecord* record);
    bool end(CMDAlarmRecord* record);
    CMDAlarmRecordStatus* find(const QString& rid);
    CMDAlarmRecordStatus* helper_find(const QString& rid);
    CMDAlarmConditionRT* findCondition(const QString& rid);
    void enumAll(QStringList& alarmIDList);
protected:
    bool delete1(const QString& rid);
    void deleteAll();    
protected:
    bool acknowledgeDirect(const QString& alarmID, const QString& comment = ""); //!报警知悉
    bool confirmDirect(const QString& alarmID, const QString& comment = ""); //! 报警确认
    bool suppressDirect(const QString& alarmID, const QString& comment = ""); //! 报警抑制
    bool shelveDirect(const QString& alarmID, const QString& comment = ""); //! 报警搁置
    bool addCommentDirect(const QString& alarmID, const QString& comment = ""); //! 报警备注
protected:
    QMap<QString, CMDAlarmRecordStatus*> m_recordMap;
    QMutex m_recordMapMutex;

/*!
\section 将管理的全部报警状态记录追加到 dataRecordset
*/
public:
    bool toDatabaseRecordset(CMDDatabaseRecordset* dataRecordset);
};

#endif // MDALARMRECORDSTATUSMGR_H
