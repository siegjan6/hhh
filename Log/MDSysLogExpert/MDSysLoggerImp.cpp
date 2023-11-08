#include "MDSysLoggerImp.h"
#include "MDSysLogger.h"
#include "MDSysLogDataImp.h"
#include "MDSysLogExpertImp.h"
#include "MDSysLogEngine.h"
#include <QDir>
#include <QCoreApplication>

/*!
 * \brief 日志记录器参数类的实现类
 */
CMDSysLoggerParaImp::CMDSysLoggerParaImp()
{
    m_name = "";
    m_comment = "";

    m_fileDir = buildDefaultFileDir();
    m_subFileDir = "";
    m_fileMainName = "logs"; //! 主文件名（后缀固定为 .txt）
    m_maxBackupFileIndex = 3; //! 最大文件数，只保留最近的指定个数的文件
    m_maxFileSize = 1 * 1024 * 1024;

    m_enableListAppender = false; //! 是否允许向内存链表添加日志记录
    m_maxCountInList = 2048; //! 内存链表最大长度
    m_enableFileAppender = true; //! 是否允许向文本文件添加日志记录

    m_engineType = CMDSysLoggerPara::LogEngineType::NoEngine;

    m_enableOutput = true;
}

CMDSysLoggerParaImp::CMDSysLoggerParaImp(const CMDSysLoggerParaImp& obj)
{
    m_name = obj.m_name;
    m_comment = obj.m_comment;

    m_enableListAppender = obj.m_enableListAppender;
    m_maxCountInList = obj.m_maxCountInList;

    m_enableFileAppender = obj.m_enableFileAppender;
    m_fileDir = obj.m_fileDir;
    m_subFileDir = obj.m_subFileDir;
    m_fileMainName = obj.m_fileMainName;
    m_maxBackupFileIndex = obj.m_maxBackupFileIndex;
    m_maxFileSize = obj.m_maxFileSize;

    m_engineType = obj.m_engineType;
    m_enableOutput = obj.m_enableOutput;
}

CMDSysLoggerParaImp& CMDSysLoggerParaImp::operator = (const CMDSysLoggerParaImp& obj)
{
    if( this != &obj )
    {
        m_name = obj.m_name;
        m_comment = obj.m_comment;

        m_enableListAppender = obj.m_enableListAppender;
        m_maxCountInList = obj.m_maxCountInList;

        m_enableFileAppender = obj.m_enableFileAppender;
        m_fileDir = obj.m_fileDir;
        m_subFileDir = obj.m_subFileDir;
        m_fileMainName = obj.m_fileMainName;
        m_maxBackupFileIndex = obj.m_maxBackupFileIndex;
        m_maxFileSize = obj.m_maxFileSize;

        m_engineType = obj.m_engineType;
        m_enableOutput = obj.m_enableOutput;
    }
    return *this;
}

QString CMDSysLoggerParaImp::buildDefaultFileDir()
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString defaultFileDir;
    #ifdef Q_OS_LINUX
        defaultFileDir = appPath + "/logs/";
    #else // windows
        defaultFileDir = appPath + "\\logs\\";
    #endif
     return defaultFileDir;
}

/*!
 * \brief 日志记录器类的实现类
 */
CMDSysLoggerImp::CMDSysLoggerImp()
{
    m_Log4Qt_Logger = NULL;
    m_listAppender = NULL;
    m_logEngine = NULL;
}

CMDSysLoggerImp::~CMDSysLoggerImp()
{
}

/*!
\section 初始化与释放
*/

// 2015-7-8 bug备忘：必须为每个追加器使用独立的布局对象，如果共用布局对象的话，
// 释放追加器的时候会报错，错误似乎与引用计数有关，但具体原因还未查明。
// 跟踪：CMDSysLoggerImp::release 的 m_Log4Qt_Logger->removeAllAppenders();
// ------------------------------------------------------------------------------------------------------------------
bool CMDSysLoggerImp::init(Log4Qt::Logger* logger, const CMDSysLoggerPara& para)
{
    if(m_Log4Qt_Logger)
        return false;

    m_para = para;

    m_Log4Qt_Logger = logger;
    if(m_Log4Qt_Logger)
    {
        if(para.isEnableListAppender())
        {
            Log4Qt::MindSCADAListAppender* listAppender = new Log4Qt::MindSCADAListAppender(newLayout());
            listAppender->setName(QLatin1String("List Appender for MindSCADA System Log"));
            listAppender->setMaxCount(para.maxCountInList());
            listAppender->activateOptions();
            m_Log4Qt_Logger->addAppender(listAppender);

            m_listAppender = listAppender; // 记录了内存链表的指针，快速访问日志记录
                                                                 // （不需要每次都按名字查找追加器）
        }

        if(para.isEnableFileAppender())
        {
            QString existedFileDir = makeSureFileDirExist(para.fileDir() + para.subFileDir());
            QString mainFile = existedFileDir + para.fileMainName() + ".txt";
            Log4Qt::MindSCADARollingFileAppender* rollFileAppender = new Log4Qt::MindSCADARollingFileAppender(newLayout(), mainFile);
            rollFileAppender->setMaximumFileSize(para.maxFileSize());
            rollFileAppender->setMaxBackupIndex(para.maxBackupFileIndex());
            rollFileAppender->setName(QLatin1String("RollFile Appender for MindSCADA System Log"));
            rollFileAppender->setAppendFile(true);  // 追加到当前主文件，之前的内容被保留
            rollFileAppender->activateOptions();
            m_Log4Qt_Logger->addAppender(rollFileAppender);
        }

        initEngine(para); // 初始化日志引擎
    }
    return (m_Log4Qt_Logger != NULL);
}

