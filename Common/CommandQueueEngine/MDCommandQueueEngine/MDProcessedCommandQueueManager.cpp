#include "MDProcessedCommandQueueManager.h"

const int MAX_PROCESSED_COUNT = 1024 * 40;    // 已处理队列的最大长度

CMDProcessedCommandQueueManager::CMDProcessedCommandQueueManager()
{
    m_modeToHandleProcessed = MDModeToHandleProcessedCommand::mode_recycle;
    m_maxRecycleQueueLength = 1024;
    m_delayFlag = false;
    m_delayTimeLength = 5000;

    m_totalProcessed = 0;
    m_totalProcessedHistory = 0;
}

CMDProcessedCommandQueueManager::~CMDProcessedCommandQueueManager()
{
    cleanQueue();
}

/*!
\section 处理模式；延迟处理
*/
MDModeToHandleProcessedCommand CMDProcessedCommandQueueManager::modeToHandleProcessed()
{
    return m_modeToHandleProcessed;
}

void CMDProcessedCommandQueueManager::setModeToHandleProcessed(MDModeToHandleProcessedCommand mode)
{
    m_modeToHandleProcessed = mode;
}

bool CMDProcessedCommandQueueManager::isDelayHandleProcessed()
{
    return m_delayFlag;
}

void CMDProcessedCommandQueueManager::setDelayHandleProcessed(bool flag)
{
    m_delayFlag = flag;
}

int CMDProcessedCommandQueueManager::delayTimeLength()
{
    return m_delayTimeLength;
}

void CMDProcessedCommandQueueManager::setDelayTimeLength(int timeLength)
{
    m_delayTimeLength = timeLength;
}

/*!
\section 根据配置，决定如何处理命令，参考主函数 dealWithCommand
*/
void CMDProcessedCommandQueueManager::dealWithCommand(CMDCommand* processedCommand)
{
    m_totalProcessed ++;
    m_totalProcessedHistory ++;

    if(isDelayHandleProcessed()) //! 允许延迟处理吗？
    {
        addToProcessedQueue(processedCommand);

        if(isTimeToDelayHandling()) //! 如果为 true，应该检查命令的延迟时间
            checkProcessedToHandle();
    }
    else // 不延迟，立即处理
    {
        handleProcessedCommand(processedCommand);
    }
}

void CMDProcessedCommandQueueManager::handleProcessedCommand(CMDCommand* processedCommand)
{
    if(MDModeToHandleProcessedCommand::mode_delete ==  modeToHandleProcessed())
    {
        delete processedCommand;
    }
    else if(MDModeToHandleProcessedCommand::mode_recycle ==  modeToHandleProcessed())
    {
        addToRecycleQueue(processedCommand);
    }
    else{}
}

bool CMDProcessedCommandQueueManager::isTimeToDelayHandling()
{
    bool result = false;

    if(m_lastTimeStampForDelayHandling.isNull())
        m_lastTimeStampForDelayHandling = QDateTime::currentDateTime();
    else
    {
        QDateTime curDateTime = QDateTime::currentDateTime();
        qint64 timeSpan = m_lastTimeStampForDelayHandling.msecsTo(curDateTime);
        if(timeSpan >= m_delayTimeLength)
        {
            m_lastTimeStampForDelayHandling = curDateTime;
            result = true;
        }
    }
    return result;
}

/*!
 * \brief 处理 m_processedQueue 中延迟时间已到的命令
 */
void CMDProcessedCommandQueueManager::checkProcessedToHandle()
{
    QMutexLocker locker(&m_processedQueueMutex);

    QLinkedList<CMDCommand*>::iterator iPosition = m_processedQueue.begin();
    while(iPosition != m_processedQueue.end())
    {
        CMDCommand* command = *iPosition;

        qint64 timeSpan = command->timeOnProcessed().msecsTo(QDateTime::currentDateTime());
        if(timeSpan >= m_delayTimeLength)
        {
            /*!
            先移除，再处理，避免在极端情况下 command
            被 delete 多次；风险是 command 内存泄漏
            */
            iPosition = m_processedQueue.erase(iPosition);

            handleProcessedCommand(command);
        }
        else iPosition ++;
    }
}

void CMDProcessedCommandQueueManager::cleanQueue()
{
    cleanProcessedQueue();
    cleanRecycleQueue();
}

/*!
\section 管理已处理队列
*/
void CMDProcessedCommandQueueManager::cleanProcessedQueue()
{
    QMutexLocker locker(&m_processedQueueMutex);

    while(!m_processedQueue.isEmpty())
        delete m_processedQueue.takeFirst();
}

qint64 CMDProcessedCommandQueueManager::totalProcessed()
{
    return m_totalProcessed;
}

void CMDProcessedCommandQueueManager::resetTotalProcessed()
{
    m_totalProcessed = 0;
}

qint64 CMDProcessedCommandQueueManager::totalProcessedHistory()
{
    return m_totalProcessedHistory;
}

int CMDProcessedCommandQueueManager::sizeOfProcessedQueue()
{
    QMutexLocker locker(&m_processedQueueMutex);

    return m_processedQueue.size();
}

bool CMDProcessedCommandQueueManager::addToProcessedQueue(CMDCommand* processedCommand)
{
    QMutexLocker locker(&m_processedQueueMutex);
    m_processedQueue.append(processedCommand);

    /*! 控制已处理队列的最大长度，否则，极端情况下，（例如，每秒10万次以上的提交）
        队列的长度可能持续增加，造成内存持续上涨。
        副作用：可能导致延迟时间失效，如果在到达延迟时间之前，
        队列已经超长，则最早进入队列的命令将被删除。
        -----------------------------------------------------------*/
    if(m_processedQueue.count() > MAX_PROCESSED_COUNT)
        delete m_processedQueue.takeFirst();

    return true;
}

/*!
\section 管理回收队列
*/
void CMDProcessedCommandQueueManager::cleanRecycleQueue()
{
    QMutexLocker locker(&m_recycleQueueMutex);

    int size = m_recycleQueue.size();
    for(int i = 0; i < size; i++)
        delete m_recycleQueue.at(i);
    m_recycleQueue.clear();
}

int CMDProcessedCommandQueueManager::sizeOfRecycleQueue()
{
    QMutexLocker locker(&m_recycleQueueMutex);

    return m_recycleQueue.size();
}

bool CMDProcessedCommandQueueManager::addToRecycleQueue(CMDCommand* processedCommand)
{
    QMutexLocker locker(&m_recycleQueueMutex);
    m_recycleQueue.append(processedCommand);

    /*! 控制回收队列的最大长度，否则，极端情况下，（例如，每秒10万次以上的提交）
        队列的长度可能持续增加，造成内存持续上涨。
        -----------------------------------------------------------*/
    if(m_recycleQueue.count() > m_maxRecycleQueueLength)
        delete m_recycleQueue.takeFirst();

    return true;
}

CMDCommand* CMDProcessedCommandQueueManager::fetchRecycleCommand()
{
    QMutexLocker locker(&m_recycleQueueMutex);

    CMDCommand* command = NULL;
    if(!m_recycleQueue.isEmpty())
        command = m_recycleQueue.takeFirst();

    return command;
}

qint32 CMDProcessedCommandQueueManager::maxRecycleQueueLength()
{
    return m_maxRecycleQueueLength;
}

void CMDProcessedCommandQueueManager::setMaxRecycleQueueLength(qint32 maxRecycle)
{
    m_maxRecycleQueueLength = maxRecycle;
}
