#include "BaseProtocolCommand.h"
#include "BaseChannel.h"
#include "BaseDevice.h"
#include "BaseDataBlock.h"
CBaseProtocolCommand::CBaseProtocolCommand()
{
    m_isRetryCommand = false;
    m_commandType=ProtocolCommandType::Read;
    m_retries=0;
    m_channel=NULL;
    m_device=NULL;
    m_dataBlock=NULL;
    m_communicationQuality=false;
    m_commandLength=0;
    m_receiveDataLength=0;
    m_replyTimeout=1000;
    m_receiveBufferSize=0;
    m_commandResult=ProtocolCommandResult::None;
    m_startTimeoutTime.start();
    m_writeVarList.clear();
    m_okReadMsg = false;
}

CBaseProtocolCommand::~CBaseProtocolCommand()
{
    for(int i=0;i<m_writeVarList.count();i++)
    {
        delete m_writeVarList[i];
    }
    m_writeVarList.clear();
}

//!命令类型
ProtocolCommandType CBaseProtocolCommand::getCommandType()
{
    return m_commandType;
}

void CBaseProtocolCommand::setCommandType(ProtocolCommandType cmdType)
{
    m_commandType = cmdType;
}
//!重试次数
quint32 CBaseProtocolCommand::getRetries()
{
    return m_retries;
}

void CBaseProtocolCommand::setRetries(quint32 retries)
{
    m_retries = retries;
}

//!关联Device
CBaseDevice *CBaseProtocolCommand::getDevice()
{
    return m_device;
}

void CBaseProtocolCommand::setDevice(CBaseDevice *device)
{
    m_device = device;
    if(m_device)
    {
       m_retries =  m_device->retries();
       m_replyTimeout = m_device->replyTimeout();
    }
}

//!关联Channel
CBaseChannel *CBaseProtocolCommand::getChannel()
{
    return m_channel;
}

void CBaseProtocolCommand::setChannel(CBaseChannel *channel)
{
    m_channel = channel;
}

//!关联DataBlock
CBaseDataBlock *CBaseProtocolCommand::getDataBlock()
{
    return m_dataBlock;
}

void CBaseProtocolCommand::setDataBlock(CBaseDataBlock *dataBlock){
    m_dataBlock = dataBlock;
}

//!待发命令长度
qint16 CBaseProtocolCommand::getCommandLength()
{
    return m_commandLength;
}

void CBaseProtocolCommand::setCommandLength(qint16 commandLength)
{
    m_commandLength = commandLength;
}
//!接收缓冲区已经接收的数据长度
quint16 CBaseProtocolCommand::getReceiveDataLength()
{
    return m_receiveDataLength;
}

void CBaseProtocolCommand::setReceiveDataLength(quint16 receiveDataLength)
{
    m_receiveDataLength = receiveDataLength;
}
//!接收缓冲区的大小
quint16 CBaseProtocolCommand::getReceiveBufferSize()
{
    return m_receiveBufferSize;
}

void CBaseProtocolCommand::setReceiveBufferSize(quint16 receiveBufferSize)
{
    m_receiveBufferSize = receiveBufferSize;
}
//!命令分析结果
ProtocolCommandResult CBaseProtocolCommand::getCommandResult()
{
    return m_commandResult;
}
void CBaseProtocolCommand::setCommandResult(ProtocolCommandResult commandResult)
{
    m_commandResult = commandResult;
}
//!命令超时
quint32 CBaseProtocolCommand::getReplyTimeout()
{
    return m_replyTimeout;
}
void CBaseProtocolCommand::setReplyTimeout(quint32 replyTimeout)
{
    m_replyTimeout = replyTimeout;
}
//!待写变量链表
void CBaseProtocolCommand::addWriteVar(IMDRTDBVariable* var)
{
    m_writeVarList.append(var);
}
void CBaseProtocolCommand::addWriteVar(QList<IMDRTDBVariable *>& nsVarList)
{
    m_writeVarList.append(nsVarList);
}
IMDRTDBVariable* CBaseProtocolCommand::getWriteVarAt(int index)
{
    if(index>=0 && index<m_writeVarList.count())
    {
        return m_writeVarList[index];
    }
    return NULL;
}
quint16 CBaseProtocolCommand::getWriteVarCount()
{
   return m_writeVarList.count();
}
/*!
\section 超时处理
*/
void CBaseProtocolCommand::startReplyTimeout()
{
    if(NULL != m_device)
    {
       m_replyTimeout = m_device->replyTimeout();
    }
    m_startTimeoutTime.restart();
}

bool CBaseProtocolCommand::isTimedOut()
{
    quint32 timeDiff= m_startTimeoutTime.elapsed();
    if(timeDiff > m_replyTimeout)
    {
        return true;
    }
    return false;
}

void CBaseProtocolCommand::reSetRetries()
{
    if(NULL!=m_device && NULL!=m_dataBlock)
    {
        if(MDVariableDataQuality::Good==m_dataBlock->quality() ||
           MDVariableDataQuality::None==m_dataBlock->quality())
        {
            m_retries=m_device->retries();
        }
        else
        {
            m_retries=0;
        }
    }
}
void CBaseProtocolCommand::reduceRetries()
{
    if(m_retries>0)
    {
        m_retries--;
    }
}
void CBaseProtocolCommand::startRetryCommand()
{
    m_isRetryCommand = true;
}
void CBaseProtocolCommand::stopRetryCommand()
{
    m_isRetryCommand = false;
}
bool CBaseProtocolCommand::isRetryCommand()
{
    return m_isRetryCommand;
}
bool  CBaseProtocolCommand::isOKReadMsg()
{
    return m_okReadMsg;
}

void  CBaseProtocolCommand::setReadMsgFlag(bool flag)
{
    m_okReadMsg = flag;
}
