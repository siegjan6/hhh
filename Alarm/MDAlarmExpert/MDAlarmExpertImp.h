/*!

\author dGFuZ3p3

\section 功能

报警专家类，实现运行时的报警处理。

\section  基本模式如何报警？

m_variableMgr 用于对报警变量做索引，当变量变化时，
可快速找到关联的报警条件。

m_recordMgr 用于管理基本的报警记录，其规则如下：
当报警条件满足时，新建或更新一条报警；
当报警条件不满足时，消除一条报警。
\note 代码参考 CMDAlarmExpertImp::handleCondition 。

扩展的报警模式，则是依赖于 m_recordMgr 进行处理，
具体可参考 CMDAlarmRecordMgr 的成员 m_statusMgr，当
对 CMDAlarmRecordMgr 操作时，m_statusMgr 也会进行
相应的操作。

\section  多线程安全性分析

要求外部在同一线程中进行初始化和释放，即 init 和 release 必须在
同一线程被调用，该线程称为 "初始化线程"；（这是容易被满足的要求）

运行期间的报警处理【产生、消除和确认】及报警条件变化，
均交由 “报警引擎线程” 处理，必定是在同一线程被调用；

“报警引擎线程”在初始化之后被启动，在释放之前被终止，
即"初始化线程"和“报警引擎线程”不会同时操作数据结构，
不存在线程同步问题；（参考 init 和 release）

获取报警，可能在任意的外部线程被调用，无法控制。

\section  可能在多线程中被使用和修改的数据结构

（1）m_configer（主要是关联的 CMDAlarmConditionConfiger 指针）
（2）m_variableMgr（管理报警变量）
初始化和释放-修改，初始化线程
条件变化时-修改，报警引擎线程
处理报警时-使用，报警引擎线程
\note 上述对象管理的数据结构，不需做线程保护

（3）m_recordMgr（管理基本的报警记录）
初始化和释放-修改，初始化线程
处理报警时-修改，报警引擎线程
确认报警-修改，报警引擎线程
获取报警时-使用，任意的外部线程（参考 alarmRecordSet）
\note 上述对象管理的数据结构，必须做线程保护

*/

#ifndef MDALARMEXPERTIMP_H
#define MDALARMEXPERTIMP_H

#include "MDAlarmVariable.h"
#include "MDAlarmRecord.h"
#include "MDAlarmRecordMgr.h"
#include "MDAlarmRecordStatusMgr.h"
#include "MDAlarmCommandEngine.h"
#include <QReadWriteLock>

#include "CommonDevice/MDCommonDeviceMgr.h"
#include "MDSwitchFactory/MDFuncSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"

#include <QUaServer>
#ifdef UA_ENABLE_HISTORIZING
#ifndef SQLITE_HISTORIZER
#include "../../Common/MDUaServer/QUaserver/wrapper/quainmemoryhistorizer.h"
#else
#include "../../Common/MDUaServer/QUaserver/wrapper/quasqlitehistorizer.h"
#endif // !SQLITE_HISTORIZER
#endif // UA_ENABLE_HISTORIZING

class CMDAlarmConfiger;
class CMDAlarmConditionConfiger;
class CMDAlarmLevelConfiger;
class CMDAlarmContacter;
class CMDRawVariableEvent;
class CMDHistoryExpert;
class CMDAlarmRecord;
class IMDDataItem;
class CMDCommonDevice;
class CMDDataArchivesEngine;
class CMDAlarmExpertImp : QObject
{
    Q_OBJECT
public:
    CMDAlarmExpertImp();
    ~CMDAlarmExpertImp();
/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, CMDAlarmConfiger* configer, CMDAlarmContacter* contacter,
                           short source, CMDHistoryExpert* historyExpert, unsigned long timeout);
    //void load();
	void load(void* ptr=NULL);
    void unload();
public:
    MDAlarmMode m_alarmMode ;// MDAlarmMode::common;
    CMDAlarmConfiger* m_configer ;// NULL;
protected:
    QString m_pid;
    CMDAlarmContacter* m_contacter ;//  NULL;
    unsigned long m_timeout;

/*!
\section 设置报警条件来源；解析报警条件
*/
public:
    //! 报警条件来源，若要改变，必须在 init 之前设置
    void setAlarmSource(short source);
    short alarmSource();
protected:
    short m_alarmSource ;// 0; //! 缺省来自项目，即 CMDAlarmConfiger
protected:
    void helper_parseConditions(QList<CMDAlarmCondition*>& conditionList, CMDAlarmLevelConfiger* levelConfiger, void* ptr = NULL);
    bool helper_bindConditionWithAlarmVariable(CMDAlarmCondition* cond, const QString& varName, CMDAlarmLevelConfiger* levelConfiger);
    void buildConditionsFromConfiger(void* ptr = NULL);
