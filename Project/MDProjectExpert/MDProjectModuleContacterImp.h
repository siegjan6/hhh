/*!

\author dGFuZ3p3

\section 功能

实现了子模块专家的联系人类，这些类实现了子模块对外部的访问。

*/

#ifndef MDPROJECTMODULECONTACTERIMP_H
#define MDPROJECTMODULECONTACTERIMP_H

#include "../../include/MDVariableExpert/MDVariableContacter.h"
#include "../../include/MDEventExpert/MDEventContacter.h"
#include "../../include/MDActionExpert/MDActionContacter.h"
#include "../../include/MDScriptExpert/MDScriptContacter.h"
#include "../../include/MDHistoryExpert/MDHistoryContacter.h"
#include "../../include/MDAlarmExpert/MDAlarmContacter.h"
#include "../../include/MDLogExpert/MDLogContacter.h"
#include "../../include/MDSecurityPolicyExpert/MDSecurityPolicyContacter.h"
#include "../../include/MDDeviceCloudExpert/MDDeviceCloudContacter.h"
#include "../../include/MDRecipeExpert/MDRecipeContacter.h"

class CMDProjectExpertImp;
class CMDProjectModuleExpert;
/*!
 * \brief 实现 事件专家 的联系人类
 */
class CMDEventContacterImp : public CMDEventContacter
{
public:
    CMDEventContacterImp();
    CMDProjectExpertImp* m_projectExpertImp ;// NULL;
public:
    virtual CMDAction* findAction(const QString& actionName);
    virtual bool executeAction(CMDEvent* sourceEvent, CMDAction* action);
public:
    virtual bool log(CMDLogMessage* msg);
};

/*!
 * \brief 实现 动作专家 的联系人类
 */
class CMDActionContacterImp : public CMDActionContacter
{
public:
    CMDActionContacterImp();
    CMDProjectExpertImp* m_projectExpertImp ;// NULL;
public:
    virtual CMDProjectModuleExpert* findActionExecuter(CMDAction* action);
    virtual bool executeAction(CMDAction* action);
    virtual bool variable(const QString& varName, IMDVariable* var);
    virtual QVariant variableValue(const QString& varName);
public:
    virtual bool checkSP_WithUI(CMDActionSPSession* spSession);
public:
    virtual bool log(CMDLogMessage* msg);
};

/*!
 * \brief 实现 变量专家 的联系人类
 */
class CMDVariableContacterImp : public CMDVariableContacter
{
public:
    CMDVariableContacterImp();
    CMDProjectExpertImp* m_projectExpertImp ;// NULL;
public:
    virtual bool onVariableChanged(IMDChangeVariable* changedVariable);
public:
    virtual bool checkSP_WithUI(CMDActionSPSession* spSession);
public:
    virtual void  onDeviceVariablesChange(const QList<QByteArray>& jsonMsgList);
    virtual void  onDeviceVariablesBatchNotify(const QList<QByteArray>& redisCmdList);
    virtual void  onVariablesBatchNotify(const QStringList& varNameList, const QList<QVariant>& varValueList);
};

/*!
 * \brief 实现 脚本专家 的联系人类
 */
class CMDObjectWrapper;
class CMDScriptContacterImp : public CMDScriptContacter
{
public:
    CMDScriptContacterImp();
    CMDProjectExpertImp* m_projectExpertImp ;// NULL;
public:
    virtual bool objectWrappers(QList<CMDObjectWrapper*>& wrapperList);
};

/*!
 * \brief 实现 历史专家 的联系人类
 */
class CMDHistoryContacterImp : public CMDHistoryContacter
{
public:
    CMDHistoryContacterImp();
    CMDProjectExpertImp* m_projectExpertImp ;// NULL;
public:
    virtual bool variable(const QString& varName, IMDVariable* var);
    virtual void onQueryResult(const QString& queryID, int result);
};

/*!
 * \brief 实现 报警专家 的联系人类
 */
