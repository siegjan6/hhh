#include <QFile>
#include <QCoreApplication>
#include <QTextStream>
#include <QDir>

#include "MDDriverManager.h"
#include "BaseDriver.h"
#include "BaseDevice.h"
#include "BaseDataBlock.h"
#include "MDDriverInformation.h"
#include "SortJsonObject.h"
CMDDriverManager::CMDDriverManager()
{
    m_isRuntimeMode = false;
    m_isVirtualRunMode = false;
    initialAllDriverInformation();
    m_logSwitch = NULL;
    m_propBrowser = NULL;
    m_RTDBUI = NULL;
}
CMDDriverManager::~CMDDriverManager()
{
    removeAllDevice();    
    removeAllDriverInformation();
    //delete all driver  Y2hw
    QMutexLocker locker(&m_driversMapMutex);
    IDriver *drv=NULL;
    foreach(drv,m_driversMap.values())
    {
        if(NULL!=drv)
        {
            delete (CBaseDriver*) drv;
        }
    }
    m_driversMap.clear();
}
/*!
\section 项目相关
*/
void CMDDriverManager::setProjectPath(const QString & projectPath)
{
    m_projectPath = projectPath;
    m_logSwitch = CMDLogSwitchFactory::createSwitchWithLogger(m_projectPath, "RTDB.DriverManager" , "驱动管理");

    //设置每个驱动的项目名
    IDriver *drv=NULL;
    QMutexLocker locker(&m_driversMapMutex);
    foreach(drv,m_driversMap.values())
    {
        if(NULL!=drv)
        {
            drv->setProjectPath(projectPath);
        }
    }
}

void CMDDriverManager::setPropertyBrowse(CPropertyBrowser* propBrowser)
{
    m_propBrowser = propBrowser;
    QMutexLocker locker(&m_deviceListMutex);
    for(int i=0;i<m_deviceList.count();i++)
    {
        if(NULL==m_deviceList[i])
        {
            continue;
        }
        m_deviceList[i]->setPropertyBrowse(propBrowser);
    }
}

void CMDDriverManager::setRTDBManagerUI(IMDRTDBManagerUI* RTDBUI)
{
    m_RTDBUI = RTDBUI;
    QMutexLocker locker(&m_deviceListMutex);
    for(int i=0;i<m_deviceList.count();i++)
    {
        if(NULL==m_deviceList[i])
        {
            continue;
        }
        m_deviceList[i]->setRTDBManagerUI(RTDBUI);
    }
}

// Runtime模式:默认为编辑模式
bool CMDDriverManager::isRuntimeMode()
{
    return m_isRuntimeMode;
}
void CMDDriverManager::setRuntimeMode(bool isRuntime)
{
    m_isRuntimeMode = isRuntime;
}

bool CMDDriverManager::isVirtualRunMode()
{
    return m_isVirtualRunMode;
}
void CMDDriverManager::setVirtualRunMode(bool isVirtualRunMode)
{
    m_isVirtualRunMode = isVirtualRunMode;
}

void CMDDriverManager::initial()
{
}
void CMDDriverManager::release()
{
    removeAllVariables();//Y2hw
    removeAllDevice();

    //release all driver  Y2hw
    QMutexLocker locker(&m_driversMapMutex);
    IDriver *drv=NULL;
    foreach(drv,m_driversMap.values())
    {
        if(NULL!=drv)
        {
            drv->release();
        }
    }

    if(m_logSwitch != NULL && m_logSwitch->isOn())
        m_logSwitch->LOG_INFO("CMDDriverManager::removeAllDevice");
    m_logSwitch = NULL;
}
MDTreeItemType CMDDriverManager::itemType()
{
    return MDTreeItemType::DriverManager;
}

void CMDDriverManager::setItemType(MDTreeItemType itemType)
{
}

void *CMDDriverManager::parentItem(MDTreeItemType parentItemType)
{
   return NULL;
}

void CMDDriverManager::setParentItem(void* parent,MDTreeItemType parentItemType)
{
}


