/*!

\author dGFuZ3p3

\section 功能

设备云的客户端类，负责连接到 Redis 服务器，并处理与服务器的数据交互。

*/

#ifndef MDDEVICECLOUDCLIENT_H
#define MDDEVICECLOUDCLIENT_H

#include <QObject>
#include <QTimer>
#include "MDRedisClient.h"

#include "MDDeviceGetter.h"
#include "../../include/interface/Common/IMDDataType.h"
#include "MDDeviceCommandEngine.h"
#include "MDRTDBInterfaces/IMDChangeVariable.h"

#include "MDSysLogExpert.h"
#define LOG_SOURCE "DeviceCloud"

class CMDDeviceCloudExpertImp;
class CDeviceCloudClientPara;
class CMDSwitch;
class CMDLogSwitch;
class MMKV;
class CMDDeviceCloudClient : public QObject
{
    Q_OBJECT
public:
    explicit CMDDeviceCloudClient(QObject *parent = 0);
    ~CMDDeviceCloudClient();
/*!
\section 初始化与释放
*/
public:
    bool load(CMDDeviceCloudExpertImp* expert, CDeviceCloudClientPara* clientPara);
    bool loadKV(CMDDeviceCloudExpertImp* expert, CDeviceCloudClientPara* clientPara);
    void unload();
    QString m_cloudClientID;
    CMDDeviceCloudExpertImp* m_expert;
public:
    void handleStatusChanged(const QString& msg);
protected:
    bool isIpAFile(const QString& strIP);
    void test_load_from_file(const QString& strTestFile);
    bool m_testing;

/*!
\section 管理与服务器的连接
*/
public:
    bool isPublishRedisConnected();
private:
    CMDRedisClient *m_subscribeRedis;
    CMDRedisClient *m_publishRedis;
    CMDRedisClient *m_variableRedis;
    CMDRedisClient *m_alarmRedis;
    MMKV* m_mmkv;
    bool m_kvLoaded;
private:
    bool setMMKVValue(const QString& name, const QVariant& value);
public slots:
    void onRedisConnected();
    void onRedisDisconnected();

/*!
\section 发送命令到服务器
*/
public:
    void sendCommand_GetDeviceIDList();
    void sendCommand_GetDeviceIDList_ByEngine();
public:
    void sendCommand_GetDeviceInfo(const QString& deviceID);
    bool sendCommand_GetDeviceInfo_ByEngine(const QString& deviceID);
    void sendCommand_ReloadDevice(const QString& deviceID);
public:
    bool sendAlarm(short type, const QString& name, const QString& msg, const QDateTime& startTime, const QDateTime& endTime);
public:
    void buildAlarmCmd(short type, const QString& name, const QString& msg, const QDateTime& startTime, const QDateTime& endTime,
                                        QList<QByteArray>& rtCmd, QList<QByteArray>& rtDeviceCmd,
                                        QList<QByteArray>& archiveAddCmd, QList<QByteArray>& archiveDelCmd);
    QString buildArchiveAlarmValue(const QString& typeString, const QString& alarmID, const QString& msg, const QString& startTimeString, const QString& endTimeString);
public:
    bool sendVariable(IMDChangeVariable* changedVariable);
    bool sendDeviceVariableChanged(const QList<QByteArray>& jsonMsgList);
    bool sendVariableBatch(const QList<QByteArray>& redisCmdList);
    bool sendVariableBatchToKVCache(const QStringList& varNameList, const QList<QVariant>& varValueList);
protected:
    void buildVariableCmd(IMDChangeVariable* changedVariable, bool shouldArchive,
                                                QList<QByteArray>& rtCmd, QList<QByteArray>& archiveCmd);
    bool helperShouldArchive(IMDChangeVariable* changedVariable);
    QString helperBuildJsonString(const QStringList& nameList, const QStringList& valueList);

protected:
    //! 设备命令引擎：避免在槽函数 onSubscribeMessage 中发送数据
    //! ----------------------------------------------------------------------
    CMDDeviceCommandEngine m_deviceCommandEngine;

/*!
\section 处理来自服务器的设备信息
*/
public slots:
    void messageRet(QList<QByteArray>& msg);
protected:
    bool handleCloudMsg(const QString& msg); //! 处理来自云的消息
protected:
    bool handleDeviceIDList(const QVariantMap& rootMap);
    bool handleDeviceInfo(const QVariantMap& rootMap);
    QString msgSize(const QString& msg);
protected:
    CMDDeviceGetter m_deviceGetter;

/*!
\section 功能开关和日志开关
*/
public:
    void createSwitchs(const QString& pid, bool create);
public:
    CMDSwitch* m_sHandleCloudMsg;
    CMDSwitch* m_sSendAlarm;
    CMDSwitch* m_sSendVar;
    CMDSwitch* m_sSendVar2KVCache;
public:
     CMDLogSwitch* m_lsDeviceFlow;
     CMDLogSwitch* m_lsDeviceDetail;
     CMDLogSwitch* m_lsSendAlarm;
     CMDLogSwitch* m_lsSendVar;
     CMDLogSwitch* m_lsSendVar2KVCache;
};

#endif // MDDEVICECLOUDCLIENT_H
