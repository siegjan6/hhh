#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QDir>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QSettings>
#include <QCoreApplication>
#include <QFileInfo>
#include <QCryptographicHash>

#include "MDRTDBManager.h"
#include "MDDriverManagerFactory.h"
#include "MDVariableArchivesFactory.h"
#include "MDVariableQueueManager.h"
#include "MDVariableManager.h"
#include "MDVariableGroup.h"
#include "MDVariableTreeModel.h"
#include "MDDeviceTreeModel.h"
#include "MDChangeVariable.h"
#include "MDRTDBVariableUaBase.h"
#include "SortJsonObject.h"
#include "MDVariableDataConverter.h"

#ifdef UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS
#include <QUaOffNormalAlarm>
#include <QUaExclusiveLevelAlarm>
#endif // UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS

#if defined(Q_OS_WIN)
#include <Windows.h>
#endif

CMDRTDBManager::CMDRTDBManager()
{
    m_pServer = NULL;
    m_isRuntimeMode = false;
    m_isRunning = false;
    m_isVirtualRunMode = false;
    m_simulatiorIsRunning = false;
    m_saveVariableConfigBfrQuit = false;
    m_variableChangeNotifier = NULL;
    m_mdDriverManager = CMDDriverManagerFactory::createMDDriverManager();
    m_mdVariableArchivesManager = CMDVariableArchivesFactory::createVariableArchivesManager();
    m_mdVariableManager = new CMDVariableManager();
    m_mdMiddleVariableManager = new CMDVariableManager();

    m_mdVariableManager->setRTDBManager(this);
    m_mdMiddleVariableManager->setRTDBManager(this);

    m_mdVariableGroup = new CMDVariableGroup();
    m_mdDriverManager->initial();
    m_mdRTDBSimulatiorEngine.setRTDBManager(this);
    ((CMDVariableGroup*)m_mdVariableGroup)->setRTDBManager(this);
    m_mdVariableArchivesManager->setRTDBManager(this);

    //!逻辑开关
    releaseAllLogicSwitches();

	// Set custom port
	//server.setPort(8080);

	// Load server certificate
	//QFile certServer;
	//certServer.setFileName("server.crt.der");
	//Q_ASSERT(certServer.exists());
	//certServer.open(QIODevice::ReadOnly);
	//server.setCertificate(certServer.readAll());
	//certServer.close();


}
CMDRTDBManager::~CMDRTDBManager()
{
    //unLoadProject();
    //m_ChangeVariableQueueManager.removeAll();
    //m_mdDriverManager->release(); //has been called by unLoadProject
    m_mdVariableArchivesManager->stop();
    delete m_mdVariableArchivesManager;
    delete m_mdMiddleVariableManager;
    delete m_mdDriverManager;
    delete m_mdVariableGroup;
    delete m_mdVariableManager;
    m_mdVariableArchivesManager = NULL;
    m_mdMiddleVariableManager = NULL;
    m_mdDriverManager = NULL;
    m_mdVariableGroup = NULL;
    m_mdVariableManager = NULL;
    m_pServer = NULL;
}
bool CMDRTDBManager::getSwitchState(MDLogicSwitchType swType)
{
    bool bstate = false;

    switch(swType)
    {
    case MDLogicSwitchType::Middle:
        if(m_middleVarSwitch->isOn())
            bstate = true;
        break;
    case MDLogicSwitchType::Archive:
        if(m_varArchiveSwitch->isOn())
            bstate = true;
        break;
    case MDLogicSwitchType::SingleWrite:
        if(m_writeSingleVarSwitch->isOn())
            bstate = true;
        break;
    case MDLogicSwitchType::MultiWrite:
        if(m_writeMultiVarsSwitch->isOn())
            bstate = true;
        break;
    case MDLogicSwitchType::DynamicAddDevice:
        if(m_dynamicAddDeviceSwitch->isOn())
            bstate = true;
        break;
    case MDLogicSwitchType::DynamicAddVar:
        if(m_dynamicAddVarSwitch->isOn())
            bstate = true;
        break;
    case MDLogicSwitchType::DynamicRunDevice:
        if(m_dynamicStartDeviceSwitch->isOn())
            bstate = true;
        break;
    case MDLogicSwitchType::NotifyVarChange:
        if(m_notifyVarChangeSwitch->isOn())
            bstate = true;
        break;
    case MDLogicSwitchType::NotifyDeviceVarChange:
        if(m_notifyDeviceVarChangeSwitch->isOn())
            bstate = true;
        break;
    case MDLogicSwitchType::Log:
        if(m_logSwitch->isOn())
            bstate = true;
        break;
    }
    return bstate;
}
//!创建开关对象
void CMDRTDBManager::createAllSwitches()
{
    /*
    //!逻辑
    QString temp = m_projectPath + "/";
    #ifdef Q_OS_LINUX
    temp = m_projectPath + "\\";
    #endif
    */
    m_middleVarSwitch = CMDFuncSwitchFactory::createSwitch(m_projectPath, "RTDB.MiddleVariable", "中间变量");

    m_varArchiveSwitch = CMDFuncSwitchFactory::createSwitch(m_projectPath, "RTDB.ArchiveVariable", "变量归档");

    m_writeSingleVarSwitch = CMDFuncSwitchFactory::createSwitch(m_projectPath, "RTDB.WriteSingleVariable", "写单个变量");

    m_writeMultiVarsSwitch = CMDFuncSwitchFactory::createSwitch(m_projectPath, "RTDB.WriteMultiVariable", "批量写变量");

    m_dynamicAddDeviceSwitch = CMDFuncSwitchFactory::createSwitch(m_projectPath, "RTDB.DynamicAddDevice", "动态添加设备");

    m_dynamicAddVarSwitch = CMDFuncSwitchFactory::createSwitch(m_projectPath, "RTDB.DynamicAddVariable", "动态添加变量");

    m_dynamicStartDeviceSwitch = CMDFuncSwitchFactory::createSwitch(m_projectPath, "RTDB.DynamicStartDevice", "动态运行设备");

    m_notifyVarChangeSwitch = CMDFuncSwitchFactory::createSwitch(m_projectPath, "RTDB.NotifyVariableChange", "通知变量变化");

    m_notifyDeviceVarChangeSwitch = CMDFuncSwitchFactory::createSwitch(m_projectPath, "RTDB.NotifyDevVariableChange", "通知设备变量变化");

    //!日志
    m_logSwitch = CMDLogSwitchFactory::createSwitchWithLogger(m_projectPath, "RTDB.RTDBManager" , "RTDB管理");
}

