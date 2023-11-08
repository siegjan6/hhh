/*!

\author dGFuZ3p3

\section 功能

通知事件类。

\section 用法 - 参考 CMDCommandQueueEngineImp::notifyHandlerToWork
\code
bool CMDCommandQueueEngineImp::notifyHandlerToWork()
{
    QMutexLocker statusLocker(&m_handlerMutex);

    if(!m_handler)
        return false;

    CNotificationToHandleEvent* notification = new CNotificationToHandleEvent(this);
    if(notification)
        QCoreApplication::postEvent(m_handler, notification);

    return (notification != NULL);
}
\endcode

*/

#ifndef MDCOMMANDQUEUEENGINEHELPER_H
#define MDCOMMANDQUEUEENGINEHELPER_H

#include <QEvent>

class CMDCommandQueueEngineImp;
class CNotificationToHandleEvent : public QEvent
{
    friend class CMDCommandQueueEngine;
    friend class CMDCommandQueueEngineThread;
public:
    CNotificationToHandleEvent(CMDCommandQueueEngineImp* eventEngine);
    virtual ~CNotificationToHandleEvent();
public:
    CMDCommandQueueEngineImp* m_engineImp;
public:
    static Type eventType();
protected:
    static Type m_evType;
};

#endif // MDCOMMANDQUEUEENGINEHELPER_H
