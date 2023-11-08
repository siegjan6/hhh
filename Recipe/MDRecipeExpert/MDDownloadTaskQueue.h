#ifndef MDRECIPEDOWNLOADTASKQUEUE_H
#define MDRECIPEDOWNLOADTASKQUEUE_H
#include <QObject>
#include <QList>
#include <QMutex>
#include "MDRecipeFile.h"
#include "MDSwitchFactory/MDFuncSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"
#include "MDDownloadTask.h"

enum class AddRecipeResult
{
    empty_name,
    name_exist,
    queue_full,
    success
};

class CMDDownloadTaskQueue : public QObject
{
    Q_OBJECT
public:
    CMDDownloadTaskQueue();
    ~CMDDownloadTaskQueue();

/*!
\section 下载任务队列
*/
public:
    AddRecipeResult addNewTask(CMDDownloadExpert* downloadExpert, const QString& name, CMDRecipe* memRecipe = NULL);
    bool copyFirstTask(CMDDownloadTask& task);
public:
    void taskNameList(QStringList& nameList);
    int taskCount();
public:
    QString taskName(int iIndex);
    bool findTask(const QString& name);
protected:
    bool findTask_NoLocker(const QString& name);
public:
    void deleteSomeTask(const QStringList& nameList);
    void deleteTask(const QString& name);
    void deleteAllTask();
protected:
    QList<CMDDownloadTask*> m_taskList;
    QMutex m_taskListMutex;
};

#endif // MDRECIPEDOWNLOADTASKQUEUE_H
