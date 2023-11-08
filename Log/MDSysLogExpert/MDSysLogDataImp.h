/*!

\author dGFuZ3p3

\section 功能

日志记录数据类的实现类，代表了一条日志记录的信息，
该类实际是包装了 log4qt 库的 Log4Qt::LoggingEvent 供外部使用。

\section 修改历史

2015.4 / dGFuZ3p3 / 创建；
2015.4.9 / dGFuZ3p3 / 功能完成并写注释。

*/

#ifndef MDSYSLOGDATAIMP_H
#define MDSYSLOGDATAIMP_H

#include "log4qt/loggingevent.h"

class CMDSysLogDataImp
{
public:
    CMDSysLogDataImp();
    void setLogging(const Log4Qt::LoggingEvent& logging);
public:
    QString m_source;
    QString m_message;
public:
    Log4Qt::LoggingEvent m_log4logging;
};

#endif // MDSYSLOGDATAIMP_H
