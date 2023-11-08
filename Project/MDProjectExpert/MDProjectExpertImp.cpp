#include "MDProjectExpertImp.h"

#include "../../include/MDActionHelper/MDAction.h"
#include "../../include/MDActionExpert/MDActionExpert.h"
#include "../../include/MDVariableHelper/MDVariable.h"
#include "../../include/MDVariableHelper/MDVariableProperty.h"
#include "../../include/MDEventHelper/MDRawEvent.h"
#include <QDebug>

#include "../../include/MDProjectExpert/MDProjectContacter.h"
#include <QThread>

#include "MDSecurityPolicyConfiger.h"
#include "MDProjectConfiger.h"

#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include <QVariant>
#include "IMDRTDBManager.h"

#include "MDRecipeExpert.h"
#include "MDRecipe.h"
#include "../../include/MDRTDBManager/MDRTDBManagerFactory.h"
//extern QUaServer* m_pUaServer;
//#include "../../include/interface/Common/IUaServer.h"
#include "../../include/MDUaSingleton/CSigleton.h"
#include "MDSwitchFactory/MDFuncSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"
#include <QFile>

#ifdef _WIN32
#include "MMKV/MMKV.h"
#elif defined(__linux__)
#include "MMKV.h"
#endif
#include <QDir>
//extern void* m_pUaServer;


//void* CMDProjectExpertImp::m_pserver;
//static void* m_pUaServer=0;
static CMDSecurityPolicyExpert* p_SecurityPolicyExpert;
CMDProjectExpertImp::CMDProjectExpertImp()
{
    m_loadStatus = MDProjectLoadStatus::ready;
    m_resultOnLoaded = true;
    m_projectContacter = NULL;

    m_proxyMgr.init(this);
    m_contacterMgr.init(this);
    m_IProjectExpert.m_projectExpert = this;

    m_engineForHandleChangedVariable.m_projectExpert = this;
    m_engineForExecuteHMIAction.m_projectExpert = this;
    m_engineInMainThread.m_projectExpert = this;

    m_sHandleVarChange = NULL;
    m_sHandleVarEvent = NULL;
    m_sHandleVarAlarm = NULL;
    m_sVarChangeToHMI = NULL;
    m_sVarChangeToScript = NULL;
    m_sHandleAlarmConfigFromDeviceCloud = NULL;
    m_sHandleVarConfigFromDeviceCloud = NULL;
    m_lsProject_Load_UnLoad = NULL;
    m_lsOnVariableChanged = NULL;
    m_sVarChangeToKV = NULL;
    m_scriptTrace = NULL;
    p_SecurityPolicyExpert = 0;
}

CMDProjectExpertImp::~CMDProjectExpertImp()
{
}

