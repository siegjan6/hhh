#include "MDSysLogData.h"
#include "MDSysLogDataImp.h"
#include "log4qt/helpers/datetime.h"

CMDSysLogData::CMDSysLogData()
{
    m_imp = new CMDSysLogDataImp;
}

CMDSysLogData::~CMDSysLogData()
{
    if(m_imp)
        delete m_imp;
}

QDateTime CMDSysLogData::timeStamp() const
{
    QDateTime dt;
    if(m_imp)
        dt = Log4Qt::DateTime::fromMilliSeconds(m_imp->m_log4logging.timeStamp());
    return dt;
}

QString CMDSysLogData::timeStamp_ISO8601() const
{
    if(!m_imp)
        return "";
    QString format = "ISO8601";
    QString line = Log4Qt::DateTime::fromMilliSeconds(m_imp->m_log4logging.timeStamp()).toString(format);
    return line;
}

int CMDSysLogData::level() const
{
    return (m_imp ? m_imp->m_log4logging.level().toInt() : 0);
}

QString CMDSysLogData::source() const
{
    return (m_imp ? m_imp->m_source : "");
}

QString CMDSysLogData::message() const
{
    return (m_imp ? m_imp->m_message : "");
}
