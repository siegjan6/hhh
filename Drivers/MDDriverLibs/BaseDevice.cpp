#include "BaseDevice.h"
#include<QDir>
#include<QSettings>
#include<QCoreApplication>
#include<QMessageBox>
#include"MDMultiLanguage.h"
#include "MDPropertyBrowser/PropertyBrowser.h"
#include "CustomEvent.h"
CBaseDevice::CBaseDevice()
{
    m_translateMap.insert("GUID_General", tr("通用"));
    m_translateMap.insert("Enable", tr("启用"));
    m_translateMap.insert("Name", tr("设备名"));
    m_translateMap.insert("Description", tr("描述"));

    m_name = "";
    m_dynamicDeviceID="";
    m_description= "";
    m_driver=NULL;
    m_channel=NULL;
    m_enabled=false;
    m_retries=3;
    m_replyTimeout=1000;


    m_running=false;

    m_cmdTransimitCount=0;
    m_cmdReceiveCount=0;
    m_cmdTimeoutCount=0;
    m_cmdRetryCount=0;
    m_cmdErrorCount=0;

    m_dataBlockList.clear();
    m_itemType = MDTreeItemType::Device;
    m_propBrowser = NULL;
    m_RTDBUI = NULL;
    m_qualityVariable = NULL;
}
CBaseDevice::~CBaseDevice()
{
    removeAllDataBlocks();
    m_translateMap.clear();
}
//TreeItemType
MDTreeItemType CBaseDevice::itemType()
{
    return m_itemType;
}

void CBaseDevice::setPropertyBrowse(CPropertyBrowser* propBrowser)
{
    m_propBrowser = propBrowser;
}

CPropertyBrowser* CBaseDevice::getPropertyBrowse()
{
    return m_propBrowser;
}

void CBaseDevice::setRTDBManagerUI(IMDRTDBManagerUI* RTDBUI)
{
    m_RTDBUI = RTDBUI;
}

IMDRTDBManagerUI* CBaseDevice::getRTDBManagerUI()
{
    return m_RTDBUI;
}

void CBaseDevice::setItemType(MDTreeItemType itemType)
{
    m_itemType = itemType;
}

void *CBaseDevice::parentItem(MDTreeItemType parentItemType)
{
   return m_driverManager;
}
void CBaseDevice::setParentItem(void* parent,MDTreeItemType parentItemType)
{
    m_driverManager = (IMDDriverManager*)parent;
}
bool CBaseDevice::isRuntimeMode()
{
    if(m_driverManager)
    {
        return  m_driverManager->isRuntimeMode();
    }
    return false;
}

//动态设备ID
QString CBaseDevice::dynamicDeviceID()
{
    return m_dynamicDeviceID;
}

void CBaseDevice::setDynamicDeviceID(const QString & deviceID)
{
    m_dynamicDeviceID = deviceID;
}

//获取当前通信参数
QString CBaseDevice::getCurCommParameter()
{
    CBaseChannel* thechannel = channel();
    if(thechannel != NULL)
        return thechannel->getCurCommParameter();
    else
        return "";
}
//名称
QString CBaseDevice::name()
{
    return m_name;
}

void CBaseDevice::setName(const QString & name)
{
    m_name = name;
}

void CBaseDevice::setNameProp(const QString & name)
{
    if(name.isEmpty())
    {
       //QMessageBox::warning(NULL, "Error", "设备名不能为空！", QMessageBox::Ok);
        QMessageBox box;
        box.setText(tr("设备名不能为空！"));
        box.setWindowTitle(tr("错误"));
        box.setStandardButtons(QMessageBox::Ok);

        CMDMultiLanguage::instance()->translateWidget(&box);

        box.exec();
       return;
    }
    IMDDriverManager *driverManager = (IMDDriverManager*)parentItem(MDTreeItemType::DriverManager);
    IDevice *deviceTemp = driverManager->getDevice(name);
    if( deviceTemp!=NULL && this !=deviceTemp)
    {
      //QMessageBox::warning(NULL, "Error", "设备名重复！", QMessageBox::Ok);
       QMessageBox box;
       box.setText(tr("设备名重复！"));
       box.setWindowTitle(tr("错误"));
       box.setStandardButtons(QMessageBox::Ok);

       CMDMultiLanguage::instance()->translateWidget(&box);

       box.exec();
       return;
    }

    m_name = name;

    if(m_RTDBUI != NULL)
    {
        CCustomEvent* event = new CCustomEvent(NameChangedEvent,1);
        event->setParam(QVariant((qulonglong)this));
        event->setValue(QVariant(name));
        QCoreApplication::postEvent((QObject*)m_RTDBUI, event);
    }
}

