#ifndef MDDOWNLOADTASK_H
#define MDDOWNLOADTASK_H
#include "MDRecipe.h"
#include "MDRecipeProperty_Runtime.h"

class CMDRecipeContacter;
class CMDDownloadExpert;
class CMDDownloadTask
{
public:
    CMDDownloadTask();
    CMDDownloadTask(CMDDownloadExpert* downloadExpert, const QString& name, CMDRecipe* memRecipe);
    ~CMDDownloadTask();
    CMDDownloadTask(const CMDDownloadTask& obj);
    CMDDownloadTask& operator = (const CMDDownloadTask& obj);
    void copy(const CMDDownloadTask* source);
    void clear();
    void resetStatus();
public:
    CMDRecipe m_srcRecipe;
    CMDRecipe m_itemsRemained;
    CMDRecipe m_itemsCanNotQuick;
    CMDRecipe m_itemsCanQuick;
    bool m_isMemRecipe;
    QString m_name;
    CMDDownloadExpert* m_downloadExpert;
public:	// --- 正常下载流程控制
    bool begin();
    bool end(DownloadFinishFlag finishFlag, const QString& errMsg);
    bool checkCompleted(bool checkBeforeDownload);
    bool helper_isItemCompleted(CMDRecipeItem* item);
    bool helper_stringTobool(const QString& str);
    bool tryToDownloadSome();	// 根据下载模式，尝试下载部分变量
    bool tryToDownloadSub();	// 在“分包下载”模式下，如果完成一包，则尝试下载下一包
protected:
    int m_completedSubCount;	// 每包写指令发出后，累计完成的子包变量个数，确保上一包完成才继续下一包
    bool downloadSub();			// 按照“分包下载”模式下载
protected:
    void switchFastToSub();
    CMDRecipeProperty_Runtime m_para;
protected:
    void preHandleDynamic();				// 预处理 m_FileRecipe：如果是动态赋值，则进行转换
    void preSetDynamicRI(CMDRecipeItem* item);// 对 m_FileRecipe 中的 pFileRI 进行转换
    void preBuildRIList();					// 根据 m_FileRecipe 构建下载值队列，该过程必须在 PreHandleDynamic() 之后

/*!
\section 超时与重试
*/
public:
    bool checkTimedOut();
    void resetTimedout();
protected:
    QDateTime m_timeoutBeginTime; // 超时检测开始时间
    int m_retryCount; // 重试次数
};

#endif // MDDOWNLOADTASK_H
