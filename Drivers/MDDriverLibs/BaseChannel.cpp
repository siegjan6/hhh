#include "BaseChannel.h"
#include "CommunicationEvent.h"
#include <QSettings>
#include<QCoreApplication>

#include "MDSwitchFactory/MDFuncSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"

CBaseChannel::CBaseChannel()
{
    m_commType=CommunicationType::Serial;

    m_commParameter = "";
    m_communicationComponent=NULL;

    m_cmdTransimitCount=0;
    m_cmdReceiveCount=0;
    m_cmdTimeoutCount=0;
    m_cmdRetryCount=0;
    m_cmdErrorCount=0;

    m_startCommunicationStatus=false;
    m_commandProcessThread=NULL;
    m_parseProcessThread=NULL;

    m_pollSwitch = NULL;
    m_logSwitch = NULL;
}

CBaseChannel::~CBaseChannel()
{
    stopCommunicaton();
  //Y2hw
    QT_TRY
    {
        if(NULL!=m_commandProcessThread)
        {
            m_commandProcessThread->wait();
            m_commandProcessThread->deleteLater();
            m_commandProcessThread = NULL;
        }
    }
    QT_CATCH(...)
    {
        m_commandProcessThread = NULL;
    }

    QT_TRY
    {
        if(NULL!=m_parseProcessThread)
        {
            m_parseProcessThread->wait();
            m_parseProcessThread->deleteLater();
            m_parseProcessThread = NULL;
        }

    }
    QT_CATCH(...)
    {
        m_parseProcessThread = NULL;
    }
    //断开与设备的连接
   // if(NULL!=m_communicationComponent)
   // {
    //    m_communicationComponent->disConnect(10000);
   // }
    removeAllCmdFromList();
//Y2hw
    if(m_logSwitch != NULL && m_logSwitch->isOn())
    {
       m_logSwitch->LOG_INFO("release");
    }
}
/*!
\section 项目相关
*/
void CBaseChannel::setDriverName(const QString & driverName)
{
    m_driverName = driverName;
}

void CBaseChannel::setProjectPath(const QString & projectPath)
{
    m_projectPath = projectPath;
}

/*!
\section 通信类型
*/
CommunicationType CBaseChannel::commType()
{
    return m_commType;
}

void CBaseChannel::setCommType(CommunicationType commType)
{
    m_commType = commType;
}
/*!
\section 通信参数
*/
QString CBaseChannel::commParameter()
{
    return m_commParameter;
}

void CBaseChannel::setCommParameter(const QString & commParameter)
{
    m_commParameter = commParameter;
}
QString CBaseChannel::getCurCommParameter()
{
    if(m_communicationComponent != NULL)
        return m_communicationComponent->getCurCommParam();
    else
        return "";
}
ICommunicationComponent *CBaseChannel::getCommunicationComponent()
{
    return m_communicationComponent;
}

void CBaseChannel::setCommunicationComponent(ICommunicationComponent *communicationComponent)
{
    m_communicationComponent = communicationComponent;
}
/*!
\section 设备操作
*/
bool CBaseChannel::addDevice(CBaseDevice *device)
{
    if(NULL==device)
    {
        return false;
    }
    if(false==findDevice(device->name()))
    {
        QMutexLocker locker(&m_deviceListMutex);
        device->setChannel(this);
        m_deviceList.append(device);

        //!创建逻辑开关  Y2hw
        if(!m_projectPath.isEmpty())
        {
            if(m_pollSwitch == NULL || m_logSwitch == NULL)
            {
                //!根据通信类型组成开关Group名
                QString strCommPara = "",strGroupName = "";
                int nPos = -1;
                switch(m_commType)
                {
                case CommunicationType::Serial:
                    nPos = m_commParameter.indexOf(',');
                    if(nPos > 0)
                        strCommPara = m_commParameter.left(nPos);
                    break;

                case CommunicationType::TCP:
                case CommunicationType::UDP:
                    if(m_commType == CommunicationType::TCP)
                        strCommPara = "TCP,";
                    else
                        strCommPara = "UDP,";
                    nPos = m_commParameter.indexOf(",");
                    if(nPos > 0)
                    {
                        nPos = m_commParameter.indexOf(",",nPos+1);
                        if(nPos > 0)
                        {
                            strCommPara += m_commParameter.left(nPos);
                            strCommPara += ",";
                        }
                        //以太设备可能有相同的IP
                        strCommPara += device->name();
                    }
                    break;
                 default:
                    nPos = m_commParameter.indexOf(',');
                    if(nPos > 0)
                    {
                        strCommPara = m_commParameter.left(nPos);
                    }
                    else
                    {
                        strCommPara = m_commParameter;
                        if(strCommPara.isEmpty())
                            strCommPara = device->name();
                    }
                }
                if(strCommPara.length() > 0)
                {
                    //!逻辑开关，Group名有‘：’
                    if(m_pollSwitch == NULL)
                    {
                        strGroupName = m_driverName + ":";
                        strGroupName += strCommPara;
                        m_pollSwitch = CMDFuncSwitchFactory::createSwitch(m_projectPath, strGroupName, tr("通信通道"));                       
                    }
                    //!逻辑开关，Group名无‘：’
                    if(m_logSwitch == NULL)
                    {
                        strGroupName = m_driverName + ".";
                        strGroupName += strCommPara;
                        QString strComment = QString(tr("%1通道")).arg(strGroupName);
                        m_logSwitch = CMDLogSwitchFactory::createSwitchWithLogger(m_projectPath, strGroupName, strComment);
                    }
                }
            }
        }
        return true;
    }

    return false;
}

bool CBaseChannel::removeDevice(const QString & devName)
{
    bool bRet=false;
    QMutexLocker locker(&m_deviceListMutex);
    for(int iDev=0;iDev<m_deviceList.count();iDev++)
    {
        if(0==QString::compare(m_deviceList[iDev]->name(), devName,Qt::CaseInsensitive))
        {
            m_deviceList[iDev]->setChannel(NULL);
            m_deviceList.removeAt(iDev);
            bRet=true;
            break;
        }
    }
    return bRet;
}

bool CBaseChannel::removeDevice(CBaseDevice *device)
{
    bool bRet = false;
    if(NULL==device)
    {
        return bRet;
    }
    //1.移除一台设备时，原设计：先停止，后移除。可能存在一种情况：设备的命令正在执行时，重试切换通道时，可能又重启了设备，
    //所以这里再次stopDevice一次
    //2.如果该设备的命令正在执行，如果device被删除，将会出错。
    QMutexLocker locker(&m_deviceListMutex);
    bRet=m_deviceList.removeOne(device);
    removeDeviceAllCommandFromQueue(device);//Y2hw
    device->setChannel(NULL);
    return bRet;
}
/*
void CBaseChannel::removeAllDevices()
{
    QMutexLocker locker(&m_deviceListMutex);
    foreach (CBaseDevice *device, m_deviceList)
    {
         if(NULL!=device)
         {
            device->setChannel(NULL);
        }
    }
    m_deviceList.clear();

}
*/
CBaseDevice *CBaseChannel::getDevice(int iDeviceIndex)
{
    CBaseDevice *dev=NULL;
    QMutexLocker locker(&m_deviceListMutex);
    if(iDeviceIndex>=0 && iDeviceIndex<m_deviceList.count())
    {
        dev=m_deviceList[iDeviceIndex];
    }
    return dev;
}

CBaseDevice *CBaseChannel::getDevice(const QString & devName)
{
    CBaseDevice *dev;
    QMutexLocker locker(&m_deviceListMutex);
    for(int iDev=0;iDev<m_deviceList.count();iDev++)
    {
        if(0==QString::compare(devName,m_deviceList[iDev]->name(), Qt::CaseInsensitive))
        {
            dev=m_deviceList[iDev];
            break;
        }
    }
    return dev;
}