class CMDAlarmContacterImp : public CMDAlarmContacter
{
public:
    CMDAlarmContacterImp();
    CMDProjectExpertImp* m_projectExpertImp ;// NULL;
public:
    virtual bool variable(const QString& varName, IMDVariable* var);
    IMDRTDBVariable* getVariable(const QString& varName);
    //! 获取变量数值，若返回 isNull，则该数值不可使用
    virtual QVariant variableValue(const QString& varName);
    virtual QString currentAccount();
public:
    virtual bool checkSP_WithUI(CMDActionSPSession* spSession);
    virtual bool handleAlarm(short type, const QString& name, const QString& msg, const QDateTime& startTime, const QDateTime& endTime);
};

/*!
 * \brief 实现 日志专家 的联系人类
 */
class CMDLogContacterImp : public CMDLogContacter
{
public:
    CMDLogContacterImp();
    CMDProjectExpertImp* m_projectExpertImp ;// NULL;
public:
    virtual bool variable(const QString& varName, IMDVariable* var);
    //! 获取变量数值，若返回 isNull，则该数值不可使用
    virtual QVariant variableValue(const QString& varName);
    virtual QString currentAccount();
};

/*!
 * \brief 实现 安全策略专家 的联系人类
 */
class CMDSecurityPolicyContacterImp : public CMDSecurityPolicyContacter
{
public:
    CMDSecurityPolicyContacterImp();
    CMDProjectExpertImp* m_projectExpertImp ;// NULL;
public:
    virtual void onLogin(const QString& name, const int& resultCode, const QString& resultMsg);
    virtual void onLogout(const QString& name);
public:
    virtual bool log(CMDLogMessage* msg);
};

/*!
 * \brief 实现 设备云专家 的联系人类
 */
class CMDDeviceCloudContacterImp : public CMDDeviceCloudContacter
{
public:
    CMDDeviceCloudContacterImp();
    CMDProjectExpertImp* m_projectExpertImp ;// NULL;
public:
    virtual bool handleDeviceData(const QString& deviceID,
                                        const QVariantList& driverList,  const QVariantList& varList,
                                         const QVariantList& alarmInfoList,
                                         const QVariantList& dataArchiveList);
public:
    virtual void handleStatusChanged(const QString& statusMsg);
};

/*!
 * \brief 实现 配方专家 的联系人类
 */
class IMDVariable;
class CMDRecipeContacterImp : public CMDRecipeContacter
{
public:
    CMDRecipeContacterImp();
    CMDProjectExpertImp* m_projectExpertImp ;// NULL;
public:
    virtual bool writeVarGeneral(CMDRecipe* recipe);
    virtual bool writeVarQuick(CMDRecipe* recipe);
    virtual QVariant varValue(const QString &varName);
    virtual bool isQuickVar(const QString &varName);
    virtual int getVariableQuality(const QString& varName);
public:
    virtual void onDownloadTaskAdded(const QString &name, int showProcessDialog=1);
    virtual void onBeginDownloading(const QString &name, int total);
    virtual void onEndDownloading(const QString &name, DownloadFinishFlag finishFlag);
    virtual void onDownloadProcess(const QString &msg, int total, int completed);
};

/*!
 * \brief 联系人类的管理类，用于简化 CMDProjectExpertImp 的实现代码
 */
class CMDContacterMgr
{
public:
    CMDContacterMgr();
    void init(CMDProjectExpertImp* projectExpertImp);
public:
    CMDEventContacterImp m_eventContacterImp;
    CMDActionContacterImp m_actionContacterImp;
    CMDVariableContacterImp m_variableContacterImp;
    CMDScriptContacterImp m_scriptContacterImp;
    CMDHistoryContacterImp m_historyContacterImp;
    CMDAlarmContacterImp m_alarmContacterImp;
    CMDLogContacterImp m_logContacterImp;
    CMDSecurityPolicyContacterImp m_securityContacterImp;
    CMDDeviceCloudContacterImp m_deviceCloudContacterImp;
    CMDRecipeContacterImp m_recipeContacterImp;
};

#endif // MDPROJECTMODULECONTACTERIMP_H
