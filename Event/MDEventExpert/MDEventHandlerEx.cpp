#include "MDEventHandlerEx.h"

#include "MDEventHandler.h"
#include "MDEventConstant.h"
#include "MDRawEvent.h"
#include "MDEvent.h"
#include "MDEventContacter.h"

#include "EPC.h"
#include "MDEventProperty_Runtime.h"
#include "MDLogMessage.h"

/*!
 * \brief 事件处理器类
 */
CMDEventHandlerEx::CMDEventHandlerEx(CMDEvent* event)
    : CMDEventHandler(event)
{
    m_lsEventProcess = NULL;
    m_eventContacter = NULL;

    if(event && event->configProperty() &&
        MDEventType::combined == event->configProperty()->type())
    {
        m_eventExpression.compile(event->configProperty()->expressionString());
    }
}

CMDEventHandlerEx::~CMDEventHandlerEx()
{

}

/*!
\section 实现事件处理流程
*/
bool CMDEventHandlerEx::ProcessRawEvent(CMDRawEvent* rawEvent)
{
    m_lsEventProcess = rawEvent->logSwitch();

    CMDEvent* event = nsEvent();
    CMDEventProperty_Runtime* runtimeProperty = (event ? event->runtimeProperty() : NULL);
    if(!runtimeProperty)
        return false;

    // 判断自身是否被触发
    // --------------------------------------------
    qint8 result = CheckRawEvent(rawEvent);
    if(-1 == result) // -1：原始事件与本事件类型不匹配，不做任何处理
    {
        if(m_lsEventProcess->isOn())
            m_lsEventProcess->LOG_ERROR("原始事件与本事件类型不匹配，不做任何处理");
        return false;
    }

    bool boolResult = (1 == result);
    runtimeProperty->setExpFlag(boolResult);    // 设置表达式状态

    if(boolResult)
    {
        if(m_lsEventProcess->isOn())
            m_lsEventProcess->LOG_INFO_IS("项目事件被触发，准备执行动作...");
        executeAction();
    }

    // 判断关联事件是否被触发，关联事件的结果依赖于本事件的表达式状态
    // ---------------------------------------------------------------------------------
    handleRelatedEvent();

    // 关联事件判断完毕，是否自动重置表达式状态？
    // -----------------------------------------------------------------
    if(runtimeProperty->autoResetExpFlag())
        runtimeProperty->setExpFlag(false);

    return true;
}

qint8 CMDEventHandlerEx::CheckRawEvent(CMDRawEvent* rawEvent)
{
    return -1;
}

bool CMDEventHandlerEx::handleRelatedEvent()
{
    CMDEvent* event = nsEvent();
    QList<CMDEvent*>* relatedEventList = (event ? event->relatedEventList() : NULL);
    if(!relatedEventList)
        return false;

    if(m_lsEventProcess->isOn())
    {
        QString rawEventString = QString("%1").arg(relatedEventList->size());
        m_lsEventProcess->LOG_INFO_IS("项目事件关联了组合事件，数量=" + rawEventString);
    }

    for (int i = 0; i < relatedEventList->size(); ++i)
    {
        CMDEvent* relatedEvent = relatedEventList->at(i);

        if(m_lsEventProcess->isOn())
            m_lsEventProcess->LOG_INFO_IS("开始处理关联的组合事件：" + relatedEvent->configProperty()->name());

        CMDEventHandlerEx* handler = (CMDEventHandlerEx*)(relatedEvent ? relatedEvent->handler() : NULL);
        if(handler)
        {
            if(m_lsEventProcess->isOn())
                m_lsEventProcess->LOG_INFO_IS("评估组合事件表达式：" + relatedEvent->configProperty()->expressionString());
            bool result = handler->CheckEventExpression();
            if(result)
            {
                if(m_lsEventProcess->isOn())
                    m_lsEventProcess->LOG_INFO_IS("组合事件被触发，执行动作");

                handler->executeAction();
            }
        }

        if(m_lsEventProcess->isOn())
            m_lsEventProcess->LOG_INFO_IS("结束处理关联的组合事件：" + relatedEvent->configProperty()->name());
    }

    return true;
}

/*!
\section 计算事件表达式
*/
bool CMDEventHandlerEx::CheckEventExpression()
{
    CMDEvent* event = nsEvent();
    QList<CMDEvent*>* relatedEventList = (event ? event->relatedEventList() : NULL);
    if(!relatedEventList)
        return false;

    for (int i = 0; i < relatedEventList->size(); ++i)
    {
        CMDEvent* relatedEvent = relatedEventList->at(i);
        m_eventExpression.setVariableValue(relatedEvent->configProperty()->name(),
                                                                    QVariant(relatedEvent->runtimeProperty()->expFlag()));
    }

    QVariant vResult = m_eventExpression.excute();
    bool result = vResult.toBool();
    return result;
}

void CMDEventHandlerEx::getEventNameInExpression(QStringList& EventNameList)
{
    EventNameList = m_eventExpression.referencedVarList();
}

/*!
\section 事件触发后，执行动作或记录到日志
*/
bool CMDEventHandlerEx::executeAction()
{
    CMDEvent* event = nsEvent();

    if(event->configProperty()->isEnableLog())
    {
        log(event);
    }

    QList<CMDAction*>* actionList = (event ? event->actionList() : NULL);
    if(actionList)
    {
        if(m_lsEventProcess->isOn())
        {
            QString para = QString("%1").arg(actionList->size());
            m_lsEventProcess->LOG_INFO_IS("项目事件关联了动作，数量=" + para);
        }

        foreach (CMDAction* ac, *actionList)
        {
            if(m_lsEventProcess->isOn())
                m_lsEventProcess->LOG_INFO_IS("开始执行动作：" + ac->configProperty()->name());

            m_eventContacter->executeAction(event, ac);
        }
    }

    return true;
}

