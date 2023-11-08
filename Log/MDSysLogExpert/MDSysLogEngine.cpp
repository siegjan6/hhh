#include "MDSysLogEngine.h"
#include "MDSysLoggerImp.h"

// 仅供 CMDSysLogEngine 使用的命令类，由于传递 CMDRawEvent 对象指针
// ---------------------------------------------------------------------------------
class CMDLogCommand : public CMDCommand
{
    friend class CMDSysLogEngine;
protected:
    CMDLogCommand(Log4Qt::Level level, const QString& msg, const QString& source);
    virtual ~CMDLogCommand();
    void setLogPara(Log4Qt::Level level, const QString& msg, const QString& source);
protected:
    Log4Qt::Level m_level;
    QString m_msg;
    QString m_source;
};
CMDLogCommand::CMDLogCommand(Log4Qt::Level level, const QString& msg, const QString& source)
{
    m_level = level;
    m_msg = msg;
    m_source = source;
}

CMDLogCommand::~CMDLogCommand()
{
}

void CMDLogCommand::setLogPara(Log4Qt::Level level, const QString& msg, const QString& source)
{
    m_level = level;
    m_msg = msg;
    m_source = source;
}

// ---------------------------------------------------------------------------------

CMDSysLogEngine::CMDSysLogEngine()
{
    m_logger = NULL;
    setComment("LoggerEngine.Default");
    setMaxRecycleQueueLength(100*1024);
}

void CMDSysLogEngine::setLogger(CMDSysLoggerImp* logger)
{
    m_logger = logger;
    setComment("LoggerEngine." + logger->m_para.comment());
}

bool CMDSysLogEngine::asyncProcessLog(Log4Qt::Level level, const QString& msg, const QString& source)
{
    CMDLogCommand* logCommand = (CMDLogCommand*)fetchRecycleCommand();
    if(logCommand)
    {
        logCommand->setLogPara(level, msg, source);
        submitCommand(logCommand);
    }
    else submitCommand(new CMDLogCommand(level, msg, source));
    return true;
}

void CMDSysLogEngine::onProcessCommand(CMDCommand* command)
{
    if(m_logger)
    {
        CMDLogCommand* logCommand = (CMDLogCommand*)command;
        m_logger->processLog(logCommand->m_level,
                                                               logCommand->m_msg,
                                                               logCommand->m_source);
    }
}
