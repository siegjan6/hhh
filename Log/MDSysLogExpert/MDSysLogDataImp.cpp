#include "MDSysLogDataImp.h"
#include "log4qt_for_MindSCADA/layout_MindSCADA.h"

CMDSysLogDataImp::CMDSysLogDataImp()
{
}

void CMDSysLogDataImp::setLogging(const Log4Qt::LoggingEvent& logging)
{
    m_log4logging = logging;

    // 由于 LoggingEvent 不包含代表“来源”的字段，此处将原始的 LoggingEvent.mMessage
    // 分解到 m_source 和 m_message，方便后续访问
    // -----------------------------------------------------------------------------------------------------
    Log4Qt::MindSCADALayout::parseSourceAndMsg(m_log4logging, m_source, m_message);
}
