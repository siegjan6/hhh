#include "MDEventExpertImp.h"
#include "MDObjectWithEvent.h"
#include "MDRawEvent.h"
#include "MDEventHandler.h"
#include "MDEvent.h"
#include "MDEventEngine.h"
#include "MDEventClassFactory.h"
#include "MDEventContacter.h"

#include "MDSysLogExpert.h"
#define LOG_SOURCE "Event"

CMDEventExpertImp::CMDEventExpertImp()
{
    m_pid = "";
    m_timeout = 0;
    m_configer = NULL;
    m_eventContacter = NULL;
    m_eventEngine = NULL;
    m_projectObjContainer = NULL;
    m_variantObjContainer_Value = NULL;
    m_variantObjContainer_Quality = NULL;
    m_timeObjContainer = NULL;
    m_combinedObjContainer = NULL;

    m_lsEventProcess = NULL;
}

CMDEventExpertImp::~CMDEventExpertImp()
{
//    stop(2000);
}

/*!
\section 初始化与释放
*/
void CMDEventExpertImp::setParaBeforeLoad(const QString& pid, CMDEventConfiger* configer, CMDEventContacter* eventContacter, unsigned long timeout)
{
    m_pid = pid;
    m_configer = configer;
    m_eventContacter = eventContacter;
    m_timeout = timeout;
}

bool CMDEventExpertImp::load()
{
    createSwitchs(true);

    start();

    return true;
}

void CMDEventExpertImp::unload()
{
    stop();

    createSwitchs(false);
}

void CMDEventExpertImp::createSwitchs(bool create)
{
    if(create)
    {
        QString subFileDir = "ProjectExpert";
        m_lsEventProcess = CMDLogSwitchFactory::createSwitchWithSubFileDir(m_pid, "EventExpert.EventProcess", "事件处理流程", subFileDir);
    }
    else
    {
        m_lsEventProcess = NULL;
    }
}

bool CMDEventExpertImp::start()
{
    CMDLogSwitch* lsProjectLoad = CMDLogSwitchFactory::findByGroup(m_pid, "ProjectExpert.Load_and_Unload");

    lsProjectLoad->LOG_INFO("创建运行时事件");
    createContainerForObjectWithEvent();

    initRuntimeEventPool();

    lsProjectLoad->LOG_INFO("启动事件引擎");
    startEventEngine();

    return true;
}

void CMDEventExpertImp::stop()
{
    CMDLogSwitch* lsProjectLoad = CMDLogSwitchFactory::findByGroup(m_pid, "ProjectExpert.Load_and_Unload");

    lsProjectLoad->LOG_INFO("停止事件引擎");
    stopEventEngine();

    lsProjectLoad->LOG_INFO("释放运行时事件");
    releaseRuntimeEventPool();

    destroyContainerForObjectWithEvent();
}

bool CMDEventExpertImp::startEventEngine()
{
    if(!m_eventEngine)
        m_eventEngine = new CMDEventEngine(this);

    return (m_eventEngine ? m_eventEngine->start() : false);
}

void CMDEventExpertImp::stopEventEngine()
{
    if(m_eventEngine)
        m_eventEngine->stop(m_timeout);
}

/*!
\section 容器，将事件根据对象类型分类管理
*/
bool CMDEventExpertImp::createContainerForObjectWithEvent()
{
    if(!m_projectObjContainer)
        m_projectObjContainer = new CContainerForObjectWithEvent;

    if(!m_variantObjContainer_Value)
        m_variantObjContainer_Value = new CContainerForObjectWithEvent;

    if(!m_variantObjContainer_Quality)
        m_variantObjContainer_Quality = new CContainerForObjectWithEvent;

    if(!m_combinedObjContainer)
        m_combinedObjContainer = new CContainerForObjectWithEvent;

    if(!m_timeObjContainer)
        m_timeObjContainer = new CContainerForObjectWithEvent;

    return true;
}

void CMDEventExpertImp::destroyContainerForObjectWithEvent()
{
    if(m_projectObjContainer)
    {
        delete m_projectObjContainer;
        m_projectObjContainer = NULL;
    }

    if(m_variantObjContainer_Value)
    {
        delete m_variantObjContainer_Value;
        m_variantObjContainer_Value = NULL;
    }

    if(m_variantObjContainer_Quality)
    {
        delete m_variantObjContainer_Quality;
        m_variantObjContainer_Quality = NULL;
    }

    if(m_combinedObjContainer)
    {
        delete m_combinedObjContainer;
        m_combinedObjContainer = NULL;
    }

    if(m_timeObjContainer)
    {
        delete m_timeObjContainer;
        m_timeObjContainer = NULL;
    }
}

