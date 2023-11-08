/*!
\author enli 2015.01.08
\section 管理设备通信调度、通信物理接口连接。由CMDDriverManager::startAllDevices->startDevice->bindDeviceToChannel创建
\一次产生所有Dev到时间的命令，全部处理完后，再一次产生所有到点扫描命令。送到扫描命令队列的是cmd对象无实际发送数据
\在扫描调度线程中一次执行每个cmd的实际发送数据
*/

#ifndef BASECHANNEL_H
#define BASECHANNEL_H


#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <QWaitCondition>
#include <QObject>

#include "MDDriverLibs_global.h"

#include "IDriver.h"
#include "ICommunicationComponent.h"
#include "MDDriverManagerFactory.h"
#include "MDCommunicationComponentFactory.h"
#include "BaseProtocolCommand.h"
#include "BaseDevice.h"
#include "BaseProtocolCommand.h"

class CMDSwitch;
class CMDLogSwitch;
//命令队列类型
enum class CommandQueueType
{
    Read = 0,
    Write = 1,
    Parse = 2,
};
//线程类型
enum class ThreadType
{
    //!数据块收发命令调度
    CommandThread =0,
    //!命令返回处理（正常返回，超时返回，未发送成功返回）
    ParseThread = 1,
};
//线程通信事件类型
enum class ThreadEventType
{
    ShutDowmEvent = 0,
    CommandEvent =1,
    DataIOEvent = 2,//未使用
    ParseEvent = 3,
};
class CMDSwitch;
class CCommandProcessThread;
class CParseProcessThread;

class MDDRIVERLIBSSHARED_EXPORT CBaseChannel:public QObject
{
    Q_OBJECT
    friend class CCommandProcessThread;
    friend class CParseProcessThread;
public:
    CBaseChannel();
    ~CBaseChannel();
    /*!
    \section 项目相关
    */
public:
    //!设置驱动名，利于逻辑开关和日志开关的信息过滤
    void setDriverName(const QString & driverName);
    void setProjectPath(const QString & projectPath);
private:
    QString m_driverName;//取驱动名的最后一部分，eg:MDOmronHostLink
    QString m_projectPath;
    /*!
    \section 通信参数与通信组件    */
public:
    //!通信类型
    CommunicationType commType();
    void setCommType(CommunicationType commType);
    //!通信参数
    QString commParameter();
    void setCommParameter(const QString & commParameter);
    //!获取当前通信参数（主从切换）
    QString getCurCommParameter();

    //!通信组件
    ICommunicationComponent *getCommunicationComponent();
    void setCommunicationComponent(ICommunicationComponent *communicationComponent);

    //!创建通信组件，并指定通信回调接口
    //!具体驱动需要根据需要重载，自己创建派生于接口ICommunicationComponent的通信组件（如板卡驱动）
    virtual void createCommunicationComponent();
    virtual void releaseCommunicationComponent();

private:
    QString m_commParameter;
    CommunicationType m_commType;
    ICommunicationComponent *m_communicationComponent;
    /*!
    \section 设备管理
    */
public:
    bool addDevice(CBaseDevice *device);
    bool removeDevice(const QString & devName);
    bool removeDevice(CBaseDevice *device);
    void removeAllDevices();//Y2hw
    CBaseDevice *getDevice(qint32 iDeviceIndex);
    CBaseDevice *getDevice(const QString &devName);
    bool findDevice(const QString & devName);
    qint32 getDeviceCount();

private:
    QList<CBaseDevice*> m_deviceList;
    QMutex      m_deviceListMutex;    
    /*!
    \section 命令队列管理
    */
public:
    //!塞命令到队列，唯有重试的写命令的bPriority为True
    bool addCommandToQueue(CBaseProtocolCommand *cmd, CommandQueueType cmdQueueType, bool bPriority=false);
    //!处理完一个cmd后，从命令队列移除
    CBaseProtocolCommand *removeHeadCommandFromQueue(CommandQueueType cmdQueueType);
    //!获取第一个cmd，不移除
    CBaseProtocolCommand *getHeadCommandFromQueue(CommandQueueType cmdQueueType);
    //!当写完一个命令立即读数据块时，把已在队列的cmd移除。以便可以添加到队列头部
    bool removeCommandFromQueue(CBaseProtocolCommand *cmd,CommandQueueType cmdQueueType);
    //!停止通信时移除所有队列的内容
    bool removeAllCommandFromQueue(CommandQueueType cmdQueueType);
    //!停止某个设备时，移除该设备的所有命令
    bool removeDeviceAllCommandFromQueue(CBaseDevice *dev);
    qint32 getCommandCountFromQueue(CommandQueueType cmdQueueType);
    //!获取首个写命令，检查是否是重试命令
    //!重试命令时：改为读
    CBaseProtocolCommand *removeHeadCommandFromQueue();
private:
    QList<CBaseProtocolCommand*> m_readCmdList;
    QList<CBaseProtocolCommand*> m_writeCmdList;
    QList<CBaseProtocolCommand*> m_parseCmdList;
    QList<CBaseProtocolCommand*> m_processCmdList;//Y2hw
    QMutex      m_readCmdListMutex;
    QMutex      m_writeCmdListMutex;
    QMutex      m_parseCmdListMutex;
    QMutex      m_processCmdListMutex;//Y2hw
    //删除设备时，可能命令在处理
    void        removeAllCmdFromList();//Y2hw
    /*!
    \section 通信控制
    */
public:
    //!启用通信调度+解析线程，调用bystartDevice
    bool startCommunication();
    //!关闭通信调度+解析线程
    bool stopCommunicaton();