QString CBaseDevice::description()
{
    return m_description;
}

void CBaseDevice::setDescription(const QString & description)
{
    m_description = description;
}

//关联Driver
IDriver *CBaseDevice::driver()
{
    return m_driver;
}
//!设置库对象，便于上层从Dev直接获取到所属驱动
void CBaseDevice::setDriver(IDriver *driver)
{
    m_driver = driver;
}

//关联Channel
CBaseChannel *CBaseDevice::channel()
{
    return m_channel;
}

void CBaseDevice::setChannel(CBaseChannel *channel)
{
    m_channel = channel;
    QMutexLocker locker(&m_datablockListMutex);
    for(int iDB=0;iDB<m_dataBlockList.count();iDB++)
    {
        ((CBaseDataBlock*)m_dataBlockList[iDB])->setChannel(m_channel);
    }
}

//启用状态
bool CBaseDevice::isEnabled()
{
    return m_enabled;
}

void CBaseDevice::setEnabled(bool enabled)
{
    m_enabled = enabled;
}
//超时时间（毫秒）
quint32 CBaseDevice::replyTimeout()
{
    return m_replyTimeout;
}

void CBaseDevice::setReplyTimeout(quint32 replyTimeout)
{
    m_replyTimeout = replyTimeout;
}

//命令重试次数
quint32 CBaseDevice::retries()
{
    return m_retries;
}

void CBaseDevice::setRetries(quint32 retries)
{
    m_retries = retries;
}

void CBaseDevice::resetStatistics()
{
    QMutexLocker locker(&m_objMutex);
    m_cmdTransimitCount=0;
    m_cmdReceiveCount=0;
    m_cmdTimeoutCount=0;
    m_cmdRetryCount=0;
    m_cmdErrorCount=0;
}


qint64 CBaseDevice::transmitCount()
{
   QMutexLocker locker(&m_objMutex);
   return m_cmdTransimitCount;
}
void CBaseDevice::incrementTransmitCount()
{
    QMutexLocker locker(&m_objMutex);
    m_cmdTransimitCount++;
}

qint64 CBaseDevice::receiveCount()
{
   QMutexLocker locker(&m_objMutex);
   return m_cmdReceiveCount;
}
void CBaseDevice::incrementReceiveCount()
{
    QMutexLocker locker(&m_objMutex);
    m_cmdReceiveCount++;

    if(m_qualityVariable != NULL && m_qualityVariable->currentValue().toInt() != 192)
    {
        int nStatus = 192;
        QVariant varDataValue = nStatus;
        m_qualityVariable->refreshData(varDataValue,
                                       QDateTime::currentDateTime(),
                                       MDVariableDataQuality::Good, false);
    }
}
qint64 CBaseDevice::timeoutCount()
{
    QMutexLocker locker(&m_objMutex);
   return m_cmdTimeoutCount;
}
void CBaseDevice::incrementTimeoutCount()
{
    QMutexLocker locker(&m_objMutex);
    m_cmdTimeoutCount++;
}
qint64 CBaseDevice::retryCount()
{
   QMutexLocker locker(&m_objMutex);
   return m_cmdRetryCount;
}

void CBaseDevice::incrementRetryCount()
{
    QMutexLocker locker(&m_objMutex);
    m_cmdRetryCount++;
}

