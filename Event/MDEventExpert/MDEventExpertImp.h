/*!

\author dGFuZ3p3

\section 功能

事件专家类，实现运行时的事件处理。

*/

#ifndef MDEVENTEXPERTIMP_H
#define MDEVENTEXPERTIMP_H

#include <QString>
#include "MDEventProperty_Config.h"
#include "MDEventConfiger.h"
#include "MDSwitchFactory/MDFuncSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"

class CMDRawEvent;
class CContainerForObjectWithEvent;
class CMDEvent;
class CMDEventEngine;
class CMDEventContacter;
class CMDEventExpertImp
{
public:
    CMDEventExpertImp();
    ~CMDEventExpertImp();

/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, CMDEventConfiger* configer, CMDEventContacter* eventContacter, unsigned long timeout);
    bool load();
    void unload();
public:
    CMDEventConfiger* m_configer ;// NULL;
    QString m_pid;
    CMDEventContacter* m_eventContacter ;// NULL;
    unsigned long m_timeout;
public:
    bool start();
    void stop();

//! 启动与停止【事件处理引擎】
public:
    bool startEventEngine();
    void stopEventEngine();
protected:
    CMDEventEngine* m_eventEngine ;// NULL;

/*!
\section 处理事件
*/
public:
    bool asyncProcessRawEvent(CMDRawEvent* rawEvent);
    bool syncProcessRawEvent(CMDRawEvent* rawEvent);
protected:
    void createSwitchs(bool create);
    CMDLogSwitch* m_lsEventProcess;

/*!
\section 容器，将事件根据对象类型分类管理
*/
public:
    bool createContainerForObjectWithEvent();
    void destroyContainerForObjectWithEvent();
protected:
    CContainerForObjectWithEvent* m_projectObjContainer ;// NULL;
    CContainerForObjectWithEvent* m_variantObjContainer_Value ;// NULL;
    CContainerForObjectWithEvent* m_variantObjContainer_Quality ;// NULL;
    CContainerForObjectWithEvent* m_timeObjContainer ;// NULL;
protected:
    CContainerForObjectWithEvent* m_combinedObjContainer ;// NULL;

/*!
\section 管理项目定义的事件对象
*/
public:
    void initRuntimeEventPool();
    void releaseRuntimeEventPool();
public:
    CMDEvent* new_RuntimeEvent(CMDEventProperty_Config* configProperty);
    bool bindEvent_RuntimeActionList(CMDEvent* event);
public:
    void bindCombinedWithIndependent(const QList<CMDEvent*>& combinedEventList);
    bool bindCombinedWithIndependent(CMDEvent* combinedEvent);
protected:
    QMap<QString, CMDEvent*> m_runtimeEventMap;
};

#endif // MDEVENTEXPERTIMP_H
