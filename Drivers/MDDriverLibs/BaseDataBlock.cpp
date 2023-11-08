#include<QApplication>
#include<QMessageBox>

#include "BaseDataBlock.h"
#include "BaseDevice.h"
#include "BaseChannel.h"
#include "MDMultiLanguage.h"
#include "CustomEvent.h"
#include <QDebug>

CBaseDataBlock::CBaseDataBlock()
{
    m_translateMap.insert("GUID_General", tr("通用"));
    m_translateMap.insert("Enable", tr("启用"));
    m_translateMap.insert("Name", tr("数据块名"));
    m_translateMap.insert("Description", tr("描述"));
    m_translateMap.insert("PollRate", tr("扫描频率(秒)"));

    m_name = "";
    m_description= "";

    m_device=NULL;
    m_channel=NULL;
    m_enabled=false;
    m_quality=MDVariableDataQuality::None;
    m_pollRate=1000;
    m_writeEnabled=true;

    m_running=false;
    m_readMessageState=false;
    m_readCmd=NULL;
    m_cmdTransimitCount=0;
    m_cmdReceiveCount=0;
    m_cmdTimeoutCount=0;
    m_cmdRetryCount=0;
    m_cmdErrorCount=0;
    m_startTimeToRead.start();

    m_msgMonitorAscii = MDMonitorType::Binary;
    m_msgMonitor = false;
    m_MsgMonitorWnd = NULL;
    m_itemType = MDTreeItemType::DataBlock;
    m_nVerifyFlag = FLAG_RIGHT;
}
CBaseDataBlock::~CBaseDataBlock()
{
    removeAllVariable();
    //Y2hw
    if(m_readCmd != NULL)
        delete m_readCmd;
    //Y2hw
    m_translateMap.clear();
}
/*!
\section TreeItemType
*/
MDTreeItemType CBaseDataBlock::itemType()
{
    return m_itemType;
}

void CBaseDataBlock::setItemType(MDTreeItemType itemType)
{
    m_itemType = itemType;
}

void *CBaseDataBlock::parentItem(MDTreeItemType parentItemType)
{
   return m_device;
}
void CBaseDataBlock::setParentItem(void* parent,MDTreeItemType parentItemType)
{
    m_device = (CBaseDevice*)parent;
}
void CBaseDataBlock::copy(IDataBlock *otherDataBlock)
{
    if(NULL== otherDataBlock)
    {
        return;
    }
    m_name = otherDataBlock->name();
    m_description = otherDataBlock->description();
    m_enabled= ((CBaseDataBlock*)otherDataBlock)->isEnabled();
    m_pollRate = ((CBaseDataBlock*)otherDataBlock)->pollRate ();
    m_writeEnabled = ((CBaseDataBlock*)otherDataBlock)->isWriteEnabled();
 }
/*!
\section 属性
*/
//名称
QString CBaseDataBlock::name()
{
    return m_name;
}

//可能通过属性框设置或对话框设置。
//属性框设置时调用到这个函数
void CBaseDataBlock::setName(QString name)
{
    m_name = name;
}

void CBaseDataBlock::setNameProp(QString name)
{
    if(name.isEmpty())
    {
       //QMessageBox::warning(NULL, "Error", "数据块名称不能为空！", QMessageBox::Ok);
        QMessageBox box;
        box.setText(tr("数据块名称不能为空！"));
        box.setWindowTitle(tr("错误"));;
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();

       return;
    }

    IDataBlock *dataBlockTemp = device()->getDataBlock(name);
    if( dataBlockTemp!=NULL && this!=dataBlockTemp)
    {
       //QMessageBox::warning(NULL, "Error", "数据块名称重复！", QMessageBox::Ok);
        QMessageBox box;
        box.setText(tr("数据块名称重复！"));
        box.setWindowTitle(tr("错误"));
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
       return;
    }

    m_name = name;

    if(device()->getRTDBManagerUI() != NULL)
    {
        CCustomEvent* event = new CCustomEvent(NameChangedEvent,1);
        event->setParam(QVariant((qulonglong)this));
        event->setValue(QVariant(name));
        QCoreApplication::postEvent((QObject*)device()->getRTDBManagerUI(), event);
    }
}

QString CBaseDataBlock::description()
{
    return m_description;
}

void CBaseDataBlock::setDescription(QString description)
{
    m_description = description;
}
//关联Device
CBaseDevice *CBaseDataBlock::device()
{
    return (CBaseDevice *)m_device;
}


