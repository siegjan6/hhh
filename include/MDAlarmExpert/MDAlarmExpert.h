/*!

\author dGFuZ3p3

\section 功能

报警专家类，实现运行时的报警处理。

*/

#ifndef MDALARMEXPERT_H
#define MDALARMEXPERT_H

#include <QString>
#include <QDateTime>
#include <QVariantList>
#include "MDAlarmExpert_Global.h"
#include "../../include/MDAlarmConfiger/MDAlarmConfiger.h"

class CMDRawVariableEvent;
class CMDAlarmContacter;
class CMDAlarmConfiger;
class CMDAlarmExpertImp;
class CMDHistoryExpert;
class CMDDatabaseRecordset;
class QUaFolderObject;
class MDALARMEXPERTSHARED_EXPORT CMDAlarmExpert
{
public:
    CMDAlarmExpert();
    ~CMDAlarmExpert();
/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, CMDAlarmConfiger* configer, CMDAlarmContacter* contacter,
                           short source, CMDHistoryExpert* historyExpert, unsigned long timeout = 0);
    void load();
	void load(void* ptr);
    void unload();
public:
    CMDAlarmConfiger* configer();

/*!
\section 报警触发；缓存最近报警到内存；获取最近报警
*/
public:
    bool submitRawAlarmEvent(CMDRawVariableEvent* rawEvent);
    QDateTime recentUpdateTime(); //! 获取最近一次报警数据更新的时间（如触发、确认等）
public:
    bool alarmRecordSet(CMDDatabaseRecordset* dataRecordset); //! 获取最近报警（内存缓存的报警）

/*!
\section 报警条件改变
*/
public:
    bool submitDeviceAlarm(const QString& deviceID, const QVariantList &deviceAlarmList);

/*!
\section 报警确认
*/
public:
    bool acknowledgeRequired(const QString& alarmName);
    // mode = 0: from ua call
    // mode = 1: from mindscada call
    bool acknowledge(const QString& alarmID, const QString& comment = "", const int mode = 0);
    void acknowledgeAll(const int mode = 0);
    bool confirmRequired(const QString& alarmName);
    bool confirm(const QString& alarmID, const QString& comment = "", const int mode = 0);
    void confirmAll(const int mode = 0);
    bool suppress(const QString& alarmID, const QString& comment = "");
    bool shelve(const QString& alarmID, const QString& comment = "");
    bool addComment(const QString& alarmID, const QString& comment = "");
    int getAlarmCount();
    QString getAlarmByIndex(int index);
    QString getAlarmById(QString id);
    QString getAlarmConfById(QString id);

protected:
    CMDAlarmExpertImp* m_imp;
};

#endif // MDALARMEXPERT_H