bool CBaseChannel::findDevice(const QString & devName)
{
    bool bRet=false;
    QMutexLocker locker(&m_deviceListMutex);
    for(int iDev=0;iDev<m_deviceList.count();iDev++)
    {
        if(0==QString::compare(devName,m_deviceList[iDev]->name(),Qt::CaseInsensitive))
        {
          bRet=true;
          break;
        }
    }
    return bRet;
}
/*!
\section 命令操作
\bPriority：重试写命令优先级 = TRUE，命令添加到头部
*/
bool CBaseChannel::addCommandToQueue(CBaseProtocolCommand *cmd, CommandQueueType cmdQueueType, bool bPriority)
{
    bool bRet=false;
    if(NULL==cmd)
    {
        return bRet;
    }
    switch (cmdQueueType)
    {
     case CommandQueueType::Read:
        {
            if(cmd->getCommandType()==ProtocolCommandType::Read)
            {
                QMutexLocker locker(&m_readCmdListMutex);
                if(false==bPriority)
                {
                    m_readCmdList.append(cmd);
                }
                else
                {
                    m_readCmdList.insert(0,cmd);
                }
                bRet=true;
            }
        }
        break;
     case CommandQueueType::Write:
        {
            if(cmd->getCommandType()==ProtocolCommandType::Write ||
               cmd->getCommandType()==ProtocolCommandType::BlockWrite)
            {
                QMutexLocker locker(&m_writeCmdListMutex);
                if(false==bPriority)
                {
                    m_writeCmdList.append(cmd);
                }
                else
                {
                    m_writeCmdList.insert(0,cmd);
                }
                bRet=true;
            }
        }
        break;
     case CommandQueueType::Parse:
        {
            if(cmd->getCommandType()==ProtocolCommandType::Read)
            {
                QMutexLocker locker(&m_parseCmdListMutex);
                if(false==bPriority)
                {
                    m_parseCmdList.append(cmd);
                }
                else
                {
                    m_parseCmdList.insert(0,cmd);
                }
                bRet=true;
            }
        }
        break;
     default:
        break;
    }
    return bRet;
}
//!处理完一个cmd后，从命令队列移除
CBaseProtocolCommand *CBaseChannel::removeHeadCommandFromQueue(CommandQueueType cmdQueueType)
{
    CBaseProtocolCommand *msg=NULL;
    switch (cmdQueueType)
    {
    case CommandQueueType::Read:
        {
            QMutexLocker locker(&m_readCmdListMutex);
            if(m_readCmdList.count()>0)
            {
                msg=m_readCmdList[0];
                m_readCmdList.removeAt(0);
            }
        }
        break;
    case CommandQueueType::Write:
        {
            QMutexLocker locker(&m_writeCmdListMutex);
            if(m_writeCmdList.count()>0)
            {
                msg=m_writeCmdList[0];
                m_writeCmdList.removeAt(0);
            }
        }
        break;
    case CommandQueueType::Parse:
        {
            QMutexLocker locker(&m_parseCmdListMutex);
            if(m_parseCmdList.count()>0)
            {
                msg=m_parseCmdList[0];
                m_parseCmdList.removeAt(0);
            }
        }
        break;
    default:
        break;
    }
    return msg;
}
//!当写完一个命令立即读数据块时，把已在队列的cmd移除。以便可以添加到队列头部
bool CBaseChannel::removeCommandFromQueue(CBaseProtocolCommand *cmd, CommandQueueType cmdQueueType)
{
    bool bRet=false;
    if(NULL==cmd)
    {
        return bRet;
    }
    switch(cmdQueueType)
    {
    case CommandQueueType::Read:
        if(cmd->getCommandType()==ProtocolCommandType::Read)
        {
            QMutexLocker locker(&m_readCmdListMutex);
            bRet=m_readCmdList.removeOne(cmd);
        }
        break;
    case CommandQueueType::Write:
        if(cmd->getCommandType()==ProtocolCommandType::Write)
        {
            QMutexLocker locker(&m_writeCmdListMutex);
            bRet=m_writeCmdList.removeOne(cmd);
        }
        break;
    case CommandQueueType::Parse:
        if(cmd->getCommandType()==ProtocolCommandType::Read)
        {
            QMutexLocker locker(&m_parseCmdListMutex);
            bRet=m_parseCmdList.removeOne(cmd);
        }
        break;
    default:
        break;
    }
    return bRet;
}
//!停止通信时移除所有队列的内容
bool CBaseChannel::removeAllCommandFromQueue(CommandQueueType cmdQueueType)
{
    bool bRet=false;
    switch(cmdQueueType)
    {
    case CommandQueueType::Read:
        {
            QMutexLocker locker(&m_readCmdListMutex);
            for(int iCmd=0;iCmd<m_readCmdList.count();iCmd++)
            {
                m_readCmdList[iCmd]->getDataBlock()->resetReadMessageState();
            }
            m_readCmdList.clear();
            bRet=true;
        }
        break;
    case CommandQueueType::Write:
        {
            QMutexLocker locker(&m_writeCmdListMutex);
            for(int iCmd=0;iCmd<m_writeCmdList.count();iCmd++)
            {
                 delete m_writeCmdList[iCmd];
            }
            m_writeCmdList.clear();
            bRet=true;
        }
        break;
    case CommandQueueType::Parse:
        {
            QMutexLocker locker(&m_parseCmdListMutex);
            for(int iCmd=0;iCmd<m_parseCmdList.count();iCmd++)
            {
                if(m_parseCmdList[iCmd]->getCommandType() != ProtocolCommandType::Read)
                {
                    delete m_parseCmdList[iCmd];
                }
            }
            m_parseCmdList.clear();
            bRet=true;
        }
        break;
    default:
        break;
    }
    return bRet;
}
void CBaseChannel::removeAllDevices()
{
    removeAllCmdFromList();
    QMutexLocker locker(&m_deviceListMutex);
    for(int i = 0; i<m_deviceList.count(); i++)
    {
        m_deviceList.at(i)->stop();
        m_deviceList.at(i)->setChannel(NULL);
    }
    m_deviceList.clear();
}
void CBaseChannel::removeAllCmdFromList()
{
    removeAllCommandFromQueue(CommandQueueType::Read);
    removeAllCommandFromQueue(CommandQueueType::Write);
    removeAllCommandFromQueue(CommandQueueType::Parse);
}

//!停止某个设备时，移除该设备的所有命令
bool CBaseChannel::removeDeviceAllCommandFromQueue(CBaseDevice *dev)
{
    if(NULL==dev)
    {
        return false;
    }

    m_readCmdListMutex.lock();
    for(int iCmd=0;iCmd<m_readCmdList.count();)
    {
        if(0==QString::compare(dev->name(),m_readCmdList[iCmd]->getDevice()->name(),Qt::CaseInsensitive))
        {
            //清除数据块读标志
            m_readCmdList[iCmd]->getDataBlock()->resetReadMessageState();
            m_readCmdList.removeAt(iCmd);
            break;
        }
        else
        {
            iCmd++;
        }

    }
    m_readCmdListMutex.unlock();


    m_writeCmdListMutex.lock();
    for(int iCmd=0;iCmd<m_writeCmdList.count();)
    {
        if(0==QString::compare(dev->name(),m_writeCmdList[iCmd]->getDevice()->name(),Qt::CaseInsensitive))
        {
            //释放写命令对象
            delete m_writeCmdList[iCmd];
            m_writeCmdList.removeAt(iCmd);
        }
        else
        {
            iCmd++;
        }
    }
    m_writeCmdListMutex.unlock();

    m_parseCmdListMutex.lock();
    for(int iCmd=0;iCmd<m_parseCmdList.count();)
    {
        if(0==QString::compare(dev->name(),m_parseCmdList[iCmd]->getDevice()->name(),Qt::CaseInsensitive))
        {
            if(m_parseCmdList[iCmd]->getCommandType() != ProtocolCommandType::Read)
            {
                delete m_writeCmdList[iCmd];
            }
            m_parseCmdList.removeAt(iCmd);
        }
        else
        {
            iCmd++;
        }
    }
    m_parseCmdListMutex.unlock();

    //活动队列是否有命令
    bool bFound = false;
    int nCmdCnt = 0;
    while(true)
    {
        m_processCmdListMutex.lock();
        nCmdCnt = m_processCmdList.count();
        for(int iCmd=0; iCmd<nCmdCnt; iCmd++)
        {
            if(m_processCmdList[iCmd]->getDevice()->name().compare(dev->name(),Qt::CaseInsensitive) == 0)
            {
                bFound = true;
                break;
            }
        }
        m_processCmdListMutex.unlock();
        if(bFound == false || nCmdCnt == 0)
            break;
        QThread::msleep(50);
    }
    return true;
}

