#include "MDProjectWrappersUaCollecter.h"
#include "MDProjectUaWrappers.h"
#include "MDProjectExpertImp.h"
#include "MDEvent.h"
#include "MDRecipeEvent.h"

#include "../../include/MDScriptEngine/MDScriptEngine.h"
#include "../../include/MDRTDBInterfaces/IMDRTDBManager.h"

CMDProjectWrappersUaCollecter::CMDProjectWrappersUaCollecter()
{
    m_projectExpertImp = NULL;
}

/*!
\section 初始化与释放；wrapper 管理
*/
void CMDProjectWrappersUaCollecter::create(CMDProjectExpertImp* projectExpertImp)
{
    m_projectExpertImp = projectExpertImp;
    QUaServer* p_server = projectExpertImp->uaServer();
    QUaFolderObject * objsFolder = p_server->objectsFolder();
    QUaFolderObject * modulesFolder = objsFolder->addFolderObject("Modules");

    // 项目
    p_server->registerType<CMDProjectExpertUaWrapper>();
    CMDProjectExpertUaWrapper* g_Project = modulesFolder->addChild<CMDProjectExpertUaWrapper>("g_Project");
    if(g_Project)
    {
        g_Project->setProp(&projectExpertImp);
    }

    // 变量专家
    p_server->registerType<CMDVariableExpertUaWrapper>();
    CMDVariableExpertUaWrapper* g_Variable = modulesFolder->addChild<CMDVariableExpertUaWrapper>("g_Variable");
    if(g_Variable)
    {
        g_Variable->setProp(&projectExpertImp->m_variableExpert);
    }

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

//    // 数据归档
//    CMDDataArchiveExpertWrapper* daeWrapper = new CMDDataArchiveExpertWrapper(&projectExpertImp->m_historyExpert, "g_DataSource");
//    m_wrappersMgr.addWrapper(daeWrapper);

//    // 数据查询
//    CMDDataQueryExpertWrapper* dqeWrapper = new CMDDataQueryExpertWrapper(projectExpertImp->m_historyExpert.qTaskMgr(), "g_Database");
//    m_wrappersMgr.addWrapper(dqeWrapper);

    // 窗体
    p_server->registerType<CMDHMIExpertUaWrapper>();
    CMDHMIExpertUaWrapper* g_Window = modulesFolder->addChild<CMDHMIExpertUaWrapper>("g_Window");
    if(g_Window)
    {
        g_Window->setProp(&projectExpertImp->m_proxyMgr.m_hmiProxy);
    }

    // 安全
    p_server->registerType<CMDSecurityExpertUaWrapper>();
    CMDSecurityExpertUaWrapper* g_Authorize = modulesFolder->addChild<CMDSecurityExpertUaWrapper>("g_Authorize");
    if(g_Authorize)
    {
        g_Authorize->setProp(&projectExpertImp->m_securityExpert);
    }

    // 报警
    p_server->registerType<CMDAlarmExpertUaWrapper>();
    CMDAlarmExpertUaWrapper* g_Alarm = modulesFolder->addChild<CMDAlarmExpertUaWrapper>("g_Alarm");
    if(g_Alarm)
    {
        g_Alarm->setProp(&projectExpertImp->m_alarmExpert);
    }

    // 配方
    p_server->registerType<CMDRecipeExpertUaWrapper>();
    CMDRecipeExpertUaWrapper* g_Recipe = modulesFolder->addChild<CMDRecipeExpertUaWrapper>("g_Recipe");
    if(g_Recipe)
    {
        g_Recipe->setProp(&projectExpertImp->m_recipeExpert);
        // Enable object for events
        g_Recipe->setSubscribeToEvents(true);
        // Create event with object as originator
        p_server->registerType<MDRecipeEvent>(QUaNodeId(0,"MDRecipeEventType"));
        g_Recipe_ua_event = g_Recipe->createEvent<MDRecipeEvent>();
        g_Recipe_ua_event->setDisplayName("MDServerRecipeEvent");
    }
}

void CMDProjectWrappersUaCollecter::destroy()
{
    //m_wrappersMgr.releaseWrappers();
    // todo: 释放UA各个g_模块
}

///*!
//\section 事件触发
//*/
//void CMDProjectWrappersUaCollecter::fireVariableEvent(IMDChangeVariable* changedVariable)
//{
//    //if(changedVariable->isEnableScript())
//    {
//        QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

//        QList<QVariant> paras;
//        paras.append(QVariant(changedVariable->name()));
//        paras.append(changedVariable->currentValue());
//        CMDScriptEngine::fireEventInPool("g_Variable", "onValueChanged", paras, PID);

//        QList<QVariant> paras1;
//        paras1.append(changedVariable->currentValue());
//        QString vName = changedVariable->name();
//        CMDScriptEngine::fireEventInPool(vName, "onValueChanged", paras1, PID);
//    }
//}