//获取驱动息信
IMDDriverInformation* CMDDriverManager::getDriverInformationAt(int index)
{

    IMDDriverInformation* driverInformation = NULL;
    QMutexLocker locker(&m_driverInformationListMutex);
    if (index < m_driverInformationList.count() && index>=0)
    {
        driverInformation = m_driverInformationList[index];
    }
    return driverInformation;
}
//获取驱动信息
IMDDriverInformation *CMDDriverManager::getDriverInformation(const QString & driverID)
{
    IMDDriverInformation* driverInformation = NULL;
    QMutexLocker locker(&m_driverInformationListMutex);
    for(int i=0;i<m_driverInformationList.count();i++)
    {
        if(NULL==m_driverInformationList[i])
        {
            continue;
        }
        if(0==QString::compare(m_driverInformationList[i]->id(), driverID,Qt::CaseInsensitive))
        {
            driverInformation = m_driverInformationList[i];
            break;
        }
    }
    return driverInformation;
}
//获取驱动信息个数
qint32 CMDDriverManager::getDriverInformationCount()
{
    int iCount = 0;
    QMutexLocker locker(&m_driverInformationListMutex);
    iCount =  m_driverInformationList.count();
    return iCount;
}
//初始化所有的驱动信息
void CMDDriverManager::initialAllDriverInformation()
{
    removeAllDriverInformation();
    //********************************* Y2hw ************************************
    /*
    QString configFile=QCoreApplication::applicationDirPath()+"/drivers/Drivers.xml";

    QFile file(configFile);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return;
    }

    QDomDocument domDocument;
    if (!domDocument.setContent(&file, true))
    {
        file.close();
        return;
    }
    QMutexLocker locker(&m_driverInformationListMutex);

    QDomElement rootElement = domDocument.firstChildElement();
    QDomElement driverRootElement = rootElement.firstChildElement();
    while ( !driverRootElement.isNull() )
    {
        if(0==QString::compare(driverRootElement.tagName(),"Drivers", Qt::CaseInsensitive))
        {
            QDomElement driverElement = driverRootElement.firstChildElement();
            while ( !driverElement.isNull())
            {
                if(0==QString::compare(driverElement.tagName(),"Driver", Qt::CaseInsensitive))
                {
                    QDomElement element = driverElement.firstChildElement();
                    QString driverName="";
                    QString driverID="";
                    QString driverDescription="";
                    while ( !element.isNull() )
                    {
                        if(0==QString::compare(element.tagName(),"Name", Qt::CaseInsensitive))
                        {
                            driverName = element.text();
                        }
                        else if(0==QString::compare(element.tagName(),"ID", Qt::CaseInsensitive))
                        {
                            driverID = element.text();
                            driverID = driverID.trimmed();
                        }
                        else if(0==QString::compare(element.tagName(),"Description", Qt::CaseInsensitive))
                        {
                            driverDescription = element.text();
                        }
                        element =element.nextSiblingElement();
                    }
                    if(!driverID.isEmpty() && !driverName.isEmpty())
                    {
                        CMDDriverInformation *driverInformation = new CMDDriverInformation();
                        driverInformation->setID(driverID);
                        driverInformation->setName(driverName);
                        driverInformation->setDescription(driverDescription);
                        m_driverInformationList.append(driverInformation);
                    }
                }
                driverElement = driverElement.nextSiblingElement();
            }
        }
        driverRootElement = driverRootElement.nextSiblingElement();
    }
    file.close();
    */
    QString driverpath=QCoreApplication::applicationDirPath()+"/drivers/";

    QDir filedir = QDir(driverpath);
    QStringList namefileter;
#if defined(Q_OS_WIN)
    namefileter.append("*.dll");
#elif defined(Q_OS_LINUX)
    namefileter.append("*.so");
#endif
    QFileInfoList files = filedir.entryInfoList(namefileter);

    QMutexLocker locker(&m_driverInformationListMutex);
    IDriver *drv=NULL;
    QT_TRY
    {
        for(int i =0; i<files.count(); i++)
        {
            QString libName = QString("%1/drivers/%2").arg(QCoreApplication::applicationDirPath()).arg(files.at(i).fileName());

            QLibrary *driverLib=new QLibrary(libName);
            if (driverLib->load())
            {
               createDriverInstance pCreateDriverInstance = (createDriverInstance)driverLib->resolve("createDriverInstance");

               if(NULL!=pCreateDriverInstance)
               {
                   drv = pCreateDriverInstance();
                   CMDDriverInformation *driverInformation = new CMDDriverInformation();
                   driverInformation->setID(drv->driverID());
                   driverInformation->setName(drv->driverDescription());
                   driverInformation->setDescription(drv->driverDescription());
                   m_driverInformationList.append(driverInformation);
                }
            }
        }
    }
    QT_CATCH(...)
    {

    }
    //------------------------------------------------------------------------------------
}
//删除所有驱动信息
void CMDDriverManager::removeAllDriverInformation()
{
    QMutexLocker locker(&m_driverInformationListMutex);
    for(int i=0;i<m_driverInformationList.count();i++)
    {
        delete m_driverInformationList[i];
    }
    m_driverInformationList.clear();
}


