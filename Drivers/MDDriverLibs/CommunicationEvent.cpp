#include "CommunicationEvent.h"
#include "BaseChannel.h"

CCommunicationEvent::CCommunicationEvent()
{
    m_channel=NULL;
}

CCommunicationEvent::~CCommunicationEvent()
{
}


//关联Channel
CBaseChannel *CCommunicationEvent::getChannel()
{
    return m_channel;
}

void CCommunicationEvent::setChannel(CBaseChannel *channel)
{
    m_channel = channel;
}


void CCommunicationEvent::onEvent(const CommunicationEventType eventType)
{
    if(NULL!=m_channel)
    {
       // m_channel->IOEvent(eventType);
    }
}
