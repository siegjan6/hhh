/*!

\author dGFuZ3p3

\section 功能 - 项目专家类的实现类，实现运行时的项目管理。

加载/卸载项目；
管理子模块专家。

*/

#ifndef MDPROJECTEXPERTIMP_H
#define MDPROJECTEXPERTIMP_H

#include "MDProjectExpert.h"
#include "MDProjectConfiger.h"
#include "MDVariableExpert.h"
#include "MDEventExpert.h"
#include "MDActionExpert.h"
#include "MDScriptExpert.h"
#include "MDHistoryExpert.h"
#include "MDSecurityPolicyExpert.h"
#include "MDAlarmExpert.h"
#include "MDLogExpert.h"
#include "MDDeviceCloudExpert.h"
#include "MDRecipeExpert.h"
#include "dbqueryexpert.h"

#include "MDProjectModuleContacterImp.h"
#include "MDProjectModuleExpertProxy.h"
#include "MDProjectWrappersCollecter.h"
#include "MDProjectWrappersUaCollecter.h"

#include "CommandEngineForHandleChangedVariable.h"
#include "CommandEngineForExecuteHMIAction.h"
#include "CommandEngineInMainThread.h"
#include "IMDProjectExpertImp.h"
#include "MDStdTimer.h"

#include "MDSysLogExpert.h"
#ifndef LOG_SOURCE
#define LOG_SOURCE "ProjectExpert"
#endif // !LOG_SOURCE

#include <QUaServer>
#include <QUaBaseObject>
#include <QUaOffNormalAlarm>
#ifdef UA_ENABLE_HISTORIZING
#ifndef SQLITE_HISTORIZER
#include "../../Common/MDUaServer/QUaserver/wrapper/quainmemoryhistorizer.h"
#else
#include "../../Common/MDUaServer/QUaserver/wrapper/quasqlitehistorizer.h"
#endif // !SQLITE_HISTORIZER
#endif // UA_ENABLE_HISTORIZING

