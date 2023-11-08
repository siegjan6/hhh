#include "MDEvent.h"
#include "MDEventData.h"

CMDEvent::CMDEvent(CMDEventProperty_Config* configProperty)
{
    m_data = new CMDEventData(configProperty);
}

CMDEvent::~CMDEvent()
{
    if(m_data)
        delete m_data;
}

/*!
\section 属性
*/
CMDEventProperty_Config* CMDEvent::configProperty() const
{
    return (m_data ? m_data->m_configProperty : NULL);
}

CMDEventProperty_Runtime* CMDEvent::runtimeProperty() const
{
    return (m_data ? m_data->m_runtimeProperty : NULL);
}

MDEventType CMDEvent::type()
{
    if(m_data && m_data->m_configProperty)
        return m_data->m_configProperty->type();
    else
        return MDEventType::null_event;
}

/*!
\section 事件处理器
*/
CMDEventHandler* CMDEvent::handler() const
{
    return (m_data ? m_data->m_handler : NULL);
}
void CMDEvent::setHandler(CMDEventHandler* handler)
{
    if(m_data)
        m_data->m_handler = handler;
}

/*!
\section 关联事件与关联动作
*/
QList<CMDEvent*>* CMDEvent::relatedEventList()
{
    return (m_data ? &m_data->m_relatedEventList : NULL);
}

QList<CMDAction*>* CMDEvent::actionList()
{
    return (m_data ? &m_data->m_actionList : NULL);
}
