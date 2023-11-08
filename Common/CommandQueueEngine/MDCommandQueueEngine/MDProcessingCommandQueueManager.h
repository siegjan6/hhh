/*!

\author dGFuZ3p3

\section 功能

管理【等待命令】和【正在处理的命令】。

*/

#ifndef MDPROCESSINGCOMMANDQUEUEMANAGER_H
#define MDPROCESSINGCOMMANDQUEUEMANAGER_H

#include <QList>
#include <QMutex>

class CMDCommand;
class CMDCommandQueueEngine;
class CMDCommandQueueEngineImp;
class CMDProcessingCommandQueueManager
{
    friend class CMDCommandQueueEngineImp;
public:
    CMDProcessingCommandQueueManager();
    ~CMDProcessingCommandQueueManager();

/*!
\section 进行命令处理
m_outsideEngine： 外部引擎，由外部对命令进行具体处理；
m_insideEngine： 内部引擎，m_outsideEngine 处理完成后，进行
                             后续的内部处理
*/
protected:
    CMDCommandQueueEngine* m_outsideEngine;
    CMDCommandQueueEngineImp* m_insideEngine;
protected:
    void syncProcessProcessingQueue();
    CMDCommand* takeFirst();
protected:
    int transferWaitingToProcessing();
    void loopProcessCommandQueue();
    void cleanQueue();

    //! 退出标识，若设为true，则命令处理流程会尽快结束
protected:
    void setExiting(bool exiting);
    bool m_exiting;

/*!
\section 管理普通命令的【等待队列】
*/
public:
    bool addToWaitingQueue(CMDCommand* command);
    int sizeOfWaitingQueue();
protected:
    void cleanWaitingQueue();
    QList<CMDCommand*> m_waitingQueue;
    QMutex m_waitingQueueMutex;

/*!
\section 管理普通命令的【正在处理队列】
*/
public:
    int sizeOfProcessingQueue();
    CMDCommand* takeFirstCommon();
protected:
    void cleanProcessingQueue();
    QList<CMDCommand*> m_processingQueue;
    QMutex m_processingQueueMutex;

/*!
\section 管理优先命令的【等待队列】
*/
public:
    bool addToPriorWaitingQueue(CMDCommand* command);
    int sizeOfPriorWaitingQueue();
    CMDCommand* takeFirstPrior();
protected:
    bool m_priorEnabled; //! 优先处理开关
protected:
    void cleanPriorWaitingQueue();
    QList<CMDCommand*> m_priorWaitingQueue;
    QMutex m_priorWaitingQueueMutex;
};

#endif // MDPROCESSINGCOMMANDQUEUEMANAGER_H