int CBaseChannel::getCommandCountFromQueue(CommandQueueType cmdQueueType)
{
    int iCount=0;
    switch(cmdQueueType)
    {
    case CommandQueueType::Read:
        {
            QMutexLocker locker(&m_readCmdListMutex);
            iCount=m_readCmdList.count();
        }
        break;
    case CommandQueueType::Write:
        {
            QMutexLocker locker(&m_writeCmdListMutex);
            iCount=m_writeCmdList.count();
        }
        break;
    case CommandQueueType::Parse:
        {
            QMutexLocker locker(&m_parseCmdListMutex);
            iCount=m_parseCmdList.count();
        }
        break;
    default:
        break;

    }
    return iCount;
}
//!运行设备，同时触发扫描和解析线程
bool CBaseChannel::startDevice(CBaseDevice *dev)
{
    //!启动设备
    if(NULL==dev)
    {
        return false;
    }
    dev->start();
    if(false==m_startCommunicationStatus)
    {
        startCommunication();
    }
    return true;
}
//!停止设备通信，移除该设备的所有队列
bool CBaseChannel::stopDevice(CBaseDevice *dev)
{
    if(NULL==dev)
    {
        return false;
    }
    dev->stop();
    //removeDeviceAllCommandFromQueue(dev);//Y2hw
    return true;
}

//!通知未连接成功设备变量品质错误
void  CBaseChannel::notifyNotConnectDevice()
{
    QMutexLocker locker(&m_deviceListMutex);
    for(int iDev=0;iDev<m_deviceList.count();iDev++)
    {
        m_deviceList[iDev]->notifyNotConnectDevice();
    }
}
//!切换通道。先停止通信，切换，重启通信
bool CBaseChannel::switchCommunicationChannel()
{
    //记录已经停止的设备,不重新启动
    QList<CBaseDevice*> stoppedDevList;
    m_deviceListMutex.lock();
    for(int iDev=0;iDev<m_deviceList.count();iDev++)
    {
        if(!m_deviceList[iDev]->isRunning())//Y2hw
            stoppedDevList.append(m_deviceList[iDev]);
        m_deviceList[iDev]->stop();
    }
    m_deviceListMutex.unlock();

    m_communicationComponent->switchChannel();
    //日志
    if(m_logSwitch != NULL && m_logSwitch->isOn())
        m_logSwitch->LOG_INFO("switchChannel");

    m_deviceListMutex.lock();
    for(int iDev=0;iDev<m_deviceList.count();iDev++)
    {
        if(stoppedDevList.indexOf(m_deviceList[iDev]) == -1)//Y2hw
            m_deviceList[iDev]->start();
    }
    m_deviceListMutex.unlock();
    stoppedDevList.clear();//Y2hw
    return true;
}
void CBaseChannel::createCommunicationComponent()
{
    releaseCommunicationComponent();
    if (NULL == m_communicationComponent)
    {
        m_communicationComponent = CMDCommunicationComponentFactory::CreateCommunicationComponent();

        m_communicationComponent->setCommStatus(CommunicationStatus::DisConnected);

        m_communicationComponent->setCommType(commType());
        m_communicationComponent->setCommParameter(commParameter());
    }
}
void CBaseChannel::releaseCommunicationComponent()
{
    if (NULL != m_communicationComponent)
    {
         m_communicationComponent->disConnect(1000);
         delete m_communicationComponent;
         m_communicationComponent = NULL;
         //日志
         if(m_logSwitch != NULL && m_logSwitch->isOn())
             m_logSwitch->LOG_INFO("releaseCommunicationComponent");//Y2hw
    }
}
bool CBaseChannel::getStartCommunicationStatus()
{
    return m_startCommunicationStatus;
}
//!启动线程
bool CBaseChannel::startCommunication()
{
  //  if(NULL != m_communicationComponent)
    {
        m_startCommunicationStatus=true;

        resetEventStatus(ThreadEventType::CommandEvent);
        resetEventStatus(ThreadEventType::ParseEvent);
        resetEventStatus(ThreadEventType::ShutDowmEvent);

        m_commandProcessThread=new CCommandProcessThread(this);
        if(NULL!=m_commandProcessThread)
        {
          //connect(m_commandProcessThread,SIGNAL(evt_sendEvent(ThreadEventType)),this,SLOT(sendEvent(ThreadEventType)));
          m_commandProcessThread->start(QThread::HighPriority);
          if(m_logSwitch != NULL && m_logSwitch->isOn())
          {
             m_logSwitch->LOG_INFO("startCommunication");
          }
        }
        m_parseProcessThread=new CParseProcessThread(this);
        if(NULL!=m_parseProcessThread)
        {
           //connect(m_parseProcessThread,SIGNAL(evt_sendEvent(ThreadEventType)),this,SLOT(sendEvent(ThreadEventType)));
           m_parseProcessThread->start(QThread::HighPriority);
        }
        //m_communicationComponent->stopConnect();
        //m_communicationComponent->setCommType(commType());
        //m_communicationComponent->setCommParameter(commParameter());

        sendEvent(ThreadEventType::CommandEvent);
        return true;
    }
    return false;
}
//!关闭线程
bool CBaseChannel::stopCommunicaton()
{
    if(m_startCommunicationStatus == false)
        return true;

    bool bRet=true;

    m_startCommunicationStatus=false;
    sendEvent(ThreadEventType::ShutDowmEvent);

    //******************** Y2hw ******************
    if(m_logSwitch != NULL && m_logSwitch->isOn())
        m_logSwitch->LOG_INFO("send shutdown");

    /*
    QT_TRY
    {
        if(NULL!=m_commandProcessThread)
        {
            m_commandProcessThread->wait();
            m_commandProcessThread->deleteLater();
            m_commandProcessThread = NULL;
        }
    }
    QT_CATCH(...)
    {
        m_commandProcessThread = NULL;
    }

    QT_TRY
    {
        if(NULL!=m_parseProcessThread)
        {
            m_parseProcessThread->wait();
            m_parseProcessThread->deleteLater();
            m_parseProcessThread = NULL;
        }

    }
    QT_CATCH(...)
    {
        m_parseProcessThread = NULL;
    }
    //断开与设备的连接
   // if(NULL!=m_communicationComponent)
   // {
    //    m_communicationComponent->disConnect(10000);
   // }
    removeAllCommandFromQueue(CommandQueueType::Read);
    removeAllCommandFromQueue(CommandQueueType::Write);
    removeAllCommandFromQueue(CommandQueueType::Parse);
*/
    if(m_logSwitch != NULL && m_logSwitch->isOn())
        m_logSwitch->LOG_INFO("leave stopCommunicaton");
    //---------------------------------------------------
    return bRet;
}
//!构建所有设备应该扫描的命令
int CBaseChannel::setupReadCommand(bool bForce)
{
    //!这部分判断移到run中，利于知道一次新轮询的开始
    //int iCount=getCommandCountFromQueue(CommandQueueType::Read);
    //if(iCount<=0)
    //!增加逻辑开关
    if(m_pollSwitch != NULL && m_pollSwitch->isOn())
    {
        QMutexLocker locker(&m_deviceListMutex);
        for(int iDev=0;iDev<m_deviceList.count();iDev++)
        {
            m_deviceList[iDev]->setupReadCommand(bForce);
        }
    }
    return getCommandCountFromQueue(CommandQueueType::Read);
}
/*
void CBaseChannel::startAllDevices()
{
    QMutexLocker locker(&m_deviceListMutex);
    for(int iDev=0;iDev<m_deviceList.count();iDev++)
    {
        m_deviceList[iDev]->start();
    }
    if(m_deviceList.count()>0 && false==m_startCommunicationStatus)
    {
        startCommunication();
    }
}
void CBaseChannel::stopAllDevices()
{
    QMutexLocker locker(&m_deviceListMutex);
    for(int iDev=0;iDev<m_deviceList.count();iDev++)
    {
        m_deviceList[iDev]->stop();
        removeDeviceAllCommandFromQueue(m_deviceList[iDev]);
    }
    m_deviceList.clear();
}*/
int CBaseChannel::getDeviceCount()
{
    int nCount=0;
    QMutexLocker locker(&m_deviceListMutex);
    nCount=m_deviceList.count();
    return nCount;
}

