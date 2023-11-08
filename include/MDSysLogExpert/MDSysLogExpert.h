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
、使用宏定义，简化对缺省记录器的使用，参考 MDLOG_INFO 等。

\section 修改历史

2015.4 / dGFuZ3p3 / 创建；
2015.4.9 / dGFuZ3p3 / 功能完成并写注释；
2015.7.15 / dGFuZ3p3 / 增加 CMDSysLogExpert::logger 接口，便于使用定制记录器；
2015.7.16 / dGFuZ3p3 / 增加宏定义，可简化对缺省记录器的使用。

*/

#ifndef MDSYSLOGEXPERT_H
#define MDSYSLOGEXPERT_H

#include "MDSysLogExpert_Global.h"
#include "MDSysLogger.h"
#include <QString>

/*!
\section 宏定义，可简化对缺省记录器的使用，

缺省的 LOG_SOURCE 为空，具体的模块应该重定义该宏，例如：
\code
#define LOG_SOURCE "Runtime"
\endcode
并且重定义该宏的语句必须在 #include "MDSysLogExpert.h" 之后，如下所示：
\code
#include "MDSysLogExpert.h"
#define LOG_SOURCE "Runtime"
\endcode
*/
#ifndef LOG_SOURCE
#define LOG_SOURCE ""
#endif
//! 记录到缺省的日志记录器，日志来源由宏 LOG_SOURCE 指定
#define MDLOG_INFO(msg)  CMDSysLogExpert::defaultLogger()->logInfo(msg, LOG_SOURCE)
#define MDLOG_WARN(msg)  CMDSysLogExpert::defaultLogger()->logWarn(msg, LOG_SOURCE)
#define MDLOG_ERROR(msg)  CMDSysLogExpert::defaultLogger()->logError(msg, LOG_SOURCE)

//! 记录到缺省的日志记录器，日志来源可以指定
#define MDLOG_INFO_SOURCE(msg, source)  CMDSysLogExpert::defaultLogger()->logInfo(msg, source)
#define MDLOG_WARN_SOURCE(msg, source)  CMDSysLogExpert::defaultLogger()->logWarn(msg, source)
#define MDLOG_ERROR_SOURCE(msg, source)  CMDSysLogExpert::defaultLogger()->logError(msg, source)

//! 记录到指定的日志记录器，日志来源、记录器名称和日志文件名由宏 LOG_SOURCE 指定
#define MDLOG_INFO_TO_LOGGER(msg)  CMDSysLogExpert::logInfo(msg, LOG_SOURCE)
#define MDLOG_WARN_TO_LOGGER(msg)  CMDSysLogExpert::logWarn(msg, LOG_SOURCE)
#define MDLOG_ERROR_TO_LOGGER(msg)  CMDSysLogExpert::logError(msg, LOG_SOURCE)

class CMDSysLogger;
class MDSYSLOGEXPERTSHARED_EXPORT CMDSysLogExpert
{
protected:
    CMDSysLogExpert();
    ~CMDSysLogExpert();
/*!
\section 初始化与释放
*/
public:
    //! 初始化环境和缺省的日志记录器
    static bool init(const CMDSysLoggerPara& defaultLoggerPara);
    static void release();
public:
    static CMDSysLogger* defaultLogger(); //! 缺省的 logger

/*!
\section 管理 logger
*/
public:
    /*!
     创建一个 logger。

     \param name 名字，用于标识 logger。
     \param loggerPara 创建 logger 的参数。
     \returns logger 对象的指针，若 logger 不存在，则根据 loggerPara 新建一个 logger。
     */
    static CMDSysLogger* createLogger(const QString& name, const CMDSysLoggerPara& loggerPara);
    /*!
     获取一个 logger。

     \param name 名字，用于标识 logger。
     \returns logger 对象的指针，若 logger 不存在，则根据【缺省参数】新建一个 logger。
     */
    static CMDSysLogger* logger(const QString& name);
public:
    static CMDSysLogger* findLogger(const QString& name);
    static bool destroyLogger(const QString& name);
public:
    static bool enableLoggerOutput(const QString& name, bool enable);
    static bool enableAllLoggerOutput();
    static bool disableAllLoggerOutput();

/*!
\section 简化外部使用的接口
如果 source 为空，则记录到缺省的日志记录器；
如果 source 不为空，则记录到缺省的日志记录器，同时记录到名称为 source 的日志记录器。
*/
public:
    static void logInfo(const QString& msg, const QString& source = "");
    static void logWarn(const QString& msg, const QString& source = "");
    static void logError(const QString& msg, const QString& source = "");
};

#endif // MDSYSLOGEXPERT_H
