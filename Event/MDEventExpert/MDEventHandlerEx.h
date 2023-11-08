/*!

\author dGFuZ3p3

\section 功能

事件处理器类及其派生类。

*/

#ifndef MDEVENTHANDLEREX_H
#define MDEVENTHANDLEREX_H

#include "../../include/MDExpression/MDExpression.h"
#include "MDEventHandler.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"

class CMDEvent;
class CMDRawEvent;
class CMDEventContacter;
/*!
 * \brief 事件处理器类
 */
class CMDLogSwitch;
class CMDEventHandlerEx : public CMDEventHandler
{
public:
    CMDEventHandlerEx(CMDEvent* nsEvent);
    virtual ~CMDEventHandlerEx();
    CMDLogSwitch* m_lsEventProcess;
/*!
\section 实现事件处理流程
*/
public:
    bool ProcessRawEvent(CMDRawEvent* rawEvent);
    virtual qint8 CheckRawEvent(CMDRawEvent* rawEvent);
    bool handleRelatedEvent();

/*!
\section 计算事件表达式
*/
public:
    bool CheckEventExpression();
    void getEventNameInExpression(QStringList& EventNameList);
protected:
    CMDExpression m_eventExpression;

/*!
\section 事件触发后，执行动作或记录到日志
*/
public:
    void setEventContacter(CMDEventContacter* eventContacter);
protected:
    bool executeAction();
    bool log(CMDEvent* event);
    CMDEventContacter* m_eventContacter;
};

/*!
 * \brief 变量数值事件处理器类
 */
class CMDVariantValueEventHandler : public CMDEventHandlerEx
{
public:
    CMDVariantValueEventHandler(CMDEvent* nsEvent);
    virtual ~CMDVariantValueEventHandler();
public:
    qint8 CheckRawEvent(CMDRawEvent* rawEvent);    
};

/*!
 * \brief 变量品质事件处理器类
 */
class CMDVariantQualityEventHandler : public CMDEventHandlerEx
{
public:
    CMDVariantQualityEventHandler(CMDEvent* nsEvent);
    virtual ~CMDVariantQualityEventHandler();
public:
    qint8 CheckRawEvent(CMDRawEvent* rawEvent);
};

/*!
 * \brief 周期事件处理器类
 */
class CMDTimerCycleEventHandler : public CMDEventHandlerEx
{
public:
    CMDTimerCycleEventHandler(CMDEvent* nsEvent);
    virtual ~CMDTimerCycleEventHandler();
public:
    quint32 m_timeCycle;
    QDateTime m_lastTimeStamp;
    qint8 CheckRawEvent(CMDRawEvent* rawEvent);
};

#endif // MDEVENTHANDLEREX_H
