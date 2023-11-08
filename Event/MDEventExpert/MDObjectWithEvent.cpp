#include "MDObjectWithEvent.h"
#include "MDEvent.h"
#include "MDEventHandlerEx.h"
#include "MDRawEvent.h"

/*!
 * \brief 关联了事件的对象类
 */
CMDObjectWithEvent::CMDObjectWithEvent()
{
}

CMDObjectWithEvent::CMDObjectWithEvent(const QString& name, MDObjectType objectType)
    : CMDObject(name, objectType)
{
}

CMDObjectWithEvent::~CMDObjectWithEvent()
{

}

bool CMDObjectWithEvent::initAddEvent(CMDEvent* event)
{
    if(event)
        m_eventList.append(event);

    return (event != NULL);
}

void CMDObjectWithEvent::ProcessRawEvent(CMDRawEvent* rawEvent)
{
    if(rawEvent->logSwitch()->isOn())
    {
        QString rawEventString = QString("%1").arg(m_eventList.size());
        rawEvent->logSwitch()->LOG_INFO_IS("原始事件关联了项目事件，数量=" + rawEventString);
    }

    for (int i = 0; i < m_eventList.size(); ++i)
    {
        CMDEvent* event = m_eventList.at(i);

        CMDEventHandlerEx* handler = (CMDEventHandlerEx*)event->handler();
        if(handler)
        {
            if(rawEvent->logSwitch()->isOn())
                rawEvent->logSwitch()->LOG_INFO_IS("开始处理项目事件：" + event->configProperty()->name());

            handler->ProcessRawEvent(rawEvent);

            if(rawEvent->logSwitch()->isOn())
                rawEvent->logSwitch()->LOG_INFO_IS("结束处理项目事件：" + event->configProperty()->name());
        }
    }
}

/*!
 * \brief 对象容器类
 */
CMDObjectContainer::CMDObjectContainer()
{
}

CMDObjectContainer::~CMDObjectContainer()
{
    deleteAll();
}

CMDObject* CMDObjectContainer::addObj(const QString& name, MDObjectType objectType, qint8& result)
{
    CMDObject* existObject = findObj(name);
    if(existObject)
    {
        result = 0;
        return existObject;
    }

    CMDObject* newObject = newObj(name, objectType, result);
    if(newObject)
    {
        m_objectMap.insert(name.toUpper(), newObject);
    }
    result = 1;
    return newObject;
}

CMDObject* CMDObjectContainer::findObj(const QString& name)
{
    CMDObject* object = NULL;

    QMap<QString, CMDObject*>::const_iterator iPosition = m_objectMap.find(name.toUpper());
    if(iPosition != m_objectMap.constEnd())
        object = iPosition.value();

    return object;
}

bool CMDObjectContainer::deleteObj(const QString& name)
{
    bool result = false;

    QMap<QString, CMDObject*>::iterator iPosition = m_objectMap.find(name.toUpper());
    if(iPosition != m_objectMap.constEnd())
   {
        CMDObject* object = iPosition.value();
        delete object;

        m_objectMap.erase(iPosition);

        result = true;
    }
    return result;
}

void CMDObjectContainer::deleteAll()
{
    QMap<QString, CMDObject*>::const_iterator iPosition = m_objectMap.constBegin();
    while(m_objectMap.constEnd() != iPosition)
    {
        delete iPosition.value();
        ++ iPosition;
    }
    m_objectMap.clear();
}

CMDObject*CMDObjectContainer::newObj(const QString& name, MDObjectType objectType, qint8& result)
{
    result = -1;

    return 0;
}

/*!
 * \brief 关联了事件的对象容器类
 */
CContainerForObjectWithEvent::CContainerForObjectWithEvent()
{

}

CMDObject*CContainerForObjectWithEvent::newObj(const QString& name, MDObjectType objectType, qint8& result)
{
    CMDObjectWithEvent* objectWithEvent = new CMDObjectWithEvent(name, objectType);
    if(!objectWithEvent)
        result = -1;
    return objectWithEvent;
}

bool CContainerForObjectWithEvent::ProcessRawEvent(CMDRawEvent* rawEvent)
{
    // 原始对象引发【原始事件】，该对象可能关联了一些事件
    CMDObject* objectWithEvent = findObj(rawEvent->rawObjName());
    if(!objectWithEvent)
        return false;

    ((CMDObjectWithEvent*)objectWithEvent)->ProcessRawEvent(rawEvent);
    return true;
}

bool CContainerForObjectWithEvent::ProcessRawTimeEvent(CMDRawEvent* rawEvent)
{
    QMap<QString, CMDObject*>::const_iterator iPosition = m_objectMap.constBegin();
    while(m_objectMap.constEnd() != iPosition)
    {
        CMDObject* objectWithEvent = iPosition.value();
        ((CMDObjectWithEvent*)objectWithEvent)->ProcessRawEvent(rawEvent);
        ++ iPosition;
    }

    return true;
}