//关联Channel
CBaseChannel *CBaseDataBlock::getChannel()
{
    return m_channel;
}

void CBaseDataBlock::setChannel(CBaseChannel *channel)
{
    m_channel = channel;
}

//启用状态
bool CBaseDataBlock::isEnabled()
{
    return m_enabled;
}

void CBaseDataBlock::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

//通信品质
MDVariableDataQuality CBaseDataBlock::quality()
{
    return m_quality;
}

void CBaseDataBlock::setQuality(MDVariableDataQuality quality)
{
    m_quality = quality;
}

//时间戳
QDateTime CBaseDataBlock::timeStamp()
{
    return m_timeStamp;
}

void CBaseDataBlock::setTimeStamp(QDateTime timeStamp)
{
    m_timeStamp = timeStamp;
}

//扫描周期
quint32 CBaseDataBlock::pollRate()
{
    return m_pollRate;
}

void CBaseDataBlock::setPollRate(quint32 pollRate)
{
    m_pollRate = pollRate;
}

int CBaseDataBlock::pollRateProp()
{
    return (int)m_pollRate;
}

void CBaseDataBlock::setPollRateProp(int pollRate)
{
    m_pollRate = (quint32)pollRate;
}

//可写状态
bool CBaseDataBlock::isWriteEnabled()
{
   return m_writeEnabled;
}

void CBaseDataBlock::setWriteEnabled(bool enabled)
{
   m_writeEnabled = enabled;
}
void CBaseDataBlock::resetReadMessageState()
{
    m_readMessageState=false;
}
void CBaseDataBlock::resetStatistics()
{
    m_cmdTransimitCount=0;
    m_cmdReceiveCount=0;
    m_cmdTimeoutCount=0;
    m_cmdRetryCount=0;
    m_cmdErrorCount=0;
}


qint64 CBaseDataBlock::transmitCount()
{
   return m_cmdTransimitCount;
}
void CBaseDataBlock::incrementTransmitCount()
{
    m_cmdTransimitCount++;
}

qint64 CBaseDataBlock::receiveCount()
{
   return m_cmdReceiveCount;
}
void CBaseDataBlock::incrementReceiveCount()
{
    m_cmdReceiveCount++;
}
qint64 CBaseDataBlock::timeoutCount()
{
   return m_cmdTimeoutCount;
}
void CBaseDataBlock::incrementTimeoutCount()
{
    m_cmdTimeoutCount++;
}
qint64 CBaseDataBlock::retryCount()
{
   return m_cmdRetryCount;
}

void CBaseDataBlock::incrementRetryCount()
{
    m_cmdRetryCount++;
}

qint64 CBaseDataBlock::errorCount()
{
   return m_cmdErrorCount;
}
void CBaseDataBlock::incrementErrorCount()
{
    m_cmdErrorCount++;
}

QDateTime CBaseDataBlock::lastReadTime()
{
   return m_lastReadTime;
}
QDateTime CBaseDataBlock::lastWriteTime()
{
   return m_lastWriteTime;
}
QDateTime CBaseDataBlock::lastErrorTime()
{
   return m_lastErrorTime;
}

std::vector<std::string> CBaseDataBlock::nameStringVector()
{
    std::vector<std::string> namevector;
    for(int i=0;i<m_variableList.count();i++)
    {
        if(NULL!=m_variableList[i])
        {
            IMDRTDBVariable *var = m_variableList[i];
            QString ua_address = var->address();
            int index = ua_address.indexOf("Objects.");
            if(index==0)
                ua_address = ua_address.mid(QString("Objects.").length());
            ua_address = ua_address.replace('.','/');
            namevector.push_back(ua_address.toStdString());
            qDebug() << ua_address;
        }
    }
    return namevector;
}

