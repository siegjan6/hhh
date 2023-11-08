#include "MDProjectModuleExpertProxy.h"
#include "MDProjectExpertImp.h"
#include <QDir>
#include <QJsonDocument>

#include "../../include/MDVariableExpert/MDVariableExpert.h"
#include "../../include/MDEventExpert/MDEventExpert.h"
#include "../../include/MDActionExpert/MDActionExpert.h"
#include "../../include/MDHistoryExpert/MDHistoryExpert.h"

#include "../../include/MDProjectExpert/MDProjectContacter.h"

/*!
 * \brief 变量专家代理类
 */
CMDVariableExpertProxy::CMDVariableExpertProxy()
{
}

qint8 CMDVariableExpertProxy::executeAction(CMDAction* action)
{
    return m_variableExpert->executeAction(action);
}

/*!
 * \brief 事件专家代理类
 */
CMDEventExpertProxy::CMDEventExpertProxy()
{
}

/*!
 * \brief 动作专家代理类
 */
CMDActionExpertProxy::CMDActionExpertProxy()
{
}

/*!
 * \brief 历史专家代理类
 */
CMDHistoryExpertProxy::CMDHistoryExpertProxy()
{
}

qint8 CMDHistoryExpertProxy::executeAction(CMDAction* action)
{
    return m_historyExpert->executeAction(action);
}

/*!
 * \brief HMI 专家代理类
 */
CMDHMIExpertProxy::CMDHMIExpertProxy()
{
}

bool CMDHMIExpertProxy::open(const QString& name, int pwid/* = 0*/, int x /*= 0*/, int y /*= 0*/, int w /*= 0*/, int h /*= 0*/)
{
    CMDActionProperty_Config* cp = CAPC_Factory::newConfig(MDActionType::window_Open, NULL);
    if(!cp)
        return false;

    CAPC_WindowOperation* wo = (CAPC_WindowOperation*)cp;
    wo->setPwid(pwid);
    wo->setGeometry(QRect(x,y,w,h));

    // 此处生成的动作名称必须为空，
    // 外部模块将根据名字决定是否删除动作对象
    // ---------------------------------------------------------------------
    cp->actionID().setObjName(QDir::cleanPath(name));
    CMDAction* openAction = new CMDAction(cp);
    executeAction(openAction);
    return true;
}

void CMDHMIExpertProxy::close(const QString& name)
{
    CMDActionProperty_Config* cp = CAPC_Factory::newConfig(MDActionType::window_Close, NULL);
    if(!cp)
        return;

    // 此处生成的动作名称必须为空，
    // 外部模块将根据名字决定是否删除动作对象
    // ---------------------------------------------------------------------
    cp->actionID().setObjName(QDir::cleanPath(name));
    CMDAction* openAction = new CMDAction(cp);
    executeAction(openAction);
}

void CMDHMIExpertProxy::move(const QString& name, int left, int top)
{
    CMDActionProperty_Config* cp = CAPC_Factory::newConfig(MDActionType::window_Move, NULL);
    if(!cp)
        return;

    CAPC_WindowOperation* wo = (CAPC_WindowOperation*)cp;
    wo->setLeft(left);
    wo->setTop(top);

    // 此处生成的动作名称必须为空，
    // 外部模块将根据名字决定是否删除动作对象
    // ---------------------------------------------------------------------
    cp->actionID().setObjName(QDir::cleanPath(name));
    CMDAction* openAction = new CMDAction(cp);
    executeAction(openAction);
}

void CMDHMIExpertProxy::hide(const QString& name)
{
    CMDActionProperty_Config* cp = CAPC_Factory::newConfig(MDActionType::window_Hide, NULL);
    if(!cp)
        return;

    // 此处生成的动作名称必须为空，
    // 外部模块将根据名字决定是否删除动作对象
    // ---------------------------------------------------------------------
    cp->actionID().setObjName(QDir::cleanPath(name));
    CMDAction* hideAction = new CMDAction(cp);
    executeAction(hideAction);
}

