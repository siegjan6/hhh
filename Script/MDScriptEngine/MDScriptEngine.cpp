#include "MDScriptEngine.h"
#include "MDScriptEngineImp.h"
#include "MDScriptEnginePool.h"
//#include "MDSwitchFactory/MDLogSwitchFactory.h"

//CMDLogSwitch* CMDScriptEngine::pScriptEngineLogSwitch = NULL;

CMDScriptEngine::CMDScriptEngine()
{
    m_imp = new CMDScriptEngineImp();
    connect(m_imp,SIGNAL(scriptDebug(const QString&, const QString&)),this,SLOT(slotOnScriptDebug(const QString&, const QString&)));
}

CMDScriptEngine::~CMDScriptEngine()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 引擎的初始化/释放（PID 是引擎分组的标识）
*/
bool CMDScriptEngine::init(bool newIsolate, const QString& PID)
{
//    if(!CMDScriptEngine::pScriptEngineLogSwitch)
//    {
//        QString subFileDir = "ProjectExpert";
//        CMDScriptEngine::pScriptEngineLogSwitch = CMDLogSwitchFactory::createSwitchWithSubFileDir(PID, "ScriptExpert.ScriptEngine", "脚本引擎运行", subFileDir);
//    }
    return (m_imp ? m_imp->init(newIsolate, PID) : false);
}

void CMDScriptEngine::release()
{
    if(m_imp)
        m_imp->release();
}

/*!
\section 运行代码
*/
bool CMDScriptEngine::run(const QString& sourceCode, const QString& dbgSource/* = ""*/)
{
    return (m_imp ? m_imp->run(sourceCode, dbgSource) : false);
}

bool CMDScriptEngine::runDynamically(const QString& tmpCode)
{
    return (m_imp ? m_imp->runDynamically(tmpCode) : false);
}

bool CMDScriptEngine::runFile(const QString& jsFile)
{
    return (m_imp ? m_imp->runFile(jsFile) : false);
}

bool CMDScriptEngine::runAgain()
{
    return (m_imp ? m_imp->runAgain() : false);
}

QString CMDScriptEngine::getCode() const
{
    return (m_imp ? m_imp->getCode() : "");
}

QString CMDScriptEngine::helperFileCode(const QString& jsFile)
{
    return CMDScriptEngineImp::helperFileCode(jsFile);
}

QString CMDScriptEngine::helperSampleCode()
{
    return CMDScriptEngineImp::helperSampleCode();
}

/*!
\section 触发事件（PID 是引擎分组的标识）
*/
void CMDScriptEngine::fireEvent(const QString& objName, const QString& eventName, QList<QVariant>& paras)
{
    QString funcName = objName + "_" + eventName;
    callFunc(funcName, paras);
}

void CMDScriptEngine::fireEventInPool(const QString& objName, const QString& eventName, QList<QVariant>& paras, const QString& PID)
{
    QString funcName = objName + "_" + eventName;
    callFuncInPool(funcName, paras, PID);
}

void CMDScriptEngine::callFunc(const QString& funcName, QList<QVariant>& paras)
{
    if(m_imp)
        m_imp->callFunc(funcName, paras);
}

void CMDScriptEngine::callFuncInPool(const QString& funcName, QList<QVariant>& paras, const QString& PID)
{
    CMDScriptEnginePoolMgr::callFuncInPool(funcName, paras, PID);
}

/*!
\section 添加对象到引擎，在引擎运行期间，对象必须是有效的
*/
void CMDScriptEngine::addObject(CMDObjectWrapper* so)
{
    if(m_imp)
        m_imp->addObject(so);
}

void CMDScriptEngine::addObjects(const QList<CMDObjectWrapper*>& soList)
{
    if(m_imp)
        m_imp->addObjects(soList);
}

void CMDScriptEngine::addObjects(const CMDObjectWrapperMgr& wm)
{
    if(m_imp)
        m_imp->addObjects(wm);
}

void CMDScriptEngine::slotOnScriptDebug(const QString& msg, const QString& source/*=""*/)
{
//    if(CMDScriptEngine::pScriptEngineLogSwitch)
//        CMDScriptEngine::pScriptEngineLogSwitch->LOG_INFO(msg, source);
    emit scriptDebug(msg, source);
}
