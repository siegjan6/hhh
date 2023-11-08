#include "MDActionExpertImp.h"
#include "MDActionEngine.h"

#include "../../include/MDActionHelper/MDAction.h"
#include "MDActionContacter.h"
#include "MDActionSPSession.h"
#include "MDLogMessage.h"
#include "MDExpression.h"


CMDActionExpertImp::CMDActionExpertImp()
{
    m_pid = "";
    m_configer = NULL; //! 配置器
    m_actionContacter = NULL;
    m_timeout = 0;
    m_actionEngine = NULL;
    m_checkActionSPEngine = NULL; //! 管理执行前需要进行安全检查的动作

    m_sProcessAction = NULL;
    m_lsActionProcess = NULL;
}

CMDActionExpertImp::~CMDActionExpertImp()
{
//    stop(2000);
}

/*!
\section 初始化与释放
*/
void CMDActionExpertImp::setParaBeforeLoad(const QString& pid, CMDActionConfiger* configer, CMDActionContacter* actionContacter, unsigned long timeout)
{
    m_pid = pid;
    m_configer = configer;
    m_actionContacter = actionContacter;
    m_timeout = timeout;
}

bool CMDActionExpertImp::load()
{
    createSwitchs(true);

    start();

    return true;
}

void CMDActionExpertImp::unload()
{
    stop();

    if(m_actionEngine)
    {
        delete m_actionEngine;
        m_actionEngine = NULL;
    }
    if(m_checkActionSPEngine)
    {
        delete m_checkActionSPEngine;
        m_checkActionSPEngine = NULL;
    }

    createSwitchs(false);
}

void CMDActionExpertImp::createSwitchs(bool create)
{
    if(create)
    {
        QString subFileDir = "ProjectExpert";
        m_lsActionProcess = CMDLogSwitchFactory::createSwitchWithSubFileDir(m_pid, "ActionExpert.ActionProcess", "动作处理流程", subFileDir);
        m_sProcessAction = CMDFuncSwitchFactory::createSwitch(m_pid, "ActionExpert.ProcessAction", "处理动作");
    }
    else
    {
        m_sProcessAction = NULL;
        m_lsActionProcess = NULL;
    }
}

bool CMDActionExpertImp::start()
{
    CMDLogSwitch* lsProjectLoad = CMDLogSwitchFactory::findByGroup(m_pid, "ProjectExpert.Load_and_Unload");

    lsProjectLoad->LOG_INFO("创建运行时动作");
    initRuntimeActionPool();

    lsProjectLoad->LOG_INFO("启动动作引擎");
    startActionEngine();

    lsProjectLoad->LOG_INFO("m_delayMgr.init(this)");
    m_delayMgr.init(this);

    return true;
}

void CMDActionExpertImp::stop()
{
    CMDLogSwitch* lsProjectLoad = CMDLogSwitchFactory::findByGroup(m_pid, "ProjectExpert.Load_and_Unload");

    lsProjectLoad->LOG_INFO("m_delayMgr.release");
    m_delayMgr.release();

    lsProjectLoad->LOG_INFO("停止动作引擎");
    stopActionEngine();

    lsProjectLoad->LOG_INFO("释放运行时动作");
    releaseRuntimeActionPool();
}

//! 启动与停止【动作处理引擎】
bool CMDActionExpertImp::startActionEngine()
{
    if(!m_checkActionSPEngine)
        m_checkActionSPEngine  = new CCommandEngineForCheckActionSP(this);
    if(m_checkActionSPEngine)
        m_checkActionSPEngine->startInCurrentThread();

    if(!m_actionEngine)
        m_actionEngine = new CMDActionEngine(this);

    return (m_actionEngine ? m_actionEngine->start() : false);
}

void CMDActionExpertImp::stopActionEngine()
{
    if(m_checkActionSPEngine)
        m_checkActionSPEngine->stop(m_timeout);

    if(m_actionEngine)
        m_actionEngine->stop(m_timeout);
}

CMDActionEngine* CMDActionExpertImp::actionEngine()
{
    return m_actionEngine;
}

/*!
\section 处理需要执行的动作
*/
bool CMDActionExpertImp::asyncProcessAction(const QString& actionName)
{
    if(m_lsActionProcess->isOn())
        m_lsActionProcess->LOG_INFO_IS("begin action: " + actionName);
    if(!m_sProcessAction->isOn())
    {
        if(m_lsActionProcess->isOn())
            m_lsActionProcess->LOG_INFO_IS("【处理动作】功能已关闭！");

        return false;
    }

    CMDAction* action = findAction(actionName);
    if(!action)
    {
        if(m_lsActionProcess->isOn())
            m_lsActionProcess->LOG_ERROR("动作不存在：" + actionName);
        return false;
    }

    return asyncProcessAction(action);
}

bool CMDActionExpertImp::asyncProcessAction(CMDAction* action)
{
    if(m_lsActionProcess->isOn())
        m_lsActionProcess->LOG_INFO_IS("begin action: " + action->configProperty()->name());
    if(!m_sProcessAction->isOn())
    {
        if(m_lsActionProcess->isOn())
            m_lsActionProcess->LOG_INFO_IS("【处理动作】功能已关闭！");

        return false;
    }

    bool result = false;
    if(NULL == action->runtimeProperty()->enabledExeSP())
    {
        result = asyncProcessAction_NoSP(action);
    }
    else
    {
        if(m_lsActionProcess->isOn())
            m_lsActionProcess->LOG_INFO_IS("动作启用了安全策略");

        if(m_checkActionSPEngine)
            m_checkActionSPEngine->submitActionWithSP(action);

        result = true;
    }

    return result;
}