/*!
\section 关联的变量
*/
//添加变量
bool CBaseDataBlock::addVariable(IMDRTDBVariable* nsVar,QString &error)
{
    bool ret = false;
    if(NULL != nsVar)
    {
        if(NULL == getVariable(nsVar->name()) && checkVarAddress(nsVar,error))
        {
            QMutexLocker locker(&m_variableListMutex);
            m_variableList.append(nsVar);            
            ((IMDTreeItem*)nsVar)->setParentItem(this,MDTreeItemType::DataBlock);
            ret =true;
        }
    }
    return ret;
}
//移除变量
bool CBaseDataBlock::removeVariable(IMDRTDBVariable* nsVar)
{
    bool ret = false;
    if(NULL != nsVar)
    {
        QMutexLocker locker(&m_variableListMutex);
        ((IMDTreeItem*)nsVar)->setParentItem(NULL,MDTreeItemType::DataBlock);
        ret = m_variableList.removeOne(nsVar);
    }
    return ret;
}
//删除所有变量
void CBaseDataBlock::removeAllVariable()
{
    QMutexLocker locker(&m_variableListMutex);
    for(int i=0;i<m_variableList.count();i++)
    {
        if(NULL != m_variableList[i])
        {
            ((IMDTreeItem*)m_variableList[i])->setParentItem(NULL,MDTreeItemType::DataBlock);
        }
    }
    m_variableList.clear();
}


//获取变量
IMDRTDBVariable* CBaseDataBlock::getVariableAt(int index)
{
    IMDRTDBVariable *var = NULL;
    QMutexLocker locker(&m_variableListMutex);
    if (index < m_variableList.count() && index>=0)
    {
        var = m_variableList[index];
    }
    return var;
}
//获取变量
IMDRTDBVariable *CBaseDataBlock::getVariable(const QString &varName)
{
    IMDRTDBVariable *var = NULL;
    QMutexLocker locker(&m_variableListMutex);
    for(int i=0;i<m_variableList.count();i++)
    {
        if(NULL==m_variableList[i])
        {
            continue;
        }
        QString varpath = m_variableList[i]->name();
        //qDebug() << varpath << " ?= " << varName;
        if(0==QString::compare(varpath, varName,Qt::CaseInsensitive))
        {
            var = m_variableList[i];
            break;
        }
    }
    return var;
}
//获取变量数量
int CBaseDataBlock::getVariableCount()
{
    int iCount = 0;
    QMutexLocker locker(&m_variableListMutex);
    iCount = m_variableList.count();
    return iCount;
}
/*!
section 数据包截取
*/

bool CBaseDataBlock::msgMonitor()
{
    return m_msgMonitor;
}

void CBaseDataBlock::setMsgMonitor(bool monitor)
{
    m_msgMonitor = monitor;
}

IDataBlock::MDMonitorType CBaseDataBlock::monitorMode()
{
    return m_msgMonitorAscii;
}

void CBaseDataBlock::setMonitorMode(MDMonitorType monitorType)
{
    m_msgMonitorAscii = monitorType;
}

void CBaseDataBlock::setMsgWindow(QDialog* dialog)
{
    m_MsgMonitorWnd = dialog;
}
void CBaseDataBlock::emitPacket(bool bSend, quint8 *dataBuf,  qint16 startIndex, qint16 length)
{
        QString strPacket = ">> ";
        if(bSend == false)
            strPacket = "<< ";
        QString str = "";
        if(m_msgMonitorAscii)//ascii
        {
            for(int i = 0; i<length; i++)
            {
                str.setNum(dataBuf[i+startIndex ],10);
                strPacket.append(str);
            }
        }
        else//raw data
        {
            char* destbuf = new char[length];
            memcpy(destbuf,dataBuf,length);
            strPacket += QString(destbuf) ;
            delete[] destbuf;
        }
        CPacketEvent* packetevent = new CPacketEvent((QEvent::Type)(QEvent::User+2000));
        packetevent->m_DataBlockPacket = strPacket;

        QCoreApplication::postEvent(m_MsgMonitorWnd, packetevent);
}