void CMDSysLoggerImp::release()
{
    releaseEngine();

    if(m_Log4Qt_Logger)
    {
        m_Log4Qt_Logger->removeAllAppenders();
        m_Log4Qt_Logger = NULL;
    }
}

Log4Qt::MindSCADALayout* CMDSysLoggerImp::newLayout()
{
    Log4Qt::MindSCADALayout* nsLayout = new Log4Qt::MindSCADALayout();
    if(nsLayout)
    {
        nsLayout->setName(QLatin1String("Layout for MindSCADA System Log"));
        nsLayout->activateOptions();
    }
    return nsLayout;
}

QString CMDSysLoggerImp::makeSureFileDirExist(const QString& fileDir)
{
    if(helper_makeSureFileDirExist(fileDir))
        return fileDir;

    QString dFileDir = CMDSysLoggerParaImp::buildDefaultFileDir();
    if(helper_makeSureFileDirExist(dFileDir))
        return dFileDir;

    QString appDir = QCoreApplication::applicationDirPath();
    #ifdef Q_OS_LINUX
        appDir = appDir + "/";
    #else // windows
        appDir = appDir + "\\";
    #endif
    return appDir;
}

bool CMDSysLoggerImp::helper_makeSureFileDirExist(const QString& fileDir)
{
    QDir dir(fileDir);
    dir.makeAbsolute();
    if(!dir.exists())
        dir.mkpath(fileDir);
    return dir.exists();
}

void CMDSysLoggerImp::initEngine(const CMDSysLoggerPara& para)
{
    if(CMDSysLoggerPara::LogEngineType::Default == para.logEngineType())
    {
        m_logEngine = &CMDSysLogExpertImp::m_defaultEngine;
    }
    else if(CMDSysLoggerPara::LogEngineType::Independant == para.logEngineType())
    {
        m_logEngine = new CMDSysLogEngine;
    }
    else{};

    if(m_logEngine && !m_logEngine->isRunning())
    {
       m_logEngine->setLogger(this);
       m_logEngine->start();
    }
}

void CMDSysLoggerImp::releaseEngine()
{
    if(!m_logEngine)
        return;

    m_logEngine->stop();
    if(m_logEngine != &CMDSysLogExpertImp::m_defaultEngine)
    {
        delete m_logEngine;
        m_logEngine = NULL;
    }
}

CMDSysLogData* CMDSysLoggerImp::helper_newLogData(const Log4Qt::LoggingEvent& logging, CMDSysLogData* recycleLog)
{
    CMDSysLogData* log = (recycleLog ? recycleLog : new CMDSysLogData);
    if(log)
        log->m_imp->setLogging(logging);
    return log;
}

/*!
\section 记录到日志；访问最近的日志
*/
void CMDSysLoggerImp::logInfo(const QString& msg, const QString& source)
{
    if(!m_para.isEnableOutput())
        return;

    if(m_logEngine)
        m_logEngine->asyncProcessLog(Log4Qt::Level::INFO_INT, msg, source);
    else
        processLog(Log4Qt::Level::INFO_INT, msg, source);
}

void CMDSysLoggerImp::logWarn(const QString& msg, const QString& source)
{
    if(!m_para.isEnableOutput())
        return;

    if(m_logEngine)
        m_logEngine->asyncProcessLog(Log4Qt::Level::WARN_INT, msg, source);
    else
        processLog(Log4Qt::Level::WARN_INT, msg, source);
}

void CMDSysLoggerImp::logError(const QString& msg, const QString& source)
{
    if(!m_para.isEnableOutput())
        return;

    if(m_logEngine)
        m_logEngine->asyncProcessLog(Log4Qt::Level::ERROR_INT, msg, source);
    else
        processLog(Log4Qt::Level::ERROR_INT, msg, source);
}

void CMDSysLoggerImp::processLog(Log4Qt::Level level, const QString& msg, const QString& source)
{
    if(!m_Log4Qt_Logger)
        return;

    QString source_msg = source + Log4Qt::MindSCADALayout::sourceSeperator() + msg;
    switch (level.toInt())
    {
    case Log4Qt::Level::INFO_INT:
            m_Log4Qt_Logger->info(source_msg);
        break;
    case Log4Qt::Level::WARN_INT:
            m_Log4Qt_Logger->warn(source_msg);
        break;
    case Log4Qt::Level::ERROR_INT:
            m_Log4Qt_Logger->error(source_msg);
        break;
    default:
        break;
    }
}

int CMDSysLoggerImp::logCount()
{
    return (m_listAppender ? m_listAppender->count() : 0);
}

CMDSysLogData* CMDSysLoggerImp::log(int index)
{
    return (m_listAppender ? m_listAppender->at(index) : NULL);
}

