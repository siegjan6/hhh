#include "MDLogExpert.h"
#include "MDLogExpertImp.h"

CMDLogExpert::CMDLogExpert()
{
    m_imp = new CMDLogExpertImp;
}

CMDLogExpert::~CMDLogExpert()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化与释放
*/
void CMDLogExpert::setParaBeforeLoad(const QString& pid, CMDHistoryExpert* historyExpert, CMDLogContacter* contacter)
{
    if(m_imp)
        m_imp->setParaBeforeLoad(pid, historyExpert, contacter);
}

void CMDLogExpert::load()
{
    if(m_imp)
        m_imp->load();
}

void CMDLogExpert::unload()
{
    if(m_imp)
        m_imp->unload();
}

/*!
\section 缓存最近日志到内存；获取最近日志
*/
bool CMDLogExpert::log(CMDLogMessage& msg)
{
    return (m_imp ? m_imp->log(msg) : false);
}

int CMDLogExpert::maxCount()
{
    return m_imp->m_recordMgr.maxCount();
}

void CMDLogExpert::setMaxCount(int maxCount)
{
    m_imp->m_recordMgr.setMaxCount(maxCount);
}

bool CMDLogExpert::logRecordSet(CMDDatabaseRecordset* dataRecordset)
{
    return (m_imp ? m_imp->logRecordSet(dataRecordset) : false);
}

QDateTime CMDLogExpert::recentUpdateTime()
{
    return (m_imp ? m_imp->readUpdateTime() : QDateTime());
}
