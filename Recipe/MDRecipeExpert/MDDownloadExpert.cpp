#include "MDDownloadExpert.h"
#include "MDRecipeExpertImp.h"
#include "MDRecipeFile.h"
#include <QMetaObject>
#include "MDRecipeContacter.h"
#include "MDRecipeConfiger.h"

///
/// \brief CMDDownloadExpert::CMDDownloadExpert
///
CMDDownloadExpert::CMDDownloadExpert()
{
    m_recipeExpert = NULL;
    m_contacter = NULL;
    m_downloadProcessContater = NULL;

    m_downloadingTask.m_downloadExpert = this;
    m_downloadWorker = NULL;

    createSwitchs(false);
}

void CMDDownloadExpert::setParaBeforeLoad(CMDRecipeExpertImp* recipeExpert, CMDRecipeContacter* contacter)
{
    m_recipeExpert = recipeExpert;
    m_contacter = contacter;
}

void CMDDownloadExpert::setDownloadProcessContacter(CMDRecipeContacter* contacter)
{
    m_downloadProcessContater = contacter;
}

void CMDDownloadExpert::load()
{
    createSwitchs(true);

    m_downloadWorker = new CMDDownloadWorker(this);
    m_downloadWorker->startThread();
}

void CMDDownloadExpert::unload()
{
    m_downloadWorker->stopThread(100);

    m_downloadingTask.clear();
    endAll(DownloadFinishFlag::canceled);

    createSwitchs(false);
}

void CMDDownloadExpert::createSwitchs(bool create)
{
    if(create)
    {
        QString subFileDir = "ProjectExpert";
        m_lsDownloadProcess = CMDLogSwitchFactory::createSwitchWithSubFileDir(m_recipeExpert->m_pid, "RecipeExpert.DownloadProcess", "配方下载流程", subFileDir);
        m_lsDownloadProcess->turnOn();
        m_lsDownloadQueue = CMDLogSwitchFactory::createSwitchWithSubFileDir(m_recipeExpert->m_pid, "RecipeExpert.DownloadQueue", "配方下载队列", subFileDir);
        m_lsDownloadQueue->turnOn();
        m_lsFailToDownload = CMDLogSwitchFactory::createSwitchWithSubFileDir(m_recipeExpert->m_pid, "RecipeExpert.FailToDownload", "配方下载失败", subFileDir);
        m_lsFailToDownload->turnOn();
        m_lsBeginEndDownload = CMDLogSwitchFactory::createSwitchWithSubFileDir(m_recipeExpert->m_pid, "RecipeExpert.BeginEndDownload", "配方下载开始和结束", subFileDir);
        m_lsBeginEndDownload->turnOn();
    }
    else
    {
        m_lsDownloadProcess = NULL;
        m_lsDownloadQueue = NULL;
        m_lsFailToDownload = NULL;
        m_lsBeginEndDownload = NULL;
    }
}

void CMDDownloadExpert::traceProcessINFO(const QString& msg)
{
    m_lsDownloadProcess->LOG_INFO(msg);
    m_downloadProcessContater->onDownloadProcess(msg, -1, -1);
}

void CMDDownloadExpert::traceProcessINFO(const QString& msg, int total, int completed)
{
    m_lsDownloadProcess->LOG_INFO(msg);
    m_downloadProcessContater->onDownloadProcess(msg, total, completed);
}

void CMDDownloadExpert::traceProcessERROR(const QString& msg)
{
    m_lsDownloadProcess->LOG_ERROR(msg);
    m_downloadProcessContater->onDownloadProcess(msg, -2, -2);
}

/*!
\section 下载队列
*/
AddRecipeResult CMDDownloadExpert::addNewTask(const QString& name, CMDRecipe* memRecipe, int showProcessDialog/*=1*/)
{
    AddRecipeResult result = m_taskQueue.addNewTask(this, name, memRecipe);
    if(result == AddRecipeResult::success)
    {
        traceProcessINFO(QString("addNewTask: %1 showProcessDialog:%2").arg(name).arg(showProcessDialog));
        m_downloadProcessContater->onDownloadTaskAdded(name, showProcessDialog);

        emit newTaskAdded(); // 通知下载线程处理
    }

    return result;
}

/*!
\section 下载任务控制
*/

