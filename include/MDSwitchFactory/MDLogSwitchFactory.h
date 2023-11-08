#ifndef MDLOGSWITCHFACTORY_H
#define MDLOGSWITCHFACTORY_H

#include "MDSwitchFactory_Global.h"
#include "MDSwitchFactory.h"
#include "../../include/MDSysLogExpert/MDSysLogger.h"

class CMDLogSwitch;
class MDSWITCHFACTORYSHARED_EXPORT CMDLogSwitchFactory : public CMDSwitchFactory
{
public:
    CMDLogSwitchFactory();
    CMDLogSwitchFactory(const CMDMObjPara& para);
    virtual~CMDLogSwitchFactory();
public:
    static CMDLogSwitch* createSwitch(const QString& pid, const QString& group, const QString& comment, CMDSysLogger* logger);
    static CMDLogSwitch* createSwitch(const QString& pid, const QString& group, const QString& comment, const CMDSysLoggerPara& loggerPara);
    static CMDLogSwitch* createSwitchWithLogger(const QString& pid, const QString& group, const QString& comment, const QString& loggerName = "");
    static CMDLogSwitch* createSwitchWithSubFileDir(const QString& pid, const QString& group, const QString& comment, const QString& subFileDir = "");
protected:
    static CMDLogSwitch* createSwitch(const QString& pid, const QString& group, const QString& comment);
public:
    static CMDLogSwitch* findByGroup(const QString& pid, const QString& group);
protected:
    virtual CMDMObj* newMObj(const CMDMObjPara& para);
    virtual CMDMObjFactory* newFactory(const CMDMObjPara& para);
};

class MDSWITCHFACTORYSHARED_EXPORT CMDLogSwitch : public CMDSwitch
{
    friend class CMDLogSwitchFactory;
protected:
    CMDLogSwitch();
    CMDLogSwitch(const CMDMObjPara& para, CMDMObjFactory* factory);
    virtual~CMDLogSwitch();
    //! 如果开关状态为 on，记录日志（调用频率不高时使用，代码更简洁）
    //! 代码示例：
    //! m_xxSwitch->LOG_INFO("日志消息");
public:
    void LOG_INFO(const QString& msg, const QString& source = "");
    void LOG_WARN(const QString& msg, const QString& source = "");
    void LOG_ERROR(const QString& msg, const QString& source = "");
    //! 忽略开关状态，直接记录日志（调用频率高时使用，避免不必要的开销）
    //! 代码示例：
    //! if(m_xxSwitch->isOn())
    //!     m_xxSwitch->LOG_INFO_IS("日志消息");
    //! 当开关关闭时，可避免字符串的构造和释放引起的开销
public:
    void LOG_INFO_IS(const QString& msg, const QString& source = "");
    void LOG_WARN_IS(const QString& msg, const QString& source = "");
    void LOG_ERROR_IS(const QString& msg, const QString& source = "");
public:
    CMDSysLogger* logger();
};

#endif // MDLOGSWITCHFACTORY_H