//删除所有设备
void CMDDriverManager::removeAllDevice()
{
    QMutexLocker locker(&m_deviceListMutex);
    for(int i=0;i<m_deviceList.count();i++)
    {
        IDriver * drv = m_deviceList[i]->driver();
        if(NULL != drv)
        {
            drv->removeDevice(m_deviceList[i]->name());
        }
        delete m_deviceList[i];
    }
    m_deviceList.clear();
}
//添加设备
bool CMDDriverManager::addDevice(QString name,QString description,IMDDriverInformation * driverInform,QString&error)
{
    bool ret = false;
    if(driverInform == NULL)
    {
        return ret;
    }
    if(NULL == getDevice(name))
    {
        IDriver *drv=NULL;
        drv=getDriver(driverInform->id());
        if(NULL==drv)
        {
            drv=addDriver(driverInform->id());
        }
        if(NULL==drv)
        {
            error = "驱动不存在";
            return ret;
        }
        IDevice *device = (IDevice *)drv->createDevice();
        device->setName(name);
        device->setDescription(description);
        ret =addDevice(device);
        if(ret == false)
        {
            delete device;
        }
        else
        {
            ret = true;
        }
    }
    else
    {
        error = "设备已经存在!";
    }

    return ret;
}
//复制粘贴
IDevice* CMDDriverManager::copyPasteDevice(IDevice* device)
{
    IDevice* retdev = NULL;
    bool ret = false;
    if(NULL == device)
    {
        return retdev;
    }
    for(int i=0;i<65535;i++)
    {
        QString devName =device->name();
        if(i>0)
        {
            devName = QString("%1%2").arg(device->name()).arg(i);
        }
        if(NULL == getDevice(devName))
        {
            IDriver *driver = device->driver();
            if(NULL == driver)
            {
                break;
            }
            IDevice *newDevice = driver->createDevice();
            if(NULL == newDevice)
            {
                break;
            }
            newDevice->copy(device);
            newDevice->setName(devName);
            ret =addDevice(newDevice);
            if(ret == false)
            {
                delete newDevice;
            }
            else
            {
                for(int n=0;n<device->getDataBlockCount();n++)
                {
                    newDevice->copyPasteDataBlock(device->getDataBlockAt(n));
                }
                retdev = newDevice;
            }
            break;
        }
    }
    return retdev;
}

