
#include <QEvent>
#include "MDVariableChangeThread.h"
/*
CNotificationHandler::CNotificationHandler()
{

}

// 在【命令队列处理线程】内执行代码的处理器；
// 由于该对象在 CMDCommandQueueEngineThread::run() 中创建，
// 因此其事件处理函数 ::event 一定是在【命令队列处理线程】内执行
// ----------------------------------------------------------------------------
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

// 命令队列处理线程
CMDCommandQueueEngineThread::CMDCommandQueueEngineThread(QObject *parent) :
    QThread(parent)
{
    m_handler = NULL;
}

bool CMDCommandQueueEngineThread::isReadyToWork()
{
    return (isRunning() &&
                m_handler != NULL);
}

void CMDCommandQueueEngineThread::run()
{
    m_handler = new CNotificationHandler();
    QScopedPointer<CNotificationHandler> handlerSP(m_handler);

    exec();
}*/
