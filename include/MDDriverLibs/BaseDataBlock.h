/*!
\author enli 2015.01.08
\section 数据块实现基类.
*/
#ifndef BASEDATABLOCK_H
#define BASEDATABLOCK_H

#include <QDateTime>
#include <QMutex>
#include <QEvent>
#include <QMap>
#include "MDDriverLibs_global.h"
#include "IDataBlock.h"
#include "BaseProtocolCommand.h"
#include "SortJsonObject.h"
class CBaseDevice;
class CBaseChannel;

#define REGISTER_FORMAT_RIGHT                     0
#define REGISTER_FORMAT_ERROR                     1

#define ADDRESS_FORMAT_RIGHT                       0
#define ADDRESS_FORMAT_ERROR                       1

#define LENGTH_RIGHT                                          0
#define LENGTH_NEGATIVENUMBER_ERROR       1
#define LENGTH_OVERRUN_ERROR                       2

#define FLAG_RIGHT                                                0
#define FLAG_REGISTER_ERROR                             1
#define FLAG_STARTADDRESS_ERROR                    2
#define FLAG_ENDADDRESS_ERROR                       3
#define FLAG_LENGT_NEGATIVE_ERROR                4
#define FLAG_LENGT_OVERRUN_ERROR                5

class MDDRIVERLIBSSHARED_EXPORT CBaseDataBlock:public IDataBlock
{
    Q_OBJECT
    Q_PROPERTY(QMap translateName READ translateName)
    Q_PROPERTY(QString GUID_General)
    Q_PROPERTY(bool Enable READ isEnabled WRITE setEnabled)
    Q_PROPERTY(QString Name READ name WRITE setNameProp)
    Q_PROPERTY(QString Description READ description WRITE setDescription)
    Q_PROPERTY(int PollRate READ pollRate WRITE setPollRate)

public:
    CBaseDataBlock();
    ~CBaseDataBlock();
/*!
\section IMDTreeItem接口
*/
public:
    MDTreeItemType itemType();
    void setItemType(MDTreeItemType itemType);
    void *parentItem(MDTreeItemType parentItemType);
    void setParentItem(void* parent,MDTreeItemType parentItemType);
private:
    MDTreeItemType m_itemType;
/*!
\section 关联变量
*/
public:
    //!添加变量
    bool addVariable(IMDRTDBVariable* nsVar,QString &error);
    //!移除变量
    bool removeVariable(IMDRTDBVariable* nsVar);
    //!删除所有变量
    void removeAllVariable();
    //!获取变量
    IMDRTDBVariable* getVariableAt(int index);
    //!获取变量CBaseChannel *getChannel();
    IMDRTDBVariable *getVariable(const QString & varName);
    //!获取变量数量
    int getVariableCount();
    //!数据包记录到文件
    bool msgMonitor();
    void setMsgMonitor(bool monitor);
    MDMonitorType monitorMode();
    void setMonitorMode(MDMonitorType monitorType);
    void setMsgWindow(QDialog* dialog);
    void emitPacket(bool bSend, quint8 *dataBuf,  qint16 startIndex, qint16 length);

private:
    QList<IMDRTDBVariable*> m_variableList;
    QMutex m_variableListMutex;

    //!属性框组名及属性名翻译
    QMap<QString,QVariant> translateName(){return m_translateMap;}
    QMap<QString,QVariant>  m_translateMap;
/*!
\section 基本属性
*/
public:
    //!名称
    QString name();
    void setName(QString name);
    //名称属性
    void setNameProp(QString name);

    //!描述
    QString description();
    void setDescription(QString description);
    //!关联Device
    CBaseDevice *device();
    //!关联Channel
    CBaseChannel *getChannel();
    void setChannel(CBaseChannel *channel);
    //!启用状态
    bool isEnabled();
    void setEnabled(bool enabled);
    //!通信品质
    MDVariableDataQuality quality();
    void setQuality(MDVariableDataQuality quality);
    //!时间戳
    QDateTime timeStamp();
    void setTimeStamp(QDateTime timeStamp);
    //!扫描周期
    quint32 pollRate();
    void setPollRate(quint32 pollRate);