qint64 CBaseDevice::errorCount()
{
    QMutexLocker locker(&m_objMutex);
   return m_cmdErrorCount;
}
void CBaseDevice::incrementErrorCount()
{
    QMutexLocker locker(&m_objMutex);
    m_cmdErrorCount++;
}
void CBaseDevice::copy(IDevice* otherDevice)
{
    if(NULL== otherDevice)
    {
        return;
    }
    m_name = otherDevice->name();
    m_description = otherDevice->description();
    m_replyTimeout=( (CBaseDevice*)otherDevice)->replyTimeout();
    m_retries = ( (CBaseDevice*)otherDevice)->retries();
    m_enabled = ( (CBaseDevice*)otherDevice)->isEnabled();

}
/*!
\section 数据块操作
*/
//复制粘贴数据块
IDataBlock* CBaseDevice::copyPasteDataBlock(IDataBlock* db)
{
    IDataBlock* retDB = NULL;
    bool ret = false;
    if(NULL == db)
    {
        return retDB;
    }
    for(int i=0;i<65535;i++)
    {
        //默认名
        QString dbName =db->name();
        if(i>0)
        {
            dbName = QString("%1%2").arg(db->name()).arg(i);
        }
        if(NULL == getDataBlock(dbName))
        {
            IDataBlock *newDB= createDataBlock(this,false);
            if(NULL == newDB)
            {
                break;
            }
            newDB->copy(db);
            newDB->setName(dbName);
            ret =addDataBlock(newDB);
            if(ret == false)
            {
                delete newDB;
            }
            else
            {
                //ret = true;
                retDB = newDB;
            }
            break;
        }
    }
    return retDB;
}

//添加变量
bool CBaseDevice::addVariable(IMDRTDBVariable* nsVar,QString &error)
{
    if(nsVar==NULL)
    {
        return false;
    }
    QString address = nsVar->address();

    int pos = address.indexOf(".");
    if(pos<=0)
    {
        if(address.compare("CommStatus",Qt::CaseInsensitive) ==0)
        {
            int nDataType = (int)nsVar->originalDataType();
           if(nDataType >= (int)MDDataType::Byte && nDataType <= (int)MDDataType::UInt64)
           {
               m_qualityVariable = nsVar;
               return true;
           }
           else
           {
               error = tr("数据类型错误!");
               return false;
           }
        }
        else
        {
           error = tr("地址验证错误!");
           return false;
        }
    }
    QString dbName = address.left(pos);
    IDataBlock * db = getDataBlock(dbName);
    if(db==NULL)
    {
        error = tr("数据块名错误!");
        return false;
    }
    return db->addVariable(nsVar,error);
}


bool CBaseDevice::addDataBlock(IDataBlock *db)
{
    if(NULL==db)
    {
        return false;
    }
    if(false==findDataBlock(db->name()))
    {
        QMutexLocker locker(&m_datablockListMutex);
        db->setParentItem(this,MDTreeItemType::Device);
        ((CBaseDataBlock*)db)->setChannel(m_channel);
        m_dataBlockList.append(db);
        return true;
    }
    return false;
}
bool CBaseDevice::removeDataBlock(const QString & dbName)
{
    bool bRet=false;
    QMutexLocker locker(&m_datablockListMutex);
    for(int iDB=0;iDB<m_dataBlockList.count();iDB++)
    {
        if(0==QString::compare(m_dataBlockList[iDB]->name(),dbName, Qt::CaseInsensitive))
        {
            IDataBlock *db = m_dataBlockList[iDB];
            m_dataBlockList.removeAt(iDB);
            delete db;
            bRet=true;
            break;
        }
    }
    return bRet;
}

bool CBaseDevice::removeDataBlock(IDataBlock *db)
{
    if(NULL==db)
    {
        return false;
    }
    QMutexLocker locker(&m_datablockListMutex);
    return m_dataBlockList.removeOne(db);
}

void CBaseDevice::removeAllDataBlocks()
{
    QMutexLocker locker(&m_datablockListMutex);

    foreach (IDataBlock *db, m_dataBlockList)
    {
        if(NULL != db)
         {
            delete db;
            db=NULL;
        }
    }
    m_dataBlockList.clear();
}

int CBaseDevice::getDataBlockCount()
{
     int nCount=0;
     QMutexLocker locker(&m_datablockListMutex);
     nCount=m_dataBlockList.count();
     return nCount;
}

IDataBlock *CBaseDevice::getDataBlockAt(int iDBIndex)
{
    IDataBlock *db=NULL;
    QMutexLocker locker(&m_datablockListMutex);
    if(iDBIndex>=0 && iDBIndex<m_dataBlockList.count())
    {
        db=m_dataBlockList[iDBIndex];
    }
    return db;
}

