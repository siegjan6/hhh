/*!
author enli 2015.01.08
\section 目前不使用
*/
#ifndef MDVARIABLECHANGETHREAD_H
#define MDVARIABLECHANGETHREAD_H

#include <QThread>
/*
// 在【命令队列处理线程】内执行代码的处理器
class CNotificationHandler : public QObject
{
    Q_OBJECT
    friend class CMDCommandQueueEngineThread;
protected:
    CNotificationHandler();
protected:
    bool event(QEvent *);
};

// 命令队列处理线程
class CMDCommandQueueEngineThread : public QThread
{
    Q_OBJECT
    friend class CMDCommandQueueEngineImp;
public:
    explicit CMDCommandQueueEngineThread(QObject *parent = 0);
public:
    bool isReadyToWork();
protected:
    CNotificationHandler* m_handler;
protected:
    void run();
signals:

public slots:

};
*/

#endif // MDVARIABLECHANGETHREAD_H
