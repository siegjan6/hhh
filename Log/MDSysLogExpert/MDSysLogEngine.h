/*!

\author dGFuZ3p3

\section 功能

日志队列引擎类，管理需要记录的日志。

*/

#ifndef MDSYSLOGENGINE_H
#define MDSYSLOGENGINE_H

#include "log4qt/loggingevent.h"
#include "../../include/MDCommandQueueEngine/MDCommandQueueEngine.h"

class CMDSysLoggerImp;
class CMDSysLogEngine : public CMDCommandQueueEngine
{
public:
    CMDSysLogEngine();
public:
    void setLogger(CMDSysLoggerImp* logger);
protected:
    CMDSysLoggerImp* m_logger;
public:
    bool asyncProcessLog(Log4Qt::Level level, const QString& msg, const QString& source);
public:
    virtual void onProcessCommand(CMDCommand* command);
};

#endif // MDSYSLOGENGINE_H

