/*!

\author dGFuZ3p3

\section 功能

命令队列处理线程类，命令在此线程内执行。

*/

#ifndef MDCOMMANDQUEUEENGINETHREAD_H
#define MDCOMMANDQUEUEENGINETHREAD_H

#include <QThread>

class CMDCommandQueueEngineImp;
class CMDCommandQueueEngineThread : public QThread
{
    Q_OBJECT
    friend class CMDCommandQueueEngineImp;
public:
    explicit CMDCommandQueueEngineThread(QObject *parent = 0);
    ~CMDCommandQueueEngineThread();
protected:
    CMDCommandQueueEngineImp* m_engine;
protected:
    void run();
signals:

public slots:

};

#endif // MDCOMMANDQUEUEENGINETHREAD_H
