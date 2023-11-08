#include "NotificationHandler.h"
#include "../../../include/MDCommandQueueEngine/MDCommandQueueEngine.h"
#include <QEvent>
#include "MDCommandQueueEngineHelper.h"
#include "MDCommandQueueEngineImp.h"

CNotificationHandler::CNotificationHandler()
{

}

/*!
 * \brief
、通过 CMDCommandQueueEngine::start() 启动，对象在 CMDCommandQueueEngineThread::run() 中创建；
、通过 CMDCommandQueueEngine::startInThread() 启动，对象在调用 CMDCommandQueueEngine::startInThread() 的线程创建；
、因此其事件处理函数 ::event 一定是在相应的线程内执行。
 */
bool CNotificationHandler::event(QEvent * e)
{
    if(e->type() == CNotificationToHandleEvent::eventType())
    {
        CNotificationToHandleEvent* notification = (CNotificationToHandleEvent*)e;
        if(notification && notification->m_engineImp)
            notification->m_engineImp->processCommandQueue();

        return true;
    }
    return QObject::event(e);
}