/*!
\section 加载/卸载
*/
bool CMDProjectExpertImp::load(const QString& projectName, CMDProjectContacter* projcetContacter)
{
    if(!canLoad())
        return false;

    QString pPath, nameOnly;
    CMDProjectConfiger::parseProjectName(projectName, pPath, nameOnly);
    createSwitchs(pPath, true);
    // change to init kv in initCache
//    QString prjKVPath =  pPath + "/KV";
//#ifdef _WIN32
//    std::wstring std_rootDir = prjKVPath.toStdWString();
//    MMKV::initializeMMKV(std_rootDir);
//#elif defined(__linux__)
//    std::string std_rootDir = prjKVPath.toStdString();
//    MMKV::initializeMMKV(std_rootDir, MMKVLogInfo);
//#endif

    m_projectContacter = projcetContacter;

    LOG_LOAD("------------------------------------------- 开始加载 ...");
    setLoadStatus(MDProjectLoadStatus::loading);

    //// Add ua server description
    //UaServerGlobal::m_pUaServer = &m_server;
    //m_pUaServer = &m_server;
    //G_UASERVER = QUaServer::Instance();

    m_server.setApplicationName("MindSCADA");
    m_server.setApplicationUri("urn:www.mindteco.com:mind_scada");
    m_server.setProductName("MindSCADOPCAUaServer");
    m_server.setProductUri("mindteco.com");
    m_server.setManufacturerName("Mindteco Inc.");
    m_server.setSoftwareVersion("1.1.2-master");
    m_server.setBuildNumber("02021D71");

    //m_server.registerType<QVector<T>>();
    //typedef QList<QString> StringList;
    qRegisterMetaType<QList<QString>>();

#ifdef UA_ENABLE_HISTORIZING
    // set historizer (must live at least as long as the server)
#ifndef SQLITE_HISTORIZER
    //QUaInMemoryHistorizer historizer;
#else
//	QUaSqliteHistorizer historizer;
//	QQueue<QUaLog> logOut;
//	if (!historizer.setSqliteDbName("history.sqlite", logOut))
//	{
//		for (auto log : logOut)
//		{
//			qDebug() << "[" << log.level << "] :" << log.message;
//		}
//		return -1;
//	}
    historizer.setTransactionTimeout(2 * 1000); // db transaction every 2 secs
#endif // !SQLITE_HISTORIZER
    // set the historizer
    // NOTE : historizer must live at least as long as server
    m_server.setHistorizer(historizer);
#endif // UA_ENABLE_HISTORIZING

    LOG_LOAD("加载配置文件 ...");
    m_projectConfiger.load(projectName);

    LOG_LOAD("加载【历史】 ...");
    m_historyExpert.setParaBeforeLoad(pPath, m_projectConfiger.dataArchives(), &m_contacterMgr.m_historyContacterImp);
    m_historyExpert.load();
    LOG_LOAD("加载【日志】 ...");
    m_logExpert.setParaBeforeLoad(pPath, &m_historyExpert, &m_contacterMgr.m_logContacterImp);
    m_logExpert.load();

    LOG_LOAD("加载【安全】 ...");
    m_securityExpert.setParaBeforeLoad(pPath, m_projectConfiger.securityConfiger(), &m_contacterMgr.m_securityContacterImp);
    m_securityExpert.load();
    if(!doSecurityCheckOnLoad())
    {
        return false;
    }

    LOG_LOAD("启动【处理引擎】 ...");
    m_engineForHandleChangedVariable.startInCurrentThread();
    m_engineForExecuteHMIAction.startInCurrentThread();
    m_engineInMainThread.startInCurrentThread();

    m_deviceCloudExpert.preLoadPara();



//    QUaFolderObject* objsFolder = G_UASERVER->objectsFolder();
//    if (objsFolder) {
//        objsFolder->addFolderObject("Test");
//    }
//    qDebug() << "uaserver1=" << G_UASERVER;

//	LOG_LOAD("加载【报警】 ...");
//    QUaServer* _server = m_projectConfiger.nsRTDBManager()->uaServer();
//	m_alarmExpert.setParaBeforeLoad(pPath, m_projectConfiger.alarmConfiger(), &m_contacterMgr.m_alarmContacterImp,
//		m_deviceCloudExpert.isEnabled() ? 1 : 0, &m_historyExpert);
//    m_alarmExpert.load(&m_server);
//    m_alarmExpert.load();

    LOG_LOAD("加载【动作】 ...");
    m_actionExpert.setParaBeforeLoad(pPath, m_projectConfiger.actionConfiger(), &m_contacterMgr.m_actionContacterImp);
    m_actionExpert.load();
    LOG_LOAD("加载【事件】 ...");
    m_eventExpert.setParaBeforeLoad(pPath, m_projectConfiger.eventConfiger(), &m_contacterMgr.m_eventContacterImp);
    m_eventExpert.load();

    LOG_LOAD("加载【配方】 ...");
    m_recipeExpert.setParaBeforeLoad(pPath, m_projectConfiger.recipeConfiger(), m_projectConfiger.path(), &m_contacterMgr.m_recipeContacterImp);
    m_recipeExpert.load();

    LOG_LOAD("加载【全局脚本】 ...");
    m_wrappersCollecter.create(this);
    m_scriptExpert.setParaBeforeLoad(pPath, m_projectConfiger.path(), &m_contacterMgr.m_scriptContacterImp);
    m_scriptExpert.load();
    m_scriptExpert.runAll();

    LOG_LOAD("load [opcua] ...");
    m_wrappersUaCollecter.create(this);

    //! 设备云模块处理设备信息需要【实时数据库】和【报警】模块准备就绪，因此最后才启动
    //! 设备云模块先于【实时数据库】和【报警】启动可保证第一次推送到值能收到或者进入kv缓存
    m_deviceCloudExpert.setParaBeforeLoad(pPath, &m_contacterMgr.m_deviceCloudContacterImp);
    if(m_deviceCloudExpert.load())
    {
        LOG_LOAD("加载【设备云客户端】 ...");
    }

    //! 关键：最后才启动【实时数据库】，此时处理变量变化的其他模块都已准备就绪
    //! --------------------------------------------------------------------------------------
    LOG_LOAD("加载【实时数据库】 ...");
    m_projectConfiger.nsRTDBManager()->setUaServer(&m_server);
    m_projectConfiger.nsRTDBManager()->setRuntimeMode(true);
    m_projectConfiger.nsRTDBManager()->setVirtualRunMode(m_projectContacter->isVirtualRunMode());
    m_projectConfiger.loadRTDB();

    LOG_LOAD("加载【报警】 ...");
    m_alarmExpert.setParaBeforeLoad(pPath, m_projectConfiger.alarmConfiger(), &m_contacterMgr.m_alarmContacterImp,
        m_deviceCloudExpert.isEnabled() ? 1 : 0, &m_historyExpert);

    LOG_LOAD("加载【变量】，并启动【实时数据库】 ...");
    m_variableExpert.setParaBeforeLoad(pPath, m_projectConfiger.nsRTDBManager(), &m_contacterMgr.m_variableContacterImp);

    m_alarmExpert.load(&m_server);

    CMDProjectPara* prjPara = m_projectConfiger.projectPara();
    m_server.setPort(prjPara->port());
    if(prjPara->caMode() == 1)
        m_server.setAnonymousLoginAllowed(true);
    else {
        m_server.setAnonymousLoginAllowed(false);
        //        enum class ConnectAuthMode {
        //            /// 匿名
        //            ANONYMOUS = 1,
        //            /// 用户密码
        //            USER_PASSWORD = 2,
        //            /// 授权
        //            CERTIFICATE = 4,
        //            /// 授权+私钥
        //            CERTIFICATE_PRIVATEKEY = 8,

        //            /// length
        //            MAXLENGTH
        //        };
        if((prjPara->caMode() & 2) == 2)
        {
            // ConnectAuthMod::USER_PASSWORD
            p_SecurityPolicyExpert = &m_securityExpert;
            m_server.setUserValidationCallback(
                [](const QString &strUserName, const QString &strPassword) {
                bool ret = (p_SecurityPolicyExpert ? p_SecurityPolicyExpert->userValidation(strUserName, strPassword) : false);
                qDebug() << "OPCUA client connect with user and password:" << strUserName << strPassword << ret;
                return ret;
            });
        }

#ifdef UA_ENABLE_ENCRYPTION
        QString prjPath = m_projectConfiger.path();
        QString certPath = "";
        QString pkeyPath = "";
#ifdef Q_OS_LINUX
        certPath = prjPath + "/SecurityPolicy/";
        pkeyPath = prjPath + "/SecurityPolicy/";
#else
        certPath = prjPath + "\\SecurityPolicy\\";
        pkeyPath = prjPath + "\\SecurityPolicy\\";
#endif
        if(!prjPara->certificatePath().isEmpty())
            certPath += prjPara->certificatePath();
        if(!prjPara->privateKeyPath().isEmpty())
            pkeyPath += prjPara->privateKeyPath();

        /// opc ua server auth and security config
        // Load server certificate
        QFile certFile;
        certFile.setFileName(certPath); // mindteco_opcua_cert.der"
        if(certFile.exists()) {
            certFile.open(QIODevice::ReadOnly);
            // passing certificate
            m_server.setCertificate(certFile.readAll());
            certFile.close();
        }
        else{
            LOG_LOAD(QString("opc ua server certificate file %1 not exist!").arg(certPath));
        }

        // Load server private key
        QFile privFile;
        privFile.setFileName(pkeyPath); // mindteco_opcua_key.der"
        if(privFile.exists()) {
            privFile.open(QIODevice::ReadOnly);
            // passing private key
            m_server.setPrivateKey(privFile.readAll());
            privFile.close();
        }
        else{
            LOG_LOAD(QString("opc ua server private key file %1 not exist!").arg(pkeyPath));
        }
#endif
    }

    m_variableExpert.load();
    /*if (_server)
        _server->start();*/
    bool b = m_server.start();
    LOG_LOAD(QString("ua server start  result=%1").arg(b));




    m_stdTimer.m_projectExpert = this;
    m_stdTimer.start(100);

    setLoadStatus(MDProjectLoadStatus::loaded);
    setResultOnLoaded(true);

	
    LOG_LOAD("------------------------------------------- 加载完成！", true, 0);

    m_wrappersCollecter.fireProjectEvent_Loaded();

    return true;
}

