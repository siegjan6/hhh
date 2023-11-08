#include "MDEventExpert.h"
#include "MDEventExpertImp.h"

CMDEventExpert::CMDEventExpert()
{
    m_imp = new CMDEventExpertImp;
}

CMDEventExpert::~CMDEventExpert()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化与释放
*/
void CMDEventExpert::setParaBeforeLoad(const QString& pid, CMDEventConfiger* configer, CMDEventContacter* actionContacter, unsigned long timeout)
{
    m_imp->setParaBeforeLoad(pid, configer, actionContacter, timeout);
}

bool CMDEventExpert::load()
{
    return (m_imp ? m_imp->load() : false);
}

void CMDEventExpert::unload()
{
    if(m_imp)
        m_imp->unload();
}

CMDEventConfiger* CMDEventExpert::configer()
{
    return (m_imp ? m_imp->m_configer : NULL);
}

/*!
\section 启动与停止【事件处理引擎】
*/
bool CMDEventExpert::start()
{
    return (m_imp ? m_imp->start() : false);
}

void CMDEventExpert::stop()
{
    if(m_imp)
        m_imp->stop();
}

/*!
\section 处理事件
*/
bool CMDEventExpert::asyncProcessRawEvent(CMDRawEvent* rawEvent)
{
    return (m_imp ? m_imp->asyncProcessRawEvent(rawEvent) : false);
}
