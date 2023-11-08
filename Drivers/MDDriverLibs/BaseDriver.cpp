#include <QStringList>
#include <QLibrary>

#include "BaseDriver.h"
#include "MDWriteVariable.h"

#include "MDSwitchFactory/MDLogSwitchFactory.h"
#include "MDPropertyBrowser/PropertyBrowser.h"
CBaseDriver::CBaseDriver()
{
    m_driverLib = NULL;
    m_createChannelInstance = NULL;
    m_createDeviceInstance = NULL;
    m_createDataBlockInstance = NULL;
    m_logSwitch = NULL;
}

CBaseDriver::~CBaseDriver()
{
    if(NULL!=m_driverLib)
    {
      m_driverLib->unload();
      m_driverLib=NULL;
    }
}
/*!
\section 项目相关
*/

void CBaseDriver::setProjectPath(const QString & projectPath)
{
    m_projectPath = projectPath;
    //设置每个通信channe的项目名
    QMutexLocker locker(&m_channelListMutex);
    for(int i=0;i<m_channelList.count();i++)
    {
        m_channelList[i]->setProjectPath(projectPath);
    }
}

void CBaseDriver::initial()
{
}

void CBaseDriver::release()
{
    removeAllDevices();
    if(m_logSwitch != NULL && m_logSwitch->isOn())
    {
       m_logSwitch->LOG_INFO("release");
    }
    m_logSwitch = NULL;
}

bool CBaseDriver::addDevice(IDevice *dev)
{
    bool ret = false;
    if(NULL != dev)
    {
        if(NULL==getDevice(dev->name()))
        {
            QMutexLocker locker(&m_deviceListMutex);
            m_deviceList.append(dev);
            ret = true;
        }
    }
    return ret;
}
bool CBaseDriver::removeDevice(const QString &devName)
{
    bool ret=false;
    if(devName.isEmpty() || devName.isNull())
    {
        return ret;
    }
    stopDevice(devName);

    QMutexLocker locker(&m_deviceListMutex);
    for(int i=0;i<m_deviceList.count();i++)
    {
        if(0==QString::compare(devName,m_deviceList[i]->name(), Qt::CaseInsensitive))
        {
            m_deviceList.removeAt(i);
            ret = true;
            break;
        }
    }
    return ret;
}
void CBaseDriver::removeAllDevices()
{
    stopAllDevices();
    QMutexLocker locker(&m_deviceListMutex);
    m_deviceList.clear();
}
IDevice * CBaseDriver::getDevice(const QString &devName)
{
    IDevice * dev = NULL;
    if(devName.isEmpty() || devName.isNull())
    {
        return dev;
    }

    QMutexLocker locker(&m_deviceListMutex);
    for(int i=0;i<m_deviceList.count();i++)
    {
        if(0==QString::compare(devName,m_deviceList[i]->name(), Qt::CaseInsensitive))
        {
            dev = m_deviceList[i];
            break;
        }
    }
    return dev;
}
bool CBaseDriver::startDevice(const QString &devName)
{
    bool ret=false;
    if(devName.isEmpty() || devName.isNull())
    {
        return ret;
    }

    IDevice * dev = getDevice(devName);
    if(dev == NULL)
    {
        return ret;
    }
    ret=startDevice(dev);

    return ret;
}
bool CBaseDriver::startDevice(IDevice * dev)
{
    bool ret=false;
    if(dev == NULL)
    {
        return ret;
    }
    QString error;
    if(false == bindDeviceToChannel(dev,error))
    {
        return ret;
    }

    CBaseChannel *channel = ((CBaseDevice*)dev)->channel();
    if(channel == NULL)
    {
        return ret;
    }

    ret = channel->startDevice((CBaseDevice *)dev);
    if(m_logSwitch == NULL)
        m_logSwitch = CMDLogSwitchFactory::createSwitchWithLogger(m_projectPath, driverID(), driverID());
    return ret;
}
void CBaseDriver::startAllDevices()
{
    QMutexLocker locker(&m_deviceListMutex);
    for(int i=0;i<m_deviceList.count();i++)
    {
        startDevice(m_deviceList[i]);
    }
}
bool CBaseDriver::stopDevice(const QString &devName)
{
    bool ret=false;
    if(devName.isEmpty() || devName.isNull())
    {
        return ret;
    }

    IDevice * dev = getDevice(devName);
    if(dev == NULL)
    {
        return ret;
    }
    ret = stopDevice(dev);
    return ret;

}
bool CBaseDriver::stopDevice(IDevice * dev)
{
    bool ret=false;
    if(dev == NULL)
    {
        return ret;
    }
    ret = removeDeviceFromChannel(dev);
    return ret;

}
void CBaseDriver::stopAllDevices()
{
    if(m_logSwitch != NULL && m_logSwitch->isOn())
        m_logSwitch->LOG_INFO("Begin for stopCommunicaton");
    m_channelListMutex.lock();
    for(int i=0;i<m_channelList.count();i++)
    {
        m_channelList[i]->stopCommunicaton();
    }
    m_channelListMutex.unlock();

    if(m_logSwitch != NULL && m_logSwitch->isOn())
        m_logSwitch->LOG_INFO("Begin for stopDevice");

    //Y2hw
    /*
    m_deviceListMutex.lock();
    for(int i=0;i<m_deviceList.count();i++)
    {
        stopDevice(m_deviceList[i]);
    }
    m_deviceListMutex.unlock();
    */

    if(m_logSwitch != NULL && m_logSwitch->isOn())
        m_logSwitch->LOG_INFO("Begin for delete channel");
    m_channelListMutex.lock();
    for(int i=0;i<m_channelList.count();i++)
    {
        m_channelList[i]->removeAllDevices();//Y2hw
        delete m_channelList[i];
    }
    m_channelList.clear();
    m_channelListMutex.unlock();
}
//!绑定设备到通信信道
bool CBaseDriver::bindDeviceToChannel(IDevice *dev,QString&error)
{
    bool bRet=false;
    if(dev == NULL)
    {
        return bRet;
    }
    CBaseChannel *basechannel=NULL;

    ////串口通信模式允许一个通道管理多个设备，其他的通信模式只能管理一个设备
    if (CommunicationType::Serial == dev->commType() )
    {
        basechannel = getSerialChannel(dev->commParameter());
    }
    if (NULL == basechannel)
    {
        //创建通道
        basechannel = createChannel();
        if (NULL != basechannel)
        {
            //设置驱动名到channel
            int nPos = driverID().lastIndexOf('.');
            if(nPos > 0)
            {
                basechannel->setProjectPath(m_projectPath);
                QString strdriverName = "RTDB.Driver.";
                strdriverName += driverID().mid(nPos+1);
                basechannel->setDriverName(strdriverName);
                basechannel->setProjectPath(m_projectPath);
            }
            basechannel->setCommType((CommunicationType)dev->commType());
            basechannel->setCommParameter(dev->commParameter());
            if(!addChannel(basechannel))
            {
                delete basechannel;
                basechannel = NULL;
            }
        }
    }
    if (NULL != basechannel)
    {
        bRet = basechannel->addDevice((CBaseDevice*)dev);
    }
    return bRet;
}
//!从通信信道解除设备
bool CBaseDriver::removeDeviceFromChannel(IDevice *dev)
{
    bool bRet=false;
    if(dev == NULL)
    {
        return bRet;
    }

    CBaseChannel *channel=((CBaseDevice*)dev)->channel();
    if(channel == NULL)
    {
        return bRet;
    }
    //停止设备通信，如channel中无其它设备，通信链路也停止，并移除通道
    channel->stopDevice((CBaseDevice *)dev);
    bRet = channel->removeDevice((CBaseDevice *)dev);
    if(channel->getDeviceCount()<=0)
    {
        channel->stopCommunicaton();
        removeChannel(channel);
        delete channel;
    }
    return bRet;
}

