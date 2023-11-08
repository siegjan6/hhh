#include "MDProcessingCommandQueueManager.h"
#include "../../../include/MDCommandQueueEngine/MDCommandQueueEngine.h"
#include "MDCommandQueueEngineImp.h"

const int MAX_WAITING_COUNT = 1024 * 40;    // 等待队列的最大长度

CMDProcessingCommandQueueManager::CMDProcessingCommandQueueManager()
{
    m_outsideEngine = NULL;
    m_insideEngine = NULL;

    m_exiting = false;
    m_priorEnabled = false;
}

CMDProcessingCommandQueueManager::~CMDProcessingCommandQueueManager()
{
    cleanQueue();
}

/*!
\section 进行命令处理
m_outsideEngine： 外部引擎，由外部对命令进行具体处理；
m_insideEngine： 内部引擎，m_outsideEngine 处理完成后，进行
                             后续的内部处理
*/
void CMDProcessingCommandQueueManager::setExiting(bool exiting)
{
    m_exiting = exiting;
}

void CMDProcessingCommandQueueManager::loopProcessCommandQueue()
{
    // 循环处理机制：
    // 在 syncProcessProcessingQueue 执行期间，外部仍然可以
    // 向 【等待队列】 添加命令，因此 syncProcessProcessingQueue 执行完毕后
    // 将检查 【等待队列】，直到将 【等待队列】 上的命令全部处理完毕；
    // 注意，此处【普通等待队列】的命令将被转移到【普通处理队列】，
    // 但是只检查了【优先等待队列】的大小，并不转移。
    // ----------------------------------------------------------------------------------------------------
    while(transferWaitingToProcessing() > 0 ||
                sizeOfPriorWaitingQueue() > 0)
    {
        if(m_exiting)
            break;

        syncProcessProcessingQueue();
    }
}

int CMDProcessingCommandQueueManager::transferWaitingToProcessing()
{
    QMutexLocker waitingLocker(&m_waitingQueueMutex);
    QMutexLocker processingLocker(&m_processingQueueMutex);

    // 将 m_waitingQueue 的命令逐个转移到 m_processingQueue，
    // 已经转移的命令将从 m_waitingQueue 移除
    // -------------------------------------------------------------------
    while(!m_waitingQueue.isEmpty())
    {
        if(m_exiting)
            break;

        m_processingQueue.append(m_waitingQueue.first());
        m_waitingQueue.removeFirst();
    }

    return m_processingQueue.size();
}

void CMDProcessingCommandQueueManager::syncProcessProcessingQueue()
{
    // 关键点1：由于引擎完全无法掌控外部处理命令的过程，因此在通过
    //                  takeFirst 取出一个命令后，命令处理期间没有任何锁定。
    // 这样处理是为了避免发生如下的“死锁”状态：
    // 一旦命令处理期间引擎需要退出，则处理线程可能被强制终止
    // （外部不会无限等待线程退出），而一旦线程被强制终止，线程所
    // 占用的资源（譬如 processingLocker）将不会被正常释放，
    // 这就可能导致外部请求锁的操作进入“死锁”状态。
    // ----------------------------------------------------------------------------------------------------

    // 关键点2：command 从队列被移除后才开始处理，处理期间
    //                   command 处于“无人监管”状态，如果此时引擎线程被强制退出，
    //                   则可能会发生内存泄漏（最坏的情况）
    // ----------------------------------------------------------------------------------------------------
    CMDCommand* command = takeFirst();
    while(command != NULL)
    {
        // 外部引擎：
        // 外部重载的 CMDCommandQueueEngine::syncProcessCommand 将
        // 对 command 进行具体处理
        // -----------------------------------------------------------------
        if(m_outsideEngine)
            m_outsideEngine->onProcessCommand(command);

        // 内部引擎，进行后续的内部处理
        // -------------------------------------------------------------------------
        command->setTimeOnProcessed(QDateTime::currentDateTime());
        if(m_insideEngine)
            m_insideEngine->handleProcessedCommand(command);

        if(m_exiting)
            break;

        command = takeFirst();
    }
}