void CMDProjectExpertImp::LOG_LOAD(const QString& msg, bool finished, int finishCode)
{
    m_lsProject_Load_UnLoad->LOG_INFO(msg);

    m_projectContacter->onLoadProgress(msg, finished, finishCode);
}

void CMDProjectExpertImp::LOG_UNLOAD(const QString& msg)
{
    m_lsProject_Load_UnLoad->LOG_INFO(msg);
}

bool CMDProjectExpertImp::doSecurityCheckOnLoad()
{
    CMDActionSP* sp = m_projectConfiger.projectPara()->spList()->find(MDActionType::project_Load);
    if(sp && sp->isEnable())
    {
        LOG_LOAD("检查加载安全策略 ...");

        CMDActionSPSession spSession(*sp);
        if(!m_projectContacter->checkSP_WithUI(&spSession))
        {
            setLoadStatus(MDProjectLoadStatus::loaded);
            setResultOnLoaded(false);
            LOG_LOAD("加载失败！没有通过加载安全策略！", true, -1);
            return false;
        }
        LOG_LOAD("加载安全策略通过！");
    }

    if(m_projectConfiger.securityConfiger()->para()->isEnableLoginBeforeRun())
    {
        LOG_LOAD("进行登录验证 ...");
        if(!m_projectContacter->loginByUI())
        {
            setLoadStatus(MDProjectLoadStatus::loaded);
            setResultOnLoaded(false);
            LOG_LOAD("加载失败！没有通过登录验证！", true, -1);
            return false;
        }
        LOG_LOAD("登录验证通过！");
    }

    return true;
}

