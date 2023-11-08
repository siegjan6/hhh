#include "MDSysLogger.h"
#include "MDSysLoggerImp.h"

/*!
 * \brief 日志记录器参数类
 * 被 CMDSysLogExpert::init 和 CMDSysLogExpert::createLogger 使用
 */
CMDSysLoggerPara::CMDSysLoggerPara()
{
    m_imp = new CMDSysLoggerParaImp;
}

CMDSysLoggerPara::~CMDSysLoggerPara()
{
    if(m_imp)
        delete m_imp;
}

CMDSysLoggerPara::CMDSysLoggerPara(const CMDSysLoggerPara& obj)
{
    m_imp = new CMDSysLoggerParaImp;
    if(m_imp && obj.m_imp)
        *m_imp = *obj.m_imp;
}

CMDSysLoggerPara& CMDSysLoggerPara::operator = (const CMDSysLoggerPara& obj)
{
    if( this != &obj )
    {
        if(m_imp && obj.m_imp)
            *m_imp = *obj.m_imp;
    }
    return *this;
}

QString CMDSysLoggerPara::name() const
{
    return (m_imp ? m_imp->m_name : "");
}
void CMDSysLoggerPara::setName(const QString& name)
{
    if(m_imp)
        m_imp->m_name = name;
}

QString CMDSysLoggerPara::comment() const
{
    return (m_imp ? m_imp->m_comment : "");
}
void CMDSysLoggerPara::setComment(const QString& comment)
{
    if(m_imp)
        m_imp->m_comment = comment;
}

bool CMDSysLoggerPara::isEnableListAppender() const
{
    return (m_imp ? m_imp->m_enableListAppender : false);
}
void CMDSysLoggerPara::enableListAppender(bool enable)
{
    if(m_imp)
        m_imp->m_enableListAppender = enable;
}

int CMDSysLoggerPara::maxCountInList() const
{
    return (m_imp ? m_imp->m_maxCountInList : 0);
}
void CMDSysLoggerPara::setMaxCountInList(int maxCount)
{
    if(m_imp)
        m_imp->m_maxCountInList = maxCount;
}

bool CMDSysLoggerPara::isEnableFileAppender() const
{
    return (m_imp ? m_imp->m_enableFileAppender : false);
}
void CMDSysLoggerPara::enableFileAppender(bool enable)
{
    if(m_imp)
        m_imp->m_enableFileAppender = enable;
}

QString CMDSysLoggerPara::fileDir() const
{
    return (m_imp ? m_imp->m_fileDir : "");
}
void CMDSysLoggerPara::setFileDir(const QString& fileDir)
{
    if(m_imp)
        m_imp->m_fileDir = fileDir;
}

QString CMDSysLoggerPara::subFileDir() const
{
    return (m_imp ? m_imp->m_subFileDir : "");
}
void CMDSysLoggerPara::setSubFileDir(const QString& subFileDir)
{
    if(m_imp)
        m_imp->m_subFileDir = subFileDir;
}

QString CMDSysLoggerPara::fileMainName() const
{
    return (m_imp ? m_imp->m_fileMainName : "");
}
void CMDSysLoggerPara::setFileMainName(const QString& fileMainName)
{
    if(m_imp)
        m_imp->m_fileMainName = fileMainName;
}

int CMDSysLoggerPara::maxBackupFileIndex() const
{
    return (m_imp ? m_imp->m_maxBackupFileIndex : 0);
}
void CMDSysLoggerPara::setMaxBackupFileIndex(int maxIndex)
{
    if(m_imp)
        m_imp->m_maxBackupFileIndex = maxIndex;
}

qint64 CMDSysLoggerPara::maxFileSize() const
{
    return (m_imp ? m_imp->m_maxFileSize : 0);
}
void CMDSysLoggerPara::setMaxFileSize(qint64 maxFileSize)
{
    if(m_imp)
        m_imp->m_maxFileSize = maxFileSize;
}

CMDSysLoggerPara::LogEngineType CMDSysLoggerPara::logEngineType() const
{
    return (m_imp ? m_imp->m_engineType : CMDSysLoggerPara::LogEngineType::NoEngine);
}
void CMDSysLoggerPara::setLogEngineType(LogEngineType type)
{
    if(m_imp)
        m_imp->m_engineType = type;
}

bool CMDSysLoggerPara::isEnableOutput() const
{
    return (m_imp ? m_imp->m_enableOutput : false);
}
void CMDSysLoggerPara::enableOutput(bool enable)
{
    if(m_imp)
        m_imp->m_enableOutput = enable;
}

/*!
 * \brief 日志记录器类
  */
CMDSysLogger::CMDSysLogger()
{
    m_imp = new CMDSysLoggerImp;
}

CMDSysLogger::~CMDSysLogger()
{
    if(m_imp)
        delete m_imp;
}

CMDSysLoggerPara& CMDSysLogger::para() const
{
    return m_imp->m_para;
}

void CMDSysLogger::logInfo(const QString& msg, const QString& source)
{
    if(m_imp)
        m_imp->logInfo(msg, source);
}

void CMDSysLogger::logWarn(const QString& msg, const QString& source)
{
    if(m_imp)
        m_imp->logWarn(msg, source);
}

void CMDSysLogger::logError(const QString& msg, const QString& source)
{
    if(m_imp)
        m_imp->logError(msg, source);
}

int CMDSysLogger::logCount()
{
    return (m_imp ? m_imp->logCount() : 0);
}

CMDSysLogData* CMDSysLogger::log(int iIndex)
{
    return (m_imp ? m_imp->log(iIndex) : 0);
}