bool CMDDriverManager::addDevice(IDevice* device)
{
    bool ret = false;
    if(NULL == device)
    {
        return ret;
    }
    if(NULL != getDevice(device->name()))
    {
        return ret;
    }
    IDriver * drv = device->driver();
    if(NULL == drv)
    {
        return ret;
    }
    if(false == drv->addDevice(device))
    {
        return ret;
    }
    QMutexLocker locker(&m_deviceListMutex);
    device->setPropertyBrowse(m_propBrowser);
    device->setRTDBManagerUI(m_RTDBUI);
    m_deviceList.append(device);
    device->setParentItem(this,MDTreeItemType::DriverManager);
    ret = true;
    return ret;
}
//移除设备
bool CMDDriverManager::removeDevice(IDevice* device)
{
    bool ret = false;
    if(NULL != device)
    {
        IDriver * drv = device->driver();
        if(NULL != drv)
        {
            drv->removeDevice(device->name());
        }
        QMutexLocker locker(&m_deviceListMutex);
        ret = m_deviceList.removeOne(device);
    }
    return ret;
}
bool CMDDriverManager::removeDevice(const QString & devName)
{
    bool ret = false;
    QMutexLocker locker(&m_deviceListMutex);
    for(int i=0;i<m_deviceList.count();i++)
    {
        if(NULL==m_deviceList[i])
        {
            continue;
        }
        if(0==QString::compare(m_deviceList[i]->name(), devName,Qt::CaseInsensitive))
        {
             IDevice *device = m_deviceList[i];
             m_deviceList.removeAt(i);

             IDriver * drv = device->driver();
             if(NULL != drv)
             {
                 drv->removeDevice(device->name());
             }
             delete device;
             ret = true;
             break;
        }
    }
    return ret;
}
//获取设备
IDevice* CMDDriverManager::getDeviceAt(int index)
{
    IDevice* device = NULL;
    QMutexLocker locker(&m_deviceListMutex);
    if (index < m_deviceList.count() && index>=0)
    {
        device = m_deviceList[index];
    }
    return device;
}
IDevice* CMDDriverManager::getDevice(const QString &devName)
{
    IDevice* device = NULL;
    QMutexLocker locker(&m_deviceListMutex);
    for(int i=0;i<m_deviceList.count();i++)
    {
        if(NULL==m_deviceList[i])
        {
            continue;
        }
        if(0==QString::compare(m_deviceList[i]->name(), devName,Qt::CaseInsensitive))
        {
            device = m_deviceList[i];
            break;
        }
    }
    return device;

}
//获取设备数量
qint32 CMDDriverManager::getDeviceCount()
{
    int iCount = 0;
    QMutexLocker locker(&m_deviceListMutex);
    iCount =  m_deviceList.count();
    return iCount;
}

void CMDDriverManager::startDevice(const QString &devName)
{
    if(devName.isEmpty()|| devName.isNull())
    {
        return;
    }

    IDevice *device = getDevice(devName);
    if(NULL==device)
    {
      return;
    }
    IDriver * drv = device->driver();
    if(NULL==drv)
    {
      return;
    }
    drv->startDevice(devName);
}

void CMDDriverManager::startAllDevices()
{
    QMutexLocker locker(&m_driversMapMutex);
    IDriver *drv=NULL;
    foreach(drv,m_driversMap.values())
    {
        if(NULL!=drv)
        {
            drv->startAllDevices();
        }
    }
}

void CMDDriverManager::stopDevice(const QString &devName)
{
    if(devName.isEmpty()|| devName.isNull())
    {
        return;
    }

    IDevice *device = getDevice(devName);
    if(NULL==device)
    {
      return;
    }
    IDriver * drv = device->driver();
    if(NULL==drv)
    {
      return;
    }
    drv->stopDevice(devName);

    /*/
    for(int iIndex=0;iIndex<devList.count();iIndex++)
    {
        if(devList[iIndex].isEmpty()|| devList[iIndex].isNull())
        {
          continue;
        }
        QMutexLocker locker(&m_deviceDriversMapMutex);
        QString devName=devList[iIndex].toUpper();
        if(m_deviceDriversMap.contains(devName))
        {
            IDriver *drv=m_deviceDriversMap.find(devName).value();
            if(NULL!=drv)
            {
                drv->stopDevice(devName);
            }
        }
    }/*/
}

void CMDDriverManager::stopAllDevices()
{
    QMutexLocker locker(&m_driversMapMutex);
    IDriver *drv=NULL;
    foreach(drv,m_driversMap.values())
    {
        if(NULL!=drv)
        {
            drv->stopAllDevices();
        }
    }
}
//移除所有变量
bool CMDDriverManager::removeAllVariables()
{
        QMutexLocker locker(&m_deviceListMutex);
        for(int i=0;i<m_deviceList.count();i++)
        {
            if(NULL==m_deviceList[i])
            {
                continue;
            }
            m_deviceList[i]->removeAllVariables();
        }
        return true;
}