bool CMDProjectExpertImp::unload()
{
    if(!canUnload())
        return false;

    m_wrappersCollecter.fireProjectEvent_Unloading();

    LOG_UNLOAD("------------------------------------------- 开始卸载 ...");
    setLoadStatus(MDProjectLoadStatus::unloading);

    m_stdTimer.stop();

    //! 关键：最先关闭【实时数据库】，在后续的卸载过程中不会收到任何变量变化消息
    //! --------------------------------------------------------------------------------------
    LOG_UNLOAD("卸载【变量】，并关闭【实时数据库】 ...");
    m_variableExpert.unload();

    LOG_UNLOAD("卸载【事件】 ...");
    m_eventExpert.unload();
    LOG_UNLOAD("卸载【动作】 ...");
    m_actionExpert.unload();

    if(m_deviceCloudExpert.isEnabled())
    {
        LOG_UNLOAD("卸载【设备云客户端】 ...");
        m_deviceCloudExpert.unload();
    }

    LOG_UNLOAD("卸载【历史】 ...");
    m_historyExpert.unload();
    LOG_UNLOAD("卸载【报警】 ...");
    m_alarmExpert.unload();
    LOG_UNLOAD("卸载【配方】 ...");
    m_recipeExpert.unload();

    LOG_UNLOAD("卸载【全局脚本】 ...");
    m_scriptExpert.unload();
    m_wrappersCollecter.destroy();

    LOG_UNLOAD("卸载【处理引擎】 ...");
    m_engineForHandleChangedVariable.stop();
    m_engineForExecuteHMIAction.stop();
    m_engineInMainThread.stop();

    LOG_UNLOAD("卸载【安全】 ...");
    m_securityExpert.unload();

    LOG_UNLOAD("卸载【日志】 ...");
    m_logExpert.unload();

    LOG_UNLOAD("卸载配置文件 ...");
    m_projectConfiger.unLoad();

	//m_server.stop();

    setLoadStatus(MDProjectLoadStatus::unloaded);
    LOG_UNLOAD("------------------------------------------- 卸载完成！");

    createSwitchs("", false);

    CMDMObjFactory::deleteFactories(m_projectConfiger.path());

    return true;
}

MDProjectLoadStatus CMDProjectExpertImp::loadStatus()
{
    return m_loadStatus;
}

void CMDProjectExpertImp::setLoadStatus(MDProjectLoadStatus status)
{
    m_loadStatus = status;
}

bool CMDProjectExpertImp::resultOnLoaded()
{
    return m_resultOnLoaded;
}

void CMDProjectExpertImp::setResultOnLoaded(bool result)
{
    m_resultOnLoaded = result;
}