bool CBaseDataBlock::setupReadCommand(bool bPollNow,bool bForce)
{
    bool bRet=false;

    QMutexLocker locker(&m_objMutex);
    if(false==m_enabled || false==m_running || NULL==m_channel || NULL==m_device || NULL==m_readCmd)
    {
        return bRet;
    }
    //!验证错误时，不扫描
    if(m_nVerifyFlag != FLAG_RIGHT)
    {
        return bRet;
    }
    if(false==bPollNow && m_firstReadDone == false && bForce == false)
    {
        uint timeDiff=m_startTimeToRead.elapsed();
        if(timeDiff<pollRate())
        {
            return bRet;
        }
    }

    if(false==m_readMessageState)
    {
        m_readCmd->stopRetryCommand();
        m_readCmd->setCommandResult(ProtocolCommandResult::None);
        m_readCmd->reSetRetries();
        if(true==bPollNow)
        {
            m_channel->addCommandToQueue(m_readCmd,CommandQueueType::Read,true);
        }
        else
        {
            m_channel->addCommandToQueue(m_readCmd,CommandQueueType::Read,false);
        }
        m_firstReadDone = false;
        m_readMessageState=true;
        m_cmdTransimitCount++;
        //!增加Device统计
        if(m_device != NULL)
            m_device->incrementTransmitCount();

        m_startTimeToRead.restart();
        bRet=true;
    }
    else
    {
        if(false==bPollNow)
        {
            bRet=false;
        }
        else
        {
            m_channel->removeCommandFromQueue(m_readCmd,CommandQueueType::Read);
            m_channel->addCommandToQueue(m_readCmd,CommandQueueType::Read,true);
            bRet=true;
        }
    }
    return bRet;
}
//!处理返回数据包时设置一些统计参数
void CBaseDataBlock::onReceiveCommand(bool isReadCmd, ProtocolCommandResult cmdRes)
{
    QMutexLocker locker(&m_objMutex);
    if(isReadCmd)
    {
        m_lastReadTime=QDateTime::currentDateTime();
        if(m_lastReadTime.isValid()) {
            this->setTimeStamp(m_lastReadTime);

            //!refresh to device
            if(m_device != NULL)
                m_device->setLastReadTime(m_lastReadTime);
        }

        m_readMessageState=false;
    }
    else
    {
        m_lastWriteTime=QDateTime::currentDateTime();
        if(m_lastWriteTime.isValid()) {
            //!refresh to device
            if(m_device != NULL)
                m_device->setLastWriteTime(m_lastWriteTime);
        }
    }

    if(cmdRes == ProtocolCommandResult::Right || cmdRes == ProtocolCommandResult::RightAndWithoutRet)
    {
        m_quality=MDVariableDataQuality::Good;
        m_cmdReceiveCount++;
        //!增加Device统计
        if(m_device != NULL)
            m_device->incrementReceiveCount();
    }
    else
    {
        //m_cmdErrorCount++;//Y2hw
        m_lastErrorTime=QDateTime::currentDateTime();
        if(m_lastWriteTime.isValid()) {
            //!refresh to device
            if(m_device != NULL)
                m_device->setLastErrorTime(m_lastErrorTime);
        }
    }

    switch(cmdRes)
    {
     case ProtocolCommandResult::Error:
     case ProtocolCommandResult::ErrorAndWithoutRet:
        m_quality=MDVariableDataQuality::ProtocolError;
        m_cmdReceiveCount++;
        m_cmdErrorCount++;//Y2hw
        //!增加Device统计
        if(m_device != NULL)
        {
            m_device->incrementErrorCount();
            m_device->incrementReceiveCount();
        }
        break;

    case ProtocolCommandResult::FailedToSend:
        m_quality=MDVariableDataQuality::ConnectDeviceFailed;
        break;

    case ProtocolCommandResult::TimeOut:
        m_quality=MDVariableDataQuality::TimeoutError;
        break;

    case ProtocolCommandResult::None:
        m_quality=MDVariableDataQuality::None;
        break;

    case ProtocolCommandResult::DataBlockBConfigErr:
        m_quality=MDVariableDataQuality::DataBlockConfigError;
        break;
    }
    /*
    if(isReadCmd)
    {
        m_lastReadTime=QDateTime::currentDateTime();
        this->setTimeStamp(m_lastReadTime);
        m_readMessageState=false;
        if(true==isRight)
        {
            m_quality=MDVariableDataQuality::Good;
        }
        else
        {
            m_quality=MDVariableDataQuality::ProtocolError;
            m_cmdErrorCount++;
            m_lastErrorTime=QDateTime::currentDateTime();
        }
    }
    else
    {
        m_lastWriteTime=QDateTime::currentDateTime();
        if(false==isRight)
        {
            m_cmdErrorCount++;
            m_lastErrorTime=QDateTime::currentDateTime();
        }
    }
    m_cmdReceiveCount++;
    */

}

