#include "MDProjectModuleContacterImp.h"
#include "MDProjectExpertImp.h"

/*!
 * \brief 实现 事件专家 的联系人类
 */
CMDEventContacterImp::CMDEventContacterImp()
{
}

CMDAction* CMDEventContacterImp::findAction(const QString& actionName)
{
   return (m_projectExpertImp ?
               m_projectExpertImp->m_actionExpert.findAction(actionName) : NULL);
}

bool CMDEventContacterImp::executeAction(CMDEvent* sourceEvent, CMDAction* action)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->m_actionExpert.asyncProcessAction(action) : false);
}

bool CMDEventContacterImp::log(CMDLogMessage* msg)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->m_logExpert.log(*msg) : false);
}

/*!
 * \brief 实现 动作专家 的联系人类
 */
CMDActionContacterImp::CMDActionContacterImp()
{
}

CMDProjectModuleExpert* CMDActionContacterImp::findActionExecuter(CMDAction* action)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->findActionExecuter(action) : NULL);
}

bool CMDActionContacterImp::executeAction(CMDAction* action)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->executeAction(action) : false);
}

bool CMDActionContacterImp::checkSP_WithUI(CMDActionSPSession* spSession)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->checkSP_WithUI(spSession) : false);
}

bool CMDActionContacterImp::log(CMDLogMessage* msg)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->m_logExpert.log(*msg) : false);
}

bool CMDActionContacterImp::variable(const QString& varName, IMDVariable* var)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->m_variableExpert.readVariableValue(var) : false);
}

QVariant CMDActionContacterImp::variableValue(const QString& varName)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->m_variableExpert.readVariableValue(varName) : false);
}

/*!
 * \brief 实现 变量专家 的联系人类
 */
CMDVariableContacterImp::CMDVariableContacterImp()
{
}

bool CMDVariableContacterImp::onVariableChanged(IMDChangeVariable* changedVariable)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->onVariableChanged(changedVariable) : false);
}

bool CMDVariableContacterImp::checkSP_WithUI(CMDActionSPSession* spSession)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->checkSP_WithUI(spSession) : false);
}

void CMDVariableContacterImp::onDeviceVariablesChange(const QList<QByteArray>& jsonMsgList)
{
    if(m_projectExpertImp)
        m_projectExpertImp->onDeviceVariablesChange(jsonMsgList);
}

void CMDVariableContacterImp::onDeviceVariablesBatchNotify(const QList<QByteArray>& redisCmdList)
{
    if(m_projectExpertImp)
        m_projectExpertImp->onDeviceVariablesBatchNotify(redisCmdList);
}

void CMDVariableContacterImp::onVariablesBatchNotify(const QStringList& varNameList, const QList<QVariant>& varValueList)
{
    if(m_projectExpertImp)
        m_projectExpertImp->onVariablesBatchNotify(varNameList, varValueList);
}

/*!
 * \brief 实现 脚本专家 的联系人类
 */
CMDScriptContacterImp::CMDScriptContacterImp()
{
}

bool CMDScriptContacterImp::objectWrappers(QList<CMDObjectWrapper*>& wrapperList)
{
    if(m_projectExpertImp)
        m_projectExpertImp->m_wrappersCollecter.m_wrappersMgr.Wrappers(wrapperList);
    return true;
}

/*!
 * \brief 实现 历史专家 的联系人类
 */
CMDHistoryContacterImp::CMDHistoryContacterImp()
{
}

bool CMDHistoryContacterImp::variable(const QString& varName, IMDVariable* var)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->m_variableExpert.readVariableValue(var) : false);
}

void CMDHistoryContacterImp::onQueryResult(const QString& queryID, int result)
{
    if(m_projectExpertImp)
        m_projectExpertImp->m_engineInMainThread.submitQueryResult(queryID, result);
}

/*!
 * \brief 实现 报警专家 的联系人类
 */
CMDAlarmContacterImp::CMDAlarmContacterImp()
{
}

bool CMDAlarmContacterImp::variable(const QString& varName, IMDVariable* var)
{
    if(var)
    {
        var->setName(varName);
    }
    return (m_projectExpertImp ?
                m_projectExpertImp->m_variableExpert.readVariableValue(var) : false);
}

IMDRTDBVariable* CMDAlarmContacterImp::getVariable(const QString& varName)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->m_variableExpert.getVariable(varName) : NULL);
}

QVariant CMDAlarmContacterImp::variableValue(const QString& varName)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->m_variableExpert.readVariableValue(varName) : false);
}

QString CMDAlarmContacterImp::currentAccount()
{
    return (m_projectExpertImp ?
                m_projectExpertImp->m_securityExpert.currentAccount() : "");
}

bool CMDAlarmContacterImp::checkSP_WithUI(CMDActionSPSession* spSession)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->checkSP_WithUI(spSession) : false);
}

bool CMDAlarmContacterImp::handleAlarm(short type, const QString& name, const QString& msg, const QDateTime& startTime, const QDateTime& endTime)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->m_deviceCloudExpert.sendAlarm(type, name, msg, startTime, endTime) : false);
}