bool CMDProjectExpertImp::canLoad()
{
    return (MDProjectLoadStatus::ready == m_loadStatus ||
                 MDProjectLoadStatus::unloaded == m_loadStatus);
}

bool CMDProjectExpertImp::canUnload()
{
    return (MDProjectLoadStatus::loaded == m_loadStatus);
}

/*!
\section 处理动作
*/
CMDProjectModuleExpert* CMDProjectExpertImp::findActionExecuter(CMDAction* action)
{
    CMDProjectModuleExpert* moduleExpert = NULL;
    switch (action->configProperty()->actionID().type())
    {
    case MDActionType::variable_WriteValue:
    case MDActionType::variable_ReadValue:
            {
                moduleExpert = &m_proxyMgr.m_variableProxy;
            }
        break;
    case MDActionType::recipe_Upload:
    case MDActionType::recipe_Download:
            {
                moduleExpert = NULL;
            }
        break;
    case MDActionType::window_Open:
    case MDActionType::window_Close:
            {
                moduleExpert = &m_proxyMgr.m_hmiProxy;
            }
        break;
    case MDActionType::historyGroup_Save:
            {
                moduleExpert = &m_proxyMgr.m_historyProxy;
            }
        break;
    default:
        break;
    }
    return moduleExpert;
}

bool CMDProjectExpertImp::executeAction(CMDAction* action)
{
    CMDProjectModuleExpert* actionExecuter = action->actionExecuter();
    if(actionExecuter)
    {
        actionExecuter->executeAction(action);
    }

    return false;
}

void CMDProjectExpertImp::onExecuteHMIAction(CMDAction* action)
{
    if(m_projectContacter)
        m_projectContacter->onExecuteHMIAction(action);
}

/*!
\section 处理变量变化
*/
bool CMDProjectExpertImp::onVariableChanged(IMDChangeVariable* changedVariable)
{
    if(MDProjectLoadStatus::unloading == loadStatus())
        return false;

    if(m_lsOnVariableChanged->isOn())
        m_lsOnVariableChanged->LOG_INFO_IS("onVariableChanged: " + changedVariable->name());

    if(!m_sHandleVarChange->isOn())
    {
        if(m_lsOnVariableChanged->isOn())
            m_lsOnVariableChanged->LOG_INFO_IS("【处理变量变化】功能已关闭！");

        delete changedVariable;
        return false;
    }

    if(m_sHandleVarEvent->isOn())
    {
        if(m_lsOnVariableChanged->isOn())
            m_lsOnVariableChanged->LOG_INFO_IS("通知【事件专家】");

        // 通知事件系统，changedVariable 会被复制一份，
        // rawEvent 由 m_eventExpert 负责删除
        // ------------------------------------------------------------------
        CMDRawVariableEvent* rawEvent = new CMDRawVariableEvent(MDEventType::variable_ValueChanged, changedVariable);
        m_eventExpert.asyncProcessRawEvent(rawEvent);
    }

    if(m_sHandleVarAlarm->isOn())
    {
        if(m_lsOnVariableChanged->isOn())
            m_lsOnVariableChanged->LOG_INFO_IS("通知【报警专家】");

        // 通知报警系统，changedVariable 会被复制一份,
        // rawEventForAlarm 由 m_alarmExpert 负责删除
        // ------------------------------------------------------------------
        CMDRawVariableEvent* rawEventForAlarm = new CMDRawVariableEvent(MDEventType::variable_ValueChanged, changedVariable);
        m_alarmExpert.submitRawAlarmEvent(rawEventForAlarm);
    }

    // 将通知转换到主线程：（1）通知 HMI；（2）触发脚本；
    // 在主线程处理完成后，changedVariable 将被 delete
    // ------------------------------------------------------------------
    if(m_lsOnVariableChanged->isOn())
        m_lsOnVariableChanged->LOG_INFO_IS("将通知转换到主线程【图形；脚本等】");
    m_engineForHandleChangedVariable.submitVariableChangeMsg(changedVariable);

    return true;
}

