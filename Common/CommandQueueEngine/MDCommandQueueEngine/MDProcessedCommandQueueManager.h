/*!

\author dGFuZ3p3

\section 功能

管理【已经处理的命令】和【回收命令】。

\section 线程特性

此类提供的接口都是线程安全的。

\code
CMDProcessedCommandQueueManager pcqManager;
pcqManager.setModeToHandleProcessed(MDModeToHandleProcessedCommand::mode_delete);
pcqManager.setDelayTimeLength(2500);
\endcode

*/

#ifndef MDPROCESSEDCOMMANDQUEUEMANAGER_H
#define MDPROCESSEDCOMMANDQUEUEMANAGER_H

#include <QtGlobal>
#include <QMutex>
#include <QDateTime>
#include <QLinkedList>
#include "../../../include/MDCommandQueueEngine/MDCommandQueueEngine.h"

class CMDProcessedCommandQueueManager
{
public:
    CMDProcessedCommandQueueManager();
    ~CMDProcessedCommandQueueManager();

/*!
\section 处理模式；延迟处理（缺省处理模式：立即回收）
*/
public:
    MDModeToHandleProcessedCommand modeToHandleProcessed();
    void setModeToHandleProcessed(MDModeToHandleProcessedCommand mode);
protected:
    MDModeToHandleProcessedCommand m_modeToHandleProcessed;
public:
    bool isDelayHandleProcessed();
    void setDelayHandleProcessed(bool flag);

    int delayTimeLength();
    void setDelayTimeLength(int timeLength);
protected:
    bool m_delayFlag; //! 延迟标识
    int m_delayTimeLength; //! 延迟时间长度

/*!
\section 根据配置，决定如何处理命令，参考主函数 dealWithCommand
*/
public:
    /*!
     处理已经处理的命令。

     \param processedCommand 一个已经处理完成的命令。
     \returns 无。
     \note 这是类的一个主要处理函数。
     */
    void dealWithCommand(CMDCommand* processedCommand);
    void cleanQueue(); //! 清理队列，队列所占内存将被释放
public:
    void checkProcessedToHandle();
    void handleProcessedCommand(CMDCommand* processedCommand);
public:
    bool isTimeToDelayHandling(); //! 如果为 true，应该检查命令的延迟时间是否已到
protected:
    QDateTime m_lastTimeStampForDelayHandling;

/*!
\section 管理已处理队列
*/
public:
    int sizeOfProcessedQueue();
    bool addToProcessedQueue(CMDCommand* processedCommand);
    qint64 totalProcessed();
    void resetTotalProcessed();
    qint64 totalProcessedHistory();
protected:
    void cleanProcessedQueue();
    QLinkedList<CMDCommand*> m_processedQueue;
    QMutex m_processedQueueMutex;
    qint64 m_totalProcessed;
    qint64 m_totalProcessedHistory;

/*!
\section 管理回收队列
*/
public:
    int sizeOfRecycleQueue();
    bool addToRecycleQueue(CMDCommand* processedCommand);
    CMDCommand* fetchRecycleCommand();
protected:
    void cleanRecycleQueue();
    QList<CMDCommand*> m_recycleQueue;
    QMutex m_recycleQueueMutex;
public:
    /*!
     调整回收队列的最大容量，在处理高吞吐量的任务时，
     可以调整此参数获得最佳性能。
     */
    qint32 maxRecycleQueueLength();
    void setMaxRecycleQueueLength(qint32 maxRecycle);
protected:
    qint32 m_maxRecycleQueueLength;
};

#endif // MDPROCESSEDCOMMANDQUEUEMANAGER_H