bool CBaseDriver::addChannel(CBaseChannel *channel)
{
    bool bRet=false;
    if(NULL!=channel)
    {
      QMutexLocker locker(&m_channelListMutex);
      m_channelList.append(channel);
      bRet=true;
    }
    return bRet;
}

bool CBaseDriver::removeChannel(CBaseChannel *channel)
{
    bool bRet=false;
    if(NULL!=channel)
    {
        QMutexLocker locker(&m_channelListMutex);
        bRet=m_channelList.removeOne(channel);
    }
    return bRet;
}
//!根据通信参数，查看是否已有串口
CBaseChannel *CBaseDriver::getSerialChannel(const QString &serialParameter)
{
    CBaseChannel *channel=NULL;
    QMutexLocker locker(&m_channelListMutex);
    QStringList commPropertyList1=serialParameter.split(',');
    if(commPropertyList1.length()>=0)
    {
        for(int iChannel=0;iChannel<m_channelList.count();iChannel++)
        {
            if(CommunicationType::Serial==m_channelList[iChannel]->commType())
            {
                QStringList commPropertyList2=m_channelList[iChannel]->commParameter().split(',');//;->,
                if(commPropertyList2.length()>0)
                {
                    //串口相同
                    if(QString::compare(commPropertyList2[0],commPropertyList1[0],Qt::CaseInsensitive)==0)
                    {
                        channel=m_channelList[iChannel];
                        break;
                    }
                }
            }
        }
    }
    return channel;
}

CBaseChannel *CBaseDriver::getChannel(int iChannelIndex)
{
    CBaseChannel *channel=NULL;
    QMutexLocker locker(&m_channelListMutex);
    if(iChannelIndex>=0 && iChannelIndex<m_channelList.count())
    {
        channel=m_channelList[iChannelIndex];
    }
    return channel;
}
//!设置库对象，便于后面创建channel、device、数据块
void CBaseDriver::setDriverLib(QLibrary *driverLib)
{
    m_driverLib = driverLib;
}

QLibrary* CBaseDriver::getDriverLib()
{
    return m_driverLib;
}
CBaseChannel *CBaseDriver::createChannel()
{
    if(NULL == m_createChannelInstance)
    {
        m_createChannelInstance = (createChannelInstance)m_driverLib->resolve("createChannelInstance");
    }
    CBaseChannel *channel=NULL;
    if(NULL!=m_createChannelInstance)
    {
        channel=m_createChannelInstance();
    }
    return channel;

}

CBaseDataBlock *CBaseDriver::createDataBlock()
{
    if(NULL == m_createDataBlockInstance)
    {
        m_createDataBlockInstance = (createDataBlockInstance)m_driverLib->resolve("createDataBlockInstance");
    }
    CBaseDataBlock *db=NULL;
    if(NULL!=m_createDataBlockInstance)
    {
        db=m_createDataBlockInstance();
    }
    return db;
}

IDevice *CBaseDriver::createDevice()
{
    if(NULL == m_createDeviceInstance)
    {
        m_createDeviceInstance = (createDeviceInstance)m_driverLib->resolve("createDeviceInstance");
    }
    CBaseDevice *dev=NULL;
    if(NULL!=m_createDeviceInstance)
    {
        dev=m_createDeviceInstance();
        if(dev!=NULL)
        {
            dev->setDriver(this);
        }
    }
    return dev;
}
