#include "MDRecipeProperty_Runtime.h"
#include "MDRecipePropertyData.h"

CMDRecipeProperty_Runtime::CMDRecipeProperty_Runtime()
{
    m_data = new CMDRecipePropertyData_Runtime;
}

CMDRecipeProperty_Runtime::~CMDRecipeProperty_Runtime()
{
    if(m_data)
        delete m_data;
}

int CMDRecipeProperty_Runtime::retryCount() const
{
    return (m_data ? m_data->m_retryCount : 0);
}
void CMDRecipeProperty_Runtime::setRetryCount(int count)
{
    if(m_data)
        m_data->m_retryCount = count;
}

QDateTime CMDRecipeProperty_Runtime::downloadBeginTime() const
{
    return (m_data ? m_data->m_downloadBeginTime : QDateTime());
}
void CMDRecipeProperty_Runtime::setDownloadBeginTime(const QDateTime& dt)
{
    if(m_data)
        m_data->m_downloadBeginTime = dt;
}

DownloadStatus CMDRecipeProperty_Runtime::status()
{
    return m_data->m_dStatus;
}

DownloadStatus CMDRecipeProperty_Runtime::setStatus(DownloadStatus status, DownloadFinishFlag finishFlag, const QString& msg)
{
    DownloadStatus oldStatus = m_data->m_dStatus;
    m_data->m_dStatus = status;

    setFinishFlag(finishFlag, msg);

    return oldStatus;
}

bool CMDRecipeProperty_Runtime::isFinished()
{
    return (DownloadStatus::downloaded == status() || DownloadStatus::ready == status());
}

QString CMDRecipeProperty_Runtime::statusMsg()
{
//    switch(m_uStatus) {
//    case DS_READY:
//        return CModuleString::LoadString(IDS_READY);
//        break;
//    case DS_DOWNLOADING:
//        return CModuleString::LoadString(IDS_DOWNLOADING_RECIPE, m_RecipeWithDSP.GetName());
//        break;
//    case DS_DOWNLOADED:
//        {
//            CString strDownloaded = CModuleString::LoadString(IDS_FINISH_DOWNLOAD_RECIPE, m_RecipeWithDSP.GetName(), GetFinishFlagMsg());
//            return strDownloaded + _T(" - ") + CModuleString::LoadString(IDS_USED_TIME_LENGTH1, CMDLanguageExpert::Format_ms(CaculaDownloadedElapse()));
//        }
//        break;

//    default:
//        return CModuleString::LoadString(IDS_UNKNOWN_STATUS);
//    }
    return "";
}

DownloadFinishFlag CMDRecipeProperty_Runtime::finishFlag()
{
    return m_data->m_dFinishFlag;
}

DownloadFinishFlag CMDRecipeProperty_Runtime::setFinishFlag(DownloadFinishFlag finishFlag, const QString& msg)
{
    DownloadFinishFlag oldFlag = m_data->m_dFinishFlag;
    m_data->m_dFinishFlag = finishFlag;

    m_data->m_dMsg = msg;

    return oldFlag;
}

QString CMDRecipeProperty_Runtime::finishFlagMsg()
{
//    switch(m_iFinishFlag) {
//    case RECIPE_DOWNLOAD_BEGIN:
//        return CModuleString::LoadString(IDS_FINISH_RECIPE_NO, _T(""));
//        break;
//    case RECIPE_DOWNLOAD_SUCCESS:
//        return CModuleString::LoadString(IDS_FINISH_RECIPE_SUCCESS, _T(""));
//        break;
//    case RECIPE_DOWNLOAD_CANCEL:
//        return CModuleString::LoadString(IDS_FINISH_RECIPE_ERR_FROM_LOCAL, m_strErrMsg);
//        break;
//    case RECIPE_DOWNLOAD_TIMEOUT:
//        return CModuleString::LoadString(IDS_FINISH_RECIPE_ERR_LOCAL_TIMED_OUT, _T(""));
//        break;

//    default:
//        return CModuleString::LoadString(IDS_UNKNOWN_STATUS);
//    }
    return "";
}

bool CMDRecipeProperty_Runtime::isFinishSuccess()
{
    return (DownloadFinishFlag::success == finishFlag());
}
