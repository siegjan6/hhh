#include "MDCommandQueueEngineHelper.h"

QEvent::Type CNotificationToHandleEvent::m_evType = QEvent::None;
CNotificationToHandleEvent::CNotificationToHandleEvent(CMDCommandQueueEngineImp* engineImp)
                : QEvent(eventType()), m_engineImp(engineImp)
{
}

CNotificationToHandleEvent::~CNotificationToHandleEvent()
{
}

QEvent::Type CNotificationToHandleEvent::eventType()
{
    if(m_evType == QEvent::None)
    {
        m_evType = (QEvent::Type)registerEventType();
    }
    return m_evType;
}