void CMDHMIExpertProxy::callJsFunc(const QString& name, QString funcName, QString params)
{
    CMDActionProperty_Config* cp = CAPC_Factory::newConfig(MDActionType::window_RunScript, NULL);
    if(!cp)
        return;

    CAPC_WindowOperation* wo = (CAPC_WindowOperation*)cp;
    QString meta = "";
    QJsonDocument jsonDocument = QJsonDocument::fromJson(params.toLocal8Bit().data());
    if( !jsonDocument.isNull() ){
        // json 参数格式
        qDebug()<< "===> json string params format"<< params.toLocal8Bit().data();
        meta = QString("var json_bosmutus=%1;%2(json_bosmutus);").arg(params).arg(funcName);
    }
    else{
        // 字符串直接当作参数执行
        qDebug()<< "===> string params format"<< params.toLocal8Bit().data();
        meta = QString("%1(%2);").arg(funcName).arg(params);
    }
    wo->setMeta(meta);

    cp->actionID().setObjName(QDir::cleanPath(name));
    CMDAction* callJsFuncAction = new CMDAction(cp);
    executeAction(callJsFuncAction);
}

void CMDHMIExpertProxy::enterFullScreen()
{
    m_projectExpert->enterFullScreen();
}

void CMDHMIExpertProxy::exitFullScreen()
{
    m_projectExpert->exitFullScreen();
}

void CMDHMIExpertProxy::showRecipeRuntime()
{
    return m_projectExpert->showRecipeRuntime();
}
void CMDHMIExpertProxy::hideRecipeRuntime()
{
    return m_projectExpert->hideRecipeRuntime();
}

void CMDHMIExpertProxy::showRuntime()
{
    return m_projectExpert->showRuntime();
}
void CMDHMIExpertProxy::hideRuntime()
{
    return m_projectExpert->hideRuntime();
}

void CMDHMIExpertProxy::showByPid(int pid, int phwnd/* = 0*/)
{
    CMDActionProperty_Config* cp = CAPC_Factory::newConfig(MDActionType::window_ShowByPid, NULL);
    if(!cp)
        return;

    CAPC_WindowOperation* wo = (CAPC_WindowOperation*)cp;
    QString meta = QString("%1,%2").arg(pid).arg(phwnd);
    wo->setMeta(meta);

    cp->actionID().setObjName(meta);
    CMDAction* action = new CMDAction(cp);
    executeAction(action);
}

void CMDHMIExpertProxy::hideByPid(int pid, int phwnd/* = 0*/)
{
    CMDActionProperty_Config* cp = CAPC_Factory::newConfig(MDActionType::window_HideByPid, NULL);
    if(!cp)
        return;

    CAPC_WindowOperation* wo = (CAPC_WindowOperation*)cp;
    QString meta = QString("%1,%2").arg(pid).arg(phwnd);
    wo->setMeta(meta);

    cp->actionID().setObjName(meta);
    CMDAction* action = new CMDAction(cp);
    executeAction(action);
}

void CMDHMIExpertProxy::closeByPid(int pid, int phwnd/* = 0*/)
{
    CMDActionProperty_Config* cp = CAPC_Factory::newConfig(MDActionType::window_CloseByPid, NULL);
    if(!cp)
        return;

    CAPC_WindowOperation* wo = (CAPC_WindowOperation*)cp;
    QString meta = QString("%1,%2").arg(pid).arg(phwnd);
    wo->setMeta(meta);

    cp->actionID().setObjName(meta);
    CMDAction* action = new CMDAction(cp);
    executeAction(action);
}

qint8 CMDHMIExpertProxy::executeAction(CMDAction* action)
{
    m_projectExpert->m_engineForExecuteHMIAction.submitHMIAction(action);
    return 1;
}

/*!
 * \brief 代理类的管理类，用于简化 CMDProjectExpertImp 的实现代码
 */
CMDExpertProxyMgr::CMDExpertProxyMgr()
{

}

void CMDExpertProxyMgr::init(CMDProjectExpertImp* projectExpertImp)
{
    m_variableProxy.m_variableExpert = &(projectExpertImp->m_variableExpert);
    m_historyProxy.m_historyExpert = &(projectExpertImp->m_historyExpert);
    m_hmiProxy.m_projectExpert = projectExpertImp;
}