void CBaseChannel::cmdTransimitCountIncrement()
{
    m_cmdTransimitCount++;
}
/*!
\section 命令事件
*/
void CBaseChannel::sendEvent(ThreadEventType eventType)
{
    setEventStatus(eventType);
    switch(eventType)
    {
     case ThreadEventType::CommandEvent:
        m_commandWaitCondition.wakeAll();
        break;
     case ThreadEventType::DataIOEvent:
        m_commandWaitCondition.wakeAll();
        break;
     case ThreadEventType::ParseEvent:
        m_parseWaitCondition.wakeAll();
        break;
    case ThreadEventType::ShutDowmEvent:
        m_parseWaitCondition.wakeAll();
        m_commandWaitCondition.wakeAll();
        break;
    default:
        break;
    }
}
void CBaseChannel::setEventStatus(ThreadEventType eventType)
{
    switch(eventType)
    {
     case ThreadEventType::CommandEvent:
        m_eventStatusMutex.lock();
        m_commandEventStatus = true;
        m_eventStatusMutex.unlock();
        break;
     case ThreadEventType::DataIOEvent:
        m_eventStatusMutex.lock();
        m_IOEventStatus = true;
        m_eventStatusMutex.unlock();
        break;
     case ThreadEventType::ParseEvent:
        m_eventStatusMutex.lock();
        m_parseEventStatus = true;
        m_eventStatusMutex.unlock();
        break;
    case ThreadEventType::ShutDowmEvent:
        m_eventStatusMutex.lock();
        m_shutDowmEventStatus = true;
        m_eventStatusMutex.unlock();
        break;
    default:
        break;
    }
}
void CBaseChannel::resetEventStatus(ThreadEventType eventType)
{
    switch(eventType)
    {
     case ThreadEventType::CommandEvent:
        m_eventStatusMutex.lock();
        m_commandEventStatus =false;
        m_eventStatusMutex.unlock();
        break;
     case ThreadEventType::DataIOEvent:
        m_eventStatusMutex.lock();
        m_IOEventStatus =false;
        m_eventStatusMutex.unlock();
        break;
     case ThreadEventType::ParseEvent:
        m_eventStatusMutex.lock();
        m_parseEventStatus =false;
        m_eventStatusMutex.unlock();
        break;
    case ThreadEventType::ShutDowmEvent:
        m_eventStatusMutex.lock();
        m_shutDowmEventStatus =false;
        m_eventStatusMutex.unlock();
        break;
    default:
        break;
    }
}
bool CBaseChannel::getEventStatus(ThreadEventType eventType)
{
    bool bStatus = false;
    switch(eventType)
    {
     case ThreadEventType::CommandEvent:
        m_eventStatusMutex.lock();
        bStatus =  m_commandEventStatus;
        m_eventStatusMutex.unlock();
        break;
     case ThreadEventType::DataIOEvent:
        m_eventStatusMutex.lock();
        bStatus =  m_IOEventStatus;
        m_eventStatusMutex.unlock();
        break;
     case ThreadEventType::ParseEvent:
        m_eventStatusMutex.lock();
        bStatus =  m_parseEventStatus;
        m_eventStatusMutex.unlock();
        break;
    case ThreadEventType::ShutDowmEvent:
        m_eventStatusMutex.lock();
        bStatus =  m_shutDowmEventStatus;
        m_eventStatusMutex.unlock();
        break;
    default:
        break;
    }
    return bStatus;
}
bool CBaseChannel::waitThreadEvent(ThreadType threadType, quint64 timeOut)
{
    bool bRet = false;
    switch(threadType)
    {
     case ThreadType::CommandThread:
        m_commandWaitConditionMutex.lock();
        bRet =  m_commandWaitCondition.wait(&m_commandWaitConditionMutex,timeOut);
        m_commandWaitConditionMutex.unlock();
        break;
     case ThreadType::ParseThread:
        m_parseWaitConditionMutex.lock();
        bRet =  m_parseWaitCondition.wait(&m_parseWaitConditionMutex,timeOut);
        m_parseWaitConditionMutex.unlock();
        break;
    default:
        break;
    }
    return bRet;
}