    int pollRateProp();
    void setPollRateProp(int pollRate);

    //!可写状态
    bool isWriteEnabled();
    void setWriteEnabled(bool enabled);

    //!寄存器+开始+结束地址等验证结果
    void setVerifyFlag(int verifyflag);
    int verifyFlag();

    std::vector<std::string> nameStringVector();
private:
    QString m_name;
    QString m_description;
    CBaseDevice *m_device;
    CBaseChannel *m_channel;
    bool m_enabled;
    //if poll rate is huge,read once after start
    bool    m_firstReadDone;//Y2hw
    MDVariableDataQuality m_quality;
    QDateTime m_timeStamp;
    quint32   m_pollRate;
    quint32   m_curPollRate; //!当前的扫描周期，通信故障时，将启用通信错误的周期
    bool      m_writeEnabled;
    int       m_nVerifyFlag;//!数据配置验证
//!拷贝
public:
    virtual void copy(IDataBlock *otherDataBlock);
/*!
\section 通信管理
*/
public:
    virtual bool start();
    virtual bool stop();
    bool setupReadCommand(bool bPollNow=false,bool bForce = false);
    //!处理返回数据包时设置一些统计参数
    void onReceiveCommand(bool IsReadCmd,ProtocolCommandResult cmdRes);
    void onTimeout(bool retry);

    void resetStatistics();
    qint64 transmitCount();
    void   incrementTransmitCount();
    qint64 receiveCount();
    void   incrementReceiveCount();
    qint64 timeoutCount();
    void   incrementTimeoutCount();
    qint64 retryCount();
    void   incrementRetryCount();
    qint64 errorCount();
    void   incrementErrorCount();

    QDateTime lastReadTime();
    QDateTime lastWriteTime();
    QDateTime lastErrorTime();
    void resetReadMessageState();
     void notifyNotConnectDevice();
private:
    bool m_running;
    bool m_readMessageState;

/*!
section 数据包监控
*/
    //!是否把收发数据转换未Ascii
    MDMonitorType m_msgMonitorAscii;
    bool m_msgMonitor;
    QDialog* m_MsgMonitorWnd;

private:
    QTime m_startTimeToRead;
    CBaseProtocolCommand  *m_readCmd;
    qint64 m_cmdTransimitCount;
    qint64 m_cmdReceiveCount;
    qint64 m_cmdTimeoutCount;
    qint64 m_cmdRetryCount;
    qint64 m_cmdErrorCount;

    QDateTime m_lastReadTime;
    QDateTime m_lastWriteTime;
    QDateTime m_lastErrorTime;

    QMutex  m_objMutex;
/*!
\section 写变量
*/
public:
     bool asyncWriteVariable(IMDRTDBVariable *nsVar);
     bool asyncWriteVariables(QList<IMDRTDBVariable*>& nsVarList);
/*!
\section 具体驱动需要实现的函数
*/
public:
     //!创建命令
     virtual CBaseProtocolCommand *createCommand(ProtocolCommandType cmdType)=0;
     //!分析命令:变量值的更新函数中完成
     virtual void parseCommand(CBaseProtocolCommand *cmd)=0;
     /*
    virtual bool checkVarAddress(IMDRTDBVariable* nsVar,QString &error)=0;
     */
     virtual void serialize(CSortJsonObject& json)=0;
     virtual void deserialize(const QJsonObject &json)=0;

/*!
\section 运行时动态添加设备
*/
public:
  /*!
  动态设置参数
  \param deviceParameterMap 数据块参数。
  \returns 无。
  \note
  */
 virtual void dynamicSetParameter(const QVariantMap &dataBlockMap);


};


/*
class CDataBlockPacketEvent: public CPacketEvent
{
public:
    CDataBlockPacketEvent(Type MyEventType);
    QString getPacket();
    QEvent::Type getEventTYpe();

   QString m_strPacket;
private:
   QEvent::Type m_EventType;
};
*/
#endif // BASEDATABLOCK_H