void CMDDriverManager::monitorDataBlock(bool bEnable,qint32 dataBlockID,QString devName,QString dbName,qint32 nFlag)
{/*
   QString devNameTemp=devName.toUpper();
   QMutexLocker locker(&m_deviceDriversMapMutex);
   if(m_deviceDriversMap.contains(devNameTemp))
   {
       CBaseDriver *drv=(CBaseDriver*)m_deviceDriversMap.find(devNameTemp).value();
       CBaseDevice *dev=drv->getDeviceWithName(devName);
       if(NULL!=dev)
       {
           CBaseDataBlock *db = NULL;
           if(dataBlockID>-1)
           {
               db=dev->getDataBlock(dataBlockID);
           }
           else
           {
               db=dev->getDataBlock(dbName);
           }

           if(NULL!=db)
           {
              db->setMonitoring(bEnable);
              db->setMonitorFlag(nFlag);

           }
       }
   }*/
}

bool CMDDriverManager::asyncWriteVariables(QList<IMDRTDBVariable*>& nsVarList)
{
    bool bRet=false;   
    IDataBlock* pDataBlock = NULL;
    typedef QMap<IDataBlock*, QList<IMDRTDBVariable*>*> WriteVarsMap;
    WriteVarsMap BlockWriteVarsMap;
    for(int i = 0; i < nsVarList.count(); i++)
    {
        pDataBlock = NULL;
        IMDRTDBVariable *thewriteVar=nsVarList[i];
        if(NULL!=thewriteVar)
        {
            QString deviceName = thewriteVar->deviceName();
            if(deviceName.isEmpty())
            {
                continue;
            }

            deviceName = deviceName.toUpper();
            //找device
            IDevice* pDev = getDevice(deviceName);
            if(pDev != NULL)
            {
                //找datablock
                //从地址获取DataBlock名
                int indexofDoc = thewriteVar->address().indexOf('.');
                if(indexofDoc != -1)
                {
                    QString blockName = thewriteVar->address().left(indexofDoc);
                    pDataBlock = pDev->getDataBlock(blockName);
                }

                if(pDataBlock != NULL)
                {
                    if(BlockWriteVarsMap.contains(pDataBlock))
                    {
                        QList<IMDRTDBVariable*>* writelist = BlockWriteVarsMap[pDataBlock];
                        if(writelist != NULL)
                            writelist->append(thewriteVar);
                    }
                    else
                    {
                        QList<IMDRTDBVariable*>* writelist = new  QList<IMDRTDBVariable*>();
                        writelist->append(thewriteVar);
                        BlockWriteVarsMap.insert(pDataBlock,writelist);
                    }
                }//end if(pDataBlock != NULL)
            }//end if(pDev != NULL)
        }//end if(NULL!=thewriteVar)
    }//end for all variable

    //调用DataBlock快写
    WriteVarsMap::Iterator it;
    for ( it = BlockWriteVarsMap.begin(); it != BlockWriteVarsMap.end(); ++it )
    {
       pDataBlock = it.key();
       if(pDataBlock != NULL)
       {
           //日志
           if(m_logSwitch->isOn())
           {
               QList<IMDRTDBVariable*>* writelist = it.value();
               QString info = QString("DataBlock:%1:write %2 Variables").arg(pDataBlock->name()).arg(QString(writelist->size()));
               m_logSwitch->LOG_INFO(info);
           }
           pDataBlock->asyncWriteVariables(*(it.value()));
           delete it.value();
       }
    }

    BlockWriteVarsMap.clear();
    return bRet;
}

bool CMDDriverManager::asyncWriteVariable(IMDRTDBVariable *nsVar, QVariant data)
{
    bool bRet=false;/*/
    if(NULL!=nsVar && NULL!=data)
    {
        QString deviceName = nsVar->getDeviceName();
        if(deviceName.isEmpty()||deviceName.isEmpty())
        {
            return bRet;
        }
        QMutexLocker locker(&m_deviceDriversMapMutex);
        deviceName = deviceName.toUpper();
        if(m_deviceDriversMap.contains(deviceName))
        {
            IDriver *drv = m_deviceDriversMap.find(deviceName).value();
            if(NULL!=drv)
            {
                bRet=drv->asyncWriteVariable(nsVar,data);
            }
        }
    }/*/
    return bRet;
}

