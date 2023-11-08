#include "MDScriptExpert.h"
#include "MDScriptExpertImp.h"

CMDScriptExpert::CMDScriptExpert()
{
    m_imp = new CMDScriptExpertImp;
}

CMDScriptExpert::~CMDScriptExpert()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化与释放
*/
void CMDScriptExpert::setParaBeforeLoad(const QString& pid, const QString& projectPath, CMDScriptContacter* scriptContacter)
{
    m_imp->setParaBeforeLoad(pid, projectPath, scriptContacter);
}

void CMDScriptExpert::load()
{
    if(m_imp)
        m_imp->load();
}

void CMDScriptExpert::unload()
{
    if(m_imp)
        m_imp->unload();
}

/*!
\section 控制脚本运行
*/
bool CMDScriptExpert::runScript(const QString& nameOnly)
{
    return (m_imp ? m_imp->runScript(nameOnly) : false);
}

bool CMDScriptExpert::stopScript(const QString& nameOnly)
{
    return (m_imp ? m_imp->stopScript(nameOnly) : false);
}

void CMDScriptExpert::runAll()
{
    if(m_imp)
        m_imp->runAll();
}

void CMDScriptExpert::stopAll()
{
    if(m_imp)
        m_imp->stopAll();
}