    //void startAllDevices();
    //!由DriverManager->StartDev->BaseChannel->StartDev，运行设备，同时触发扫描和解析线程
    bool startDevice(CBaseDevice *dev);
    bool stopDevice(CBaseDevice *dev);
    //void stopAllDevices();

    //!切换通道
    virtual bool switchCommunicationChannel();

    //!当读队列中无读命令时，触发所有dev的数据块构建命令，产生命令对象到Queue    
    //!bForce：没到轮询时间也轮询，用于超时读测试
    int setupReadCommand(bool bForce = false);
    ProtocolCommandResult parseCommand(CBaseProtocolCommand *&cmd);
    //!重试命令：读命令添加到Queue的末尾；写命令添加到Queue开头
    void retryCommand(CBaseProtocolCommand *&cmd);
    //!原设计当命令发送长度<=0时，以返回命令处理
    void cancelCommand(CBaseProtocolCommand *&cmd);
    //!是否运行
    bool getStartCommunicationStatus();
    //!发送次数，此参数无显示，可以不统计
    void cmdTransimitCountIncrement();
    //!通信链路连接失败时，通知变量品质
    void notifyNotConnectDevice();
private:
    bool m_startCommunicationStatus;

    /*!
    \section 统计通信参数，目前无显示
    */
    qint64 m_cmdTransimitCount;
    qint64 m_cmdReceiveCount;
    qint64 m_cmdTimeoutCount;
    qint64 m_cmdRetryCount;
    qint64 m_cmdErrorCount;
    /*!
    \section 线程事件通信
    */
public:
    bool getEventStatus(ThreadEventType eventType);
    void setEventStatus(ThreadEventType eventType);
    void resetEventStatus(ThreadEventType eventType);
    bool waitThreadEvent(ThreadType threadType, quint64 timeOut);
public slots:
    void sendEvent(ThreadEventType eventType);
private:
    //!目前未使用
    bool m_IOEventStatus;
    //!可以扫描数据块
    bool m_commandEventStatus;
    //!可以解析命令
    bool m_parseEventStatus;
    //!停止通信
    bool m_shutDowmEventStatus;
    QMutex m_eventStatusMutex;

    /*!
    \section 扫描数据块线程
    */
    CCommandProcessThread *m_commandProcessThread;
    QWaitCondition m_commandWaitCondition;
    QMutex m_commandWaitConditionMutex;

    /*!
    \section 解析命令线程
    */
    CParseProcessThread  *m_parseProcessThread;
    QWaitCondition m_parseWaitCondition;
    QMutex m_parseWaitConditionMutex;

    /*!
    \section 功能逻辑/日志开关，控制数据收发执行
    */
public:
    //!功能开关
    CMDSwitch*  m_pollSwitch;
    //!日志开关
    CMDLogSwitch* m_logSwitch;

signals:
    void shutDownCommunicationObject();
};


class  CCommandProcessThread: public QThread
{
    Q_OBJECT
public:
    CCommandProcessThread(CBaseChannel* baseChannel);
    ~CCommandProcessThread();
private:
    CBaseChannel* m_baseChannel;
protected:
    virtual void run();
private slots:
signals:
    //void evt_SendEvent(ThreadEventType eventType);
    void evt_ConnectDevice();
};


class CParseProcessThread: public QThread
{
  Q_OBJECT
public:
    CParseProcessThread(CBaseChannel* baseChannel);
     ~CParseProcessThread();
private:
    CBaseChannel* m_baseChannel;
protected:
    virtual void run();
signals:
    //void evt_SendEvent(ThreadEventType eventType);

};

#endif // BASECHANNEL_H
