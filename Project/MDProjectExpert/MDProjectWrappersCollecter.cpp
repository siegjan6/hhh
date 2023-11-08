#include "MDProjectWrappersCollecter.h"
#include "MDProjectWrappers.h"
#include "MDProjectExpertImp.h"

#include "../../include/MDScriptEngine/MDScriptEngine.h"
#include "../../include/MDRTDBInterfaces/IMDRTDBManager.h"

CMDProjectWrappersCollecter::CMDProjectWrappersCollecter()
{
    m_projectExpertImp = NULL;
}

/*!
\section 初始化与释放；wrapper 管理
*/
void CMDProjectWrappersCollecter::create(CMDProjectExpertImp* projectExpertImp)
{
    m_projectExpertImp = projectExpertImp;

    // 项目
    CMDProjectExpertWrapper* projectWrapper = new CMDProjectExpertWrapper(projectExpertImp, "g_Project");
    m_wrappersMgr.addWrapper(projectWrapper);

    // 变量专家
    CMDVariableExpertWrapper* veWrapper = new CMDVariableExpertWrapper(&projectExpertImp->m_variableExpert, "g_Variable");
    m_wrappersMgr.addWrapper(veWrapper);

    // 变量对象
//    IMDRTDBManager* rtdbMgr = projectExpertImp->m_variableExpert.RTDBManager();
//    IMDVariableManager* varMgr = rtdbMgr->variableManager();
//    int count = varMgr->getVariableCount();
//    for (int i = 0; i < count; i++)
//    {
//        IMDRTDBVariable* var = varMgr->getVariableAt(i);
////        if(var->isEnableScript())
//        {
//            QString sName = var->name().replace('.', '_');
//            CMDVariableWrapper* wrapper = new CMDVariableWrapper(var, sName);
//            m_wrappersMgr.addWrapper(wrapper);
//        }
//    }

    // 数据归档
    CMDDataArchiveExpertWrapper* daeWrapper = new CMDDataArchiveExpertWrapper(&projectExpertImp->m_historyExpert, "g_DataSource");
    m_wrappersMgr.addWrapper(daeWrapper);

    // 数据查询
    CMDDataQueryExpertWrapper* dqeWrapper = new CMDDataQueryExpertWrapper(projectExpertImp->m_historyExpert.qTaskMgr(), "g_DataQuery");
    m_wrappersMgr.addWrapper(dqeWrapper);

    // 自定义数据查询
    CMDDBQueryExpertWrapper* dbqWrapper = new CMDDBQueryExpertWrapper(&projectExpertImp->m_dbQueryExpert, "g_DbQuery");
    m_wrappersMgr.addWrapper(dbqWrapper);

    // 窗体
    CMDHMIExpertWrapper* hmiWrapper = new CMDHMIExpertWrapper(&projectExpertImp->m_proxyMgr.m_hmiProxy, "g_Window");
    m_wrappersMgr.addWrapper(hmiWrapper);

    // 安全
    CMDSecurityExpertWrapper* securityWrapper = new CMDSecurityExpertWrapper(projectExpertImp, "g_Authorize");
    m_wrappersMgr.addWrapper(securityWrapper);

    // 配方
    CMDRecipeExpertWrapper* recipeWrapper = new CMDRecipeExpertWrapper(&projectExpertImp->m_recipeExpert, "g_Recipe");
    m_wrappersMgr.addWrapper(recipeWrapper);
}

void CMDProjectWrappersCollecter::destroy()
{
    m_wrappersMgr.releaseWrappers();
}

/*!
\section 事件触发
*/
void CMDProjectWrappersCollecter::fireVariableEvent(IMDChangeVariable* changedVariable)
{
    //if(changedVariable->isEnableScript())
    {
        QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

        QList<QVariant> paras;
        paras.append(QVariant(changedVariable->name()));
        paras.append(changedVariable->currentValue());
        CMDScriptEngine::fireEventInPool("g_Variable", "onValueChanged", paras, PID);

        QList<QVariant> paras1;
        paras1.append(changedVariable->currentValue());
        QString vName = changedVariable->name();
        CMDScriptEngine::fireEventInPool(vName, "onValueChanged", paras1, PID);
    }
}

void CMDProjectWrappersCollecter::fireProjectEvent_Loaded()
{
    QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

    QList<QVariant> paras;
    CMDScriptEngine::fireEventInPool("g_Project", "onLoaded", paras, PID);
}

void CMDProjectWrappersCollecter::fireProjectEvent_Unloading()
{
    QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

    QList<QVariant> paras;
    CMDScriptEngine::fireEventInPool("g_Project", "onUnloading", paras, PID);
}

void CMDProjectWrappersCollecter::fireQueryEvent_Finished(const QString& queryID, int result)
{
    QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

    QList<QVariant> paras;
    paras.append(QVariant(queryID));
    paras.append(QVariant(result));
    CMDScriptEngine::fireEventInPool("g_DataQuery", "onQueryResult", paras, PID);
}

void CMDProjectWrappersCollecter::fireRcpEvent_BeginDownload(const QString &name, int total)
{
    //if(xxx(RcplogFlagMan)->isEnableScript())
    {
        QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

        QList<QVariant> paras;
        paras.append(QVariant(name));
        paras.append(total);
        CMDScriptEngine::fireEventInPool("g_Recipe", "onBeginDownload", paras, PID);
    }
}

void CMDProjectWrappersCollecter::fireRcpEvent_EndDownload(const QString &name, int finishFlag)
{
    //if(xxx(RcplogFlagMan)->isEnableScript())
    {
        QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

        QList<QVariant> paras;
        paras.append(QVariant(name));
        paras.append(finishFlag);
        CMDScriptEngine::fireEventInPool("g_Recipe", "onEndDownload", paras, PID);
    }
}

void CMDProjectWrappersCollecter::fireRcpEvent_DownloadProcess(const QString &msg, int total, int completed)
{
    //if(xxx(RcplogFlagMan)->isEnableScript())
    {
        QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

        QList<QVariant> paras;
        paras.append(QVariant(msg));
        paras.append(total);
        paras.append(QVariant(completed));
        CMDScriptEngine::fireEventInPool("g_Recipe", "onDownloadProcess", paras, PID);
    }
}

void CMDProjectWrappersCollecter::fireSecurityEvent_OnLogin(const QString& name, const int& resultCode, const QString& resultMsg)
{
    //if(xxx(SecuritylogFlagMan)->isEnableScript())
    {
        QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

        QList<QVariant> paras;
        paras.append(QVariant(name));
        paras.append(resultCode);
        paras.append(QVariant(resultMsg));
        CMDScriptEngine::fireEventInPool("g_Authorize", "onLogin", paras, PID);
    }
}

void CMDProjectWrappersCollecter::fireSecurityEvent_OnLogout(const QString& name)
{
    //if(xxx(SecuritylogFlagMan)->isEnableScript())
    {
        QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

        QList<QVariant> paras;
        paras.append(QVariant(name));
        CMDScriptEngine::fireEventInPool("g_Authorize", "onLogout", paras, PID);
    }
}


