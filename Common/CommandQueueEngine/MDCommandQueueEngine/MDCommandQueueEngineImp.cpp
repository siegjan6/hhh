#include "MDCommandQueueEngineImp.h"
#include "MDCommandQueueEngineThread.h"
#include <QCoreApplication>
#include "MDProcessedCommandQueueManager.h"
#include "NotificationHandler.h"

CMDCommandQueueEngineImp::CMDCommandQueueEngineImp(CMDCommandQueueEngine* outsideEngine)
{
    m_processingQueueManager.m_outsideEngine = outsideEngine;
    m_processingQueueManager.m_insideEngine = this;

    m_status = MDCommandEngineStatus::stopped;
    m_engineThread = NULL;
    m_handler = NULL;
    m_inProcessing = false;

    m_totalSubmit = 0;
    m_totalSubmitHistory = 0;
}

CMDCommandQueueEngineImp::~CMDCommandQueueEngineImp()
{
    stop(100);
}

/*!
\section 引擎的启动/停止；引擎状态管理
start：启动一个内部线程，命令的处理代码将在该线程内执行；
startInCurrentThread：不启动额外的线程，命令的处理代码将在
                                  调用 startInCurrentThread 函数的线程内执行。
\note 只能使用 start 和 startInCurrentThread 之一来启动引擎；
           setStatus 确保了 start() 和 stop() 这两个流程是线程安全的。
*/
bool CMDCommandQueueEngineImp::start()
{
    if(!setStatus(MDCommandEngineStatus::starting))
        return false;

    m_processingQueueManager.setExiting(false);
    startThread();  // 在内部线程里创建处理器对象，
                            // CNotificationHandler::event 将在该线程里执行

    m_startTime = QDateTime::currentDateTime();
    m_totalSubmit = 0;
    m_processedQueueManager.resetTotalProcessed();

    setStatus(MDCommandEngineStatus::started);

    return true;
}

bool CMDCommandQueueEngineImp::startInCurrentThread()
{
    if(!setStatus(MDCommandEngineStatus::starting))
        return false;

    m_processingQueueManager.setExiting(false);
    new_Handler();  // 在调用线程里创建处理器对象，
                              // CNotificationHandler::event 将在该线程里执行

    m_startTime = QDateTime::currentDateTime();
    m_totalSubmit = 0;
    m_processedQueueManager.resetTotalProcessed();

    setStatus(MDCommandEngineStatus::started);

    return true;
}

bool CMDCommandQueueEngineImp::stop(unsigned long timeout)
{
    if(!setStatus(MDCommandEngineStatus::stopping))
        return false;

    m_processingQueueManager.setExiting(true);
    stopThread(timeout);

    m_processingQueueManager.cleanQueue();
    m_processedQueueManager.cleanQueue();

    delete_Handler();

    setInProcessing(false);

    setStatus(MDCommandEngineStatus::stopped);

    return true;
}

bool CMDCommandQueueEngineImp::isRunning()
{
    return (isThreadRunning() &&
               (MDCommandEngineStatus::started == status()));
}

bool CMDCommandQueueEngineImp::startThread()
{
    QMutexLocker engineThreadLocker(&m_engineThreadMutex);

    if(!m_engineThread)
        m_engineThread = new CMDCommandQueueEngineThread();
    if(m_engineThread)
    {
        //connect(m_engineThread, SIGNAL(finished()), m_engineThread, SLOT(deleteLater()));
        m_engineThread->m_engine = this;
        m_engineThread->start();

        // m_engineThread->start() 返回并不意味着线程已经开始正常运行，此处
        // 利用循环阻塞调用线程，等待 m_engineThread 进入正常运行状态；
        // 原因：调用线程在启动线程后，有可能立即调用一些需要线程运行后才可
        //           正常执行的接口，此处的阻塞最大限度确保这些调用成功。
        // --------------------------------------------------------------------------------
        // 等待时间不超过 50 ms，此时间对启动线程来说足够了
        // --------------------------------------------------------------------------
        for(int i = 0; i < 5; i++)
        {
            if(isThreadRunning_NoLocker())
                break;
            QThread::msleep(10);
        }
    }

    return true;
}

bool CMDCommandQueueEngineImp::stopThread(unsigned long timeout)
{
    QMutexLocker engineThreadLocker(&m_engineThreadMutex);

    if(!m_engineThread)
        return true;

    m_engineThread->quit();

    if(timeout > 0)
        m_engineThread->wait(timeout);
    else
        m_engineThread->wait();

    // TODO：可能调用 terminate，要非常谨慎
    // ---------------------------------------------------
    if(!m_engineThread->isFinished())
        m_engineThread->terminate();
    if(m_engineThread->isFinished())
    {
        delete m_engineThread;
        m_engineThread = NULL;
    }

    return true;
}

