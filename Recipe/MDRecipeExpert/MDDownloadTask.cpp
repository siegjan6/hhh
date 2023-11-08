#include "MDDownloadTask.h"
#include "MDRecipeFile.h"
#include "MDRecipeContacter.h"
#include <QtMath>
#include "MDDownloadExpert.h"

CMDDownloadTask::CMDDownloadTask()
{
    m_downloadExpert = NULL;

    m_isMemRecipe = false;

//    m_iTotal			= 0;
    m_completedSubCount= 0;
    m_retryCount		= 0;
//    m_dtBegin			= INIT_BEGIN_TIME;
//    m_wBeginMill		= 0;
    m_timeoutBeginTime			= QDateTime::currentDateTime();
//    m_dtEnd				= m_dtBegin;
//    m_wEndMill			= 0;
}

CMDDownloadTask::CMDDownloadTask(CMDDownloadExpert* downloadExpert, const QString& name, CMDRecipe* memRecipe)
{
    m_downloadExpert = downloadExpert;

    m_name = name;
    m_srcRecipe.configProperty()->head()->setName(name);
    m_itemsRemained.configProperty()->head()->setName(name);
    m_itemsCanNotQuick.configProperty()->head()->setName(name);
    m_itemsCanQuick.configProperty()->head()->setName(name);

    m_isMemRecipe = (memRecipe != NULL);
    if(m_isMemRecipe)
    {
        m_srcRecipe = *memRecipe;
    }

//    m_iTotal			= 0;
    m_completedSubCount= 0;
    m_retryCount		= 0;
//    m_dtBegin			= INIT_BEGIN_TIME;
//    m_wBeginMill		= 0;
    m_timeoutBeginTime			= QDateTime::currentDateTime();
//    m_dtEnd				= m_dtBegin;
//    m_wEndMill			= 0;
}

CMDDownloadTask::~CMDDownloadTask()
{
}

CMDDownloadTask::CMDDownloadTask(const CMDDownloadTask& obj)
{
    copy(&obj);
}

