/*!

\author dGFuZ3p3

\section 功能

命令队列引擎类的实现类。

*/

#ifndef MDCOMMANDQUEUEENGINEIMP_H
#define MDCOMMANDQUEUEENGINEIMP_H

#include "MDCommandQueueEngineHelper.h"
#include "MDProcessingCommandQueueManager.h"
#include "MDProcessedCommandQueueManager.h"

class CMDCommand;
class CMDCommandQueueEngineThread;
class CNotificationHandler;
class CMDCommandQueueEngineImp
{
    //Q_OBJECT
    friend class CMDCommandQueueEngine;
    friend class CMDCommandQueueEngineThread;
public:
    explicit CMDCommandQueueEngineImp(CMDCommandQueueEngine* outsideEngine);
    ~CMDCommandQueueEngineImp();

/*!
\section 设置引擎注释
强烈建议设置引擎注释，便于通过内部的引擎管理器了解引擎实例的数量和用途；
例如，事件引擎的注释为 "Event.EventEgnige"
*/
public:
    QString m_comment;

/*!
\section 引擎的启动/停止；引擎状态管理
start：启动一个内部线程，命令的处理代码将在该线程内执行；
startInCurrentThread：不启动额外的线程，命令的处理代码将在
                                  调用 startInCurrentThread 函数的线程内执行。
\note 只能使用 start 和 startInCurrentThread 之一来启动引擎。
*/
public:
    bool start();
    bool stop(unsigned long timeout);
    bool isRunning();
    bool startInCurrentThread();
protected:
    MDCommandEngineStatus status();
    bool setStatus(MDCommandEngineStatus status);
    MDCommandEngineStatus m_status;
    QMutex m_statusMutex;
protected:
    bool startThread();
    bool stopThread(unsigned long timeout);
    bool isThreadRunning();
    bool isThreadRunning_NoLocker();
protected:
    CMDCommandQueueEngineThread* m_engineThread;
    QMutex m_engineThreadMutex;

/*!
\section 管理通知处理器，处理代码将在创建该处理器的线程内执行。
*/
protected:
    CNotificationHandler* new_Handler();
    void delete_Handler();
    bool notifyHandlerToWork();
protected:
    CNotificationHandler* m_handler;
    QMutex m_handlerMutex;

/*!
\section 管理【等待处理的命令】
*/
public:
    bool submitCommand(CMDCommand* command);
    bool submitPriorCommand(CMDCommand* command);
    void cancelAllWaiting();
public:
    void processCommandQueue();
    bool isInProcessing();
protected:
    void setInProcessing(bool inProcessing);
    bool m_inProcessing;
    QMutex m_inProcessingMutex;
protected:
    CMDProcessingCommandQueueManager m_processingQueueManager;
    qint64 m_totalSubmit;
    qint64 m_totalSubmitHistory;
    QDateTime m_startTime;

/*!
\section 管理【已经处理的命令】
缺省管理方式：延迟 5 秒后删除命令
*/
public:
    void handleProcessedCommand(CMDCommand* processedCommand);
protected:
    CMDProcessedCommandQueueManager m_processedQueueManager;
};

#endif // MDCOMMANDQUEUEENGINEIMP_H
