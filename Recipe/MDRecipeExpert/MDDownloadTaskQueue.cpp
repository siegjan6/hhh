#include "MDDownloadTaskQueue.h"
#include "MDRecipeFile.h"
#include <QMetaObject>

///
/// \brief CMDDownloadTaskQueue::CMDDownloadTaskQueue
///
CMDDownloadTaskQueue::CMDDownloadTaskQueue()
{
}

CMDDownloadTaskQueue::~CMDDownloadTaskQueue()
{
    deleteAllTask();
}

/*!
\section 下载队列
*/
AddRecipeResult CMDDownloadTaskQueue::addNewTask(CMDDownloadExpert* downloadExpert, const QString& name, CMDRecipe* memRecipe)
{
    if(name.isEmpty())
    {
//        m_lsDownloadQueue->LOG_ERROR("Fail to addNewTask: empty_name");
        return AddRecipeResult::empty_name;
    }

    QMutexLocker taskListLocker(&m_taskListMutex);

    if(findTask_NoLocker(name))
    {
//        m_lsDownloadQueue->LOG_ERROR("Fail to addNewTask: name_exist - [" + name + "]");
        return AddRecipeResult::name_exist;
    }

    if(m_taskList.size() >= 4096)
    {
//        m_lsDownloadQueue->LOG_ERROR("Fail to addNewTask: queue_full - [>= 4096]");
        return AddRecipeResult::queue_full;
    }

    CMDDownloadTask* newTask = new CMDDownloadTask(downloadExpert, name, memRecipe);
    if(newTask)
        m_taskList.append(newTask);

    QString taskQueue;
    foreach (CMDDownloadTask* task, m_taskList)
    {
        taskQueue += task->m_name;
        taskQueue += ";";
    }
//    m_lsDownloadQueue->LOG_INFO(QString("Added to queue: %1, count = %2, queue = %3").arg(name).arg(m_taskList.size()).arg(taskQueue));

//    m_lsDownloadProcess->LOG_INFO(QString("Added to queue: %1, count = %2, queue = %3").arg(name).arg(m_taskList.size()).arg(taskQueue));
//    m_lsDownloadProcess->LOG_INFO("emit newTaskAdded");
//    emit newTaskAdded(); // 通知下载线程处理

    return AddRecipeResult::success;
}

bool CMDDownloadTaskQueue::copyFirstTask(CMDDownloadTask& task)
{
    QMutexLocker taskListLocker(&m_taskListMutex);

    CMDDownloadTask* firstTask = (!m_taskList.isEmpty() ? m_taskList.first() : NULL);
    if(firstTask)
    {
        task = *firstTask;
        return true;
    }
    return false;
}

void CMDDownloadTaskQueue::taskNameList(QStringList& nameList)
{
    QMutexLocker taskListLocker(&m_taskListMutex);

    foreach (CMDDownloadTask* task, m_taskList)
        nameList.append(task->m_name);
}

int CMDDownloadTaskQueue::taskCount()
{
    QMutexLocker taskListLocker(&m_taskListMutex);

    return m_taskList.size();
}

QString CMDDownloadTaskQueue::taskName(int index)
{
    QMutexLocker taskListLocker(&m_taskListMutex);

    CMDDownloadTask* task = m_taskList.at(index);
    return (task ? task->m_name : "");
}

bool CMDDownloadTaskQueue::findTask(const QString& name)
{
    QMutexLocker taskListLocker(&m_taskListMutex);

    return findTask_NoLocker(name);
}

bool CMDDownloadTaskQueue::findTask_NoLocker(const QString& name)
{
    foreach (CMDDownloadTask* task, m_taskList)
    {
        if(0 == task->m_name.compare(name, Qt::CaseInsensitive))
            return true;
    }
    return false;
}

void CMDDownloadTaskQueue::deleteSomeTask(const QStringList& nameList)
{
    QMutexLocker taskListLocker(&m_taskListMutex);

    foreach (const QString& name, nameList)
    {
        int i = 0;
        foreach (CMDDownloadTask* task, m_taskList)
        {
            if(0 == task->m_name.compare(name, Qt::CaseInsensitive))
            {
                delete task;
                m_taskList.removeAt(i);
                break;
            }
            i++;
        }
    }

    QString taskQueue;
    foreach (CMDDownloadTask* task, m_taskList)
    {
        taskQueue += task->m_name;
        taskQueue += ";";
    }
//    m_lsDownloadQueue->LOG_INFO(QString("Deleted from queue: %1, count = %2, queue = %3").arg(name).arg(m_taskList.size()).arg(taskQueue));
}

void CMDDownloadTaskQueue::deleteTask(const QString& name)
{
    QStringList nameList;
    nameList.append(name);
    deleteSomeTask(nameList);
}

void CMDDownloadTaskQueue::deleteAllTask()
{
    QMutexLocker taskListLocker(&m_taskListMutex);

    foreach (CMDDownloadTask* task, m_taskList)
    {
        delete task;
    }
    m_taskList.clear();

//    m_lsDownloadQueue->LOG_INFO(QString("queue is empty!"));
}