bool CMDProjectExpertImp::onVariableChanged_HMI_Script(IMDChangeVariable* changedVariable)
{
    if(m_deviceCloudExpert.isEnabled())
    {
        if(m_lsOnVariableChanged->isOn())
            m_lsOnVariableChanged->LOG_INFO_IS("通知【设备云专家】");

        m_deviceCloudExpert.sendVariable(changedVariable);
    }

    if(m_sVarChangeToKV->isOn())
    {
        QStringList varNameList;
        QList<QVariant> varValueList;
        varNameList.append(changedVariable->name());
        varValueList.append(changedVariable->currentValue());
        m_deviceCloudExpert.sendVariableBatchToKVCache(varNameList, varValueList);
    }

     //通知 HMI，changedVariable 会被 HMI 复制一份
    // --------------------------------------------------------------
    if(m_sVarChangeToHMI->isOn())
    {
        if(m_lsOnVariableChanged->isOn())
            m_lsOnVariableChanged->LOG_INFO_IS("通知【图形专家】");

        if(m_projectContacter)
            m_projectContacter->onVariableChanged(changedVariable);
    }

    // 触发脚本，不会复制 changedVariable
    // ----------------------------------------------------------------
    if(m_sVarChangeToScript->isOn())
    {
        if(m_lsOnVariableChanged->isOn())
            m_lsOnVariableChanged->LOG_INFO_IS("通知【脚本引擎】");

        m_wrappersCollecter.fireVariableEvent(changedVariable);
    }

    return true;
}

void CMDProjectExpertImp::onDeviceVariablesChange(const QList<QByteArray>& jsonMsgList)
{
    if(m_deviceCloudExpert.isEnabled())
    {
        m_deviceCloudExpert.sendDeviceVariableChanged(jsonMsgList);
    }
}

void CMDProjectExpertImp::onDeviceVariablesBatchNotify(const QList<QByteArray>& redisCmdList)
{
    if(m_deviceCloudExpert.isEnabled())
    {
        m_deviceCloudExpert.sendVariableBatch(redisCmdList);
    }
}

void CMDProjectExpertImp::onVariablesBatchNotify(const QStringList& varNameList, const QList<QVariant>& varValueList)
{
    if(m_deviceCloudExpert.isEnabledKV())
    {
        m_deviceCloudExpert.sendVariableBatchToKVCache(varNameList, varValueList);
    }
}

bool CMDProjectExpertImp::handleDeviceData(const QString& deviceID,
                                               const QVariantList& driverList,  const QVariantList& varList,
                                                const QVariantList& alarmInfoList,
                                                const QVariantList& dataArchiveList)
{
    // 通知 Runtime 处理，譬如关闭 RTDB 的变量界面
    m_projectContacter->onDeviceDataReceived();

    // 报警条件
    if(m_sHandleAlarmConfigFromDeviceCloud->isOn())
    {
        m_alarmExpert.submitDeviceAlarm(deviceID, alarmInfoList);
    }

    // 实时数据库
    if(m_sHandleVarConfigFromDeviceCloud->isOn())
    {
        IMDRTDBManager* rtdbMgr = m_variableExpert.RTDBManager();
        if(rtdbMgr)
            rtdbMgr->dynamicLoadDeviceParamter(deviceID, driverList, varList);
    }

    return true;
}

void CMDProjectExpertImp::handleDeviceCloudStatusChanged(const QString& statusMsg)
{
    // 通知 Runtime 处理
    m_projectContacter->onDeviceCloudStatusChanged(statusMsg);
}

/*!
\section 处理配方下载
*/
bool CMDProjectExpertImp::writeVarGeneral(CMDRecipe* recipe)
{
    CMDLogSwitch* lsRecipeDownload = CMDLogSwitchFactory::findByGroup(PID(), "RecipeExpert.DownloadProcess");
    lsRecipeDownload->LOG_INFO(recipe->configProperty()->head()->name() + "=" + recipe->configProperty()->items()->toString());

    QStringList itemIDList;
    recipe->configProperty()->items()->enumIds(itemIDList);
    foreach (QString itemID, itemIDList)
    {
        QString value = recipe->configProperty()->items()->itemValue(itemID);
        QString varPath = itemID.replace("/",".");
        m_variableExpert.writeVariableValue(varPath, QVariant(value));
    }
    return true;
}

bool CMDProjectExpertImp::writeVarQuick(CMDRecipe* recipe)
{
    CMDLogSwitch* lsRecipeDownload = CMDLogSwitchFactory::findByGroup(PID(), "RecipeExpert.DownloadProcess");
    lsRecipeDownload->LOG_INFO(recipe->configProperty()->head()->name() + "=" + recipe->configProperty()->items()->toString());

    QList<IMDVariable*> varList;

    QStringList itemIDList;
    recipe->configProperty()->items()->enumIds(itemIDList);
    foreach (QString itemID, itemIDList)
    {
        QString value = recipe->configProperty()->items()->itemValue(itemID);
        IMDVariable *recipeVar = CMDRTDBManagerFactory::createVariable();
        QString varPath = itemID.replace("/",".");
        recipeVar->setName(varPath);
        recipeVar->setCurrentValue(QVariant(value));
        varList.append(recipeVar);
    }
    m_variableExpert.writeVariables(varList);

    return true;
}

