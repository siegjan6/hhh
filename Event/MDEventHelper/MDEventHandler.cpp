#include "MDEventHandler.h"
#include "MDEventData.h"

CMDEventHandler::CMDEventHandler(CMDEvent* event)
{
    m_data = new CMDEventHandlerData(event);
}

CMDEventHandler::~CMDEventHandler()
{
    if(m_data)
        delete m_data;
}

CMDEvent* CMDEventHandler::nsEvent() const
{
    return (m_data ? m_data->m_event : NULL);
}
void CMDEventHandler::setMDEvent(CMDEvent* event)
{
    if(m_data)
        m_data->m_event = event;
}
