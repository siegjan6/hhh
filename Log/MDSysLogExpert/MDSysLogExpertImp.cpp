#include "MDSysLogExpertImp.h"
#include "log4qt/logmanager.h"
#include "MDSysLogEngine.h"

CMDSysLogger CMDSysLogExpertImp::m_defaultLogger;
QMap<QString, CMDSysLogger*> CMDSysLogExpertImp::m_mdSysLoggerMap;
CMDSysLogEngine CMDSysLogExpertImp::m_defaultEngine;
QMutex CMDSysLogExpertImp::m_resourceMutex;

CMDSysLogExpertImp::CMDSysLogExpertImp()
{
}

/*!
\section 初始化与释放

关键点： CMDSysLoggerImp 如何与 Log4Qt::Logger 的实例绑定？

m_defaultLogger ：与 Log4Qt::Logger::rootLogger() 绑定，没有父对象；

普通 logger ：与 Log4Qt::LogManager::logger() 分配的实例绑定，
由于指定了名称 name， 该实例具有一个父对象，记录一条日志时，
对象和父对象都会记录，因此记录数是 2 份；
参考 Logger *Hierarchy::createLogger(const QString &rName)。
而父对象就是 m_defaultLogger。

*/

bool CMDSysLogExpertImp::init(const CMDSysLoggerPara& defaultLoggerPara)
{
//    logger()->warn("CMDSysLogExpertImp init");

    QMutexLocker locker(&m_resourceMutex);

    // m_defaultLogger ：与 Log4Qt::Logger::rootLogger() 绑定
    CMDSysLoggerImp* defaultLogger = helper_loggerImp(&m_defaultLogger);
    return defaultLogger->init(Log4Qt::Logger::rootLogger(), defaultLoggerPara);
}

void CMDSysLogExpertImp::release()
{
    destroyAllLogger();

    QMutexLocker locker(&m_resourceMutex);

    CMDSysLoggerImp* defaultLogger = helper_loggerImp(&m_defaultLogger);
    if(defaultLogger)
        defaultLogger->release();
}

/*!
\section 管理 logger
*/
CMDSysLogger* CMDSysLogExpertImp::createLogger(const QString& name, const CMDSysLoggerPara& loggerPara)
{
    QMutexLocker locker(&m_resourceMutex);

    CMDSysLogger* nsLogger = findLogger_NoLocker(name);
    if(NULL == nsLogger)
    {
        nsLogger = new CMDSysLogger;
        if(nsLogger)
        {
            // 普通 logger ：与 Log4Qt::LogManager::logger() 分配的实例绑定，
            // 由于指定了名称 name， 该实例具有一个父对象，记录一条日志时，
            // 对象和父对象都会记录，因此记录数是 2 份；
            // 参考 Logger *Hierarchy::createLogger(const QString &rName)。
            // 而父对象就是 m_defaultLogger。

            CMDSysLoggerPara loggerParaWithName = loggerPara;
            loggerParaWithName.setName(name);
            CMDSysLoggerImp* nsLoggerImp = helper_loggerImp(nsLogger);
            nsLoggerImp->init(Log4Qt::LogManager::logger(name.toUpper()), loggerParaWithName);
            m_mdSysLoggerMap.insert(name.toUpper(), nsLogger);
        }
    }
    return nsLogger;
}

CMDSysLogger* CMDSysLogExpertImp::findLogger(const QString& name)
{
    QMutexLocker locker(&m_resourceMutex);

    return findLogger_NoLocker(name);
}

CMDSysLogger* CMDSysLogExpertImp::findLogger_NoLocker(const QString& name)
{
    CMDSysLogger* nsLogger = NULL;
    QMap<QString, CMDSysLogger*>::const_iterator iPosition = m_mdSysLoggerMap.find(name.toUpper());
    if(iPosition != m_mdSysLoggerMap.end())
    {
        nsLogger = iPosition.value();
    }
    return nsLogger;
}

bool CMDSysLogExpertImp::destroyLogger(const QString& name)
{
    QMutexLocker locker(&m_resourceMutex);

    CMDSysLogger* nsLogger = findLogger_NoLocker(name);
    if(nsLogger)
        helper_DestroyLogger(nsLogger);
    return (nsLogger != NULL);
}

void CMDSysLogExpertImp::destroyAllLogger()
{
    QMutexLocker locker(&m_resourceMutex);

    QMap<QString, CMDSysLogger*>::const_iterator iPosition = m_mdSysLoggerMap.constBegin();
    while(m_mdSysLoggerMap.constEnd() != iPosition)
    {
        helper_DestroyLogger(iPosition.value());

        ++ iPosition;
    }
    m_mdSysLoggerMap.clear();
}

bool CMDSysLogExpertImp::enableLoggerOutput(const QString& name, bool enable)
{
    bool result = false;
    if(name.isEmpty())
    {
        QMutexLocker locker(&m_resourceMutex);

        foreach (CMDSysLogger* logger, m_mdSysLoggerMap)
        {
            logger->para().enableOutput(enable);
        }
        result = true;
    }
    else
    {
        QMutexLocker locker(&m_resourceMutex);

        CMDSysLogger* logger = findLogger_NoLocker(name);
        if(logger)
        {
            logger->para().enableOutput(enable);
            result = true;
        }
    }

    return result;
}

CMDSysLoggerImp* CMDSysLogExpertImp::helper_loggerImp(CMDSysLogger* nsLogger)
{
   return (nsLogger ? nsLogger->m_imp : NULL);
}

void CMDSysLogExpertImp::helper_DestroyLogger(CMDSysLogger* nsLogger)
{
    CMDSysLoggerImp* nsLoggerImp = helper_loggerImp(nsLogger);
    nsLoggerImp->release();
    delete nsLogger;
}