//!反射添加驱动
IDriver *CMDDriverManager::addDriver(const QString &drvID)
{
    IDriver *drv=NULL;
    QT_TRY
    {
        QString libName = QString("%1/drivers/%2").arg(QCoreApplication::applicationDirPath()).arg(drvID);
        QLibrary *driverLib=new QLibrary(libName);

        if(driverLib != NULL)
        {
            if (driverLib->load())
            {
               createDriverInstance pCreateDriverInstance = (createDriverInstance)driverLib->resolve("createDriverInstance");

               if(NULL!=pCreateDriverInstance)
               {
                   drv = pCreateDriverInstance();
                   //
                   drv->setProjectPath(m_projectPath);
                   drv->setDriverLib(driverLib);
                   QMutexLocker mutexLokerTemp(&m_driversMapMutex);
                   m_driversMap.insert(drvID.toUpper(),drv);
                }
            }
            else
            {
                delete driverLib;
            }
        }
    }
    QT_CATCH(...)
    {

    }
    return drv;
}

void CMDDriverManager::removeDriver(const QString & drvID)
{
    IDriver *drv=NULL;
    QString drvIDTemp=drvID.toUpper();

    QMutexLocker mutexLokerTemp(&m_driversMapMutex);
    if(m_driversMap.contains(drvIDTemp))
    {
        drv=m_driversMap.find(drvIDTemp).value();
    }
    if(NULL!=drv)
    {
        m_driversMap.remove(drvIDTemp);
        delete (CBaseDriver*) drv;
    }
}

IDriver *CMDDriverManager::getDriver(const QString & drvID)
{
    IDriver *drv=NULL;
    QString drvIDTemp=drvID.toUpper();

    QMutexLocker mutexLokerTemp(&m_driversMapMutex);
    if(m_driversMap.contains(drvIDTemp))
    {
        drv=m_driversMap.find(drvIDTemp).value();
    }
    return drv;
}
void CMDDriverManager::saveConfiguration(const QString &configFile)
{
    /*
    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version=""1.0"" encoding=""UTF-8""");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement("MindSCADARTDBDeviceConfiguration");
    QDomAttr virsion = doc.createAttribute("virsion");
    virsion.setValue("V6.0.0.1");
    root.setAttributeNode(virsion);
    doc.appendChild(root);

    QDomElement devicesElement = doc.createElement("Devices");
    root.appendChild(devicesElement);

    QMutexLocker locker(&m_deviceListMutex);
    for (int index=0; index< m_deviceList.count();index++)
    {
       ((CBaseDevice*)m_deviceList[index])->saveConfiguration(doc,devicesElement);
    }

    QFile file(configFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
    {
        return ;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    doc.save(out,4,QDomNode::EncodingFromTextStream);
    file.close();*/

    CSortJsonObject theOb;
    //head
    CSortJsonObject head;
    head.insertSimpleValue("Version",QString("1"));
    head.insertSimpleValue("Type",QString("DeviceConfig"));
    theOb.insertObject("Head",head);

    //every child object
    CSortJsonArray jsonarr;
    CBaseDevice* dev = NULL;
    QMutexLocker locker(&m_deviceListMutex);
    for (int index=0; index< m_deviceList.count();index++)
    {
        CSortJsonObject theChild;
        dev = (CBaseDevice*)m_deviceList[index];
        //Driver ID
        theChild.insertSimpleValue("@DRV",dev->driver()->driverID());
        //device serialize
        dev->serialize(theChild);
        jsonarr.insertObject(theChild);
    }
    theOb.insertArray("DEVS",jsonarr);

    //save to file
    QByteArray ba = theOb.convertJsonObject2String(1).toUtf8();
    QFile file(configFile);
    if (!file.open(QIODevice::WriteOnly))
        return;

    QTextStream stream(&file);
    stream << ba;
    file.close();
}