public:
    bool submitDeviceAlarm(const QString& deviceID, const QVariantList &deviceAlarmList); //! 提交报警条件
    void buildConditionsFromCloud(CMDCommonDevice* device);
protected:
    CMDCommonDeviceMgr m_deviceMgr;

/*!
\section 报警命令引擎
*/
protected:
    CMDAlarmCommandEngine m_alarmEngine;

/*!
\section 报警触发；缓存最近报警到内存；获取最近报警
*/
public:
    bool submitRawAlarmEvent(CMDRawVariableEvent* rawEvent);
public:
    bool processRawAlarmEvent(CMDRawVariableEvent* rawEvent);
    bool handleCondition(CMDAlarmConditionRT* cond, CMDRawVariableEvent* rawEvent);
protected:
    void createSwitchs(bool create);
    CMDLogSwitch* m_lsAlarmProcess;
public:
    CMDAlarmVariableMgr m_variableMgr; //! 按照名字对报警变量做索引
    CMDAlarmRecordMgr m_recordMgr; //! 管理基本的报警记录
    bool alarmRecordSet(CMDDatabaseRecordset* dataRecordset);
protected:
    IMDDataGroup* m_alarmGroup ;// NULL;
public:
    QDateTime readUpdateTime();
    void writeUpdateTime();
protected:
    QDateTime m_updateTime; //! 报警队列最近更新时间，外部可据此判断队列状态是否更新
    QReadWriteLock m_rwLock; //! 线程安全地访问 m_updateTime（使用读写锁，提高并发性）

/*!
\section 报警确认及安全策略
*/
public:
    bool acknowledgeRequired(const QString& alarmName);
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
public:
    CMDActionSP* helper_acknowledgeSP(CMDAlarmCondition* cond);
    CMDActionSP* helper_confirmSP(CMDAlarmCondition* cond);
    bool checkSP_WithUI(CMDAlarmConditionRT* cond, int checkMode = 0);
public:
    bool submitAcknowledge(const QString& alarmID, const QString& comment = "");
    bool submitConfirm(const QString& alarmID, const QString& comment = "");
    bool submitSuppress(const QString& alarmID, const QString& comment = "");
    bool submitShelve(const QString& alarmID, const QString& comment = "");
    bool submitAddComment(const QString& alarmID, const QString& comment = "");
    bool acknowledgeDirect(const QString& alarmID, const QString& comment = "");
    bool confirmDirect(const QString& alarmID, const QString& comment = "");
    bool suppressDirect(const QString& alarmID, const QString& comment = "");
    bool shelveDirect(const QString& alarmID, const QString& comment = "");
    bool addCommentDirect(const QString& alarmID, const QString& comment = "");

/*!
\section 报警归档，通过历史专家类实现报警归档
*/
public:
    void setHistoryExpert(CMDHistoryExpert* historyExpert);
    bool doArchive(CMDAlarmRecord* record, int insertOrUpdate = 0);
public:
    bool updataUaConfirm(CMDAlarmRecord* record);
    bool updataUaAcknowledge(CMDAlarmRecord* record);
    bool updataUaSuppress(CMDAlarmRecord* record);
    bool updataUaShelve(CMDAlarmRecord* record);
    bool updataUaComment(CMDAlarmRecord* record);
protected:
    CMDHistoryExpert* m_historyExpert;
    CMDDataArchivesEngine* m_dataArchivesEngine;

    void deleteRecord(const QString& alarmID);

private slots:
    void slotAlarmActivated();
    void slotAlarmDeactivated();
//    void slotAlarmConditionAcknowledged();
//    void slotAlarmConditionConfirmed();
//    void slotAlarmConditionEnabled();
//    void slotAlarmConditionDisabled();
//    void slotAlarmAddedComment(const QString& comment);
//    void slotAlarmRetainChanged();
//    void slotAlarmSeverityChanged();
    void slotAlarmTriggered();

private:
    QUaServer* m_pServer;
	QUaFolderObject* m_alarmsFolder;

    template<typename T>
    T* addChild(QUaBaseObject* obj, const QUaQualifiedName& browseName, const QUaNodeId &nodeId = QUaNodeId());

};

template <typename T>
inline T* CMDAlarmExpertImp::addChild(QUaBaseObject* obj, const QUaQualifiedName& browseName, const QUaNodeId &nodeId)
{
    return obj->addChild<T>(browseName, nodeId);
}

#endif // MDALARMEXPERTIMP_H