bool CMDCommandQueueEngineImp::isThreadRunning()
{
    QMutexLocker engineThreadLocker(&m_engineThreadMutex);

    return isThreadRunning_NoLocker();
}

bool CMDCommandQueueEngineImp::isThreadRunning_NoLocker()
{
    return (m_engineThread ? m_engineThread->isRunning() : false);
}

// setStatus 确保了 start() 和 stop() 这两个流程是线程安全的
bool CMDCommandQueueEngineImp::setStatus(MDCommandEngineStatus status)
{
    QMutexLocker statusLocker(&m_statusMutex);

    // 检查：允许从 m_status 改变为 status 吗？
    // ----------------------------------------------------
    bool result = false;
    switch (status)
    {
    case MDCommandEngineStatus::starting:
           result = (MDCommandEngineStatus::stopped == m_status);
        break;
    case MDCommandEngineStatus::started:
           result = (MDCommandEngineStatus::starting == m_status);
        break;
    case MDCommandEngineStatus::stopping:
           result = (MDCommandEngineStatus::started == m_status);
        break;
    case MDCommandEngineStatus::stopped:
           result = (MDCommandEngineStatus::stopping == m_status);
        break;
    default:
        break;
    }

    // 成功：允许从 m_status 改变为 status
    // ----------------------------------------------------
    if(result)
        m_status = status;

    return result;
}

MDCommandEngineStatus CMDCommandQueueEngineImp::status()
{
    QMutexLocker statusLocker(&m_statusMutex);

    return m_status;
}

/*!
\section 管理通知处理器，处理代码将在创建该处理器的线程内执行。
*/
CNotificationHandler* CMDCommandQueueEngineImp::new_Handler()
{
    QMutexLocker statusLocker(&m_handlerMutex);

    if(!m_handler)
        m_handler = new CNotificationHandler;
    return m_handler;
}

void CMDCommandQueueEngineImp::delete_Handler()
{
    QMutexLocker statusLocker(&m_handlerMutex);

    if(m_handler)
    {
        delete m_handler;
        m_handler = NULL;
    }
}

/*!
\section 管理【等待处理的命令】
*/
bool CMDCommandQueueEngineImp::submitCommand(CMDCommand* command)
{
    m_totalSubmit ++;
    m_totalSubmitHistory ++;

    if(!m_processingQueueManager.addToWaitingQueue(command))
        return false;

    // 如果正在循环处理，无需通知。
    // 参考 CMDCommandQueueEngineImp::processCommandQueue()
    if(isInProcessing())
        return false;

    return notifyHandlerToWork();
}

bool CMDCommandQueueEngineImp::submitPriorCommand(CMDCommand* command)
{
    if(!m_processingQueueManager.addToPriorWaitingQueue(command))
        return false;

    // 如果正在循环处理，无需通知。
    // 参考 CMDCommandQueueEngineImp::processCommandQueue()
    if(isInProcessing())
        return false;

    return notifyHandlerToWork();
}

bool CMDCommandQueueEngineImp::notifyHandlerToWork()
{
    QMutexLocker statusLocker(&m_handlerMutex);

    if(!m_handler)
        return false;

    // CNotificationToHandleEvent 的实例将由 Qt 负责删除
    // --------------------------------------------------------------------------------------------
    CNotificationToHandleEvent* notification = new CNotificationToHandleEvent(this);
    if(notification)
        QCoreApplication::postEvent(m_handler, notification);

    return (notification != NULL);
}

void CMDCommandQueueEngineImp::processCommandQueue()
{
    // 开始循环处理，此期间外界可不发送通知。
    // 参考 CMDCommandQueueEngineImp::notifyThreadToWork()

    setInProcessing(true);

    m_processingQueueManager.loopProcessCommandQueue();

    setInProcessing(false);
}

bool CMDCommandQueueEngineImp::isInProcessing()
{
    QMutexLocker inProcessingLocker(&m_inProcessingMutex);
    return m_inProcessing;
}

void CMDCommandQueueEngineImp::setInProcessing(bool inProcessing)
{
    QMutexLocker inProcessingLocker(&m_inProcessingMutex);
    m_inProcessing = inProcessing;
}

void CMDCommandQueueEngineImp::cancelAllWaiting()
{
    m_processingQueueManager.cleanQueue();
}

/*!
\section 管理【已经处理的命令】
缺省管理方式：延迟 5 秒后删除命令
*/
void CMDCommandQueueEngineImp::handleProcessedCommand(CMDCommand* processedCommand)
{
    m_processedQueueManager.dealWithCommand(processedCommand);
}