void CMDDriverManager::readConfiguration(const QString &configFile)
{/*
    QFile file(configFile);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return;
    }

    QDomDocument domDocument;
    if (!domDocument.setContent(&file, true))
    {
        file.close();
        return;
    }

    QDomElement rootElement = domDocument.firstChildElement();
    QDomElement deviceRootElement = rootElement.firstChildElement();
    while ( !deviceRootElement.isNull() )
    {
        if(0==QString::compare(deviceRootElement.tagName(),"Devices", Qt::CaseInsensitive))
        {
            QDomElement deviceElement = deviceRootElement.firstChildElement();
            while ( !deviceElement.isNull())
            {
                if(0==QString::compare(deviceElement.tagName(),"Device", Qt::CaseInsensitive))
                {
                    QString driverID = deviceElement.attribute("DriverID");
                    IDriver *drv=NULL;
                    drv=getDriver(driverID);
                    if(NULL==drv)
                    {
                        drv=addDriver(driverID);
                    }
                    if(NULL!=drv)
                    {
                        IDevice *device = (IDevice *)drv->createDevice();
                        if(device != NULL)
                        {
                            ((CBaseDevice*)device)->readConfiguration(deviceElement);
                            if(false == addDevice(device))
                            {
                                delete device;
                            }
                        }
                    }
                }
                deviceElement = deviceElement.nextSiblingElement();
            }
        }
        deviceRootElement = deviceRootElement.nextSiblingElement();
    }
    file.close();
    */

    QFile file(configFile);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray ba;
    QTextStream stream(&file);
    ba = stream.readAll().toUtf8();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(ba, &error);
    if(error.error != QJsonParseError::NoError)
        return;

    //获取device对象
    QJsonObject devOb;
    QString driverID;
    QJsonArray devarray = doc.object().value("DEVS").toArray();
    int count = devarray.count();
    for(int i = 0; i < count; i++)
    {
        devOb = devarray.at(i).toObject();
        driverID = devOb.value("@DRV").toString();
        IDriver *drv=NULL;
        drv=getDriver(driverID);
        if(NULL==drv)
        {
            drv=addDriver(driverID);
        }
        if(NULL!=drv)
        {
            IDevice *device = (IDevice *)drv->createDevice();
            if(device != NULL)
            {
                ((CBaseDevice*)device)->deserialize(devOb);
                if(false == addDevice(device))
                {
                    delete device;
                }
            }
        }

    }
}

//---------------------------------------------------------------------------------------
/*!
\section 动态添加修改设备
*/
//---------------------------------------------------------------------------------------

/*!
 * \brief 动态删除设备变量
 */
void  CMDDriverManager::dynamicDeleteDevice(const QString &deviceID)
{
    QMutexLocker locker(&m_deviceListMutex);
    int count = m_deviceList.count();
    for(int i=count-1;i>=0;i--)
    {
        if(NULL==m_deviceList[i])
        {
            continue;
        }
        CBaseDevice *device = (CBaseDevice *)m_deviceList[i];
        if(0==QString::compare(device->name(), deviceID,Qt::CaseInsensitive))
        {
             m_deviceList.removeAt(i);
             IDriver * drv = device->driver();
             if(NULL != drv)
             {
                 drv->removeDevice(device->name());
             }
             delete device;
        }
    }
}
/*!
 * \brief 动态添加设备变量
 */
void  CMDDriverManager::dynamicAddDevice(const QString &deviceID,const QVariantList &deviceDriverList)
{
    for(int i=0;i<deviceDriverList.count();i++)
    {
        QVariantMap driverMap = deviceDriverList[i].toMap();
        QString driverID =driverMap["@DriverID"].toString();
        driverID.trimmed();
        if(driverID.isNull()||driverID.isEmpty())
        {
            continue;
        }
        IDriver *drv=NULL;
        drv=getDriver(driverID);
        if(NULL==drv)
        {
            drv=addDriver(driverID);
        }
        if(NULL!=drv)
        {
            QVariantList logicDeviceList =  driverMap["MDDriverLogicDevice"].toList();
            for(int j=0;j<logicDeviceList.count();j++)
            {
                QVariantMap logicDeviceMap = logicDeviceList[j].toMap();
                CBaseDevice *device = (CBaseDevice *)drv->createDevice();
                if(device == NULL)
                {
                    continue;
                }
                device->dynamicSetParameter(logicDeviceMap);
                if(false == addDevice(device))
                {
                    delete device;
                    device = NULL;
                }
                else
                {
                    device->setName(deviceID);
                }
            }//end for each device
        }
    }//end for each driver
}