bool CMDDownloadExpert::begin()
{
    if(!m_taskQueue.copyFirstTask(m_downloadingTask))
    {
        traceProcessERROR("Fail to begin : task queue is empty!");
        return false;
    }

    traceProcessINFO(QString("begin: %1").arg(m_downloadingTask.m_name));
    m_downloadProcessContater->onBeginDownloading(m_downloadingTask.m_name, m_downloadingTask.m_srcRecipe.configProperty()->items()->count());

    if(!m_downloadingTask.m_isMemRecipe)
    {
        QString recipeFileName = m_recipeExpert->m_configer->toFileName(m_downloadingTask.m_name);
        if(!CMDRecipeFile::load(recipeFileName, m_downloadingTask.m_srcRecipe))
        {
            traceProcessERROR(QString("%1=Fail to begin : ERROR on load recipe file! [%2]").arg(m_downloadingTask.m_name).arg(recipeFileName));

            end(DownloadFinishFlag::fail_to_load_file);

            return false;
        }
    }
    QString src = m_downloadingTask.m_srcRecipe.configProperty()->items()->toString();
    m_lsBeginEndDownload->LOG_INFO(QString("%1=%2").arg(m_downloadingTask.m_name).arg(src));

//    m_downloadProcessContater->onDownloadProcess("begin", 1);

    return m_downloadingTask.begin();
}

void CMDDownloadExpert::end(DownloadFinishFlag iFinishFlag)
{
    QString name = m_downloadingTask.m_name;

    m_taskQueue.deleteTask(name);

    QString errMsg;
    QString remained = m_downloadingTask.m_itemsRemained.configProperty()->items()->toString();
    if(iFinishFlag != DownloadFinishFlag::begin ||
            iFinishFlag != DownloadFinishFlag::success)
    {
        if(DownloadFinishFlag::timeout == iFinishFlag)
            errMsg = "timeout";
        else if(DownloadFinishFlag::canceled == iFinishFlag)
            errMsg = "canceled";
        else if(DownloadFinishFlag::fail_to_load_file == iFinishFlag)
            errMsg = "fail_to_load_file";

        m_lsFailToDownload->LOG_ERROR(QString("%1=%2, EndMsg =%3").arg(name).arg(remained).arg(errMsg));
    }
    m_lsBeginEndDownload->LOG_INFO(QString("%1=%2, EndMsg =%3").arg(name).arg(remained).arg(errMsg));

    m_downloadingTask.end(iFinishFlag, "");
    m_downloadingTask.clear();

    traceProcessINFO(QString("end: %1").arg(name));
    m_downloadProcessContater->onEndDownloading(name, iFinishFlag);
}

void CMDDownloadExpert::check()
{
    traceProcessINFO("begin checkDownloading");

    if(m_downloadingTask.checkCompleted(false))
    {
        traceProcessINFO(QString("m_downloadingTask OK ! - %1").arg(m_downloadingTask.m_name));

        begin();
        //end(DownloadFinishFlag::success);
    }
    else
    {

        if(m_downloadingTask.checkTimedOut())
        {
            traceProcessINFO(QString("m_downloadingTask TIMEOUT ! - %1").arg(m_downloadingTask.m_name));

            begin();
            end(DownloadFinishFlag::timeout);
        }
        else
        {
            traceProcessINFO(QString("m_downloadingTask tryToDownloadSub ! - %1").arg(m_downloadingTask.m_name));

            m_downloadingTask.tryToDownloadSub();	// 尝试下载下一包，仅对“分包下载”模式有效
        }
    }

    if(m_downloadingTask.checkCompleted(false))
    {
        traceProcessINFO(QString("m_downloadingTask OK ! - %1").arg(m_downloadingTask.m_name));

        end(DownloadFinishFlag::success);
    }

    traceProcessINFO("end checkDownloading");
}

void CMDDownloadExpert::endSome(QStringList& endList, DownloadFinishFlag finishFlag)
{
    // 1、首先将 endList 中的“正在下载的配方”和“等待配方”区分开
    QStringList waitingList;
    bool downloadingIncluded = false;
    foreach (const QString& name, endList)
    {
        if(0 != name.compare(m_downloadingTask.m_name, Qt::CaseInsensitive))
            waitingList.append(name);
        else
            downloadingIncluded = true;
    }

    // 2、endList 中的配方都要从队列移除
    m_taskQueue.deleteSomeTask(endList);

    // 3、可能需要结束正在下载的配方，并为其触发脚本事件
    if(downloadingIncluded)
    {
        end(finishFlag);
        begin();
    }

    // 4、为“等待配方”触发脚本事件
    foreach (const QString& name, waitingList)
    {
        m_contacter->onEndDownloading(name, finishFlag);
    }
}

void CMDDownloadExpert::endAll(DownloadFinishFlag finishFlag)
{
    QStringList taskList;
    m_taskQueue.taskNameList(taskList);

    endSome(taskList, finishFlag);
}