//!释放开关
void CMDRTDBManager::releaseAllLogicSwitches()
{
    m_middleVarSwitch = NULL;
    m_varArchiveSwitch = NULL;
    m_writeSingleVarSwitch = NULL;
    m_writeMultiVarsSwitch = NULL;
    m_dynamicAddDeviceSwitch = NULL;
    m_dynamicAddVarSwitch = NULL;
    m_dynamicStartDeviceSwitch = NULL;
    m_notifyVarChangeSwitch = NULL;
    m_notifyDeviceVarChangeSwitch = NULL;
    m_logSwitch = NULL;
}

void CMDRTDBManager::ChangeLanguage()
{

}

/*!
\section Runtime模式:默认为编辑模式
*/

bool CMDRTDBManager::isRuntimeMode()
{
    return m_isRuntimeMode;
}
void CMDRTDBManager::setRuntimeMode(bool isRuntime)
{
    m_isRuntimeMode= isRuntime;
    if(m_mdDriverManager)
    {
        m_mdDriverManager->setRuntimeMode(m_isRuntimeMode);
    }
}
bool CMDRTDBManager::isVirtualRunMode()
{
    return m_isVirtualRunMode;
}
void CMDRTDBManager::setVirtualRunMode(bool isVirtualRunMode)
{
    m_isVirtualRunMode= isVirtualRunMode;
    if(m_mdDriverManager)
    {
        m_mdDriverManager->setVirtualRunMode(m_isVirtualRunMode);
    }
}

/*!
\section 读写变量
*/
CMDActionSPList *CMDRTDBManager::variableActionSPList(const QString &varName)
{
    return m_mdVariableManager->variableActionSPList(varName);
}

bool CMDRTDBManager::readVariableValue(IMDVariable *var)
{
     return m_mdVariableManager->readVariableValue(var);
}

QVariant CMDRTDBManager::readVariableValue(const QString &varName)
{
     return m_mdVariableManager->readVariableValue(varName);
}

bool CMDRTDBManager::writeVariableValue(const QString &varName,QVariant varValue)
{
    //!写开关
    if(!getSwitchState(MDLogicSwitchType::SingleWrite))
        return false;
    return m_mdVariableManager->writeVariableValue(varName,varValue);
}
void CMDRTDBManager::writeInitValue()
{
    CMDRTDBVariable* var = NULL;
    int nVarCount = m_mdVariableManager->getVariableCount();
    QVariant tempVal;
    QString strInitVal;
    for(int i = 0; i<nVarCount; i++)
    {
        var = (CMDRTDBVariable*)m_mdVariableManager->getVariableAt(i);
        if(var != NULL)
        {
            strInitVal = var->initializeParameter().m_value;
            if((var->variableType() != MDVariableType::MiddleVariable ||
                    var->variableType() != MDVariableType::SystemVariable))
            {
                if(var->initializeParameter().m_initType == MDRTInitMode::LastValue)
                {
                    // 项目保存值 writeInitValue from sqllite
                    strInitVal = m_mdVariableArchivesManager->getPrjInitValue(var->name());
                }

                if(strInitVal.isEmpty() || strInitVal.isNull())
                    continue;

               switch(var->dataType())
               {
               case MDDataType::Bool:
                   if(strInitVal == "0")
                        tempVal = QVariant(false);
                   else
                       tempVal = QVariant(true);
                   break;
               case MDDataType::Byte:
               case MDDataType::SByte:
                   tempVal = QVariant(strInitVal.toUInt() & 0xFF);
                   break;
               case MDDataType::Int16:
               case MDDataType::UInt16:
                   tempVal = QVariant(strInitVal.toUShort() & 0xFFFF);
                   break;
               case MDDataType::Int32:
               case MDDataType::UInt32:
                   tempVal = QVariant(strInitVal.toUInt() & 0xFFFFFFFF);
                   break;
               case MDDataType::Int64:
               case MDDataType::UInt64:
                   tempVal = QVariant(strInitVal.toULongLong() & 0xffffffffFFFFFFFF);
                   break;
               case MDDataType::Float:
                   tempVal = QVariant(strInitVal.toFloat());
                   break;
               case MDDataType::Double:
                   tempVal = QVariant(strInitVal.toDouble());
                   break;
               case MDDataType::String:
                   tempVal = QVariant(strInitVal);
                   break;
               case MDDataType::DateTime:
                   tempVal = QVariant(QDateTime::fromString(strInitVal));
                   break;
               default:
                   break;
               }
               if(tempVal.isValid())
                    var->writeValue(tempVal);
            }
        }//end if(var != NULL)
    }//end for
}

