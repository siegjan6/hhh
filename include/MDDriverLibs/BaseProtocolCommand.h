/*!
\author enli 2015.01.08
\section 协议命令实现基类
*/
#ifndef BASEPROTOCOLCOMMAND_H
#define BASEPROTOCOLCOMMAND_H

#include <QTime>
#include "MDDriverLibs_global.h"
#include "IMDRTDBVariable.h"
/*!
\section 命令类型
*/
enum class ProtocolCommandType
{
    Read = 0,
    Write = 1,
    BlockWrite = 2,
};
/*!
\section 命令执行结果
*/
enum class ProtocolCommandResult
{
    None = 0,
    Right = 1,
    RightAndNextCmd = 2,
    SplitData = 3,
    Error = 4,
    FailedToSend = 5,
    TimeOut = 6,
    RightAndWithoutRet = 7,//发送接收后，需再次回复一个包到PLC。
    ErrorAndWithoutRet = 8,//解析时不刷新数据
    DataBlockBConfigErr = 9,
};
class CBaseDataBlock;
class CBaseDevice;
class CBaseChannel;
class MDDRIVERLIBSSHARED_EXPORT CBaseProtocolCommand
{
public:
    CBaseProtocolCommand();
    ~CBaseProtocolCommand();
/*!
\section 基本属性
*/
public:
    //!命令类型
    ProtocolCommandType getCommandType();
    void setCommandType(ProtocolCommandType cmdType);
    //!重试次数
    quint32 getRetries();
    void setRetries(quint32 retries);
    //!关联Device
    CBaseDevice *getDevice();
    void setDevice(CBaseDevice *device);
    //!关联Channel
    CBaseChannel *getChannel();
    void setChannel(CBaseChannel *channel);
    //!关联DataBlock
    CBaseDataBlock *getDataBlock();
    void setDataBlock(CBaseDataBlock *dataBlock);
    //!待发命令长度
    qint16 getCommandLength();
    void setCommandLength(qint16 commandLength);
    //!接收缓冲区已经接收的数据长度
    quint16 getReceiveDataLength();
    void setReceiveDataLength(quint16 receiveDataLength);
    //!接收缓冲区的大小
    quint16 getReceiveBufferSize();
    void setReceiveBufferSize(quint16 receiveBufferSize);
    //!命令分析结果
    ProtocolCommandResult getCommandResult();
    void setCommandResult(ProtocolCommandResult commandResult);
    //!命令超时
    quint32 getReplyTimeout();
    void setReplyTimeout(quint32 replyTimeout);
    //!待写变量链表
    void addWriteVar(IMDRTDBVariable* var);
    void addWriteVar(QList<IMDRTDBVariable *>& nsVarList);
    IMDRTDBVariable*  getWriteVarAt(int index);
    quint16 getWriteVarCount();
private:
    ProtocolCommandType m_commandType;
    quint32 m_retries;
    CBaseChannel *m_channel;
    CBaseDevice  *m_device;
    CBaseDataBlock *m_dataBlock;
    bool        m_communicationQuality;
    qint16      m_commandLength;
    quint16     m_receiveDataLength;
    quint16     m_receiveBufferSize;
    ProtocolCommandResult m_commandResult;
    quint32   m_replyTimeout;
    QList<IMDRTDBVariable*> m_writeVarList;    
/*!
\section 命令重试控制
*/
public:
    void startReplyTimeout();
    bool isTimedOut();
    void reSetRetries();
    void reduceRetries();
    bool  isRetryCommand();
    void  startRetryCommand();
    void  stopRetryCommand();
    bool  isOKReadMsg();
    void  setReadMsgFlag(bool flag);
private:
    QTime m_startTimeoutTime;
    bool  m_isRetryCommand;
    bool  m_bMonitor;
    bool  m_okReadMsg;//Y2hw
/*!
\section 具体驱动需要实现的函数
*/
public:
    virtual ProtocolCommandResult parseCommand()=0;
    virtual quint8* getReceiveData()=0;
    virtual quint8* getSendData()=0;
    virtual void  setupCommand()=0;

};

#endif // BASEPROTOCOLCOMMAND_H