/*!
\section 管理项目定义的事件对象
*/
void CMDEventExpertImp::initRuntimeEventPool()
{
    if(!m_configer)
        return;

    QList<CMDEvent*> combinedEventList;

    // 根据【配置事件】生成【运行时事件】
    QList<CMDEventProperty_Config*> configuredEventList;
    m_configer->configedEventList(configuredEventList);
    int size = configuredEventList.size();
    for(int i = 0; i < size; i++)
    {
        CMDEvent* currentEvent = 0;
        CMDEventProperty_Config* eventProperty = configuredEventList[i];
        switch (eventProperty->type())
        {
        case MDEventType::variable_ValueChanged:
        case MDEventType::variable_ValueChangedInRange:
        {
            currentEvent = new_RuntimeEvent(eventProperty);
            if(m_variantObjContainer_Value)
            {
                qint8 result;
                CMDObjectWithEvent* objectWithEvent = (CMDObjectWithEvent*)m_variantObjContainer_Value->addObj(eventProperty->objName(), MDObjectType::variable, result);
                if(objectWithEvent)
                    objectWithEvent->initAddEvent(currentEvent);
            }
        }
            break;
        case MDEventType::timer_Cycle:
        {
            currentEvent = new_RuntimeEvent(eventProperty);
            if(m_timeObjContainer)
            {
                qint8 result;
                CMDObjectWithEvent* objectWithEvent = (CMDObjectWithEvent*)m_timeObjContainer->addObj(eventProperty->name(), MDObjectType::timer, result);
                if(objectWithEvent)
                    objectWithEvent->initAddEvent(currentEvent);
            }
        }
            break;
        case MDEventType::variable_QualityChanged:
        case MDEventType::variable_ValueRead:
        case MDEventType::variable_ValueWrite:
        {
            currentEvent = new_RuntimeEvent(eventProperty);
            if(m_variantObjContainer_Quality)
            {
                qint8 result;
                CMDObjectWithEvent* objectWithEvent = (CMDObjectWithEvent*)m_variantObjContainer_Quality->addObj(eventProperty->objName(), MDObjectType::variable, result);
                if(objectWithEvent)
                    objectWithEvent->initAddEvent(currentEvent);
            }
        }
            break;
        case MDEventType::project_BeforeUnload:
        case MDEventType::project_Loaded:
        {

        }
            break;
        case MDEventType::combined:
        {
            currentEvent = new_RuntimeEvent(eventProperty);
            CMDEvent* combinedEvent = currentEvent;
            if(m_combinedObjContainer)
            {
                qint8 result;
                CMDObjectWithEvent* objectWithEvent = (CMDObjectWithEvent*)m_combinedObjContainer->addObj("_@combined@_", MDObjectType::null_obj, result);
                if(objectWithEvent)
                    objectWithEvent->initAddEvent(currentEvent);
            }
            if(combinedEvent)
                combinedEventList.append(combinedEvent);
        }
            break;
        default:
            break;
        }

        // m_runtimeEventMap 负责管理项目里全部的事件
        if(currentEvent)
            m_runtimeEventMap.insert(currentEvent->configProperty()->name().toUpper(), currentEvent);
    }

    // 将【组合事件】与【独立事件】关联，
    // 【独立事件】被触发时，关联的 【组合事件】将被判断（计算表达式）
    bindCombinedWithIndependent(combinedEventList);
}

CMDEvent* CMDEventExpertImp::new_RuntimeEvent(CMDEventProperty_Config* configProperty)
{
    CMDEvent* event = new CMDEvent(configProperty);
    if(!event)
        return NULL;

    // 将事件与处理器绑定
    CMDEventHandlerEx* newHandler = CFactoryForEventProperty::createEventHandler(configProperty->type(), event);
    if(newHandler)
    {
        newHandler->setEventContacter(m_eventContacter);
        event->setHandler(newHandler);
    }

    // 将事件与运行时的动作队列绑定
    bindEvent_RuntimeActionList(event);

    return event;
}

bool CMDEventExpertImp::bindEvent_RuntimeActionList(CMDEvent* event)
{
    if(!m_eventContacter)
        return false;

    CMDEventProperty_Config* configProperty = event->configProperty();
    if(!configProperty)
        return false;

    int actionCount = configProperty->actionNameList().size();
    for(int i = 0; i < actionCount; i++)
    {
        CMDAction* action = m_eventContacter->findAction(configProperty->actionNameList().at(i));
        if(action)
        {
            event->actionList()->append(action);
        }
    }
    return true;
}

