#include "MDActionExpert.h"
#include "MDActionExpertImp.h"

CMDActionExpert::CMDActionExpert()
{
    m_imp = new CMDActionExpertImp;
}

CMDActionExpert::~CMDActionExpert()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化与释放
*/
void CMDActionExpert::setParaBeforeLoad(const QString& pid, CMDActionConfiger* configer, CMDActionContacter* actionContacter, unsigned long timeout)
{
    m_imp->setParaBeforeLoad(pid, configer, actionContacter, timeout);
}

bool CMDActionExpert::load()
{
    return (m_imp ? m_imp->load() : false);
}

void CMDActionExpert::unload()
{
    if(m_imp)
        m_imp->unload();
}

CMDActionConfiger* CMDActionExpert::configer()
{
    return (m_imp ? m_imp->m_configer : NULL);
}

/*!
\section 启动与停止【动作处理引擎】
*/
bool CMDActionExpert::start()
{
    return (m_imp ? m_imp->start() : false);
}

void CMDActionExpert::stop()
{
    if(m_imp)
        m_imp->stop();
}

CMDCommandQueueEngine* CMDActionExpert::actionEngine()
{
    return (CMDCommandQueueEngine*)m_imp->actionEngine();
}

/*!
\section 处理动作
*/
CMDAction* CMDActionExpert::findAction(const QString& actionName)
{
    return (m_imp ? m_imp->findAction(actionName) : NULL);
}

bool CMDActionExpert::asyncProcessAction(const QString& actionName)
{
    return (m_imp ? m_imp->asyncProcessAction(actionName) : false);
}

bool CMDActionExpert::asyncProcessAction(CMDAction* action)
{
    return (m_imp ? m_imp->asyncProcessAction(action) : false);
}