/*!
 * \brief 实现 日志专家 的联系人类
 */
CMDLogContacterImp::CMDLogContacterImp()
{
}

bool CMDLogContacterImp::variable(const QString& varName, IMDVariable* var)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->m_variableExpert.readVariableValue(var) : false);
}

QVariant CMDLogContacterImp::variableValue(const QString& varName)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->m_variableExpert.readVariableValue(varName) : false);
}

QString CMDLogContacterImp::currentAccount()
{
    return (m_projectExpertImp ?
                m_projectExpertImp->m_securityExpert.currentAccount() : "");
}

/*!
 * \brief 实现 安全策略专家 的联系人类
 */
CMDSecurityPolicyContacterImp::CMDSecurityPolicyContacterImp()
{
}

bool CMDSecurityPolicyContacterImp::log(CMDLogMessage* msg)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->m_logExpert.log(*msg) : false);
}

void CMDSecurityPolicyContacterImp::onLogin(const QString& name, const int& resultCode, const QString& resultMsg)
{
    if(m_projectExpertImp)
        m_projectExpertImp->onLogin_Script(name, resultCode, resultMsg);
}

void CMDSecurityPolicyContacterImp::onLogout(const QString& name)
{
    if(m_projectExpertImp)
        m_projectExpertImp->onLogout_Script(name);
}

/*!
 * \brief 实现 设备云专家 的联系人类
 */
CMDDeviceCloudContacterImp::CMDDeviceCloudContacterImp()
{
}

bool CMDDeviceCloudContacterImp::handleDeviceData(const QString& deviceID,
                                                  const QVariantList& driverList,  const QVariantList& varList,
                                                   const QVariantList& alarmInfoList,
                                                   const QVariantList& dataArchiveList)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->handleDeviceData(deviceID,
                                                         driverList,  varList,
                                                         alarmInfoList,  dataArchiveList) : false);
}

void CMDDeviceCloudContacterImp::handleStatusChanged(const QString& statusMsg)
{
    if(m_projectExpertImp)
        m_projectExpertImp->handleDeviceCloudStatusChanged(statusMsg);
}

/*!
 * \brief 实现 配方专家 的联系人类
 */
CMDRecipeContacterImp::CMDRecipeContacterImp()
{
}

bool CMDRecipeContacterImp::writeVarGeneral(CMDRecipe* recipe)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->writeVarGeneral(recipe) : false);
}

bool CMDRecipeContacterImp::writeVarQuick(CMDRecipe* recipe)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->writeVarQuick(recipe) : false);
}

QVariant CMDRecipeContacterImp::varValue(const QString &varName)
{
    return (m_projectExpertImp ?
                m_projectExpertImp->m_variableExpert.readVariableValue(varName) : QVariant());
}

int CMDRecipeContacterImp::getVariableQuality(const QString& varName)
{
    if(m_projectExpertImp)
    {
        IMDVariable* v = (IMDVariable*)m_projectExpertImp->m_variableExpert.getVariable(varName);
        if(v)
            return (int)(v->currentQuality());
    }
    return -1;
}

bool CMDRecipeContacterImp::isQuickVar(const QString &varName)
{
    return false;
}

void CMDRecipeContacterImp::onDownloadTaskAdded(const QString &name, int showProcessDialog/*=1*/)
{

}

void CMDRecipeContacterImp::onBeginDownloading(const QString &name, int total)
{
    if(m_projectExpertImp)
        m_projectExpertImp->onBeginRcpDownload_HMI_Script(name, total);
}

void CMDRecipeContacterImp::onEndDownloading(const QString &name, DownloadFinishFlag finishFlag)
{
    if(m_projectExpertImp)
        m_projectExpertImp->onRcpDownloaded_HMI_Script(name, (int)finishFlag);
}

void CMDRecipeContacterImp::onDownloadProcess(const QString &msg, int total, int completed)
{
    if(m_projectExpertImp)
        m_projectExpertImp->onRcpDownloadProcess_HMI_Script(msg, total, completed);
}

/*!
 * \brief 联系人类的管理类，用于简化 CMDProjectExpertImp 的实现代码
 */
CMDContacterMgr::CMDContacterMgr()
{

}

void CMDContacterMgr::init(CMDProjectExpertImp* projectExpertImp)
{
    m_variableContacterImp.m_projectExpertImp = projectExpertImp;
    m_eventContacterImp.m_projectExpertImp = projectExpertImp;
    m_actionContacterImp.m_projectExpertImp = projectExpertImp;
    m_scriptContacterImp.m_projectExpertImp = projectExpertImp;
    m_historyContacterImp.m_projectExpertImp = projectExpertImp;
    m_alarmContacterImp.m_projectExpertImp = projectExpertImp;
    m_logContacterImp.m_projectExpertImp = projectExpertImp;
    m_securityContacterImp.m_projectExpertImp = projectExpertImp;
    m_deviceCloudContacterImp.m_projectExpertImp = projectExpertImp;
    m_recipeContacterImp.m_projectExpertImp = projectExpertImp;
}