//void CMDProjectWrappersUaCollecter::fireProjectEvent_Loaded()
//{
//    QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

//    QList<QVariant> paras;
//    CMDScriptEngine::fireEventInPool("g_Project", "onLoaded", paras, PID);
//}

//void CMDProjectWrappersUaCollecter::fireProjectEvent_Unloading()
//{
//    QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

//    QList<QVariant> paras;
//    CMDScriptEngine::fireEventInPool("g_Project", "onUnloading", paras, PID);
//}

//void CMDProjectWrappersUaCollecter::fireQueryEvent_Finished(const QString& queryID, int result)
//{
//    QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

//    QList<QVariant> paras;
//    paras.append(QVariant(queryID));
//    paras.append(QVariant(result));
//    CMDScriptEngine::fireEventInPool("g_Database", "onQueryResult", paras, PID);
//}

void CMDProjectWrappersUaCollecter::fireRcpEvent_BeginDownload(const QString &name, int total)
{
    //if(xxx(RcplogFlagMan)->isEnableScript())
    {
        QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

        QList<QVariant> paras;
        paras.append(QVariant(name));
        paras.append(total);
        //CMDScriptEngine::fireEventInPool("g_Recipe", "onBeginDownload", paras, PID);
        if (!g_Recipe_ua_event)
        {
            return;
        }
        // Set server_event information
        auto time = QDateTime::currentDateTime();
        g_Recipe_ua_event->setSourceName("Server");
        g_Recipe_ua_event->setMessage(QObject::tr("recipe [%1] begin download event occured in the server").arg(name));
        g_Recipe_ua_event->setTime(time.toUTC());
        g_Recipe_ua_event->setReceiveTime(time.toUTC());
        g_Recipe_ua_event->setSeverity(100);
        g_Recipe_ua_event->setStatus(0);
        g_Recipe_ua_event->setMeta(paras);
        // NOTE : since LocalTime is optional, it will be created on the fly
        // if it did not exist. Therefore the first time it will trigger
        // a model change event
        g_Recipe_ua_event->setLocalTime(time.timeZone());
        // Trigger server_event
        g_Recipe_ua_event->trigger();
    }
}

void CMDProjectWrappersUaCollecter::fireRcpEvent_EndDownload(const QString &name, int finishFlag)
{
    //if(xxx(RcplogFlagMan)->isEnableScript())
    {
        QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

        QList<QVariant> paras;
        paras.append(QVariant(name));
        paras.append(finishFlag);
        //CMDScriptEngine::fireEventInPool("g_Recipe", "onEndDownload", paras, PID);


        if (!g_Recipe_ua_event)
        {
            return;
        }
        // Set server_event information
        auto time = QDateTime::currentDateTime();
        g_Recipe_ua_event->setSourceName("Server");
        g_Recipe_ua_event->setMessage(QObject::tr("recipe [%1] end download event occured in the server").arg(name));
        g_Recipe_ua_event->setTime(time.toUTC());
        g_Recipe_ua_event->setReceiveTime(time.toUTC());
        g_Recipe_ua_event->setSeverity(100);
        g_Recipe_ua_event->setStatus(finishFlag);
        g_Recipe_ua_event->setMeta(paras);
        // NOTE : since LocalTime is optional, it will be created on the fly
        // if it did not exist. Therefore the first time it will trigger
        // a model change event
        g_Recipe_ua_event->setLocalTime(time.timeZone());
        // Trigger server_event
        g_Recipe_ua_event->trigger();
    }
}

void CMDProjectWrappersUaCollecter::fireRcpEvent_DownloadProcess(const QString &msg, int total, int completed)
{
    //if(xxx(RcplogFlagMan)->isEnableScript())
    {
        QString PID = (m_projectExpertImp ? m_projectExpertImp->PID() : "");

        QList<QVariant> paras;
        paras.append(QVariant(msg));
        paras.append(total);
        paras.append(QVariant(completed));
        //CMDScriptEngine::fireEventInPool("g_Recipe", "onDownloadProcess", paras, PID);

        if (!g_Recipe_ua_event)
        {
            return;
        }
        // Set server_event information
        auto time = QDateTime::currentDateTime();
        g_Recipe_ua_event->setSourceName("Server");
        g_Recipe_ua_event->setMessage(QObject::tr("recipe process download event [%1]").arg(msg));
        g_Recipe_ua_event->setTime(time.toUTC());
        g_Recipe_ua_event->setReceiveTime(time.toUTC());
        g_Recipe_ua_event->setSeverity(100);
        g_Recipe_ua_event->setStatus(99);
        g_Recipe_ua_event->setMeta(paras);
        // NOTE : since LocalTime is optional, it will be created on the fly
        // if it did not exist. Therefore the first time it will trigger
        // a model change event
        g_Recipe_ua_event->setLocalTime(time.timeZone());
        // Trigger server_event
        g_Recipe_ua_event->trigger();
    }
}
