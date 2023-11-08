#include "MDLogSwitchFactory.h"
#include "../../../include/MDSysLogExpert/MDSysLogExpert.h"
#include "MDSwitchFactoryImp.h"

CMDLogSwitch::CMDLogSwitch()
{
}

CMDLogSwitch::CMDLogSwitch(const CMDMObjPara& para, CMDMObjFactory* factory)
    : CMDSwitch(para, factory)
{
}

CMDLogSwitch::~CMDLogSwitch()
{
}

void CMDLogSwitch::LOG_INFO(const QString& msg, const QString& source)
{
    if(isOn())
        LOG_INFO_IS(msg, source);
}

void CMDLogSwitch::LOG_WARN(const QString& msg, const QString& source)
{
    if(isOn())
        LOG_WARN_IS(msg, source);
}

void CMDLogSwitch::LOG_ERROR(const QString& msg, const QString& source)
{
    if(isOn())
        LOG_ERROR_IS(msg, source);
}

void CMDLogSwitch::LOG_INFO_IS(const QString& msg, const QString& source)
{
    if(m_imp->m_logger)
        m_imp->m_logger->logInfo(msg, source);
}

void CMDLogSwitch::LOG_WARN_IS(const QString& msg, const QString& source)
{
    if(m_imp->m_logger)
        m_imp->m_logger->logWarn(msg, source);
}

void CMDLogSwitch::LOG_ERROR_IS(const QString& msg, const QString& source)
{
    if(m_imp->m_logger)
        m_imp->m_logger->logError(msg, source);
}

CMDSysLogger* CMDLogSwitch::logger()
{
    return m_imp->m_logger;
}

CMDLogSwitchFactory::CMDLogSwitchFactory()
{
}

CMDLogSwitchFactory::CMDLogSwitchFactory(const CMDMObjPara& para)
    : CMDSwitchFactory(para)
{

}

CMDLogSwitchFactory::~CMDLogSwitchFactory()
{
}

static CMDLogSwitchFactory g_staticLogSwitchFactory;
CMDLogSwitch* CMDLogSwitchFactory::createSwitch(const QString& pid, const QString& group, const QString& comment)
{
    CMDMObjPara para(pid, "LOG", group, comment);
    return (CMDLogSwitch*)CMDSwitchFactory::createSwitch(&g_staticLogSwitchFactory, para, false);
}

CMDLogSwitch* CMDLogSwitchFactory::createSwitch(const QString& pid, const QString& group, const QString& comment, CMDSysLogger* logger)
{
    CMDLogSwitch* ls = CMDLogSwitchFactory::createSwitch(pid, group, comment);
    if(ls)
        ls->m_imp->m_logger = logger;
    return ls;
}

CMDLogSwitch* CMDLogSwitchFactory::createSwitch(const QString& pid, const QString& group, const QString& comment, const CMDSysLoggerPara& loggerPara)
{
    QString lName = (loggerPara.name().isEmpty() ? group : loggerPara.name());
    if(!lName.contains(pid, Qt::CaseInsensitive))
        lName = lName + "_" + pid; // 根据 pid 对 logger 进行分类
    CMDSysLogger* logger = CMDSysLogExpert::createLogger(lName, loggerPara);
    return createSwitch(pid, group, comment, logger);
}

CMDLogSwitch* CMDLogSwitchFactory::createSwitchWithLogger(const QString& pid, const QString& group, const QString& comment, const QString& loggerName)
{
    QString dirSymbol;
    #ifdef Q_OS_LINUX
        dirSymbol = "/";
    #else // windows
        dirSymbol = "\\";
    #endif

    QString logPath = "logs" + dirSymbol;
    QString lName = (loggerName.isEmpty() ? group : loggerName);
    CMDSysLoggerPara loggerPara;
    loggerPara.setName(lName);
    loggerPara.setFileDir(CMDMObjPara::pidToPath(pid) + logPath);
    loggerPara.setFileMainName(lName);
    return createSwitch(pid, group, comment, loggerPara);
}

CMDLogSwitch* CMDLogSwitchFactory::createSwitchWithSubFileDir(const QString& pid, const QString& group, const QString& comment, const QString& subFileDir)
{
    QString dirSymbol;
    #ifdef Q_OS_LINUX
        dirSymbol = "/";
    #else // windows
        dirSymbol = "\\";
    #endif

    QString logPath = "logs" + dirSymbol;
    CMDSysLoggerPara loggerPara;
    loggerPara.setName(group);
    loggerPara.setFileDir(CMDMObjPara::pidToPath(pid) + logPath);
    loggerPara.setSubFileDir(subFileDir + dirSymbol);
    loggerPara.setFileMainName(group);
    return createSwitch(pid, group, comment, loggerPara);
}

CMDLogSwitch* CMDLogSwitchFactory::findByGroup(const QString& pid, const QString& group)
{
    CMDMObjPara para(pid, "LOG", group, "");
    CMDMObjFactory* f = CMDMObjFactory::findFactory(para);
    CMDMObj* mo = (f ? f->findByGroup(group) : NULL);
    return (CMDLogSwitch*)mo;
}

CMDMObj* CMDLogSwitchFactory::newMObj(const CMDMObjPara& para)
{
    CMDLogSwitch* s = new CMDLogSwitch(para, this);
    return s;
}

CMDMObjFactory* CMDLogSwitchFactory::newFactory(const CMDMObjPara& para)
{
    CMDLogSwitchFactory* f = new CMDLogSwitchFactory(para);
    f->load();
    return f;
}
