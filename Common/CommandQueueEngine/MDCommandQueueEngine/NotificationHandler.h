/*!

\author dGFuZ3p3

\section 功能

辅助的通知处理器类，处理代码将在创建该对象的线程内执行。

、通过 CMDCommandQueueEngine::start() 启动，对象在 CMDCommandQueueEngineThread::run() 中创建；
、通过 CMDCommandQueueEngine::startInThread() 启动，对象在调用 CMDCommandQueueEngine::startInThread() 的线程创建；
、因此其事件处理函数 ::event 一定是在相应的线程内执行。
*/

#ifndef NOTIFICATIONHANDLER_H
#define NOTIFICATIONHANDLER_H

#include <QObject>

class CNotificationHandler : public QObject
{
    Q_OBJECT
    friend class CMDCommandQueueEngineImp;
protected:
    CNotificationHandler();
protected:
    bool event(QEvent *);
};

#endif // NOTIFICATIONHANDLER_H