CMDCommand* CMDProcessingCommandQueueManager::takeFirst()
{
    CMDCommand* command = takeFirstPrior();
    if(command)
        return command;

    return takeFirstCommon();
}

void CMDProcessingCommandQueueManager::cleanQueue()
{
    cleanWaitingQueue();
    cleanProcessingQueue();
    cleanPriorWaitingQueue();
}

/*!
\section 管理普通命令的【等待队列】
*/
bool CMDProcessingCommandQueueManager::addToWaitingQueue(CMDCommand* command)
{
    if(m_exiting)
    {
        delete command;
        return false;
    }

    QMutexLocker locker(&m_waitingQueueMutex);
    if(m_waitingQueue.count() < MAX_WAITING_COUNT)
        m_waitingQueue.append(command);
    else
        delete command;

    return true;
}

void CMDProcessingCommandQueueManager::cleanWaitingQueue()
{
    QMutexLocker waitingLocker(&m_waitingQueueMutex);

    int waitingSize = m_waitingQueue.size();
    for(int i = 0; i < waitingSize; i++)
        delete m_waitingQueue.at(i);
    m_waitingQueue.clear();
}

int CMDProcessingCommandQueueManager::sizeOfWaitingQueue()
{
    QMutexLocker waitingLocker(&m_waitingQueueMutex);

   return m_waitingQueue.size();
}

/*!
\section 管理普通命令的【正在处理队列】
*/
void CMDProcessingCommandQueueManager::cleanProcessingQueue()
{
    QMutexLocker processingLocker(&m_processingQueueMutex);

    int processingSize = m_processingQueue.size();
    for(int i = 0; i < processingSize; i++)
        delete m_processingQueue.at(i);
    m_processingQueue.clear();
}

int CMDProcessingCommandQueueManager::sizeOfProcessingQueue()
{
    QMutexLocker processingLocker(&m_processingQueueMutex);

    return m_processingQueue.size();
}

CMDCommand* CMDProcessingCommandQueueManager::takeFirstCommon()
{
    CMDCommand* command = NULL;
    QMutexLocker processingLocker(&m_processingQueueMutex);
    if(!m_processingQueue.isEmpty())
    {
        command = m_processingQueue.first();
        m_processingQueue.removeFirst();
    }
    return command;
}

/*!
\section 管理优先命令的【等待队列】
*/
bool CMDProcessingCommandQueueManager::addToPriorWaitingQueue(CMDCommand* command)
{
    if(m_exiting)
    {
        delete command;
        return false;
    }

    // 提交优先命令将导致优先处理开关被打开，对于不使用优先命令机制的情况，
    // 此开关使额外开销降至最低，参考 takeFirstPrior
    // -------------------------------------------------------------------------------------------------------
    m_priorEnabled = true;

    QMutexLocker locker(&m_priorWaitingQueueMutex);
    if(m_priorWaitingQueue.count() < MAX_WAITING_COUNT)
        m_priorWaitingQueue.append(command);
    else
        delete command;

    return true;
}

void CMDProcessingCommandQueueManager::cleanPriorWaitingQueue()
{
    QMutexLocker locker(&m_priorWaitingQueueMutex);

    int waitingSize = m_priorWaitingQueue.size();
    for(int i = 0; i < waitingSize; i++)
        delete m_priorWaitingQueue.at(i);
    m_priorWaitingQueue.clear();
}

int CMDProcessingCommandQueueManager::sizeOfPriorWaitingQueue()
{
    QMutexLocker locker(&m_priorWaitingQueueMutex);

    return m_priorWaitingQueue.size();
}

CMDCommand* CMDProcessingCommandQueueManager::takeFirstPrior()
{
    if(!m_priorEnabled)
        return NULL;

    CMDCommand* command = NULL;
    QMutexLocker locker(&m_priorWaitingQueueMutex);
    if(!m_priorWaitingQueue.isEmpty())
    {
        command = m_priorWaitingQueue.first();
        m_priorWaitingQueue.removeFirst();
    }
    return command;
}
