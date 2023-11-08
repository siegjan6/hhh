#include "MDSysLogExpert.h"
#include "MDSysLogExpertImp.h"

CMDSysLogExpert::CMDSysLogExpert()
{
}

CMDSysLogExpert::~CMDSysLogExpert()
{
}

/*!
\section 初始化与释放
*/
bool CMDSysLogExpert::init(const CMDSysLoggerPara& defaultLoggerPara)
{
    return CMDSysLogExpertImp::init(defaultLoggerPara);
}

void CMDSysLogExpert::release()
{
    CMDSysLogExpertImp::release();
}

CMDSysLogger* CMDSysLogExpert::defaultLogger()
{
    return &CMDSysLogExpertImp::m_defaultLogger;
}

/*!
\section 管理 logger
*/
CMDSysLogger* CMDSysLogExpert::createLogger(const QString& name, const CMDSysLoggerPara& loggerPara)
{
    return CMDSysLogExpertImp::createLogger(name, loggerPara);
}

CMDSysLogger* CMDSysLogExpert::logger(const QString& name)
{
    // 根据缺省参数创建一个 logger
    CMDSysLoggerPara loggerPara;
    return CMDSysLogExpertImp::createLogger(name, loggerPara);
}

CMDSysLogger* CMDSysLogExpert::findLogger(const QString& name)
{
    return CMDSysLogExpertImp::findLogger(name);
}

bool CMDSysLogExpert::destroyLogger(const QString& name)
{
    return CMDSysLogExpertImp::destroyLogger(name);
}

bool CMDSysLogExpert::enableLoggerOutput(const QString& name, bool enable)
{
    return CMDSysLogExpertImp::enableLoggerOutput(name, enable);
}

bool CMDSysLogExpert::enableAllLoggerOutput()
{
    return CMDSysLogExpertImp::enableLoggerOutput("", true);
}

bool CMDSysLogExpert::disableAllLoggerOutput()
{
    return CMDSysLogExpertImp::enableLoggerOutput("", false);
}

/*!
\section 简化外部使用的接口
如果 source 为空，则记录到缺省的日志记录器；
如果 source 不为空，则记录到缺省的日志记录器，同时记录到名称为 source 的日志记录器。
*/
void CMDSysLogExpert::logInfo(const QString& msg, const QString& source)
{
    CMDSysLogger* logger = findLogger(source);
    if(logger)
        logger->logInfo(msg, source);
    else
    {
        // 创建一个 logger，除文件主名外，参数与缺省日志记录器相同
        CMDSysLoggerPara loggerPara = defaultLogger()->para();
        loggerPara.setFileMainName(source);
        logger = createLogger(source, loggerPara);
        if(logger)
            logger->logInfo(msg, source);
    }
}

void CMDSysLogExpert::logWarn(const QString& msg, const QString& source)
{
    CMDSysLogger* logger = findLogger(source);
    if(logger)
        logger->logWarn(msg, source);
    else
    {
        // 创建一个 logger，除文件主名外，参数与缺省日志记录器相同
        CMDSysLoggerPara loggerPara = defaultLogger()->para();
        loggerPara.setFileMainName(source);
        logger = createLogger(source, loggerPara);
        if(logger)
            logger->logWarn(msg, source);
    }
}

void CMDSysLogExpert::logError(const QString& msg, const QString& source)
{
    CMDSysLogger* logger = findLogger(source);
    if(logger)
        logger->logError(msg, source);
    else
    {
        // 创建一个 logger，除文件主名外，参数与缺省日志记录器相同
        CMDSysLoggerPara loggerPara = defaultLogger()->para();
        loggerPara.setFileMainName(source);
        logger = createLogger(source, loggerPara);
        if(logger)
            logger->logError(msg, source);
    }
}
