#include "DBQueryEngine.h"
#include "DBQueryCommand.h"
#include "MDQueryDatabaseThread.h"
#include "MDDBConnCache.h"
#include "DBSourcesMgr.h"
#include "ViewVarMgr.h"
#include "ViewMgr.h"
#include "utils.h"
#include <QSet>
#include <QAtomicInt>
#include <QSqlDatabase>
#include <MDSQLDataBaseProxy/MDDatabaseRecordset.h>

namespace DBQE
{
static QAtomicInt queryCount;

typedef QMap<QString, CMDDatabaseRecordset *> RecordsetMap;
class RecordSetCache
{
public:
    CMDDatabaseRecordset * find(const QString & queryID);
    //函数功能:查询超时后,将超时的query的ID加入Cache
    //说明:.在每次查询时,都会遍历此列表,尝试删除对应的Recordset
    void addQueryIDTimeouted(const QString & queryID);
    void deleteRecordset(const QString & queryID);
    void addRecordset(const QString & queryID, CMDDatabaseRecordset * pRecordset);
 protected:
    CMDDatabaseRecordset * findNoLock(const QString & queryID);
 protected:
    RecordsetMap m_RecordsetMap;
    QSet<QString> m_KeysTimeout; //使用QSet 保证不插入重复元素
    QMutex m_Mutux;
};

CMDDatabaseRecordset * RecordSetCache::find(const QString & queryID)
{
    QMutexLocker locker(&m_Mutux);
    return findNoLock(queryID);
}

CMDDatabaseRecordset * RecordSetCache::findNoLock(const QString & queryID)
{
    RecordsetMap::iterator ite = m_RecordsetMap.find(queryID);
    if(ite == m_RecordsetMap.end())
    {
        return 0;
    }
    return ite.value();
}

//函数功能:查询超时后,将超时的query的ID加入Cache
//说明:.在每次查询时,都会遍历此列表,尝试删除对应的Recordset
void RecordSetCache::addQueryIDTimeouted(const QString & queryID)
{
    QMutexLocker locker(&m_Mutux);
    m_KeysTimeout.insert(queryID);
}

void RecordSetCache::deleteRecordset(const QString & queryID)
{
    QMutexLocker locker(&m_Mutux);
    CMDDatabaseRecordset * pRecord = findNoLock(queryID);
    if(pRecord)
        delete pRecord;
    m_RecordsetMap.remove(queryID);

    //删除超时的查询 //TODO：待测试
    //QList<QString> keysRemoved;
    foreach(const QString & key, m_KeysTimeout)
    {
        pRecord = findNoLock(key);
        if(pRecord)
        {
            delete pRecord;
            m_RecordsetMap.remove(key);
            //keysRemoved.append(key);
            m_KeysTimeout.remove(key);
            DBQE::WriteInfoLog(QString("RecordSetCache::deleteRecordset:delete timeout query id:%1").arg(key));
        }
    }

    //foreach(const QString & key, keysRemoved)
    //{
        //m_KeysTimeout.remove(key);
    //}
}

void RecordSetCache::addRecordset(const QString & queryID, CMDDatabaseRecordset * pRecordset)
{
    QMutexLocker locker(&m_Mutux);
    if(queryID.isEmpty())
        return ;

    //id不可能重复 所以不会有内存泄漏
    CMDDatabaseRecordset * pRecord = findNoLock(queryID);
    if(pRecord)
    {
        DBQE::WriteInfoLog(QString("RecordSetCache::addRecordset:%1 has exsited.").arg(queryID));
        return ;
    }

    m_RecordsetMap.insert(queryID, pRecordset);
}

static RecordSetCache rsCache;

class ReportNotify : public IMDDBRecordSetNotify
{
public:
    virtual void NotityDBQueryResult(const QString &strQueryID, CMDDatabaseRecordset *);
};

void ReportNotify::NotityDBQueryResult(const QString & strQueryID, CMDDatabaseRecordset * recordset)
{
    DBQE::WriteInfoLog(QString("ReportNotify::NotityDBQueryResult:query id=%1").arg(strQueryID));
    rsCache.addRecordset(strQueryID, recordset);
}

static ReportNotify localNotify;

//======================== CMDDBQueryService begin ===============================

CMDDBQueryEngine::CMDDBQueryEngine()
{
    m_pQueryThread = new CMDQueryDatabaseThread;
    m_pDBSourcesMgr =new DBSourcesMgr,
    m_pViewVarMgr = new ViewVarMgr,
    m_pViewMgr = new ViewMgr;
}

CMDDBQueryEngine::~CMDDBQueryEngine()
{
    unload();
}

//启动 停止服务
bool CMDDBQueryEngine::load(const QString & configFileDir)
{
    DBQE::WriteInfoLog("CMDDBQueryEngine::load");

    m_ConfigFileDir = configFileDir;
    QString dbSrcsFile;
    dbSrcsFile.append(configFileDir);
    dbSrcsFile.append("/");
    dbSrcsFile.append("global_dbsources.json"); //TODO:使用封装好的函数
    if(!m_pDBSourcesMgr->Init(dbSrcsFile))
    {
        DBQE::WriteInfoLog("CMDDBQueryEngine::load:init db source fail");
        return false;
    }

    QString viewVarFile;
    viewVarFile.append(configFileDir);
    viewVarFile.append("/");
    viewVarFile.append("global_view_vars.json"); //TODO:使用封装好的函数
    if(!m_pViewVarMgr->Init(viewVarFile))
    {
        DBQE::WriteInfoLog("CMDDBQueryEngine::load:init view variable fail");
        return false;
    }

    m_pViewMgr->init(m_pDBSourcesMgr, m_pViewVarMgr, configFileDir);
    if(m_pQueryThread)
    {
        MDDBConnCache::Init();
        m_pQueryThread->start();
    }
    else
        DBQE::WriteInfoLog("CMDDBQueryEngine::load:query thread is null");

    //在程序首次启动时，清空运行时动态参数配置文件.
    DBQ::RuntimeViewVariable_DeleteAllFile(configFileDir);
    return true;
}

bool CMDDBQueryEngine::unload()
{
    //removeAllQueryCmd();

    if(m_pQueryThread)
    {
        MDDBConnCache::DestroyCache();
        m_pQueryThread->stop();
        delete m_pQueryThread;
        m_pQueryThread = NULL;
    }

    return true;
}

bool CMDDBQueryEngine::queryView(const QString & viewName, IMDDBRecordSetNotify * pResultNotify, QString & queryIDOut)
{
    QMutexLocker locker(&m_Mutex);

    if (NULL == m_pQueryThread)
    {
        DBQE::WriteInfoLog("CMDDBQueryEngine::queryView:thread is null");
        return false;
    }

    ViewAssist * pVA = 0;
    if(!(pVA = m_pViewMgr->getView(viewName)))
    {
        DBQE::WriteInfoLog(QString("CMDDBQueryEngine::queryView:getView fail:viewName=%1").arg(viewName));
        return false;
    }

    if(!m_pViewMgr->checkFilterParameter(viewName))
    {
        DBQE::WriteInfoLog(QString("CMDDBQueryEngine::queryView:filter parameters invaild:viewName=%1").arg(viewName));
        return false;
    }

    int count = m_pViewMgr->getDBSourcesCount(viewName);

    queryCount.ref();
    QString queryID = QString::number(queryCount.load());
    queryIDOut = queryID;

    if(count>1) //count有可能是-1
    {
        DBQE::WriteInfoLog(QString("CMDDBQueryEngine::queryView:the count of data sources > 1:viewName=%1").arg(viewName));
        //跨数据源查询
        return false;
    }
    else if(count==1)
    {
        //同数据源查询
        m_QueryStatusMap.insert(queryID, DBQUERY_STATUS_QUERYING); //queryID不会重复 所以如果map中不会有要添加的queryid
        return queryWithOneDBSource(queryID, viewName, pResultNotify);
    } else if(count == 0)
    {
        //按默认数据源查询
        m_QueryStatusMap.insert(queryID, DBQUERY_STATUS_QUERYING);
        DBQE::WriteInfoLog(QString("CMDDBQueryEngine::queryView:use current dbs:viewName=%1").arg(viewName));
        return queryWithDefaultDBSource(queryID, viewName, pResultNotify);
    }
    else
    {
        DBQE::WriteInfoLog(QString("CMDDBQueryEngine::queryView:the count for db source is error:viewName=%1").arg(viewName));
        return false;
    }

    return false;
}

QString CMDDBQueryEngine::getViewPath()
{
    return m_ConfigFileDir;
}

//函数功能:使用带有缓存机制的查询接口来查询视图 queryID全局唯一
bool CMDDBQueryEngine::queryViewWithCache(const QString & viewName, QString & queryIDOut)
{
    return queryView(viewName, &localNotify, queryIDOut);
}

CMDDatabaseRecordset * CMDDBQueryEngine::findWithCache(const QString & queryID)
{
    return rsCache.find(queryID);
}

void CMDDBQueryEngine::deleteByIDWithCache(const QString & queryID)
{
    rsCache.deleteRecordset(queryID);
}

void CMDDBQueryEngine::updateAllViewCache()
{
    QMutexLocker locker(&m_Mutex);

    QStringList viewNames;
    DBQ::View_GetAllViewNameFromDir(m_ConfigFileDir, viewNames);
    m_pViewMgr->updateAllViewCache(viewNames);
}

DBQUERY_STATUS CMDDBQueryEngine::getQueryStatus(const QString & queryID)
{
    //TODO:到一定量后在删除
    QMutexLocker locker(&m_Mutex);

    DBQueryStatusMap::iterator ite = m_QueryStatusMap.find(queryID);
    if(m_QueryStatusMap.end() == ite)
        return DBQUERY_STATUS_NOEXSIST;

    return ite.value();
}

void CMDDBQueryEngine::setQueryStatus(const QString &queryID, DBQUERY_STATUS status)
{
    QMutexLocker locker(&m_Mutex);
    m_QueryStatusMap[queryID] = status; //若没有queryID 则会将queryID添加到map中
}

bool CMDDBQueryEngine::queryWithOneDBSource(const QString & queryID, const QString & viewName, IMDDBRecordSetNotify *  pQueryResultNotify)
{
    QString strSql = m_pViewMgr->getSQLFromView(viewName);
    if(strSql.isEmpty())
    {
        DBQE::WriteInfoLog(QString("CMDDBQueryEngine::queryWithOneDBSource:get sql error:view name=%1").arg(viewName));
        return false;
    }

    CDataBaseParam * pDBConnParam = m_pViewMgr->getDBConnectionParam(viewName); //复制出一个Param 由调用者删除
    if(!pDBConnParam)
    {
        DBQE::WriteInfoLog(QString("CMDDBQueryEngine::queryWithOneDBSource:conn param is null:view name=%1").arg(viewName));
        return false;
    }

    CDBQueryCommand * pQueryCmd = new CDBQueryCommand(queryID, strSql);
    pQueryCmd->setDBParam(pDBConnParam);
    pQueryCmd->setQueryRslNotify(pQueryResultNotify);
    pQueryCmd->setQueryEngine(this);
    pQueryCmd->start(viewName);

    CMDCommand *pCmd = new CMDCommand();
    pCmd->setCmdPara(reinterpret_cast<void*>(pQueryCmd));
    m_pQueryThread->submitCommand(pCmd);

    return true;
}

bool CMDDBQueryEngine::queryWithDefaultDBSource(const QString & queryID, const QString & viewName, IMDDBRecordSetNotify *  pQueryResultNotify)
{
    DBQ::DBSource * pDBSource = m_pDBSourcesMgr->GetDefaultDBSource();
    if(!pDBSource)
    {
        DBQE::WriteInfoLog(QString("CMDDBQueryEngine::queryWithDefaultDBSource:hasn't exist default db source:view name=%1").arg(viewName));
        return false;
    }
    QString strSql = m_pViewMgr->getSQLFromViewWithDBSource(viewName, pDBSource);
    if(strSql.isEmpty())
    {
        DBQE::WriteInfoLog(QString("CMDDBQueryEngine::queryWithDefaultDBSource:generate SQL error:view name=%1").arg(viewName));
        delete pDBSource;
        return false;
    }

    CDataBaseParam * pDBConnParam = m_pViewMgr->getDBConnParamWithDBSourceName(pDBSource->dbSrcName); //复制出一个Param 由调用者删除
    if(!pDBConnParam)
    {
        DBQE::WriteInfoLog(QString("CMDDBQueryEngine::queryWithDefaultDBSource:conn param is null:view name=%1").arg(viewName));
        delete pDBSource;
        return false;
    }

    CDBQueryCommand * pQueryCmd = new CDBQueryCommand(queryID, strSql);
    pQueryCmd->setDBParam(pDBConnParam);
    pQueryCmd->setQueryRslNotify(pQueryResultNotify);
    pQueryCmd->setQueryEngine(this);
    pQueryCmd->start(viewName);

    CMDCommand *pCmd = new CMDCommand();
    pCmd->setCmdPara(reinterpret_cast<void*>(pQueryCmd));
    m_pQueryThread->submitCommand(pCmd);

    delete pDBSource;
    return true;
}
}