bool CMDEventHandlerEx::log(CMDEvent* event)
{
    CMDLogMessage msg;
    msg.setMsg("事件触发：" +
                            event->configProperty()->name() + "[" +
                            event->configProperty()->comment() + "]");
    return m_eventContacter->log(&msg);
}

void CMDEventHandlerEx::setEventContacter(CMDEventContacter* eventContacter)
{
    m_eventContacter = eventContacter;
}

/*!
 * \brief 变量数值事件处理器类
 */
CMDVariantValueEventHandler::CMDVariantValueEventHandler(CMDEvent* nsEvent)
    : CMDEventHandlerEx(nsEvent)
{

}

CMDVariantValueEventHandler::~CMDVariantValueEventHandler()
{

}

qint8 CMDVariantValueEventHandler::CheckRawEvent(CMDRawEvent* rawEvent)
{
    qint8 result = -1;

    CMDEvent* event = nsEvent();
    MDEventType type = (event ? event->type() : MDEventType::null_event);

    // 原始事件类型与事件类型并不完全对应，
    // 例如，原始变量事件有“数值变化”，
    // 而事件则会细化为“数值变化”和“数值变化范围”
    if(MDEventType::variable_ValueChanged == type)
    {
        result = 1;
    }
    else if(MDEventType::variable_ValueChangedInRange == type)
    {
        // 如果是第一次评估，将表达式构造并编译好，后续的评估只需要
        // 进行表达式计算即可
        if(event->configProperty()->expressionString().isEmpty())
        {
            CEPC_VariableValueChangedInRange* prop = (CEPC_VariableValueChangedInRange*)event->configProperty();
            CValueRangeCondition condition = prop->rangeCondition();
            QString conditionExpression = condition.toExpression(event->configProperty()->objName());
            m_eventExpression.compile(conditionExpression);
        }

        // 计算表达式
        CMDRawVariableEvent* rawVE = (CMDRawVariableEvent*)rawEvent;
        IMDChangeVariable* icv = rawVE->changedVariable();
        m_eventExpression.setVariableValue(event->configProperty()->objName(),
                                                                    icv->currentValue());
        QVariant vResult = m_eventExpression.excute();
        result = (vResult.toBool() ? 1 : 0);
    }
    else{};

    return result;
}

/*!
 * \brief 变量品质事件处理器类
 */
CMDVariantQualityEventHandler::CMDVariantQualityEventHandler(CMDEvent* nsEvent)
    : CMDEventHandlerEx(nsEvent)
{

}

CMDVariantQualityEventHandler::~CMDVariantQualityEventHandler()
{

}

qint8 CMDVariantQualityEventHandler::CheckRawEvent(CMDRawEvent* rawEvent)
{
    qint8 result = -1;

    CMDEvent* event = nsEvent();
    MDEventType type = (event ? event->type() : MDEventType::null_event);

    if(type == rawEvent->eventType() &&
        MDEventType::variable_QualityChanged == rawEvent->eventType())
    {
        // 如果是第一次评估，将表达式构造并编译好，后续的评估只需要
        // 进行表达式计算即可
        if(event->configProperty()->expressionString().isEmpty())
        {
            CEPC_VariableQualityChanged* prop = (CEPC_VariableQualityChanged*)event->configProperty();
            CValueRangeCondition condition = prop->rangeCondition();
            QString conditionExpression = condition.toExpression(event->configProperty()->objName());
            m_eventExpression.compile(conditionExpression);
        }

        // 计算表达式
        CMDRawVariableEvent* rawVE = (CMDRawVariableEvent*)rawEvent;
        IMDChangeVariable* icv = rawVE->changedVariable();
        QVariant vQuality = QVariant((short)icv->currentQuality());
        m_eventExpression.setVariableValue(event->configProperty()->objName(),
                                                                    vQuality);
        QVariant vResult = m_eventExpression.excute();
        result = (vResult.toBool() ? 1 : 0);
    }
    return result;
}

/*!
 * \brief 周期事件处理器类
 */
CMDTimerCycleEventHandler::CMDTimerCycleEventHandler(CMDEvent* nsEvent)
    : CMDEventHandlerEx(nsEvent)
{
    m_lastTimeStamp = QDateTime::currentDateTime();

    CEPC_TimeCycle* prop = (CEPC_TimeCycle*)nsEvent->configProperty();
    m_timeCycle = prop->timeCycle();
}

CMDTimerCycleEventHandler::~CMDTimerCycleEventHandler()
{

}

qint8 CMDTimerCycleEventHandler::CheckRawEvent(CMDRawEvent* rawEvent)
{
    qint8 result = -1;

    CMDEvent* event = nsEvent();
    MDEventType type = (event ? event->type() : MDEventType::null_event);

    if(type == rawEvent->eventType() &&
        MDEventType::timer_Cycle == rawEvent->eventType())
    {
        QDateTime curDateTime = QDateTime::currentDateTime();
        qint64 timeSpan = m_lastTimeStamp.msecsTo(curDateTime);
        if(timeSpan >= m_timeCycle)
        {
            m_lastTimeStamp = curDateTime;
            result = 1;
        }
    }
    return result;
}