IDataBlock *CBaseDevice::getDataBlock(const QString & dbName)
{
    IDataBlock *db=NULL;
    QMutexLocker locker(&m_datablockListMutex);
    for(int iDB=0;iDB<m_dataBlockList.count();iDB++)
    {
        if(0==QString::compare(m_dataBlockList[iDB]->name(),dbName,Qt::CaseInsensitive))
        {
            db=m_dataBlockList[iDB];
            break;
        }
    }
    return db;
}

bool CBaseDevice::findDataBlock(const QString & dbName)
{
     bool bRet=false;
     QMutexLocker locker(&m_datablockListMutex);
     for(int iDB=0;iDB<m_dataBlockList.count();iDB++)
     {
         if(0==QString::compare(m_dataBlockList[iDB]->name(),dbName,Qt::CaseInsensitive))
         {
             bRet=true;
             break;
         }
     }
     return bRet;
}
bool CBaseDevice::removeAllVariables()
{
    bool bRet=true;
    QMutexLocker locker(&m_datablockListMutex);
    for(int iDB=0;iDB<m_dataBlockList.count();iDB++)
    {
        m_dataBlockList[iDB]->removeAllVariable();
    }
    return bRet;
}
/*!
\section 通信
*/
void CBaseDevice::setupReadCommand(bool bForce)
{
    if(false==m_enabled || NULL==m_channel || false==m_running)
    {
        return;
    }
    QMutexLocker locker(&m_datablockListMutex);
    for(int iDB=0;iDB<m_dataBlockList.count();iDB++)
    {
       ((CBaseDataBlock*)m_dataBlockList[iDB])->setupReadCommand(false,bForce);
    }
}

void CBaseDevice::onTimeout(bool retry)
{
    QMutexLocker locker(&m_objMutex);
    m_cmdTimeoutCount++;
    if(true==retry)
    {
        m_cmdRetryCount++;
    }
}
bool CBaseDevice::isRunning()
{
    return m_running;
}
//!启动所有数据块
bool CBaseDevice::start()
{
    if(!m_enabled)
        return false;
    m_running=true;
    QMutexLocker locker(&m_datablockListMutex);
    for(int iDB=0;iDB<m_dataBlockList.count();iDB++)
    {
        ((CBaseDataBlock*)m_dataBlockList[iDB])->start();
    }
    return true;
}

bool CBaseDevice::stop()
{
    m_running=false;

    m_datablockListMutex.lock();
    for(int iDB=0;iDB<m_dataBlockList.count();iDB++)
    {
       ((CBaseDataBlock*)m_dataBlockList[iDB])->stop();
    }
    m_datablockListMutex.unlock();    
    //this->setChannel(NULL);
    return true;
}
void CBaseDevice::notifyNotConnectDevice()
{
    m_datablockListMutex.lock();
    for(int iDB=0;iDB<m_dataBlockList.count();iDB++)
    {
       ((CBaseDataBlock*)m_dataBlockList[iDB])->notifyNotConnectDevice();
    }
    m_datablockListMutex.unlock();
    if(m_qualityVariable != NULL && m_qualityVariable->currentValue().toInt() != 0)
    {
        int nStatus = 0;
        QVariant varDataValue = nStatus;
        m_qualityVariable->refreshData(varDataValue,
                                       QDateTime::currentDateTime(),
                                       MDVariableDataQuality::Good, false);
    }
}

void CBaseDevice::dynamicSetParameter(const QVariantMap &deviceParameterMap)
{

}

void CBaseDevice::setLastReadTime(QDateTime readTime)
{
    m_lastReadTime = readTime;
}

void CBaseDevice::setLastWriteTime(QDateTime writeTime)
{
    m_lastWriteTime = writeTime;
}

void CBaseDevice::setLastErrorTime(QDateTime errorTime)
{
    m_lastErrorTime = errorTime;
}

QDateTime CBaseDevice::lastReadTime()
{
    return m_lastReadTime;
}

QDateTime CBaseDevice::lastWriteTime()
{
    return m_lastWriteTime;
}

QDateTime CBaseDevice::lastErrorTime()
{
    return m_lastErrorTime;
}