CMDDownloadTask& CMDDownloadTask::operator = (const CMDDownloadTask& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

void CMDDownloadTask::copy(const CMDDownloadTask* source)
{
    m_downloadExpert = source->m_downloadExpert;

    m_name = source->m_name;
    m_isMemRecipe = source->m_isMemRecipe;

    m_srcRecipe = source->m_srcRecipe;
    m_itemsRemained = source->m_itemsRemained;
    m_itemsCanNotQuick = source->m_itemsCanNotQuick;
    m_itemsCanQuick = source->m_itemsCanQuick;

    m_completedSubCount= source->m_completedSubCount;
    m_retryCount	 = source->m_retryCount;
    m_timeoutBeginTime = source->m_timeoutBeginTime;
}

void CMDDownloadTask::clear()
{
    m_srcRecipe.clear();
    m_itemsRemained.clear();
    m_itemsCanNotQuick.clear();
    m_itemsCanQuick.clear();

    resetStatus();
}

void CMDDownloadTask::resetStatus()
{
//    m_RecipeWithDSP.ResetStatus();

    m_completedSubCount = 0;
    m_retryCount		= 0;
    m_timeoutBeginTime			= QDateTime::currentDateTime();

    m_para.setStatus(DownloadStatus::downloaded, DownloadFinishFlag::success, "");
}

bool CMDDownloadTask::begin()
{
    if(DownloadStatus::downloading == m_para.status())
        return false;
    m_para.setStatus(DownloadStatus::downloading, DownloadFinishFlag::begin, "");
    m_downloadExpert->m_contacter->onBeginDownloading(m_name, m_srcRecipe.configProperty()->items()->count());

    resetTimedout();					// 开始下载前，复位超时处理

    preHandleDynamic();					// 预处理 m_FileRecipe：如果是动态赋值，则进行转换
    preBuildRIList();					// 根据 m_FileRecipe 构建下载值队列，该过程必须在 PreHandleDynamic() 之后

    bool bCompleted = false;
    if(m_srcRecipe.configProperty()->head()->isSameValueIgnored())	// 需要进行下载前的数值比较
    {
        bCompleted = checkCompleted(true);
    }
//    else RecordStatus(CModuleString::LoadString(IDS_NO_DATA_COMPARE_BEFORE_DOWNLOAD));
    if(!bCompleted)
    {
        tryToDownloadSome();	// 根据下载模式，尝试下载部分变量

        if(m_srcRecipe.configProperty()->head()->retryCount() <= 0)	// 不需要进行下载检测
        {
            end(DownloadFinishFlag::success, "");
        }
    }

    return true;
}

void CMDDownloadTask::preHandleDynamic()
{
    if(m_srcRecipe.configProperty()->head()->isDynamic())
    {
//        RecordStatus(CModuleString::LoadString(IDS_DYNAMIC_DOWNLOAD, m_RecipeWithDSP.GetName()));

        QList<CMDRecipeItem*> itemList;
        m_srcRecipe.configProperty()->items()->enumItems(itemList);
        foreach (CMDRecipeItem* item, itemList)
        {
            preSetDynamicRI(item);
        }
    }
}

void CMDDownloadTask::preSetDynamicRI(CMDRecipeItem* item)
{
    QString varName = item->value();
    QVariant value = m_downloadExpert->m_contacter->varValue(varName);
    if(value.isValid())
    {
        item->setValue(value.toString());
    }
    else
    {
        //        CString strErrMsg = CModuleString::LoadString(IDS_CAN_NOT_FIND_DYNAMIC_VAR, pFileRI->GetName(), strDynamicVarName);
        //        CString strDynamic = CModuleString::LoadString(IDS_DYNAMIC_DOWNLOAD, strErrMsg);
        //        RecordStatus(strDynamic, MB_ICONERROR);
    }
}

void CMDDownloadTask::preBuildRIList()
{
//    RecordStatus(CModuleString::LoadString(IDS_BUILD_RI_LIST));

    bool isQuickDownload = (DownloadMode::quick == m_srcRecipe.downloadMode());

    m_itemsRemained = m_srcRecipe;

    if(isQuickDownload)
    {
        QList<CMDRecipeItem*> itemList;
        m_itemsRemained.configProperty()->items()->enumItems(itemList);
        foreach (CMDRecipeItem* item, itemList)
        {
            if(!m_downloadExpert->m_contacter->isQuickVar(item->id()))
                m_itemsCanNotQuick.configProperty()->items()->addItem(item->id(), item->value());
            else
                m_itemsCanQuick.configProperty()->items()->addItem(item->id(), item->value());
        }
    }
    else
    {
        m_itemsCanQuick = m_srcRecipe;
    }
}

bool CMDDownloadTask::checkCompleted(bool checkBeforeDownload)
{
    if(DownloadStatus::downloaded == m_para.status())
        return true;

    int remainCount = 0;
    QStringList listCompletedRI;
    QList<CMDRecipeItem*> itemList;
    m_itemsRemained.configProperty()->items()->enumItems(itemList);
    foreach (CMDRecipeItem* item, itemList)
    {
        if(helper_isItemCompleted(item))
        {
            listCompletedRI.append(item->id());

            m_itemsRemained.configProperty()->items()->deleteItem(item->id());

            m_completedSubCount ++;

            resetTimedout();	// 任何配方值被完成都将复位超时处理
        }
    }

    // 清除 m_FileRecipe 中已下载的项
    foreach (const QString& itemID, listCompletedRI)
    {
        m_itemsCanQuick.configProperty()->items()->deleteItem(itemID);
    }

    remainCount = m_itemsRemained.configProperty()->items()->count();
    if(remainCount > 0 &&
            0 == m_itemsCanQuick.configProperty()->items()->count())
    {
        switchFastToSub();	// 强制转换成“分包下载”
    }

    // 记录下载状态消息
//    DWORD dwResourceID = IDS_DOWNLOAD_RECIPE_COMPLETED_COUNT_ON_CHECK;
//    if(bBeforeDownload)dwResourceID = (iCompletedCount > 0 ? IDS_DOWNLOAD_RECIPE_COMPLETED_COUNT_BEFORE_DOWNLOAD : IDS_DOWNLOAD_RECIPE_COMPLETED_COUNT_BEFORE_DOWNLOAD_0);
//    CString str, strRemain;
//    str.Format(_T("%d"), iCompletedCount);
//    strRemain.Format(_T("%d"), iRemainCount);
//    CString strCompletedMsg = CModuleString::LoadString(dwResourceID, str, strRemain);
//    if(iCompletedCount > 0)strCompletedMsg = strCompletedMsg + _T("(") + CModuleString::LoadString(IDS_DOWNLOADED_VAR_LIST, helper_BuildCompletedRIString(listCompletedRI)) + _T(")");
//    RecordStatus(strCompletedMsg);

    int total = m_srcRecipe.configProperty()->items()->count();
    m_downloadExpert->traceProcessINFO(QString("checkCompleted: %1/%2").arg(total - remainCount).arg(total), total, (total - remainCount));
    if(remainCount <= 0)
    {
        end(DownloadFinishFlag::success, "");
    }

    return (remainCount <= 0);
}

bool CMDDownloadTask::helper_isItemCompleted(CMDRecipeItem* item)
{
    QVariant varValue = m_downloadExpert->m_contacter->varValue(item->id());
    int q = m_downloadExpert->m_contacter->getVariableQuality(item->id());
    if(!varValue.isValid() || q != 192)
        return false;

//    if(item->id() == "Recipe.str123")
//    {
//        int i = 0;
//    }

    bool result = false;
    switch (varValue.type())
    {
    case QVariant::Bool:
    {
        bool b = helper_stringTobool(item->value());
        result = (b == varValue.toBool());
    }
    break;
    case QVariant::Double:
    {
        qint64 q64Var = qRound64(varValue.toDouble()*qPow(10, 3));
        bool ok;
        double df = item->value().toDouble(&ok);
        if(ok)
        {
            qint64 q64Item = qRound64(df*qPow(10, 3));
            result = (q64Var == q64Item);
        }
    }
    break;
    case QVariant::String:
    {
        result = (0 == item->value().compare(varValue.toString()));
    }
    break;
    case QVariant::DateTime:
    {
        // ISO 8601 extended format:
        // either YYYY-MM-DD for dates or YYYY-MM-DDTHH:MM:SS,
        // YYYY-MM-DDTHH:MM:SSTZD (e.g., 1997-07-16T19:20:30+01:00)
        // for combined dates and times.
        QDateTime dtItem = QDateTime::fromString(item->value(), Qt::ISODate);
        QDateTime dtVar = varValue.toDateTime();
        if(dtItem.isValid() && dtVar.isValid())
            result = (dtItem == dtVar);
    }
    break;
    default:
        result = (0 == item->value().compare(varValue.toString()));
    }
    return result;
}

bool CMDDownloadTask::helper_stringTobool(const QString& str)
{
    if(0 == str.compare("T", Qt::CaseInsensitive) ||
        0 == str.compare("TRUE", Qt::CaseInsensitive) ||
        0 == str.compare("1", Qt::CaseInsensitive))
    {
        return true;
    }
    return false;
}

void CMDDownloadTask::switchFastToSub()
{
    if(DownloadMode::quick == m_srcRecipe.downloadMode())
    {
//        RecordStatus(CModuleString::LoadString(IDS_FAST_TO_SUB), MB_ICONWARNING);

//        m_FileRecipe.EnableQuickRun(FALSE);			// 强制转换成“分包下载”
//        m_FileRecipe.CopyVarData(m_RecipeFastToSub);// 复制必须“分包下载”的配方项
//        m_RecipeFastToSub.Empty();

        m_completedSubCount = m_srcRecipe.configProperty()->head()->itemsCountOnGeneral();	// 设置以便可以继续下一包，参见 tryToDownloadSub()
    }
}

void CMDDownloadTask::resetTimedout()
{
    m_timeoutBeginTime	= QDateTime::currentDateTime();
    m_retryCount = 0;
}

bool CMDDownloadTask::checkTimedOut()
{
    int iMaxRetryCount = m_srcRecipe.configProperty()->head()->retryCount();

    qint64 span = m_timeoutBeginTime.secsTo(QDateTime::currentDateTime());
    if(span >= m_srcRecipe.configProperty()->head()->timeOut())
    {
        m_retryCount ++;

//        CString strRetryCount;
//        strRetryCount.Format(_T("%d"), m_retryCount);
//        CString strMaxRetryCount;
//        strMaxRetryCount.Format(_T("%d"), iMaxRetryCount + 1);
//        RecordStatus(CModuleString::LoadString(IDS_DOWNLOAD_RECIPE_TIMED_OUT, m_RecipeWithDSP.GetName(), strRetryCount, strMaxRetryCount, _T("")), MB_ICONWARNING);

        if(m_retryCount <= iMaxRetryCount)
        {
            m_timeoutBeginTime = QDateTime::currentDateTime();	// 复位超时检测开始时间

//            CString strSeconds;
//            strSeconds.Format(_T("%.0f"), dts.GetTotalSeconds());
//            RecordStatus(CModuleString::LoadString(IDS_RETRY_DOWNLOADING, strRetryCount, strSeconds), MB_ICONWARNING);

            tryToDownloadSome();		// 重试：根据下载模式，尝试下载部分变量
        }
    }

    return (m_retryCount > iMaxRetryCount);	// 重试超过最大次数，下载超时
}

bool CMDDownloadTask::tryToDownloadSome()
{
    if(DownloadMode::quick == m_srcRecipe.downloadMode())
    {
        m_downloadExpert->m_contacter->writeVarQuick(&m_itemsCanQuick);

        if(m_srcRecipe.configProperty()->head()->retryCount() <= 0)		// 不需要重试
        {
            m_downloadExpert->m_contacter->writeVarGeneral(&m_itemsCanNotQuick);// 那么，必须“分包下载”一次 m_RecipeFastToSub 中的配方项

//            if(iSubCount > 0)
//            {
//                CString str;
//                str.Format(_T("%d"), iSubCount);
//                RecordStatus(CModuleString::LoadString(IDS_DOWNLOAD_RECIPE_SUB, str));
//            }
        }

        return true;
    }
    else return downloadSub();
}

bool CMDDownloadTask::tryToDownloadSub()
{
    // m_itemsRemained ?
    if(DownloadMode::quick == m_srcRecipe.downloadMode())
        return false;

    int iMaxSubCount = m_srcRecipe.configProperty()->head()->itemsCountOnGeneral();
    bool result = false;
    if(m_completedSubCount >= iMaxSubCount)	// 完成了一包，继续下一包
        result = downloadSub();

    return result;
}

bool CMDDownloadTask::downloadSub()
{
    if(DownloadStatus::downloaded == m_para.status())
        return false;

    m_completedSubCount = 0;	// 子包下载前复位

    CMDRecipe subRecipe;
    int iSubCount = 0;
    int iMaxSubCount = m_srcRecipe.configProperty()->head()->itemsCountOnGeneral();
    QList<CMDRecipeItem*> itemList;
    m_itemsRemained.configProperty()->items()->enumItems(itemList);
    foreach (CMDRecipeItem* item, itemList)
    {
        iSubCount ++;
        subRecipe.configProperty()->items()->addItem(item->id(), item->value());
        if(iSubCount >= iMaxSubCount)
        {
            break;
        }
    }
    m_downloadExpert->m_contacter->writeVarGeneral(&subRecipe);

//    if(iSubCount > 0)
//    {
//        CString str;
//        str.Format(_T("%d"), iSubCount);
//        RecordStatus(CModuleString::LoadString(IDS_DOWNLOAD_RECIPE_SUB, str));
//    }

    return (iSubCount > 0);
}

bool CMDDownloadTask::end(DownloadFinishFlag finishFlag, const QString& errMsg)
{
    if(DownloadStatus::downloaded == m_para.status())
        return false;

//    CUtility::GetCurrentTimeEx(m_dtEnd, m_wEndMill);

    m_para.setStatus(DownloadStatus::downloaded, finishFlag, errMsg);

    m_downloadExpert->m_contacter->onEndDownloading(m_name, finishFlag);

//    BOOL bBad = (RECIPE_DOWNLOAD_SUCCESS != iFinishFlag && RECIPE_DOWNLOAD_BEGIN != iFinishFlag);
//    UINT uInfoType = (bBad ? MB_ICONWARNING : MB_ICONINFORMATION);

//    CString strR;
//    if(bBad)
//    {
//        strR = BuildRemainedMsg();
//        CString strBadMsg = m_RecipeWithDSP.GetName() + "," + strErrMsg + ",remianed = " + strR;
//        m_FailToDownloadMonitor.AddTip(strBadMsg, "", MB_ICONWARNING);
//        m_pDownloadExpert->m_FailToDownloadTracer.Trace(strBadMsg);
//    }
//    m_pDownloadExpert->m_EndDownloadTracer.Trace(m_RecipeWithDSP.GetName() + "," + strErrMsg + ",remianed = " + strR);

//    RecordStatus(GetStatusMsg(), uInfoType);
//    RecordStatus(CString('-', 32));

//    if(m_pProject)
//        m_pProject->m_RecipeExpert.AddRecipeDownloadMsg(GetName(), iFinishFlag);

    return true;
}
