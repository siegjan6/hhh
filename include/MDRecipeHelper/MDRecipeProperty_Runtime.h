#ifndef MDRECIPEPROPERTY_RUNTIME_H
#define MDRECIPEPROPERTY_RUNTIME_H
#include <QDateTime>
#include "MDRecipeHelper_Global.h"

enum class DownloadStatus
{
    ready,
    downloading,
    downloaded
};

enum class DownloadFinishFlag
{
    begin,
    success,
    timeout,
    canceled,
    fail_to_load_file
};

class CMDRecipePropertyData_Runtime;
class MDRECIPEHELPERSHARED_EXPORT CMDRecipeProperty_Runtime
{
public:
    CMDRecipeProperty_Runtime();
    virtual ~CMDRecipeProperty_Runtime();
public:
   int retryCount() const;
   void setRetryCount(int count);

   QDateTime downloadBeginTime() const;
   void setDownloadBeginTime(const QDateTime& dt);

// ---------------------------------------------------------
// 下载状态及过程监视
public:
   DownloadStatus status();
   DownloadStatus setStatus(DownloadStatus status, DownloadFinishFlag finishFlag, const QString& msg);
   QString statusMsg();
   bool isFinished();

   DownloadFinishFlag finishFlag();
   DownloadFinishFlag setFinishFlag(DownloadFinishFlag finishFlag, const QString& msg);
   QString finishFlagMsg();
   bool isFinishSuccess();
protected:
    CMDRecipePropertyData_Runtime* m_data;
};

#endif // MDRECIPEPROPERTY_RUNTIME_H
