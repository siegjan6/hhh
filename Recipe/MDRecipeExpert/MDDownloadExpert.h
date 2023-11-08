#ifndef MDRECIPEDOWNLOADEXPERT_H
#define MDRECIPEDOWNLOADEXPERT_H
#include "MDDownloadTaskQueue.h"
#include "MDDownloadWorker.h"
#include "MDSwitchFactory/MDFuncSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"

class CMDRecipeContacter;
class CMDRecipeExpertImp;
class CMDDownloadExpert : public QObject
{
    Q_OBJECT
public:
    CMDDownloadExpert();
    void setParaBeforeLoad(CMDRecipeExpertImp* recipeExpert, CMDRecipeContacter* contacter);
    void setDownloadProcessContacter(CMDRecipeContacter* contacter);
    void load();
    void unload();
public:
    CMDRecipeExpertImp* m_recipeExpert;
    CMDRecipeContacter* m_contacter;
    CMDRecipeContacter* m_downloadProcessContater;
public:
    void traceProcessINFO(const QString& msg);
    void traceProcessINFO(const QString& msg, int total, int completed);
    void traceProcessERROR(const QString& msg);
protected:
    void createSwitchs(bool create);
    CMDLogSwitch* m_lsDownloadProcess;
    CMDLogSwitch* m_lsDownloadQueue;
    CMDLogSwitch* m_lsFailToDownload;
    CMDLogSwitch* m_lsBeginEndDownload;
/*!
\section 下载任务队列
*/
public:
    AddRecipeResult addNewTask(const QString& name, CMDRecipe* memRecipe = NULL, int showProcessDialog=1);
    CMDDownloadTaskQueue m_taskQueue;
signals:
    void newTaskAdded();

/*!
\section 当前下载任务
*/
public:
    void check();				// 通过检测 m_downloadingTask 的状态，控制队列顺序执行
    bool begin();				// 执行队列中的第一个配方，该配方成为 m_downloadingTask
    void end(DownloadFinishFlag iFinishFlag);		// 结束 m_downloadingTask
public:
    CMDDownloadTask m_downloadingTask;
public:
    void endSome(QStringList& endList, DownloadFinishFlag finishFlag);	// 取消队列 endList 中配方的执行
    void endAll(DownloadFinishFlag finishFlag);								// 取消队列 m_taskList 中配方的执行

/*!
\section 下载线程
*/
protected:
    CMDDownloadWorker* m_downloadWorker;
};

#endif // MDRECIPEDOWNLOADEXPERT_H