QDateTime CMDRTDBManager::getUpTime() {
#if defined(Q_OS_WIN)
    DWORD iRunTime = GetTickCount();
    const int Num1 = 1000;
    const int Num2 = 1900;
    time_t nowTime;
    time(&nowTime);
    time_t systemUpTime = nowTime - (iRunTime / Num1);
    struct tm * timeInfo;
    timeInfo = localtime(&systemUpTime);
    QString str_output = QString::asprintf("%d-%02d-%02d %02d:%02d:%02d",timeInfo->tm_year + Num2,
                      timeInfo->tm_mon + 1, timeInfo->tm_mday, timeInfo->tm_hour,
                      timeInfo->tm_min, timeInfo->tm_sec);
    return QDateTime::fromString(str_output,"yyyy-MM-dd HH:mm:ss");
#elif defined(Q_OS_LINUX)
    QProcess process;
    process.start("uptime -s");
    process.waitForFinished();
    QByteArray output = process.readAllStandardOutput();
    QString str_output = output;
    str_output = str_output.trimmed();
    return QDateTime::fromString(str_output,"yyyy-MM-dd HH:mm:ss");
#endif
}

void CMDRTDBManager::writeSystemValue()
{
    IMDVariableGroup *sysGroup = m_mdVariableGroup->getChildVariableGroup("SystemVariableGroup");
    if(sysGroup){

        IMDRTDBVariable *var =sysGroup->getVariable("StartDateTime");
        if(NULL!=var)
        {
            var->refreshData(QVariant(QDateTime::currentDateTime()),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
        }
        var =sysGroup->getVariable("UpDateTime");
        if(NULL!=var)
        {
            var->refreshData(QVariant(getUpTime()),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
        }
        var =sysGroup->getVariable("ProjectPath");
        if(NULL!=var)
        {
            var->refreshData(QVariant(projectPath()),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
        }
        var =sysGroup->getVariable("ProjectName");
        if(NULL!=var)
        {
            QFileInfo info = QFileInfo(projectPath());
            QString ProjectName = info.baseName();
            var->refreshData(QVariant(),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
        }
        var =sysGroup->getVariable("VHash");
        if(NULL!=var)
        {
            var->refreshData(QVariant(m_vhash),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false) ;
        }
    }
}

bool CMDRTDBManager::writeVariables(QList<IMDVariable*>& nsVarList)
{
    int nWriteCnt = nsVarList.count();
    IMDVariable* toWriteVar = NULL;
    //!批量写开关
    if(!getSwitchState(MDLogicSwitchType::MultiWrite))
    {
        //!释放列表
        for(int i= 0; i<nWriteCnt; i++)
        {
            toWriteVar = nsVarList.at(i);
            delete toWriteVar;
            toWriteVar = NULL;
        }
        nsVarList.clear();
        return false;
    }
    if(m_mdDriverManager != NULL)
    {
        QList<IMDRTDBVariable*> multiWriteVarList;
        CMDRTDBVariable* realVar = NULL;
        QVariant writeVal,originalWriteVal;
        for(int i= 0; i<nWriteCnt; i++)
        {
            toWriteVar = nsVarList.at(i);
            realVar = (CMDRTDBVariable*)m_mdVariableManager->getVariable(toWriteVar->name());
            if(realVar != NULL)
            {
                //检查是否有写保护
                if(realVar->writeProtectParameter().m_enableWrite)
                {
                    if(realVar->getWriteLimitValue(toWriteVar->currentValue(),writeVal))
                    {
                        //工程转换
                        if(CMDVariableDataConverter::getOriginalValue(realVar->dataConversionParameter(),
                                                                       realVar->originalDataType(),
                                                                       realVar->dataType(),
                                                                       realVar->getPrecision(),
                                                                       writeVal,
                                                                       originalWriteVal))
                        {
                            CMDRTDBVariable *writeVar = new CMDRTDBVariable();
                            writeVar->copy(realVar);
                            writeVar->setOriginalValue(originalWriteVal);
                            multiWriteVarList.append(writeVar);
                        }
                    }
                }
            }
            delete toWriteVar;
            toWriteVar = NULL;
        }
        nsVarList.clear();
        if(getSwitchState(MDLogicSwitchType::Log))
        {
            QString str = QString("write multi vars: %1").arg(QString(multiWriteVarList.count()));
            m_logSwitch->LOG_INFO(str);
        }
        return m_mdDriverManager->asyncWriteVariables(multiWriteVarList);
    }
    else
    {
        for(int i= 0; i<nWriteCnt; i++)
        {
            toWriteVar = nsVarList.at(i);
            delete toWriteVar;
        }
        nsVarList.clear();
        return false;
    }
}
/*!
\section 变量管理器
*/
IMDVariableManager *CMDRTDBManager::variableManager()
{
    return m_mdVariableManager;
}
IMDVariableManager *CMDRTDBManager::middleVariableManager()
{
    return m_mdMiddleVariableManager;
}
/*!
\section 变量组管理器
*/
IMDVariableGroup *CMDRTDBManager::variableGroup()
{
     return m_mdVariableGroup;
}
IMDVariableArchivesManager *CMDRTDBManager::variableArchivesManager()
{
     return m_mdVariableArchivesManager;
}

QUaServer *CMDRTDBManager::uaServer()
{
    return m_pServer;
}

void CMDRTDBManager::setUaServer(QUaServer* server)
{
    m_pServer = server;
}

void  CMDRTDBManager::createSystemVariableGroup()
{
      CMDVariableGroup *nsSystemVariableGroup =(CMDVariableGroup *) m_mdVariableGroup->getChildVariableGroup("SystemVariableGroup");
      QString error;
      if(NULL==nsSystemVariableGroup)
      {
          nsSystemVariableGroup = new CMDVariableGroup();
          nsSystemVariableGroup->setLocalName("SystemVariableGroup");
          nsSystemVariableGroup->setRTDBManager(this);
          if(!m_mdVariableGroup->addChildVariableGroup(nsSystemVariableGroup,error))
          {
                delete nsSystemVariableGroup;
                nsSystemVariableGroup = NULL;
          }
      }
      if(nsSystemVariableGroup && m_isRuntimeMode && NULL != m_pServer)
      {
          QUaFolderObject* sysFolder = m_pServer->objectsFolder()->addFolderObject("SystemVariableGroup");
		  nsSystemVariableGroup->setUAFolder(sysFolder);
          nsSystemVariableGroup->initialSystemVariableGroup();
          m_systemVariableTimer.setSystemVariableGroup(nsSystemVariableGroup);
      }
}
/*!
\section 驱动管理器
*/
IMDDriverManager  *CMDRTDBManager::driverManager()
{
    return m_mdDriverManager;
}


void CMDRTDBManager::initialMiddleVariableManager()
{
    for(int i=0;i<m_mdMiddleVariableManager->getVariableCount();i++)
    {
        CMDRTDBVariable* var = (CMDRTDBVariable*)m_mdMiddleVariableManager->getVariableAt(i);
        var->initialMiddleVariableExpression();
    }
}
void CMDRTDBManager::middleVariableDoExpression()
{
    for(int i=0;i<m_mdMiddleVariableManager->getVariableCount();i++)
    {
        CMDRTDBVariable* var = (CMDRTDBVariable*)m_mdMiddleVariableManager->getVariableAt(i);
        var->doExpression();
    }
}

/*!
\section 项目相关
*/
QString  CMDRTDBManager::projectPath()
{
    return m_projectPath;
}

void  CMDRTDBManager::setProjectPath(const QString &path)
{
    m_projectPath = path;
    if(m_projectPath.right(1)=="/")
    {
       m_rtdbPath= m_projectPath+"RTDB";
    }
    else
    {
        m_rtdbPath= m_projectPath+"/RTDB";
    }
    QDir *dir = new QDir;
    if(false == dir->exists(m_rtdbPath))
    {
        dir->mkdir(m_rtdbPath);
    }
    delete dir;
    //!设置DriverManager的项目路径
    if(m_mdDriverManager != NULL)
    {
        /*
        QString temp = m_projectPath + "/";
        #ifdef Q_OS_LINUX
        temp = m_projectPath + "\\";
        #endif
        */
        m_mdDriverManager->setProjectPath(m_projectPath);
    }
}

void  CMDRTDBManager::loadProject(const QString &path)
{
    //unLoadProject();
    setProjectPath(path);

    //!创建逻辑及日志开关
    createAllSwitches();

    readDeviceConfiguration();
    readVariableGroupConfiguration();
    createSystemVariableGroup();
}

void  CMDRTDBManager::unLoadProject()
{
     if(m_logSwitch != NULL && m_logSwitch->isOn())
     {
         m_logSwitch->LOG_INFO("unLoadProject");
         //!运行状态下，项目退出时保存一次，以便存储变量最后一次值
         if(m_saveVariableConfigBfrQuit)
         {
            saveVariableGroupConfiguration();
            m_logSwitch->LOG_INFO("saveVariableGroupConfiguration");
         }
         stopSimulatior();
         stop();
         m_logSwitch->LOG_INFO("stop");

         m_mdDriverManager->release();
         m_logSwitch->LOG_INFO("m_mdDriverManager::release");

         m_mdVariableManager->removeAllVariable();
         m_logSwitch->LOG_INFO("m_mdVariableManager::removeAllVariable");

         m_mdMiddleVariableManager->removeAllVariable();
         m_logSwitch->LOG_INFO("m_mdMiddleVariableManager::removeAllMiddleVariable");

         m_mdVariableGroup->removeAllChildVariableGroup();
         m_logSwitch->LOG_INFO("m_mdVariableGroup::removeAllChildVariableGroup");

     }
     else
     {
         if(m_saveVariableConfigBfrQuit)
         {
            saveVariableGroupConfiguration();
         }
         stopSimulatior();
         stop();

         m_mdDriverManager->release();

         m_mdVariableManager->removeAllVariable();

         m_mdMiddleVariableManager->removeAllVariable();

         m_mdVariableGroup->removeAllChildVariableGroup();
     }
     releaseAllLogicSwitches();
}

/*!
\section 运行相关
*/
bool CMDRTDBManager::isRunning()
{
    return m_isRunning;
}

void CMDRTDBManager::start()
{
    m_isRunning = true;
    stopSimulatior();
    //!以前是运行后再添加变量到驱动。现在编辑状态时已添加到数据块，支持数据块下查看变量。
    //m_mdDriverManager->removeAllVariables();
    m_mdVariableManager->checkVariableValid(m_mdDriverManager);
    initialMiddleVariableManager();
    middleVariableDoExpression();
    m_mdDriverManager->startAllDevices();
    m_mdVariableArchivesManager->start();
    m_systemVariableTimer.start();
    //写一次性赋值的系统变量
    writeSystemValue();
    //检查初始值加载
    writeInitValue();

	//auto almFolder = m_server.objectsFolder()->addFolderObject("Alarm");
	//auto motionSensor = almFolder->addChild<QUaBaseObject>("motionSensor");

	//auto moving = motionSensor->addBaseDataVariable("moving");
	//moving->setWriteAccess(true);
	//moving->setDataType(QMetaType::Bool);
	//moving->setValue(false);

	//auto motionAlarm = motionSensor->addChild<QUaOffNormalAlarm>("alarm1");
	//motionAlarm->setConditionName("Motion Sensor Alarm");
	//motionAlarm->setInputNode(moving);
	//motionAlarm->setNormalValue(false);
	//motionAlarm->setConfirmRequired(true);

    //m_server.start();
}
void CMDRTDBManager::stop()
{
    m_isRunning = false;
    m_systemVariableTimer.stop();
    m_mdVariableArchivesManager->stop();

    if(m_logSwitch != NULL && m_logSwitch->isOn())
        m_logSwitch->LOG_INFO("Begin stopAllDevices");
    m_mdDriverManager->stopAllDevices();

    if(m_logSwitch != NULL && m_logSwitch->isOn())
        m_logSwitch->LOG_INFO("End stopAllDevices");

    if(m_logSwitch != NULL && m_logSwitch->isOn())
        m_logSwitch->LOG_INFO("Begin removeAllVariables");

    m_mdDriverManager->removeAllVariables();

    if(m_logSwitch != NULL && m_logSwitch->isOn())
        m_logSwitch->LOG_INFO("End removeAllVariables");

    //m_server.stop();
    if(m_pServer)
    {
        try
        {
            // todo: crash when close the prj
            m_pServer->stop();
        }
        catch(...)
        {
            if(m_logSwitch != NULL && m_logSwitch->isOn())
                m_logSwitch->LOG_ERROR_IS("ua server stop crash catch");
        }
    }
}
/*!
\section 仿真
*/
bool CMDRTDBManager::simulatiorIsRunning()
{
    return m_simulatiorIsRunning;
}

void CMDRTDBManager::startSimulatior()
{
   //stop();//Y2hw
    stopSimulatior();
   m_simulatiorIsRunning = true;
   m_mdRTDBSimulatiorEngine.setRTDBManager(this);
   m_mdRTDBSimulatiorEngine.start();
   m_systemVariableTimer.start();

   //m_server.start();
}

void CMDRTDBManager::stopSimulatior()
{
    m_simulatiorIsRunning = false;
    m_mdRTDBSimulatiorEngine.stop();
    m_systemVariableTimer.stop();
}

/*!
\section 读写配置
*/
void  CMDRTDBManager::saveDeviceConfiguration()
{
    QString configFile = m_rtdbPath + "/Device.config";
    m_mdDriverManager->saveConfiguration(configFile);
}
void  CMDRTDBManager::readDeviceConfiguration()
{
    QString configFile = m_rtdbPath + "/Device.config";
    m_mdDriverManager->readConfiguration(configFile);
}


void  CMDRTDBManager::analyzeConfiguration(QDomElement &domElement )
{
    if ( !domElement.isNull() )
    {
        QDomElement element = domElement.firstChildElement();
        while ( !element.isNull() )
        {
            if(0==QString::compare(element.tagName(),"VariableGroups", Qt::CaseInsensitive))
            {
               ((CMDVariableGroup*) m_mdVariableGroup)->readConfiguration((CMDVariableManager *)m_mdVariableManager,m_mdDriverManager,NULL,element);
            }
            /*
            else if(0==QString::compare(element.tagName(),"Devices", Qt::CaseInsensitive))
            {
               ((CMDVariableGroup*) m_mdDriverManager)->readConfiguration(element);
            }/*/
            element = element.nextSiblingElement();
        }
    }
}
void  CMDRTDBManager::readVariableGroupConfiguration()
{
    QString  configFile = m_rtdbPath + "/Variable.config";
    /*
    QFile file(configFile);
     if(file.open(QIODevice::ReadOnly))
     {
         QDataStream in(&file);

         int version =0;
         in>>version;  //版本
         ((CMDVariableGroup*) m_mdVariableGroup)->read(  (CMDVariableManager *)m_mdVariableManager,m_mdDriverManager,NULL,in);
         file.close();
     }*/

     QFile file(configFile);
     if (!file.open(QIODevice::ReadOnly))
         return;

     QByteArray ba;
     QTextStream stream(&file); 
     stream.setCodec("UTF-8");
     ba = stream.readAll().toUtf8();
     QCryptographicHash hash(QCryptographicHash::Md5);
     hash.addData(ba);
     m_vhash = hash.result().toHex();
     file.close();

     QJsonParseError error;
     QJsonDocument doc = QJsonDocument::fromJson(ba, &error);
     if(error.error != QJsonParseError::NoError)
         return;

     if(m_isRuntimeMode && nullptr != m_pServer) {
         m_pServer->registerType<CMDRTDBVariable>();
         m_pServer->registerEnum<CMDRTDBVariable::MDDataValueType>();

//         m_pServer->registerType<TemperatureSensor>();
     }
	 

     ((CMDVariableGroup*) m_mdVariableGroup)->deserialize(doc.object(),(CMDVariableManager *)m_mdVariableManager,m_mdDriverManager,NULL);
     /*
    QString configFile= m_rtdbPath + "/VariableGroup.xml";

    QFile varFile(configFile);
    if (varFile.open(QFile::ReadOnly | QFile::Text))
    {
        QDomDocument domDocument;
        if (domDocument.setContent(&varFile, true))
        {
            QDomElement element = domDocument.firstChildElement();
            while ( !element.isNull() )
            {
                analyzeConfiguration(element );
                element = element.nextSiblingElement();
            }
        }
        varFile.close();
    }
    */

	// Add ua server description
//    m_server.setApplicationName("MindSCADA-Variable");
//    m_server.setApplicationUri("urn:www.mindteco.com:mind_scada");
//    m_server.setProductName("MindSCADOPCAUaServer");
//    m_server.setProductUri("mindteco.com");
//    m_server.setManufacturerName("Mindteco Inc.");
//    m_server.setSoftwareVersion("1.1.2-master");
//    m_server.setBuildNumber("02021D71");
}

void  CMDRTDBManager::saveVariableGroupConfiguration()
{
    /*
     *   QString  configFile = m_rtdbPath + "/VariableGroup.cfg";
      QFile file(configFile);
       if(file.open(QIODevice::WriteOnly))
       {
           QDataStream out(&file);
           //版本
           out<<int(1);
           ((CMDVariableGroup*) m_mdVariableGroup)->save(out);
           file.close();
       }

       */
    QString  configFile = m_rtdbPath + "/Variable.config";
    CSortJsonObject theOb;
    //head
    CSortJsonObject head;
    head.insertSimpleValue("Version",QString("1"));
    head.insertSimpleValue("Type",QString("VarGroupConfig"));
    theOb.insertObject("Head",head);

    //every child object
    ((CMDVariableGroup*) m_mdVariableGroup)->serialize(theOb);

    //save to file
//    QJsonDocument doc;
//    doc.setObject(theOb);

//    QByteArray ba = doc.toJson();
    QByteArray ba = theOb.convertJsonObject2String(1).toUtf8();
    QFile file(configFile);
    if (!file.open(QIODevice::WriteOnly))
        return;

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << ba;
    file.close();
       /*
         QString  configFile = m_rtdbPath + "/VariableGroup.xml";
    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version=""1.0"" encoding=""UTF-8""");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement("MindSCADARTDBVariableGroupConfiguration");
    QDomAttr virsion = doc.createAttribute("virsion");
    virsion.setValue("V6.0.0.1");
    root.setAttributeNode(virsion);
    doc.appendChild(root);

    QDomElement variableGroups = doc.createElement("VariableGroups");
    root.appendChild(variableGroups);

   ((CMDVariableGroup*) m_mdVariableGroup)->saveConfiguration(doc,variableGroups);


    QFile file(configFile);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
    {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        doc.save(out,4,QDomNode::EncodingFromTextStream);
        file.close();
    }
    */

}

/*!
\section 变量变化
*/
void  CMDRTDBManager::notifyNewChangeVariable(IMDRTDBVariable *changedVar,const VariableChangeType &varChangeType)
{
    if(getSwitchState(MDLogicSwitchType::NotifyVarChange))//允许通知
    {
        if(m_variableChangeNotifier!= NULL)
        {
            CMDChangeVariable *changeVariable =new CMDChangeVariable();
            changeVariable->copyEx((IMDVariable*)changedVar);
            changeVariable->setVariableChangeType(varChangeType);
            m_variableChangeNotifier->onChange(changeVariable);
        }
    }
}
void CMDRTDBManager::setVariableChangeNotifier(IMDVariableChangeNotifier *notifier)
{
    m_variableChangeNotifier = notifier;

}
void CMDRTDBManager::enableSaveConfigBeforeUnLoad()
{
    m_saveVariableConfigBfrQuit = true;
}

/*
bool CMDRTDBManager::start()
{
    startThread();

    return true;
}

void CMDRTDBManager::stop(unsigned long timeout)
{
    stopThread(timeout);

    m_processingQueueManager.cleanQueue();
    m_processedQueueManager.cleanQueue();

    setInProcessing(false);
}

bool CMDRTDBManager::startThread()
{
    if(!m_engineThread)
    {
        m_engineThread = new CMDCommandQueueEngineThread();
    }
    if(m_engineThread)
    {
        m_engineThread->start();
    }

    return true;
}

bool CMDRTDBManager::stopThread(unsigned long timeout)
{
    m_processingQueueManager.setExiting(true);

    if(!m_engineThread)
        return true;

    m_engineThread->quit();
    return (timeout > 0 ? m_engineThread->wait(timeout) :
                                            m_engineThread->wait());
}

bool CMDRTDBManager::isRunning()
{
    return (m_engineThread ? m_engineThread->isReadyToWork() : false);
}
*/



/*!
\\section 动态添加修改设备
*/
//---------------------------------------------------------------------------------------

/*!
 * \brief 动态加载设备参数
 */
void  CMDRTDBManager::dynamicLoadDeviceParamter(const QString &deviceID,
                                                const QVariantList &deviceDriverList,
                                                const QVariantList &deviceVarList)
{
    if(getSwitchState(MDLogicSwitchType::DynamicAddDevice))
    {
        /*!
        动态设备
        */
        if(getSwitchState(MDLogicSwitchType::DynamicAddDevice))
        {
            m_mdDriverManager->dynamicDeleteDevice(deviceID);
            m_mdDriverManager->dynamicAddDevice(deviceID,deviceDriverList);

            /*!
            动态设备变量
            */
            if(getSwitchState(MDLogicSwitchType::DynamicAddVar))
            {
                dynamicDeleteDeviceVariable(deviceID);
                dynamicAddDeviceVariable(deviceID,deviceVarList);
               if(getSwitchState(MDLogicSwitchType::Archive))
                  m_mdVariableArchivesManager->start();

               if(!getSwitchState(MDLogicSwitchType::DynamicRunDevice))
                  return;
                /*!
                新增设备时，不StartAllDevices，只启动新添加的设备
                */

                //m_mdDriverManager->startAllDevices();
                for(int i=0;i<deviceDriverList.count();i++)
                {
                    QVariantMap driverMap = deviceDriverList[i].toMap();

                    //logicDeviceList 有效？
                    if(driverMap.contains("MDDriverLogicDevice") == false)
                        continue;

                        QVariantList logicDeviceList =  driverMap["MDDriverLogicDevice"].toList();
                        for(int j=0; j<logicDeviceList.count(); j++)
                        {
                            QVariantMap logicDeviceMap = logicDeviceList[j].toMap();
                            //DriverDeviceID 有效？
                            if(logicDeviceMap.contains("@DriverDeviceID") == false)
                                continue;

                            QString deviceName=logicDeviceMap["@DriverDeviceID"].toString();
                            deviceName.trimmed();
                            //指定某个通信口运行
                            /*
                            if(logicDeviceMap.contains("MDDevicesCommunicateParameter") == false)
                                continue;
                            QVariantList commParameterList =  logicDeviceMap["MDDevicesCommunicateParameter"].toList();
                            QString commpara = "";
                            for(int i=0;i<commParameterList.count();i++)
                            {
                                QVariantMap commParameterMap = commParameterList[i].toMap();
                                QString parameterName =commParameterMap["@ParameterName"].toString();
                                QString parameterValue =commParameterMap["@ParameterValue"].toString();
                                parameterName.trimmed();
                                parameterValue.trimmed();
                                if(0==QString::compare(parameterName,"CommParameter", Qt::CaseInsensitive))
                                {
                                    commpara=parameterValue;
                                    break;
                                }

                            }
                            //if(commpara.indexOf("COM45") == 0)
                            */
                            m_mdDriverManager->startDevice(deviceName);
                        }//end for each device
                }//end for each driver
            }//end if (getSwitchState(MDLogicSwitchType::DynamicAddVar)
        }//end if(getSwitchState(MDLogicSwitchType::DynamicAddDevice)
    }//end if getSwitchState(MDLogicSwitchType::DynamicAddDevice
}
/*!
 * \brief 动态删除设备变量
 */
void  CMDRTDBManager::dynamicDeleteDeviceVariable(const QString &deviceID)
{
    IMDVariableGroup * variableGroup= m_mdVariableGroup->getChildVariableGroup(deviceID);
    if(variableGroup)
    {
        for(int i=0;i<variableGroup->getVariableCount();i++)
        {
            IMDRTDBVariable*  var = variableGroup->getVariableAt(i);
            if(var->dataSaveRulerParameter().m_saveRuler!= MDRTDataSaveRuler::None)
            {
               m_mdVariableArchivesManager->dynamicDeleteVariable(var);
            }
            if(var->variableType() == MDVariableType::MiddleVariable)
            {
                m_mdMiddleVariableManager->removeVariable(var);
            }
        }
        m_mdVariableGroup->removeChildVariableGroup(deviceID);
    }
}
/*!
 * \brief 动态添加设备变量
 */
void  CMDRTDBManager::dynamicAddDeviceVariable(const QString &deviceID,const QVariantList &deviceVarList)
{
    QString error;
    if(m_mdVariableGroup->addChildVariableGroup(deviceID,"",error))
    {
        IMDVariableGroup * variableGroup= m_mdVariableGroup->getChildVariableGroup(deviceID);
        if(variableGroup)
        {
            for(int i=0;i<deviceVarList.count();i++)
            {
                QVariantMap varMap = deviceVarList[i].toMap();
                QString varName =varMap["@VarName"].toString();
                varName.trimmed();
                if(varName.isNull()||varName.isEmpty())
                {
                    continue;
                }


                MDVariableType varType =(MDVariableType)varMap["@VarType"].toInt();
                MDDataType varDataType =(MDDataType)varMap["@DataType"].toInt();
                int varPrecision =varMap["@Decimal"].toInt();

                QString varDescription =varMap["@Description"].toString();
                QString varAddress =varMap["@AddressExp"].toString();
                MDDataType varOriginalDataType =(MDDataType)varMap["@OriginalDataType"].toInt();

                //工程
                MDVariableDataConversionParameter varDataConversionParameter ;
                varDataConversionParameter.m_enableLinearConversion=false;
                varDataConversionParameter.m_maxValue=0;
                varDataConversionParameter.m_minValue=0;
                varDataConversionParameter.m_originalMaxValue=0;
                varDataConversionParameter.m_originalMinValue=0;
                varDataConversionParameter.m_conversionType = MDVariableDataConversionType::None;
                QJsonParseError jsonError;
                QJsonDocument jsonDocument = QJsonDocument::fromJson(varMap["@Converter"].toString().toUtf8(), &jsonError);
                if (jsonError.error == QJsonParseError::NoError &&jsonDocument.isObject())
                {
                    QVariantMap dataConversionParameteMap = jsonDocument.toVariant().toMap();
                    if (!dataConversionParameteMap.isEmpty())
                    {
                        //varDataConversionParameter.m_enable=dataConversionParameteMap["Enable"].toBool();
                        //varDataConversionParameter.m_enableLinearConversion=dataConversionParameteMap["EnableLinearConversion"].toBool();
                        varDataConversionParameter.m_maxValue=dataConversionParameteMap["MaxValue"].toDouble();
                        varDataConversionParameter.m_minValue=dataConversionParameteMap["MinValue"].toDouble();
                        varDataConversionParameter.m_originalMaxValue=dataConversionParameteMap["OriginalMaxValue"].toDouble();
                        varDataConversionParameter.m_originalMinValue=dataConversionParameteMap["OriginalMinValue"].toDouble();
                        varDataConversionParameter.m_conversionType = MDVariableDataConversionType(dataConversionParameteMap["DataConversionType"].toInt());
                        if(varDataConversionParameter.m_conversionType == MDVariableDataConversionType::ByteOrderChange)
                        {
                            varDataConversionParameter.m_byteOrder = dataConversionParameteMap["ByteOrder"].toString();
                        }
                        varDataConversionParameter.m_enableLinearConversion = dataConversionParameteMap["EnableLinear"].toBool();
                    }

                }


                //驱动信息+基本信息
                IMDRTDBVariable* nsVar = m_mdVariableManager->createVariable();
                if(varType== MDVariableType::IOVariable)
                {
                    QString deviceName ="";
                    int pos = varAddress.indexOf(".");
                    if(pos>0)
                    {
                        deviceName = varAddress.left(pos);
                        varAddress = varAddress.mid(pos+1);
                    }
                    nsVar->setDeviceName(deviceName);
                    nsVar->setAddress(varAddress);
                }
                else if(varType== MDVariableType::MiddleVariable)
                {
                    nsVar->setAddress(varAddress);
                }
                nsVar->setLocalName(varName);

                nsVar->setDataType(varDataType);
                nsVar->setDescription(varDescription);
                nsVar->setPrecision(varPrecision);

                nsVar->setVariableType(varType);
                nsVar->setOriginalDataType(varOriginalDataType);

                nsVar->setDataConversionParameter(varDataConversionParameter);
                //nsVar->setDataSaveRulerParameter(m_variableArchivesDialog->m_dataSaveRulerParameter);
                //nsVar->setWriteProtectParameter(m_writeProtectDialog->m_writeProtectParameter);

                //归档信息
                int varArchive =varMap["@Archive"].toInt();
                if(varArchive!=0)
                {
                    MDRTDataSaveRulerParameter saveRulerParameter = nsVar->dataSaveRulerParameter();
                    saveRulerParameter.m_saveRuler = MDRTDataSaveRuler::DataChanged;
                    nsVar->setDataSaveRulerParameter(saveRulerParameter);
                }

                //初始化信息

                //写保护信息
                ((CMDRTDBVariable*)nsVar)->initialze();
                if(variableGroup->addVariable(nsVar,error))
                {
                    if(nsVar->variableType()==MDVariableType::MiddleVariable)
                    {
                        CMDRTDBVariable* nsRTDBVar = (CMDRTDBVariable*)nsVar;
                        nsRTDBVar->dynamicInitialMiddleVariableExpression();
                    }
                    //m_mdVariableManager->addVariable(nsVar,error);//Y2hw variableGroup->addVariable已经调用
                }
                else
                {
                    delete nsVar;
                }
            }//end for
            for ( int i=0;i<variableGroup->getVariableCount(); i++ )
            {/*
                CMDRTDBVariable* nsVar = (CMDRTDBVariable*)variableGroup->getVariableAt(i);
                nsVar->setValid(true);
                if(nsVar->variableType()==MDVariableType::MiddleVariable)
                {
                    m_mdMiddleVariableManager->addVariable(nsVar,error);
                    nsVar->dynamicInitialMiddleVariableExpression();
                }*/ // 统一移至variableGroup->addVariable下
/*
                else if(nsVar->variableType()==MDVariableType::IOVariable)
                {
                   nsVar->setValid(false);
                   IDevice *device = m_mdDriverManager->getDevice(nsVar->deviceName());
                   if(device!=NULL)
                   {
                       if(device->addVariable(nsVar,error))
                       {
                          nsVar->setValid(true);
                       }
                   }
                }*///Y2hw 移至group->addvariable
                //原注释掉的，Y2hw
                /*
                if(nsVar->dataSaveRulerParameter().m_saveRuler!= MDRTDataSaveRuler::None)
                {
                   m_mdVariableArchivesManager->dynamicAddVariable(nsVar);
                }*/
            }
            if(getSwitchState(MDLogicSwitchType::Middle))
                middleVariableDoExpression();
        }
    }
}
/*!
 * \brief 设备多个变量变化通知
 */
void  CMDRTDBManager::notifyDeviceVariablesChange(const QString& device,const QList<QByteArray>& jsonMsgList)
{
    if(m_variableChangeNotifier!= NULL)
    {
        m_variableChangeNotifier->onDeviceVariablesChange(jsonMsgList);
        if(getSwitchState(MDLogicSwitchType::Log))
        {
            QString strinfo = QString("publish %1 VariablesChange.").arg(device);
            m_logSwitch->LOG_INFO(strinfo);
        }
        /*
        QString strIniFile = QString("%1/MDRTDBNotifyDeviceVariablesChange.ini").arg(QCoreApplication::applicationDirPath());

        QSettings *configIniWrite = new QSettings(strIniFile, QSettings::IniFormat);

        QDateTime timeObj= QDateTime::currentDateTime();

        QString strKey = QString("/NotifyDevice/%1").arg(deviceID);
        configIniWrite->setValue(strKey, timeObj.toString("yyyy-MM-dd hh:mm:ss"));
        delete configIniWrite;
        */
    }
}
void  CMDRTDBManager::notifyDeviceVariablesWithHmset(const QString& device,const QList<QByteArray>& redisCmdList)
{
    if(m_variableChangeNotifier!= NULL)
    {
        m_variableChangeNotifier->onDeviceVariablesBatchNotify(redisCmdList);
        if(getSwitchState(MDLogicSwitchType::Log))
        {
            QString strinfo = QString("hmset %1 VariablesChange.").arg(device);
            m_logSwitch->LOG_INFO(strinfo);
        }
    }
}

void CMDRTDBManager::notifyDeviceVariablesToKVCache(QStringList& varNameList, QList<QVariant>& varValueList)
{
    if(m_variableChangeNotifier!= NULL)
    {
        m_variableChangeNotifier->onVariablesBatchNotify(varNameList, varValueList);
        if(getSwitchState(MDLogicSwitchType::Log))
        {
            QString strinfo = QString("notifyDeviceVariablesToKVCache count=%1, first var=[%2] value=[%3]").arg(varNameList.count()).arg(varNameList[0]).arg(varValueList[0].toString());
            m_logSwitch->LOG_INFO(strinfo);
        }
    }
}