void CBaseChannel::cancelCommand(CBaseProtocolCommand *&cmd)
{
    if(NULL==cmd)
    {
        return;
    }
    addCommandToQueue(cmd,CommandQueueType::Parse,false);
    sendEvent(ThreadEventType::ParseEvent);
    cmd=NULL;
}
//!解析命令，调用协议cmd解析，返回结果
ProtocolCommandResult CBaseChannel::parseCommand(CBaseProtocolCommand* &cmd)
{
    if(NULL==cmd)
    {
        return ProtocolCommandResult::Error;
    }
    ProtocolCommandResult ret=cmd->parseCommand();
    switch(ret)
    {
     case ProtocolCommandResult::Right:
         {
            m_cmdReceiveCount++;
            //如果写命令，构建读命令道队列头部
            if (cmd->getCommandType() == ProtocolCommandType::Write ||
                cmd->getCommandType() == ProtocolCommandType::BlockWrite)
            {
                CBaseDataBlock* db = cmd->getDataBlock();
                addCommandToQueue(cmd, CommandQueueType::Parse, false);
                sendEvent(ThreadEventType::ParseEvent);
                cmd=NULL;
                if (NULL != db)
                {
                   if (db->isEnabled())
                   {
                       //写命令完成后，执行读命令
                       if(db->setupReadCommand(true))
                       {
                           cmd = removeHeadCommandFromQueue(CommandQueueType::Read);
                       }
                   }
                }
            }
            else
            {
                addCommandToQueue(cmd,CommandQueueType::Parse,false);
                sendEvent(ThreadEventType::ParseEvent);
                cmd=NULL;
            }
        }
        break;
    case ProtocolCommandResult::RightAndNextCmd:
        {
            cmd->startReplyTimeout();
            cmd->reSetRetries();
            sendEvent(ThreadEventType::CommandEvent);
        }
        break;
    case ProtocolCommandResult::RightAndWithoutRet:
    case ProtocolCommandResult::ErrorAndWithoutRet:
        {
            m_cmdReceiveCount++;
            sendEvent(ThreadEventType::CommandEvent);
        }
        break;
    case ProtocolCommandResult::SplitData:
        {
            cmd->startReplyTimeout();
        }
        break;
    case ProtocolCommandResult::Error:
       {
            m_cmdReceiveCount++;
            m_cmdErrorCount++;
            addCommandToQueue(cmd,CommandQueueType::Parse,false);
            sendEvent(ThreadEventType::ParseEvent);
            cmd=NULL;
        }
        break;
    default:
        break;
    }
    return ret;
}
//!重试命令
void CBaseChannel::retryCommand(CBaseProtocolCommand *&cmd)
{
    if(NULL==cmd)
    {
        return;
    }
    //*************************** Y2hw *****************************
    /*
    //如果还有重试次数
    if(cmd->isTimedOut() || true==bRetry)
    {
        if(cmd->getRetries()>0)
        {
            m_cmdTimeoutCount++;
            m_cmdRetryCount++;
            if(NULL!=cmd->getDataBlock())
            {
                cmd->getDataBlock()->onTimeout(bRetry);
            }
            cmd->setCommunicationQuality(false);
            cmd->startReplyTimeout();
            cmd->reduceRetries();
            cmd->startRetryCommand();
            //读命令放到等待其他命令执行完成后再重试，写命令立即重试
            if(cmd->getCommandType() == ProtocolCommandType::Read)
            {
                addCommandToQueue(cmd,CommandQueueType::Read);
                cmd = NULL;
            }
            else
            {
                addCommandToQueue(cmd,CommandQueueType::Write,true);
            }
            sendEvent(ThreadEventType::CommandEvent);
        }
        else//无重试次数，切换通道
        {
            m_cmdTimeoutCount++;
            if(NULL!=cmd->getDataBlock())
            {
                cmd->getDataBlock()->onTimeout(bRetry);
            }
            //设置品质到变量
            cmd->setCommandResult(ProtocolCommandResult::TimeOut);
            addCommandToQueue(cmd,CommandQueueType::Parse,false);

            sendEvent(ThreadEventType::ParseEvent);

            if(false==cmd->getCommunicationQuality())
            {
                switchCommunicationChannel();
            }
            cmd=NULL;
        }
    }*/
    m_cmdTimeoutCount++;
    //写命令超时：如链路断开，添加回队列下次再试；or 进入parse
    if(cmd->getCommandType() != ProtocolCommandType::Read &&
            !cmd->isRetryCommand())
    {
        cmd->startRetryCommand();
        addCommandToQueue(cmd,CommandQueueType::Write,true);
    }
    else
    {
        if(NULL!=cmd->getDataBlock())
        {
            cmd->getDataBlock()->onTimeout(true);
        }
        //设置品质到变量
        cmd->setCommandResult(ProtocolCommandResult::TimeOut);
        addCommandToQueue(cmd,CommandQueueType::Parse,false);

        sendEvent(ThreadEventType::ParseEvent);
    }
    cmd=NULL;
    //--------------------------------------------------------------
}
//--------------------------------------CCommandProcessThread--------------------------------------------------
/*!
\section 线程处理
*/
CCommandProcessThread::CCommandProcessThread(CBaseChannel *baseChannel)
{
    m_baseChannel=baseChannel;
}
CCommandProcessThread::~CCommandProcessThread()
{
}
void CCommandProcessThread::run()
{
    if(m_baseChannel->m_logSwitch != NULL && m_baseChannel->m_logSwitch->isOn())
    {
        m_baseChannel->m_logSwitch->LOG_INFO("Enter CommandProcessThread");
    }

    m_baseChannel->createCommunicationComponent();
    CBaseProtocolCommand *currentCmd=NULL;
    qint32 waitEventTimeout = 50;
    bool bContinue=true;
    qint32 getDataTimeout =0;
    int nConnectDeviceCount =0;
    //读数据局部变量
    int nReadDataIndex = 0;
    int nReadCnt = 0;
    int iRecSize = 0;

    //一次读轮询
    int nReadCmdPollCnt = 0;
    int nTimeOutCmdCnt = 0;

    //超时+重试标志
    bool bTimeOut = false;
    bool bCommunicationOk = true;
    IMDRTDBVariable* pVariable = NULL;

    while(true==m_baseChannel->getStartCommunicationStatus() && bContinue)
    {
        //等待命令事件
        if (!m_baseChannel->getEventStatus(ThreadEventType::CommandEvent))
        {
            m_baseChannel->waitThreadEvent(ThreadType::CommandThread,waitEventTimeout);
        }
        m_baseChannel->resetEventStatus(ThreadEventType::CommandEvent);

        //退出线程
        if (m_baseChannel->getEventStatus(ThreadEventType::ShutDowmEvent))
        {
            bContinue = false;
            if(m_baseChannel->m_logSwitch != NULL && m_baseChannel->m_logSwitch->isOn())
                m_baseChannel->m_logSwitch->LOG_INFO("Get shutdown event");
            break;
        }
        //未连接上，通知该设备所有数据块变量的品质错误
        if(nConnectDeviceCount==1)
        {
            m_baseChannel->notifyNotConnectDevice();
        }
        nConnectDeviceCount++;

        if(m_baseChannel->m_pollSwitch != NULL && !m_baseChannel->m_pollSwitch->isOn())
        {
            QThread::msleep(10);
            continue;
        }
        //*************************************** 建立通信连接 **************************************
        if (m_baseChannel->getCommunicationComponent()->getCommStatus() != CommunicationStatus::Connected)
        {
            //通信连接
            if(m_baseChannel->m_logSwitch != NULL && m_baseChannel->m_logSwitch->isOn())
            {
                m_baseChannel->m_logSwitch->LOG_INFO("connect");
            }
            if(!m_baseChannel->getCommunicationComponent()->connect(5000))//等待5秒  Y2hw
            {
                continue;
            }
            if (m_baseChannel->getCommunicationComponent()->getCommStatus() != CommunicationStatus::Connected)
            {
                continue;
            }
            else
            {

            }
            bCommunicationOk = false;
            bTimeOut = false;
        }
        nConnectDeviceCount = 0;
        //---------------------------------------------------------------------------------------

        //*************************************** 构建读写命令 *************************************
        //执行命令
        //有写命令？
        //通道不正常时或第一条命令时，稍缓发送写命令
        if (NULL == currentCmd)
        {
            if(bCommunicationOk)
                currentCmd = m_baseChannel->removeHeadCommandFromQueue(CommandQueueType::Write);
        }
        //有读命令？
        if (NULL == currentCmd)
        {
            //刷新停止通信品质和parsecommand的品质可能会交叉，导致有些数据块品质不对
            while(m_baseChannel->getCommandCountFromQueue(CommandQueueType::Parse) != 0 &&
                  m_baseChannel->m_startCommunicationStatus)
            {
                QThread::msleep(100);
            }
            if(m_baseChannel->getCommandCountFromQueue(CommandQueueType::Read) == 0)
            {
                nReadCmdPollCnt = m_baseChannel->setupReadCommand(bTimeOut);
                bTimeOut = false;
                nTimeOutCmdCnt = 0;
                if(nReadCmdPollCnt == 0)//所有数据块只写
                    bCommunicationOk = true;
            }
            currentCmd = m_baseChannel->removeHeadCommandFromQueue(CommandQueueType::Read);
        }
        if (NULL == currentCmd)
        {
            QThread::msleep(10);
            continue;            
        }
        else
        {
            if(!currentCmd->getDevice()->isRunning())
            {
                currentCmd = NULL;
                QThread::msleep(10);
                continue;
            }

            //非重试命令时，重新构建命令
            if(!currentCmd->isRetryCommand())
            {
                currentCmd->setupCommand();
            }
            if(m_baseChannel->m_logSwitch != NULL && m_baseChannel->m_logSwitch->isOn())
            {
                if(currentCmd->getCommandType() == ProtocolCommandType::Write)
                {
                    pVariable = currentCmd->getWriteVarAt(0);
                    if(pVariable != NULL)
                    {
                        QString info = QString("before write %1").arg(pVariable->name());
                        m_baseChannel->m_logSwitch->LOG_INFO(info);
                    }
                }
                else if(currentCmd->getCommandType() == ProtocolCommandType::BlockWrite)
                {
                    m_baseChannel->m_logSwitch->LOG_INFO("before send a multi write command.");
                }
            }
        }
        //---------------------------------------------------------------------------------------

        //*************************************** 收发命令，处理超时 *************************************
        //!构造的命令长度不对，以返回命令处理
        if(currentCmd->getCommandLength()<=0)
        {
            //!channel的CommType可能与getCommunicationComponent的CommType不一样
            if(m_baseChannel->getCommunicationComponent()->getCommType() == CommunicationType::Board
                    || m_baseChannel->getCommunicationComponent()->getCommType() == CommunicationType::Other)
            {
                if(currentCmd->getCommandType() == ProtocolCommandType::Read)
                    m_baseChannel->removeCommandFromQueue(currentCmd,CommandQueueType::Read);
                else
                    m_baseChannel->removeCommandFromQueue(currentCmd,CommandQueueType::Write);
            }
            else
            {
                //添加到活动队列
                m_baseChannel->m_processCmdListMutex.lock();
                if(m_baseChannel->m_processCmdList.indexOf(currentCmd) == -1)
                    m_baseChannel->m_processCmdList.append(currentCmd);
                m_baseChannel->m_processCmdListMutex.unlock();

                currentCmd->setReceiveDataLength(0);
                m_baseChannel->addCommandToQueue(currentCmd,CommandQueueType::Parse);
                m_baseChannel->sendEvent(ThreadEventType::ParseEvent);
            }
            currentCmd = NULL;
        }
        else
        {
            //添加到活动队列
            m_baseChannel->m_processCmdListMutex.lock();
            if(m_baseChannel->m_processCmdList.indexOf(currentCmd) == -1)
                m_baseChannel->m_processCmdList.append(currentCmd);
            m_baseChannel->m_processCmdListMutex.unlock();

            //channel发送次数增加一次
            m_baseChannel->cmdTransimitCountIncrement();
            //发送
            if(!m_baseChannel->getCommunicationComponent()->writeData(currentCmd->getSendData(),currentCmd->getCommandLength(),5000))//5000 Y2hw
            {
                //发送失败，返回命令处理，同时切换通道
                currentCmd->setCommandResult(ProtocolCommandResult::FailedToSend);
                m_baseChannel->addCommandToQueue(currentCmd,CommandQueueType::Parse,false);
                m_baseChannel->sendEvent(ThreadEventType::ParseEvent);                
                m_baseChannel->switchCommunicationChannel();
                continue;
            }
            //!某些驱动（FX3)收到正确返回后，需再发送一条到PLC，但无返回.发完后，销毁
            if(currentCmd->getCommandResult() == ProtocolCommandResult::RightAndWithoutRet ||
                currentCmd->getCommandResult() == ProtocolCommandResult::ErrorAndWithoutRet)
            {
                //加入到parse队列
                m_baseChannel->addCommandToQueue(currentCmd,CommandQueueType::Parse,false);
                m_baseChannel->sendEvent(ThreadEventType::ParseEvent);
                m_baseChannel->sendEvent(ThreadEventType::CommandEvent);

                currentCmd = NULL;
                continue;
            }
            //是否数据块监控
            if(currentCmd->getDataBlock()->msgMonitor())
                 currentCmd->getDataBlock()->emitPacket(true,currentCmd->getSendData(),0,currentCmd->getCommandLength());
            currentCmd->startRetryCommand();
            getDataTimeout = currentCmd->getReplyTimeout();
            currentCmd->startReplyTimeout();
            nReadDataIndex = 0;
            iRecSize = 0;

            //循环收数据
            while(currentCmd != NULL && m_baseChannel->getStartCommunicationStatus())
            {
                //退出线程
                if (m_baseChannel->getEventStatus(ThreadEventType::ShutDowmEvent))
                {
                    bContinue = false;
                    if(m_baseChannel->m_logSwitch != NULL && m_baseChannel->m_logSwitch->isOn())
                        m_baseChannel->m_logSwitch->LOG_INFO("Get shutdown event");
                    break;
                }

                nReadDataIndex = 0;

                if(m_baseChannel->getCommunicationComponent()->getCommType() == CommunicationType::Serial)
                {
                    nReadCnt = getDataTimeout/10;
                    while(nReadDataIndex <= nReadCnt && m_baseChannel->getStartCommunicationStatus())
                    {
                        iRecSize = m_baseChannel->getCommunicationComponent()->readData(currentCmd->getReceiveData(),currentCmd->getReceiveDataLength(), currentCmd->getReceiveBufferSize(),0);
                        if(iRecSize != 0 || !currentCmd->getDevice()->isRunning())//Y2hw !currentCmd->getDevice()->isRunning()
                            break;
                        nReadDataIndex++;
                        QThread::msleep(10);
                    }
                }
                else if(m_baseChannel->getCommunicationComponent()->getCommType() == CommunicationType::OPCUA)
                {
                    //const std::vector<std::string> &entityBrowseNames = currentCmd->getDataBlock()->nameStringVector();
                    //m_baseChannel->getCommunicationComponent()->subscribes(entityBrowseNames);
                    iRecSize = m_baseChannel->getCommunicationComponent()->readData(currentCmd->getReceiveData(),currentCmd->getReceiveDataLength(), currentCmd->getReceiveBufferSize(),getDataTimeout);
                }
                else
                {
                    iRecSize = m_baseChannel->getCommunicationComponent()->readData(currentCmd->getReceiveData(),currentCmd->getReceiveDataLength(), currentCmd->getReceiveBufferSize(),getDataTimeout);
                }

                //设备已经停止
                // ||
                //m_baseChannel->getCommunicationComponent()->getCommStatus() != CommunicationStatus::Connected)
                if(!currentCmd->getDevice()->isRunning())
                {
                    currentCmd->setCommandResult(ProtocolCommandResult::None);
                    m_baseChannel->addCommandToQueue(currentCmd,CommandQueueType::Parse,false);

                    m_baseChannel->sendEvent(ThreadEventType::ParseEvent);
                    break;
                }
                if(iRecSize != 0 && iRecSize != -2)//-2:可能链路已断开，如：USB转串口拔掉后，返回数据个数很大
                {
                    bCommunicationOk = true;
                    bTimeOut = false;
                    nTimeOutCmdCnt = 0;
                }
                if (iRecSize > 0)//正常接收数据
                {
                    //是否数据块监控
                    if(currentCmd->getDataBlock()->msgMonitor())
                            currentCmd->getDataBlock()->emitPacket(false,currentCmd->getReceiveData(),currentCmd->getReceiveDataLength(),iRecSize);

                    currentCmd->setReceiveDataLength((quint16)(currentCmd->getReceiveDataLength()) + iRecSize);
                    //解析命令
                    ProtocolCommandResult commandResult = m_baseChannel->parseCommand(currentCmd);

                    //分包命令
                    if(commandResult == ProtocolCommandResult::RightAndNextCmd && NULL != currentCmd)
                    {
                        currentCmd->startReplyTimeout();
                        currentCmd->reSetRetries();
                        currentCmd->stopRetryCommand();
                        break;
                    }//数据没收完
                    else if(commandResult == ProtocolCommandResult::SplitData && NULL != currentCmd)
                    {
                        currentCmd->startReplyTimeout();
                        getDataTimeout = currentCmd->getReplyTimeout();
                        continue;
                    }
                    //!某些驱动（FX3)收到正确返回后，需再发送一条到PLC，但无返回
                    else if(commandResult == ProtocolCommandResult::RightAndWithoutRet || commandResult == ProtocolCommandResult::ErrorAndWithoutRet && NULL != currentCmd)
                    {
                        currentCmd->startReplyTimeout();
                        currentCmd->reSetRetries();
                        currentCmd->startRetryCommand();
                        break;
                    }
                    else
                    {
                        break;
                    }                    
                }
                else if (iRecSize < 0)//数据粘包严重
                {
                    if(iRecSize == -1)
                    {
                        currentCmd->setReceiveDataLength(0);
                        getDataTimeout =100;
                        continue;
                    }
                    else if(iRecSize == -2)
                    {
                        currentCmd->setCommandResult(ProtocolCommandResult::TimeOut);
                        m_baseChannel->addCommandToQueue(currentCmd,CommandQueueType::Parse,false);

                        m_baseChannel->sendEvent(ThreadEventType::ParseEvent);
                        break;
                    }
                }
                else //if (iRecSize = 0)//接收数据失败，重试命令
                {
                    currentCmd->setReceiveDataLength(0);
                    if(currentCmd->getCommandType() == ProtocolCommandType::Read)
                    {
                        nTimeOutCmdCnt++;
                    }
                    m_baseChannel->retryCommand(currentCmd);//重试命令
                    bTimeOut = true;
                    bCommunicationOk = false;
                    if(nTimeOutCmdCnt >= nReadCmdPollCnt)//超过重试次数就认为设备无法通信
                    {
                        m_baseChannel->switchCommunicationChannel();
                    }
                    break;
                }
            }
        }//end while read
        //---------------------------------------------------------------------------------------
        m_baseChannel->sendEvent(ThreadEventType::CommandEvent);
    }//end  thread while
    if(currentCmd!=NULL)
    {
       //已取出的为写命令时，删除对象
       if (currentCmd->getCommandType() != ProtocolCommandType::Read)
       {
           delete currentCmd;
           currentCmd = NULL;
       }
    }
    m_baseChannel->releaseCommunicationComponent();

    if(m_baseChannel->m_logSwitch != NULL && m_baseChannel->m_logSwitch->isOn())
    {
        m_baseChannel->m_logSwitch->LOG_INFO("Leave CommandProcessThread");
    }

    /*
    if(m_baseChannel->m_logSwitch != NULL && m_baseChannel->m_logSwitch->isOn())
    {
        m_baseChannel->m_logSwitch->LOG_INFO("Enter CommandProcessThread");
    }

    m_baseChannel->createCommunicationComponent();
    CBaseProtocolCommand *currentCmd=NULL;
    qint32 waitEventTimeout = 50;
    bool bContinute=true;
    qint32 getDataTimeout =0;
    int nConnectDeviceCount =0;
    int nNoReceiveDataCount =0;
    int nReadDataIndex = 0;
    int nReadCnt = 0;
    int iRecSize = 0;
    IMDRTDBVariable* pVariable = NULL;
    while(true==m_baseChannel->getStartCommunicationStatus() && bContinute)
    {
        //等待命令事件
        if (!m_baseChannel->getEventStatus(ThreadEventType::CommandEvent))
        {
            m_baseChannel->waitThreadEvent(ThreadType::CommandThread,waitEventTimeout);
        }
        m_baseChannel->resetEventStatus(ThreadEventType::CommandEvent);

        //退出线程
        if (m_baseChannel->getEventStatus(ThreadEventType::ShutDowmEvent))
        {
            bContinute = false;
            if(m_baseChannel->m_logSwitch != NULL && m_baseChannel->m_logSwitch->isOn())
                m_baseChannel->m_logSwitch->LOG_INFO("Get shutdown event");//Y2hw
            break;
        }
        //未连接上，通知该设备所有数据块变量的品质错误
        if(nConnectDeviceCount==1)
        {
            m_baseChannel->notifyNotConnectDevice();
        }
        nConnectDeviceCount++;

        if(m_baseChannel->m_pollSwitch != NULL && !m_baseChannel->m_pollSwitch->isOn())
        {
            QThread::msleep(10);
            continue;
        }
        //建立通信连接
        if (m_baseChannel->getCommunicationComponent()->getCommStatus() != CommunicationStatus::Connected)
        {
            //通信连接
            if(m_baseChannel->m_logSwitch != NULL && m_baseChannel->m_logSwitch->isOn())
            {
                m_baseChannel->m_logSwitch->LOG_INFO("connect");
            }
            if(!m_baseChannel->getCommunicationComponent()->connect(3000))//等待5秒  Y2hw
            {
                continue;
            }
            if (m_baseChannel->getCommunicationComponent()->getCommStatus() != CommunicationStatus::Connected)
            {
                continue;
            }
        }
        nConnectDeviceCount = 0;
        //执行命令
        //有写命令？
        if (NULL == currentCmd)
        {
            currentCmd = m_baseChannel->removeHeadCommandFromQueue(CommandQueueType::Write);
        }
        //有读命令？
        if (NULL == currentCmd)
        {
            m_baseChannel->setupReadCommand();
            currentCmd = m_baseChannel->removeHeadCommandFromQueue(CommandQueueType::Read);
        }
        if (NULL == currentCmd)
        {
            QThread::msleep(10);
            continue;
        }
        else
        {
            //非重试命令时，重新构建命令
            if(!currentCmd->isRetryCommand())
            {
                currentCmd->setupCommand();
            }
            if(m_baseChannel->m_logSwitch != NULL && m_baseChannel->m_logSwitch->isOn())
            {
                if(currentCmd->getCommandType() != ProtocolCommandType::Write)
                {
                    pVariable = currentCmd->getWriteVarAt(0);
                    if(pVariable != NULL)
                    {
                        QString info = QString("before write %1").arg(pVariable->name());
;                       m_baseChannel->m_logSwitch->LOG_INFO(info);
                    }
                }
                else if(currentCmd->getCommandType() != ProtocolCommandType::BlockWrite)
                {
                    m_baseChannel->m_logSwitch->LOG_INFO("before send a multi write command.");
                }
            }
        }
        //!构造的命令长度不对，以返回命令处理
        if(currentCmd->getCommandLength()<=0)
        {
            currentCmd->setReceiveDataLength(0);
            currentCmd->setCommunicationQuality(false);
            m_baseChannel->addCommandToQueue(currentCmd,CommandQueueType::Parse);
            m_baseChannel->sendEvent(ThreadEventType::ParseEvent);
            currentCmd = NULL;
        }
        else
        {
            //channel发送次数增加一次
            m_baseChannel->cmdTransimitCountIncrement();
            //发送
            if(!m_baseChannel->getCommunicationComponent()->writeData(currentCmd->getSendData(),currentCmd->getCommandLength(),3000))//5000 Y2hw
            {
                //发送失败，返回命令处理，同时切换通道
                currentCmd->setCommandResult(ProtocolCommandResult::FailedToSend);
                m_baseChannel->addCommandToQueue(currentCmd,CommandQueueType::Parse,false);
                m_baseChannel->sendEvent(ThreadEventType::ParseEvent);
                m_baseChannel->switchCommunicationChannel();
                continue;
            }
            //!某些驱动（FX3)收到正确返回后，需再发送一条到PLC，但无返回.发完后，销毁
            if(currentCmd->getCommandResult() == ProtocolCommandResult::RightAndWithoutRet ||
                currentCmd->getCommandResult() == ProtocolCommandResult::ErrorAndWithoutRet)
            {
                //加入到parse队列
                m_baseChannel->addCommandToQueue(currentCmd,CommandQueueType::Parse,false);
                m_baseChannel->sendEvent(ThreadEventType::ParseEvent);
                m_baseChannel->sendEvent(ThreadEventType::CommandEvent);

                currentCmd = NULL;
                continue;
            }
            //是否数据块监控
            if(currentCmd->getDataBlock()->getMsgMonitor())
                 currentCmd->getDataBlock()->emitPacket(true,currentCmd->getSendData(),0,currentCmd->getCommandLength());
            currentCmd->startRetryCommand();
            getDataTimeout = currentCmd->getReplyTimeout();
            currentCmd->startReplyTimeout();
            nReadDataIndex = 0;
            iRecSize = 0;
            //循环收数据
            while(NULL != currentCmd && !currentCmd->isTimedOut())
            {
                //******************************** Y2hw ****************************
                //int iRecSize = m_baseChannel->getCommunicationComponent()->readData(currentCmd->getReceiveData(),currentCmd->getReceiveDataLength(), currentCmd->getReceiveBufferSize(),getDataTimeout);
                nReadDataIndex = 0;
                //循环收数据
                if(m_baseChannel->getCommunicationComponent()->getCommType() == CommunicationType::Serial)
                {
                    nReadCnt = getDataTimeout/10;
                    while(nReadDataIndex <= nReadCnt && m_baseChannel->getStartCommunicationStatus())
                    {
                        iRecSize = m_baseChannel->getCommunicationComponent()->readData(currentCmd->getReceiveData(),currentCmd->getReceiveDataLength(), currentCmd->getReceiveBufferSize(),0);
                        if(iRecSize != 0)
                            break;
                        nReadDataIndex++;
                        QThread::msleep(10);
                    }
                }
                else
                {
                    iRecSize = m_baseChannel->getCommunicationComponent()->readData(currentCmd->getReceiveData(),currentCmd->getReceiveDataLength(), currentCmd->getReceiveBufferSize(),getDataTimeout);
                }
                if(iRecSize != 0 && currentCmd->getCommandType() == ProtocolCommandType::Read)
                    m_baseChannel->m_timeoutReadCmdCnt++;
                //-----------------------------------------------------------------
                if (iRecSize > 0)//正常接收数据
                {
                    //是否数据块监控
                    if(currentCmd->getDataBlock()->getMsgMonitor())
                            currentCmd->getDataBlock()->emitPacket(false,currentCmd->getReceiveData(),currentCmd->getReceiveDataLength(),iRecSize);

                    nNoReceiveDataCount=0;
                    currentCmd->setCommunicationQuality(true);

                    currentCmd->setReceiveDataLength((quint16)(currentCmd->getReceiveDataLength()) + iRecSize);
                    //解析命令
                    ProtocolCommandResult commandResult = m_baseChannel->parseCommand(currentCmd);

                    //分包命令
                    if(commandResult == ProtocolCommandResult::RightAndNextCmd && NULL != currentCmd)
                    {
                        currentCmd->startReplyTimeout();
                        currentCmd->reSetRetries();
                        currentCmd->stopRetryCommand();
                        break;
                    }//数据没收完
                    else if(commandResult == ProtocolCommandResult::SplitData && NULL != currentCmd)
                    {
                        currentCmd->startReplyTimeout();
                        getDataTimeout = currentCmd->getReplyTimeout();
                        continue;
                    }
                    //!某些驱动（FX3)收到正确返回后，需再发送一条到PLC，但无返回
                    else if(commandResult == ProtocolCommandResult::RightAndWithoutRet || commandResult == ProtocolCommandResult::ErrorAndWithoutRet && NULL != currentCmd)
                    {
                        currentCmd->startReplyTimeout();
                        currentCmd->reSetRetries();
                        currentCmd->startRetryCommand();
                        break;
                    }
                    else
                    {
                        break;
                    }
                }
                else if (iRecSize < 0)//数据粘包严重
                {
                    currentCmd->setCommunicationQuality(true);
                    currentCmd->setReceiveDataLength(0);
                    getDataTimeout =100;
                    nNoReceiveDataCount=0;
                }
                else //if (iRecSize = 0)//接收数据失败，重试命令
                {
                    int retries = 3;
                    nNoReceiveDataCount++;
                    currentCmd->setReceiveDataLength(0);
                    CBaseDevice *device = currentCmd->getDevice();
                    if(device)
                    {
                       retries = device->retries();
                       if(retries<=0)
                       {
                           retries =1;
                       }
                    }
                    if(currentCmd->getRetries()<=0)//重试命令会调用switchCommunicationChannel
                    {
                        nNoReceiveDataCount =0;
                    }
                    m_baseChannel->retryCommand(currentCmd);//重试命令

                    if(nNoReceiveDataCount>=retries)//超过重试次数就认为设备无法通信
                    {
                       m_baseChannel->switchCommunicationChannel();
                       nNoReceiveDataCount =0;
                    }

                    break;
                }
            }//end while 收数据
        }
        m_baseChannel->sendEvent(ThreadEventType::CommandEvent);
    }//end  thread while
    if(currentCmd!=NULL)
    {
       //已取出的为写命令时，删除对象
       if (currentCmd->getCommandType() != ProtocolCommandType::Read)
       {
           delete currentCmd;
           currentCmd = NULL;
       }
    }
    m_baseChannel->releaseCommunicationComponent();
    if(m_baseChannel->m_logSwitch != NULL && m_baseChannel->m_logSwitch->isOn())
    {
        m_baseChannel->m_logSwitch->LOG_INFO("Leave CommandProcessThread");
    }*/
}