void CMDProjectExpertImp::showRecipeRuntime()
{
   m_projectContacter->showRecipeRuntime();
}

void CMDProjectExpertImp::hideRecipeRuntime()
{
   m_projectContacter->hideRecipeRuntime();
}

void CMDProjectExpertImp::showRuntime()
{
    m_projectContacter->showRuntime();
}
void CMDProjectExpertImp::hideRuntime()
{
    m_projectContacter->hideRuntime();
}

void CMDProjectExpertImp::onBeginRcpDownload_HMI_Script(const QString &name, int total)
{
    m_wrappersCollecter.fireRcpEvent_BeginDownload(name, total);
    m_wrappersUaCollecter.fireRcpEvent_BeginDownload(name, total);
}

void CMDProjectExpertImp::onRcpDownloaded_HMI_Script(const QString &name, int finishFlag)
{
    m_wrappersCollecter.fireRcpEvent_EndDownload(name, finishFlag);
    m_wrappersUaCollecter.fireRcpEvent_EndDownload(name, finishFlag);
}

void CMDProjectExpertImp::onRcpDownloadProcess_HMI_Script(const QString &msg, int total, int completed)
{
    m_wrappersCollecter.fireRcpEvent_DownloadProcess(msg, total, completed);
    m_wrappersUaCollecter.fireRcpEvent_DownloadProcess(msg, total, completed);
}

/*!
\section 管理标准定时器
*/
void CMDProjectExpertImp::onTimeout_FromStdTimer()
{
    // 通知事件系统
    // ------------------------------------------------------------------
    CMDRawEvent* rawEvent = new CMDRawEvent();
    rawEvent->setEventType(MDEventType::timer_Cycle);
    m_eventExpert.asyncProcessRawEvent(rawEvent);
}

/*!
\section 与 HMI 交互
*/
void CMDProjectExpertImp::enterFullScreen()
{
    m_projectContacter->enterFullScreen();
}

void CMDProjectExpertImp::exitFullScreen()
{
    m_projectContacter->exitFullScreen(true);
}

/*!
\section 与 安全系统 交互
*/
void CMDProjectExpertImp::login()
{
    m_projectContacter->loginByUI();
}

int CMDProjectExpertImp::login(const QString& name, const QString& password, int checkGroup/* = 0*/)
{
    return (int)m_securityExpert.login(name, password, checkGroup);
}

void CMDProjectExpertImp::loginByAccount(const QString& name)
{
    m_securityExpert.loginByAccount(name);
}

void CMDProjectExpertImp::logout()
{
    m_securityExpert.logout();
}

void CMDProjectExpertImp::showAccountAndPowerGroupManager()
{
    m_projectContacter->showAccountAndPowerGroupManager();
}

QString CMDProjectExpertImp::currentAccount()
{
    return m_securityExpert.currentAccount();
}

MDSecurityResultCode CMDProjectExpertImp::currentLoginResult()
{
    return m_securityExpert.currentLoginResult();
}

bool CMDProjectExpertImp::checkSP_WithUI(CMDActionSPSession* spSession)
{
    if(!m_projectContacter)
        return false;

    return m_projectContacter->checkSP_WithUI(spSession);
}

void CMDProjectExpertImp::onQueryResult(const QString& queryID, int result)
{
    m_wrappersCollecter.fireQueryEvent_Finished(queryID, result);
}

QString CMDProjectExpertImp::PID()
{
    return m_projectConfiger.path();
}

