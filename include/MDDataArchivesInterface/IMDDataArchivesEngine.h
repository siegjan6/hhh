#ifndef MDDATAARCHIVESENGINE_H
#define MDDATAARCHIVESENGINE_H

#include "nsdataarchivesengine_global.h"

class CMDDataArchivesCore;
class IProjectDirector;
class CMDMsgManager;
class CMDDataArchivesNotifyParam;

namespace DataArchives {
//消息类型
enum DB_NOTIFY_TYPE
{
    DB_NOTIFY_ADD			=	0,
    DB_NOTIFY_UPDATE		=	1,
    DB_NOTIFY_DELETE		=	2,
    DB_NOTIFY_ALM_OFF		=	3,
};

}

class MDDATAARCHIVESENGINESHARED_EXPORT CMDDataArchivesEngine
{
public:
    CMDDataArchivesEngine();
    ~CMDDataArchivesEngine();
public:
    void setProjectDirector(IProjectDirector *pDirector);
    IProjectDirector *projectDirector();

    void setDebugManager(CMDMsgManager *pManager);
public:
    // 启动服务
    bool start();
    // 停止服务
    bool stop();
public:
    // 异步触发数据归档
    bool asyncCallDataArchives(DataArchives::DB_NOTIFY_TYPE nMSGType, CMDDataArchivesNotifyParam *pParam);
private:
    CMDDataArchivesCore *m_nsDataArchivesCore;
};

#endif // MDDATAARCHIVESENGINE_H