//--------------------------------------CParseProcessThread--------------------------------------------------

CParseProcessThread::CParseProcessThread(CBaseChannel *baseChannel)
{
   m_baseChannel=baseChannel;
}
CParseProcessThread::~CParseProcessThread()
{
}
void CParseProcessThread::run()
{
    if(m_baseChannel->m_logSwitch != NULL && m_baseChannel->m_logSwitch->isOn())
    {
        m_baseChannel->m_logSwitch->LOG_INFO("Enter ProcessThread");
    }
    int iWaitTimeout = 100;
    bool isTimeOut=false;
    bool bContinue=true;

    while(true==m_baseChannel->getStartCommunicationStatus() && bContinue)
    {
       isTimeOut=m_baseChannel->waitThreadEvent(ThreadType::ParseThread,iWaitTimeout);

       if (m_baseChannel->getEventStatus(ThreadEventType::ShutDowmEvent))
       {
           bContinue = false;
           break;
       }
       else if (m_baseChannel->getEventStatus(ThreadEventType::ParseEvent)||
                m_baseChannel->getCommandCountFromQueue(CommandQueueType::Parse)>0)
       {
           m_baseChannel->resetEventStatus(ThreadEventType::ParseEvent);
           //移除解析命令
           CBaseProtocolCommand *currentCmd = m_baseChannel->removeHeadCommandFromQueue(CommandQueueType::Parse);
           while (NULL != currentCmd)
           {
               currentCmd->stopRetryCommand();
               //进入数据块数据处理
               CBaseDataBlock* db = currentCmd->getDataBlock();
               CBaseDevice* dev = currentCmd->getDevice();
               if (NULL != db && NULL != dev && dev->isRunning())//Y2hw  && NULL != dev && dev->isRunning()
               {
                   db->parseCommand(currentCmd);
               }

               //从活动队列中删除
               m_baseChannel->m_processCmdListMutex.lock();
               if(m_baseChannel->m_processCmdList.indexOf(currentCmd) != -1)
                   m_baseChannel->m_processCmdList.removeOne(currentCmd);
               m_baseChannel->m_processCmdListMutex.unlock();
               //如果是写命令，销毁对象
               //****************************** Y2hw *******************************
               if (currentCmd->getCommandType() == ProtocolCommandType::Write ||
                   currentCmd->getCommandType() == ProtocolCommandType::BlockWrite)
               {
                   delete currentCmd;
                   currentCmd = NULL;
               }
               //------------------------------------------------------------------
               //下一个解析命令
               currentCmd = m_baseChannel->removeHeadCommandFromQueue(CommandQueueType::Parse);

               if (m_baseChannel->getEventStatus(ThreadEventType::ShutDowmEvent))
               {
                   bContinue = false;
                   break;
               }
           }           
       }
    }
    m_baseChannel->m_processCmdListMutex.lock();
    m_baseChannel->m_processCmdList.clear();
    m_baseChannel->m_processCmdListMutex.unlock();

    if(m_baseChannel->m_logSwitch != NULL && m_baseChannel->m_logSwitch->isOn())
    {
        m_baseChannel->m_logSwitch->LOG_INFO("Leave ProcessThread");
    }
}