void CMDEventExpertImp::bindCombinedWithIndependent(const QList<CMDEvent*>& combinedEventList)
{
    int size = combinedEventList.size();
    for(int i = 0; i < size; i++)
    {
        CMDEvent* combinedEvent = combinedEventList[i];

        bindCombinedWithIndependent(combinedEvent);
    }
}

bool CMDEventExpertImp::bindCombinedWithIndependent(CMDEvent* combinedEvent)
{
    CMDEventHandlerEx* handler = (CMDEventHandlerEx*)combinedEvent->handler();
    if(!handler)
        return false;

    QStringList eventNameList;
    handler->getEventNameInExpression(eventNameList);

    int size = eventNameList.size();
    for(int i = 0; i < size; i++)
    {
        QMap<QString, CMDEvent*>::const_iterator iPosition = m_runtimeEventMap.find(eventNameList[i].toUpper());
        if(iPosition != m_runtimeEventMap.end())
        {
            CMDEvent* independentEvent = iPosition.value();

            // 独立事件关联到组合事件，便于之后的关联触发
            // ----------------------------------------------------------------------------
            independentEvent->relatedEventList()->append(combinedEvent);

            // 组合事件关联到独立事件，便于之后的表达式计算
            // ----------------------------------------------------------------------------
            combinedEvent->relatedEventList()->append(independentEvent);
        }
    }
    return true;
}

void CMDEventExpertImp::releaseRuntimeEventPool()
{
    QMap<QString, CMDEvent*>::const_iterator iPosition = m_runtimeEventMap.constBegin();
    while(m_runtimeEventMap.constEnd() != iPosition)
    {
        delete iPosition.value();
        ++ iPosition;
    }
    m_runtimeEventMap.clear();
}

/*!
\section 处理事件
*/
bool CMDEventExpertImp::asyncProcessRawEvent(CMDRawEvent* rawEvent)
{
    rawEvent->setLogSwitch(m_lsEventProcess);
    if(m_lsEventProcess->isOn())
    {
        QString rawEventString = QString("name = %1, type = %2").arg(rawEvent->rawObjName())
                                                                                .arg((quint16)rawEvent->eventType());
        m_lsEventProcess->LOG_INFO_IS("【开始处理】原始事件: " + rawEventString);
    }

    if(m_eventEngine)
    {
        if(m_lsEventProcess->isOn())
            m_lsEventProcess->LOG_INFO_IS("原始事件->处理队列");

        m_eventEngine->asyncProcessRawEvent(rawEvent);
    }

    return true;
}

bool CMDEventExpertImp::syncProcessRawEvent(CMDRawEvent* rawEvent)
{
    bool result = false;

    switch (rawEvent->eventType())
    {
    case MDEventType::variable_ValueChanged:
    {
        if(m_variantObjContainer_Value)
        {
            if(m_lsEventProcess->isOn())
                m_lsEventProcess->LOG_INFO_IS("处理原始事件，类型=【变量数值变化】");

            result = m_variantObjContainer_Value->ProcessRawEvent(rawEvent);
        }
    }
   break;
    case MDEventType::timer_Cycle:
    {
        if(m_timeObjContainer)
        {
            if(m_lsEventProcess->isOn())
                m_lsEventProcess->LOG_INFO_IS("处理原始事件，类型=【周期】");
            result = m_timeObjContainer->ProcessRawTimeEvent(rawEvent);
        }
    }
   break;
    case MDEventType::variable_QualityChanged:
     case MDEventType::variable_ValueRead:
     case MDEventType::variable_ValueWrite:
    {
        if(m_variantObjContainer_Quality)
        {
            if(m_lsEventProcess->isOn())
                m_lsEventProcess->LOG_INFO_IS("处理原始事件，类型=【变量品质变化】");
            result = m_variantObjContainer_Quality->ProcessRawEvent(rawEvent);
        }
    }
   break;
    case MDEventType::project_Loaded:
    case MDEventType::project_BeforeUnload:
    {
        if(m_projectObjContainer)
        {
            if(m_lsEventProcess->isOn())
                m_lsEventProcess->LOG_INFO_IS("处理原始事件，类型=【项目已加载】");
            result = m_projectObjContainer->ProcessRawEvent(rawEvent);
        }
    }
   break;
    default:
    {
        if(m_lsEventProcess->isOn())
            m_lsEventProcess->LOG_ERROR("处理原始事件，类型=【未定义】，无法处理该事件！");
    };
    }

    if(m_lsEventProcess->isOn())
    {
        QString rawEventString = QString("name = %1, type = %2").arg(rawEvent->rawObjName())
                                                                                .arg((quint16)rawEvent->eventType());
        m_lsEventProcess->LOG_INFO_IS("【结束处理】原始事件: " + rawEventString);
    }

    return result;
}
