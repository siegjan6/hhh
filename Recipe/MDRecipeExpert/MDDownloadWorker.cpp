#include "MDDownloadWorker.h"
#include "MDDownloadExpert.h"

CMDDownloadWorker::CMDDownloadWorker(CMDDownloadExpert* downloadExpert, QObject *parent) :
    QObject(parent)
{
    m_downloadExpert = downloadExpert;

    m_thread = NULL;
    m_timer = NULL;
}

CMDDownloadWorker::~CMDDownloadWorker()
{
    int i = 0;
}

bool CMDDownloadWorker::startThread()
{
    QMutexLocker engineThreadLocker(&m_threadMutex);

    if(!m_thread)
    {
        m_thread = new QThread;
    }
    if(m_thread)
    {
        moveToThread(m_thread);
        QObject::connect(m_downloadExpert, SIGNAL(newTaskAdded()), this, SLOT(handleTask()));
        m_thread->start();

        // m_thread->start() 返回并不意味着线程已经开始正常运行，此处
        // 利用循环阻塞调用线程，等待 m_thread 进入正常运行状态；
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

bool CMDDownloadWorker::stopThread(unsigned long timeout)
{
    QObject::disconnect(m_downloadExpert, SIGNAL(newTaskAdded()), this, SLOT(handleTask()));

    QMutexLocker engineThreadLocker(&m_threadMutex);

    if(!m_thread)
        return true;

    emit stopTimer();

    m_thread->quit();
    if(timeout > 0)
        m_thread->wait(timeout);
    else
        m_thread->wait();

    // TODO：可能调用 terminate，要非常谨慎
    // ---------------------------------------------------
    if(!m_thread->isFinished())
    {
        m_thread->terminate();
//        m_lsDownloadProcess->LOG_INFO("thread terminated");
    }
    if(m_thread->isFinished())
    {
        delete m_thread;
        m_thread = NULL;
//        m_lsDownloadProcess->LOG_INFO("thread isFinished");
    }

    m_timer = NULL;

    return true;
}

bool CMDDownloadWorker::isThreadRunning()
{
    QMutexLocker engineThreadLocker(&m_threadMutex);

    return isThreadRunning_NoLocker();
}

bool CMDDownloadWorker::isThreadRunning_NoLocker()
{
    return (m_thread ? m_thread->isRunning() : false);
}

void CMDDownloadWorker::handleTask()
{
    if(NULL == m_timer)
    {
        m_timer = new QTimer;
        connect(m_timer, SIGNAL(timeout()), this, SLOT(handleTaskOnTimer()));
        connect(this, SIGNAL(stopTimer()), this, SLOT(destroyTimer()));
        m_timer->start(1000);
    }

    m_downloadExpert->check();
}

void CMDDownloadWorker::handleTaskOnTimer()
{
    m_downloadExpert->check();

    if(m_downloadExpert->m_taskQueue.taskCount() <= 0)
        destroyTimer();
}

void CMDDownloadWorker::destroyTimer()
{
    if(m_timer)
    {
        m_timer->stop();
        delete m_timer;
        m_timer = NULL;
    }
}
