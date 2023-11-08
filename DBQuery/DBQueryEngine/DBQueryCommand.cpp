#include "DBQueryCommand.h"
#include "MDDBConnCache.h"
#include "MDDatabaseRecordset.h"
#include "DBQueryEngine.h"
#include "utils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

namespace DBQE
{
CDBQueryCommand::CDBQueryCommand(const QString &strQueryID, const QString &strCmd)
{
    m_isStarted         = false; //! 运行标识
    m_pDBParam          = NULL;  //! 数据库参数
    m_pDBQueryRst       = NULL;  //! 查询回调指针

    m_strQueryID        = strQueryID;
    m_strQueryCommand   = strCmd;
    m_pQueryEngine = 0;
}

CDBQueryCommand::~CDBQueryCommand()
{
    if (m_pDBParam)
    {
        delete m_pDBParam;
        m_pDBParam = NULL;
    }
}

QString CDBQueryCommand::getQueryCommand()
{
    return m_strQueryCommand;
}

QString CDBQueryCommand::getQueryID()
{
    return m_strQueryID;
}

//执行查询语句
void CDBQueryCommand::execQuery()
{
    callbackQueryResult();
}

void CDBQueryCommand::callbackQueryResult()
{
    MDDBConnection * pDBConnect = MDDBConnCache::GetInst()->PullConnection(m_pDBParam);
    if(!pDBConnect)
    {
        DBQE::WriteInfoLog("CDBQueryCommand::callbackQueryResult:MDDBConnCache::GetInst() fail");
        if(m_pQueryEngine)
            m_pQueryEngine->setQueryStatus(m_strQueryID, DBQUERY_STATUS_FAIL);
        return ;
    }

    if(!pDBConnect->open())
    {
        DBQE::WriteInfoLog("CDBQueryCommand::callbackQueryResult:db connection open fail");
        if(m_pQueryEngine)
            m_pQueryEngine->setQueryStatus(m_strQueryID, DBQUERY_STATUS_FAIL);
        return ;
    }

    DBQE::WriteInfoLog(QString("CDBQueryCommand::callbackQueryResult:being query.id=%1").arg(m_strQueryID));
    //QSqlQuery * pQuery = pDBConnect->getQuery();
    DBQE::WriteInfoLog(QString("CDBQueryCommand::callbackQueryResult:sql=%1.query id=%2").arg(m_strQueryCommand).arg(m_strQueryID));
    //bool success = pQuery->exec(m_strQueryCommand);
    QSqlQuery query(QSqlDatabase::database(pDBConnect->connectionName()));
    bool success = query.exec(m_strQueryCommand);
    DBQE::WriteInfoLog(QString("CDBQueryCommand::callbackQueryResult:end query.id=%1").arg(m_strQueryID));

    if (!success)
    {
        if(m_pQueryEngine)
            m_pQueryEngine->setQueryStatus(m_strQueryID, DBQUERY_STATUS_FAIL);

        DBQE::WriteInfoLog(QString("CDBQueryCommand::callbackQueryResult:%1.query id=%2")
                                                        .arg(query.lastError().text()).arg(m_strQueryID));
        return ;
    }

    if(m_pQueryEngine)
        m_pQueryEngine->setQueryStatus(m_strQueryID, DBQUERY_STATUS_OK);

    if (m_pDBQueryRst)
    {
        CMDDatabaseRecordset *pDBRecSet = new CMDDatabaseRecordset();
        pDBRecSet->setName(m_viewName);
        pDBRecSet->setID(m_strQueryID);
        pDBRecSet->bindSqlQuery(&query);
        m_pDBQueryRst->NotityDBQueryResult(m_strQueryID, pDBRecSet);
    }
    else
    {
        DBQE::WriteInfoLog("CDBQueryCommand::callbackQueryResult:query callback is null");
    }
}

bool CDBQueryCommand::isStarted()
{
    return m_isStarted;
}

void CDBQueryCommand::setStarted(bool bStarted)
{
    m_isStarted = bStarted;
}

void CDBQueryCommand::start(const QString & viewName)
{
    m_viewName = viewName;
    if(!isStarted())
    {
        setStarted(true);
    }
}

}
