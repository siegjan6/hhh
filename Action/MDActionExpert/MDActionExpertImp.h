/*!

\author dGFuZ3p3

\section 功能

动作专家类，实现运行时的动作处理。

*/

#ifndef MDACTIONEXPERTIMP_H
#define MDACTIONEXPERTIMP_H

#include <QString>
#include <QList>
#include <QMap>
#include "MDActionConstant.h"
#include "MDActionConfiger.h"
#include "MDDelayActionMgr.h"
#include "CommandEngineForCheckActionSP.h"
#include "MDSwitchFactory/MDFuncSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"

class CMDAction;
class CMDActionEngine;
class CMDActionContacter;
class CMDActionProperty_Config;
class CMDActionSP;
class CMDActionExpertImp
{
public:
    CMDActionExpertImp();
    ~CMDActionExpertImp();
/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, CMDActionConfiger* configer, CMDActionContacter* actionContacter, unsigned long timeout);
    bool load();
    void unload();
public:
    CMDActionConfiger* m_configer ;// NULL; //! 配置器
protected:
    QString m_pid;
    CMDActionContacter* m_actionContacter ;// NULL;
    unsigned long m_timeout;
public:
    bool start();
    void stop();
public:
    //! 启动与停止【动作处理引擎】
    bool startActionEngine();
    void stopActionEngine();
    CMDActionEngine* actionEngine();
protected:
    CMDActionEngine* m_actionEngine ;// NULL;

/*!
\section 处理需要执行的动作
*/
public:
    bool asyncProcessAction(const QString& actionName);
    bool asyncProcessAction(CMDAction* action);
    bool asyncProcessAction_AfterCheckSP(CMDAction* action);
    bool asyncProcessAction_NoSP(CMDAction* action);
public:
    bool asyncProcessAction_NoDelay(CMDAction* action);
    bool syncProcessAction(CMDAction* action);
    bool log(CMDAction* action);
protected:
    CMDDelayActionMgr m_delayMgr; //! 管理需要延时处理的动作
protected:
    CCommandEngineForCheckActionSP* m_checkActionSPEngine ;// NULL; //! 管理执行前需要进行安全检查的动作
protected:
    void createSwitchs(bool create);
    CMDSwitch* m_sProcessAction;
    CMDLogSwitch* m_lsActionProcess;

/*!
\section 管理项目定义的动作对象
*/
public:
    CMDAction* findAction(const QString& name);
protected:
    void initRuntimeActionPool();
    CMDAction* new_RuntimeAction(CMDActionProperty_Config* cp, CMDActionContacter* actionContacter);
    void releaseRuntimeActionPool();
    QMap<QString, CMDAction*> m_runtimeActionMap;
protected:
    CMDActionSP* helperGetExecuteSP_OnlyIfSPEnabled(CMDActionProperty_Config* cp);
};

#endif // MDACTIONEXPERTIMP_H