bool CMDActionExpertImp::asyncProcessAction_AfterCheckSP(CMDAction* action)
{
    CMDActionSPSession spSession(*action->runtimeProperty()->enabledExeSP());
     bool result = m_actionContacter->checkSP_WithUI(&spSession);
     if(result)
     {
        asyncProcessAction_NoSP(action);
     }
     return result;
}

bool CMDActionExpertImp::asyncProcessAction_NoSP(CMDAction* action)
{
    if(!action->configProperty()->isEnableDelay())
    {
        asyncProcessAction_NoDelay(action);
    }
    else
    {
        if(m_lsActionProcess->isOn())
            m_lsActionProcess->LOG_INFO_IS("动作->延迟队列");

        m_delayMgr.add(action);
    }

    return true;
}

bool CMDActionExpertImp::asyncProcessAction_NoDelay(CMDAction* action)
{
    if(m_lsActionProcess->isOn())
        m_lsActionProcess->LOG_INFO_IS("动作->执行队列");

    if(m_actionEngine)
        m_actionEngine->asyncProcessAction(action);

    return true;
}

bool CMDActionExpertImp::syncProcessAction(CMDAction* action)
{
    if(!m_actionContacter)
        return false;

    if(action->configProperty()->isEnableExecExp())
    {
        if(!action->configProperty()->execExp().isEmpty())
        {
            CMDExpression exp(action->configProperty()->execExp());
            QList<QString> relatedVarList = exp.referencedVarList();
            for (int i = 0; i < relatedVarList.size(); ++i)
            {
//                IMDVariable* var = CMDRTDBManagerFactory::createVariable(); // 准备一个空白变量
//                var->setName(relatedVarList[i]);
//                if(m_actionContacter->variable(relatedVarList[i], var)) {
                    QVariant value = m_actionContacter->variableValue(relatedVarList[i]);
                    exp.setVariableValue(relatedVarList[i], value);
//                }
//                delete var;
            }
//            if(!exp.compile(action->configProperty()->execExp()))
//                return false;
            QVariant vResult = exp.excute();
            bool result = vResult.toBool();
            if(!result) {
                if(m_lsActionProcess->isOn())
                {
                    m_lsActionProcess->LOG_WARN_IS(QString("动作->表达式[%1]不满足条件,不执行此动作!").arg(action->configProperty()->execExp()));
                }
                return false;
            }
        }
    }

    if(action->configProperty()->isEnableLog())
    {
        log(action);
    }

    if(m_lsActionProcess->isOn())
    {
        m_lsActionProcess->LOG_INFO_IS("动作->具体模块");
        m_lsActionProcess->LOG_INFO_IS("end action：" + action->configProperty()->name());
    }

    return m_actionContacter->executeAction(action);
}

bool CMDActionExpertImp::log(CMDAction* action)
{
    CMDLogMessage msg;
    msg.setMsg("执行动作：" +
                            action->configProperty()->name() + "[" +
                            action->configProperty()->comment() + "]");
    return m_actionContacter->log(&msg);
}

/*!
\section 管理项目定义的动作对象
*/
void CMDActionExpertImp::initRuntimeActionPool()
{
    if(!m_configer)
        return;

    // 根据【配置动作】生成【运行时动作】
    QList<CMDActionProperty_Config*> configedActionList;
    m_configer->configedActionList(configedActionList);
    int size = configedActionList.size();
    for(int i = 0; i < size; i++)
    {
        CMDActionProperty_Config* actionProperty = configedActionList[i];
        CMDAction* currentAction = new_RuntimeAction(actionProperty, m_actionContacter);

        // m_runtimeActionMap 负责管理项目里全部的事件
        if(currentAction)
            m_runtimeActionMap.insert(currentAction->configProperty()->name().toUpper(), currentAction);
    }
}

CMDAction* CMDActionExpertImp::new_RuntimeAction(CMDActionProperty_Config* cp, CMDActionContacter* actionContacter)
{
    CMDAction* action = new CMDAction(cp);
    if(action)
    {
        CMDProjectModuleExpert* actionExecuter = (m_actionContacter ?
                                                                                  m_actionContacter->findActionExecuter(action) : NULL);
        action->setActionExecuter(actionExecuter);

        // 性能优化：
        // 加载时，如果“执行动作”的安全策略是启用状态，设置该指针，否则该指针为空；
        // 运行时，可根据指针快速判断和检测。
        // --------------------------------------------------------------------------------------------------------------
        action->runtimeProperty()->setEnabledExeSP(helperGetExecuteSP_OnlyIfSPEnabled(cp));
    }
    return action;
}

void CMDActionExpertImp::releaseRuntimeActionPool()
{
    QMap<QString, CMDAction*>::const_iterator iPosition = m_runtimeActionMap.constBegin();
    while(m_runtimeActionMap.constEnd() != iPosition)
    {
        delete iPosition.value();
        ++ iPosition;
    }
    m_runtimeActionMap.clear();
}

CMDAction* CMDActionExpertImp::findAction(const QString& name)
{
    CMDAction* action = NULL;

    QMap<QString, CMDAction*>::const_iterator iPosition = m_runtimeActionMap.find(name.toUpper());
    if(iPosition != m_runtimeActionMap.constEnd())
        action = iPosition.value();

    return action;
}

CMDActionSP* CMDActionExpertImp::helperGetExecuteSP_OnlyIfSPEnabled(CMDActionProperty_Config* cp)
{
    CMDActionSP* sp = cp->spList()->find(MDActionType::action_Execute);
    if(sp &&
            sp->protectMode() != MDActionProtectMode::null_mode)
    {
        return sp;
    }
    else return NULL;
}