/*!
\section 功能开关和日志开关
*/
void CMDProjectExpertImp::createSwitchs(const QString& pid, bool create)
{
    if(create)
    {
        m_sHandleVarChange = CMDFuncSwitchFactory::createSwitch(pid, "ProjectExpert.HandleVarChange", "变量变化总开关");
        m_sHandleVarEvent = CMDFuncSwitchFactory::createSwitch(pid, "ProjectExpert.HandleVarEvent", "变量事件");
        m_sHandleVarAlarm = CMDFuncSwitchFactory::createSwitch(pid, "ProjectExpert.HandleVarAlarm", "变量报警");
        m_sVarChangeToHMI = CMDFuncSwitchFactory::createSwitch(pid, "ProjectExpert.VarChangeToHMI", "变量变化：通知 HMI");
        // 目前不用此方式通知kv, 以KVCache.SendVar取而代之
        m_sVarChangeToKV = CMDFuncSwitchFactory::createSwitch(pid, "ProjectExpert.VarChangeToKV", "变量变化：通知 KV");
        m_sVarChangeToScript = CMDFuncSwitchFactory::createSwitch(pid, "ProjectExpert.VarChangeToScript", "变量变化：触发脚本");
        m_sHandleAlarmConfigFromDeviceCloud = CMDFuncSwitchFactory::createSwitch(pid, "ProjectExpert.HandleAlarmConfigFromDeviceCloud", "处理来自设备云的报警配置");
        m_sHandleVarConfigFromDeviceCloud = CMDFuncSwitchFactory::createSwitch(pid, "ProjectExpert.HandleVarConfigFromDeviceCloud", "处理来自设备云的变量配置");

        QString subFileDir = "ProjectExpert";
        m_lsProject_Load_UnLoad = CMDLogSwitchFactory::createSwitchWithSubFileDir(pid, "ProjectExpert.Load_and_Unload", "项目加载与卸载", subFileDir);
        m_lsProject_Load_UnLoad->turnOn();
        m_lsOnVariableChanged = CMDLogSwitchFactory::createSwitchWithSubFileDir(pid, "ProjectExpert.OnVariableChanged", "处理变量变化", subFileDir);
        m_scriptTrace = CMDLogSwitchFactory::createSwitchWithSubFileDir(pid, "ProjectExpert.ScriptTrace", "脚本调试跟踪", subFileDir);
    }
    else
    {
        m_sHandleVarChange = NULL;
        m_sHandleVarEvent = NULL;
        m_sHandleVarAlarm = NULL;
        m_sVarChangeToHMI = NULL;
        m_sVarChangeToScript = NULL;
        m_sHandleAlarmConfigFromDeviceCloud = NULL;
        m_sHandleVarConfigFromDeviceCloud = NULL;

        m_lsProject_Load_UnLoad = NULL;
        m_lsOnVariableChanged = NULL;
        m_scriptTrace = NULL;
    }
}

CMDSwitchFactory* CMDProjectExpertImp::funcSwitchFactory()
{
    return (m_sHandleVarChange ? (CMDSwitchFactory*)m_sHandleVarChange->factory() : NULL);
}

CMDSwitchFactory* CMDProjectExpertImp::logSwitchFactory()
{
    return (m_lsProject_Load_UnLoad ? (CMDSwitchFactory*)m_lsProject_Load_UnLoad->factory() : NULL);
}

QUaServer* CMDProjectExpertImp::uaServer()
{
    return &m_server;
}

void CMDProjectExpertImp::trace(const QString& msg, int level/* = 0*/, const QString& source/* = ""*/)
{
    if(m_scriptTrace)
    {
        switch(level)
        {
        case 0:
            m_scriptTrace->LOG_INFO_IS(msg, source);
        break;
        case 1:
            m_scriptTrace->LOG_WARN_IS(msg, source);
        break;
        case 2:
            m_scriptTrace->LOG_ERROR_IS(msg, source);
        break;
        default:
            m_scriptTrace->LOG_INFO_IS(msg, source);
        break;
        }
    }
}

QString CMDProjectExpertImp::readTextFile(const QString& fileName)
{
    QFile f(fileName);
    if(!f.exists())
        return "";
    if(f.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QString ret =  QString(f.readAll());
        f.close();
        return ret;
    }
    f.close();
    return "";
}

void CMDProjectExpertImp::onLogin_Script(const QString& name, const int& resultCode, const QString& resultMsg)
{
    if(resultCode == 0)
    {
        // login succeed
        m_variableExpert.writeVariableValue("SystemVariableGroup.User",name);
    }
    m_wrappersCollecter.fireSecurityEvent_OnLogin(name, resultCode, resultMsg);
}

void CMDProjectExpertImp::onLogout_Script(const QString& name)
{
    // logout succeed
    m_variableExpert.writeVariableValue("SystemVariableGroup.User","");
    m_wrappersCollecter.fireSecurityEvent_OnLogout(name);
}

