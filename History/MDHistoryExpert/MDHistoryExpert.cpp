#include "MDHistoryExpert.h"
#include "MDHistoryExpertImp.h"
#include "MDDataArchivesEngine.h"

CMDHistoryExpert::CMDHistoryExpert()
{
    m_imp = new CMDHistoryExpertImp;
}

CMDHistoryExpert::~CMDHistoryExpert()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化与释放
*/
void CMDHistoryExpert::setParaBeforeLoad(const QString& pid, IMDDataArchives *dataArchivesConfiger, CMDHistoryContacter* contacter)
{
    if(m_imp)
        m_imp->setParaBeforeLoad(pid, dataArchivesConfiger, contacter);
}

void CMDHistoryExpert::load()
{
    if(m_imp)
        m_imp->load();
}

void CMDHistoryExpert::unload()
{
    if(m_imp)
        m_imp->unload();
}

/*!
\section 执行保存历史组动作
*/
qint8 CMDHistoryExpert::executeAction(CMDAction* action)
{
    return (m_imp ? m_imp->executeAction(action) : false);
}

bool CMDHistoryExpert::saveHisGroup(const QString& name)
{
    return (m_imp ? m_imp->saveHisGroup(name) : false);
}

/*!
\section 查询归档数据
*/
IMDDBQueryEngine* CMDHistoryExpert::queryEngine()
{
    return (m_imp ? m_imp->queryEngine() : NULL);
}

bool CMDHistoryExpert::queryArchiveData(IMDDBQueryParam* queryPara)
{
    return (m_imp ? m_imp->queryArchiveData(queryPara) : false);
}

IMDDataArchives* CMDHistoryExpert::dataArchivesConfiger()
{
    return (m_imp ? m_imp->m_dataArchivesConfiger : NULL);
}

CMDDataArchivesEngine* CMDHistoryExpert::dataArchivesEngine()
{
    return (m_imp ? m_imp->m_dataArchivesEngine : NULL);
}

CMDQueryTaskMgr* CMDHistoryExpert::qTaskMgr()
{
    return &m_imp->m_qTaskMgr;
}

/*!
\section 设置数据源参数
*/
void CMDHistoryExpert::startSetting()
{
    CMDDataArchivesEngine* engine = dataArchivesEngine();
    if(engine)
        engine->startSetting();
}

void CMDHistoryExpert::endSetting()
{
    CMDDataArchivesEngine* engine = dataArchivesEngine();
    if(engine)
        engine->endSetting();
}

bool CMDHistoryExpert::setDatabaseParam(const QString &datasourceName, int paraType, const QString &para)
{
    CMDDataArchivesEngine* engine = dataArchivesEngine();
    if(engine)
        engine->setDatabaseParam(datasourceName, paraType, para);
    return true;
}

int CMDHistoryExpert::excuteSQLCommand(const QString& datasourceName, const QString& sqlCmd)
{
    CMDDataArchivesEngine* archiveEngine = dataArchivesEngine();
    if(archiveEngine)
    {
        return archiveEngine->exec(datasourceName, sqlCmd);
    }
    return -1;
}

QString CMDHistoryExpert::getLastError(const QString& datasourceName)
{
    CMDDataArchivesEngine* archiveEngine = dataArchivesEngine();
    if(archiveEngine)
    {
        return archiveEngine->getLastError(datasourceName);
    }
    return "";
}
