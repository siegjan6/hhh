/*!

\author dGFuZ3p3

\section 功能 - 系统日志专家类，管理日志记录器，具体功能如下：
、初始化和释放日志模块的整体环境；
、管理缺省的和自定义的日志记录器。

\section 线程特性

、此类的函数都是线程安全的。

\section 用法

、成员全部是静态的，因此不需要实例化，直接引用即可；
、init 必须在开始记录日志前被调用，release 则应该在程序结束时被调用；
、提供了一个“缺省记录器”，可通过 init 指定其参数，通过 defaultLogger 访问；
、可以创建专用的记录器，参考 createLogger、logger 和 destroyLogger；

*/

#ifndef MDSYSLOGEXPERTIMP_H
#define MDSYSLOGEXPERTIMP_H

#include "log4qt/logger.h"
#include "MDSysLogger.h"
#include "MDSysLoggerImp.h"

LOG4QT_DECLARE_STATIC_LOGGER(logger, CMDSysLogExpertImp)

class CMDSysLogEngine;
class CMDSysLogExpertImp
{
public:
    CMDSysLogExpertImp();
/*!
\section 初始化与释放
*/
public:
    static bool init(const CMDSysLoggerPara& defaultLoggerPara);
    static void release();
public:
    static CMDSysLoggerImp* helper_loggerImp(CMDSysLogger* nsLogger);
    static void helper_DestroyLogger(CMDSysLogger* nsLogger);
    static CMDSysLogger m_defaultLogger;
    static CMDSysLogEngine m_defaultEngine;

/*!
\section 管理 logger
*/
public:
    static CMDSysLogger* createLogger(const QString& name, const CMDSysLoggerPara& loggerPara);
    static CMDSysLogger* findLogger(const QString& name);
    static bool destroyLogger(const QString& name);
    static void destroyAllLogger();
    static bool enableLoggerOutput(const QString& name, bool enable);
protected:
    static CMDSysLogger* findLogger_NoLocker(const QString& name);
    static QMap<QString, CMDSysLogger*> m_mdSysLoggerMap; // 通过 Map 管理日志记录器
protected:
    static QMutex m_resourceMutex; // 线程安全地访问资源
};

#endif // MDSYSLOGEXPERTIMP_H