void CBaseDataBlock::onTimeout(bool retry)
{
    m_cmdTimeoutCount++;
    if (true == retry)
    {
        m_cmdRetryCount++;
    }
    if (NULL != m_device)
    {
        m_device->onTimeout(retry);
    }
}
/*!
\section 写变量，产生写命令道队列
*/
bool CBaseDataBlock::asyncWriteVariable(IMDRTDBVariable *nsVar)
{
    if(NULL == nsVar)
    {
        return false;
    }
    if(NULL == getVariable(nsVar->name()))
    {
        return false;
    }
    //!如果配置错误，不能写
    if(m_nVerifyFlag != FLAG_RIGHT)
    {
        delete nsVar;
        nsVar = NULL;
        return false;
    }
    CBaseProtocolCommand *cmd=createCommand(ProtocolCommandType::Write);
    if(NULL!=cmd)
    {
        cmd->addWriteVar(nsVar);
        m_channel->addCommandToQueue(cmd,CommandQueueType::Write,false);
        return true;
    }
    return false;
}

bool CBaseDataBlock::asyncWriteVariables(QList<IMDRTDBVariable *>& nsVarList)
{
    if(nsVarList.size()<=0)
    {
        return false;
    }

    //!如果配置错误，不能写
    if(m_nVerifyFlag != FLAG_RIGHT)
    {
        IMDRTDBVariable * pVar = NULL;
        for(int i = 0; i<nsVarList.count(); i++)
        {
            pVar = nsVarList.at(i);
            delete pVar;
        }
        nsVarList.clear();
        return false;
    }

    CBaseProtocolCommand *cmd=createCommand(ProtocolCommandType::BlockWrite);
    if(NULL!=cmd)
    {
        cmd->addWriteVar(nsVarList);
        m_channel->addCommandToQueue(cmd,CommandQueueType::Write,false);
        return true;
    }
    return false;
}


//!开始运行
bool CBaseDataBlock::start()
{
    bool bRet=false;
    m_running=true;
    m_firstReadDone = true;
    m_startTimeToRead.restart();
    //置位读标志，清除队列（if 有）
    m_readMessageState=false;

    if(NULL!=m_readCmd && NULL!=m_channel)
    {
        bRet=m_channel->removeCommandFromQueue(m_readCmd,CommandQueueType::Read);
        m_readCmd->setReadMsgFlag(false);
    }
    else if(NULL==m_readCmd)
    {
        m_readCmd=createCommand(ProtocolCommandType::Read);
        //!studio反复运行配置时只第一次进来，但channel每次start时都变化
//        if(NULL!=m_readCmd)
//        {
//            m_readCmd->setChannel(m_channel);
//            m_readCmd->setReadMsgFlag(false);
//            bRet=true;
//        }
    }

    if(NULL!=m_readCmd)
    {
        m_readCmd->setChannel(m_channel);
        m_readCmd->setReadMsgFlag(false);
        bRet=true;
    }
    return bRet;
}
//!停止运行
bool CBaseDataBlock::stop()
{
    bool bRet=true;
    m_running=false;
    m_startTimeToRead.restart();
    m_readMessageState=false;

    if(NULL!=m_readCmd && NULL!=m_channel)
    {
        m_channel->removeCommandFromQueue(m_readCmd,CommandQueueType::Read);
    }
    //刷新变量品质
    if(m_quality != MDVariableDataQuality::StopCommunication)
    {
        this->setTimeStamp(QDateTime::currentDateTime());
        m_quality = MDVariableDataQuality::StopCommunication;
        refreshVariablesQuality();
    }
    return bRet;
}
//!通信连接失败时，刷新变量品质、时间戳
void CBaseDataBlock::notifyNotConnectDevice()
{
    if(m_quality != MDVariableDataQuality::ConnectDeviceFailed)
    {
        this->setTimeStamp(QDateTime::currentDateTime());
        m_quality = MDVariableDataQuality::ConnectDeviceFailed;
        refreshVariablesQuality();
    }
}

void CBaseDataBlock::dynamicSetParameter(const QVariantMap &dataBlockMap)
{

}
/*
CDataBlockPacketEvent::CDataBlockPacketEvent(Type MyEventType)
{
        m_EventType = (QEvent::Type)MyEventType;
        m_strPacket = "";
}
QString CDataBlockPacketEvent::getPacket()
{
    return m_strPacket;
}
QEvent::Type CDataBlockPacketEvent::getEventTYpe()
{
    return m_EventType;
}*/

void CBaseDataBlock::setVerifyFlag(int verifyflag)
{
    m_nVerifyFlag = verifyflag;
}

int CBaseDataBlock::verifyFlag()
{
    return m_nVerifyFlag;
}