class CMDAction;
class CMDProjectContacter;
class CMDSysLogger;
class IProjectDirector;
class CMDSwitch;
class CMDLogSwitch;
class CMDSwitchFactory;
class CMDProjectExpertImp
{
    friend class CMDProjectExpert;
    friend class IMDProjectExpertImp;
    friend class CCommandEngineForExecuteHMIAction;
public:
    CMDProjectExpertImp();
    ~CMDProjectExpertImp();

/*!
\section 加载/卸载
*/
public:
    bool load(const QString& projectName, CMDProjectContacter* projcetContacter);
    bool unload();
public:
    bool canLoad();
    bool canUnload();
    MDProjectLoadStatus loadStatus();
    void setLoadStatus(MDProjectLoadStatus status);
protected:
    MDProjectLoadStatus m_loadStatus ;// MDProjectLoadStatus::ready;
public:
    //! 加载结果
    bool resultOnLoaded();
    void setResultOnLoaded(bool result);
protected:
    bool m_resultOnLoaded ;// true;
public:
    bool doSecurityCheckOnLoad();
public:
    void LOG_LOAD(const QString& msg, bool finished = false, int finishCode = 0);
    void LOG_UNLOAD(const QString& msg);
public:
    CMDProjectConfiger m_projectConfiger; //! 项目配置器
protected:
    CMDProjectContacter* m_projectContacter ;// NULL;
public:
    IMDProjectExpertImp m_IProjectExpert; //! 项目专家纯接口类的实现类

/*!
\section 用于简化代码的管理器
*/
 public:
     CMDExpertProxyMgr m_proxyMgr; //! 子模块代理的管理器
     CMDContacterMgr m_contacterMgr; //! 子模块联系人管理器

/*!
\section 管理标准定时器
*/
public:
    CMDStdTimer m_stdTimer;
    void onTimeout_FromStdTimer();

/*!
\section 与 HMI 交互
*/
public:
    void enterFullScreen();
    void exitFullScreen();

/*!
\section 与 安全系统 交互
*/
public:
    void login();
    int login(const QString& name, const QString& password, int checkGroup = 0);
    void loginByAccount(const QString& name);
    void logout();
    void showAccountAndPowerGroupManager();
    QString currentAccount();
    MDSecurityResultCode currentLoginResult();
public:
    CMDSecurityPolicyExpert m_securityExpert;
    bool checkSP_WithUI(CMDActionSPSession* spSession);
    // event
public:
    void onLogin_Script(const QString& name, const int& resultCode, const QString& resultMsg);
    void onLogout_Script(const QString& name);
/*!
\section 处理动作
*/
public:
    CMDActionExpert m_actionExpert;
    CMDProjectModuleExpert* findActionExecuter(CMDAction* action);
    bool executeAction(CMDAction* action);
public:
    CCommandEngineForExecuteHMIAction m_engineForExecuteHMIAction;
    void onExecuteHMIAction(CMDAction* action);

/*!
\section 处理变量变化
*/
public:
    CMDVariableExpert m_variableExpert;
   bool onVariableChanged(IMDChangeVariable* changedVariable);
   void onDeviceVariablesChange(const QList<QByteArray>& jsonMsgList);
   void onDeviceVariablesBatchNotify(const QList<QByteArray>& redisCmdList);
   void onVariablesBatchNotify(const QStringList& varNameList, const QList<QVariant>& varValueList);
public:
   CCommandEngineForHandleChangedVariable m_engineForHandleChangedVariable;
   bool onVariableChanged_HMI_Script(IMDChangeVariable* changedVariable);

/*!
\section 处理设备云消息
*/
public:
   bool handleDeviceData(const QString& deviceID,
                             const QVariantList& driverList,  const QVariantList& varList,
                              const QVariantList& alarmInfoList,
                              const QVariantList& dataArchiveList);
    void handleDeviceCloudStatusChanged(const QString& statusMsg);
   CMDDeviceCloudExpert m_deviceCloudExpert;

/*!
\section 处理配方下载
*/
public:
   bool writeVarGeneral(CMDRecipe* recipe);
   bool writeVarQuick(CMDRecipe* recipe);
   void showRecipeRuntime();
   void hideRecipeRuntime();
   void showRuntime();
   void hideRuntime();
public:
   void onBeginRcpDownload_HMI_Script(const QString &name, int total);
   void onRcpDownloaded_HMI_Script(const QString &name, int finishFlag);
   void onRcpDownloadProcess_HMI_Script(const QString &msg, int total, int completed);

/*!
\section 脚本调试跟踪日志
*/
   //!level 0-info,1-warnning,2-error
   void trace(const QString& msg, int level = 0, const QString& source = "");
/*!
\section 扩展脚本接口
*/
   //!读取文本文件接口
   QString readTextFile(const QString& fileName);
/*!
\section 管理其他子模块专家
*/
public:
    QString PID();
    CMDEventExpert m_eventExpert; //! 事件
    //! 脚本
    CMDScriptExpert m_scriptExpert;
    CMDProjectWrappersCollecter m_wrappersCollecter;
    //! opcua
    CMDProjectWrappersUaCollecter m_wrappersUaCollecter;
public:
    CMDHistoryExpert m_historyExpert; //! 历史
    CMDAlarmExpert m_alarmExpert; //! 报警
    CMDLogExpert m_logExpert; //! 日志
    CMDDBQueryExpert m_dbQueryExpert; //! 自定义查询
public:
    CMDRecipeExpert m_recipeExpert; //! 配方
public:
    void onQueryResult(const QString& queryID, int result);
public:
    CCommandEngineInMainThread m_engineInMainThread;

/*!
\section 功能开关和日志开关
*/
public:
    CMDSwitchFactory* funcSwitchFactory();
    CMDSwitchFactory* logSwitchFactory();
public:
    void createSwitchs(const QString& pid, bool create);
public:
    CMDSwitch* m_sHandleVarChange;
    CMDSwitch* m_sHandleVarEvent;
    CMDSwitch* m_sHandleVarAlarm;
    CMDSwitch* m_sVarChangeToHMI;
    CMDSwitch* m_sVarChangeToScript;
    CMDSwitch* m_sHandleAlarmConfigFromDeviceCloud;
    CMDSwitch* m_sHandleVarConfigFromDeviceCloud;
    CMDSwitch* m_sVarChangeToKV;
public:
    CMDLogSwitch* m_lsProject_Load_UnLoad;
    CMDLogSwitch* m_lsOnVariableChanged;
    CMDLogSwitch* m_scriptTrace;

public:
    QUaServer* uaServer();
private:
    QUaServer m_server;
    //static void* m_pserver;

#ifdef UA_ENABLE_HISTORIZING
#ifndef SQLITE_HISTORIZER
    // set historizer (must live at least as long as the server)
    QUaInMemoryHistorizer historizer;
#else
    QUaSqliteHistorizer historizer;
    QQueue<QUaLog> logOut;
#endif // !SQLITE_HISTORIZER
#endif // !UA_ENABLE_HISTORIZING
};

#endif // MDPROJECTEXPERTIMP_H
