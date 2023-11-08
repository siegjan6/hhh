#include "DBQueryCommand.h"
#include "MDDBQueryService.h"
#include "IMDDatabaseParam.h"
#include "MDDBConnCache.h"
#include "MDDatabaseRecordset.h"
#include "DBConnectionTemp.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>

CDBQueryCommand::CDBQueryCommand(const QString &strQueryID, const QString &strCmd)
{
    m_isStarted         = false; //! 运行标识
    m_pDBParam          = NULL;  //! 数据库参数
    m_pDBQueryRst       = NULL;  //! 查询回调指针

    m_strQueryID        = strQueryID;
    m_strQueryCommand   = strCmd;
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
    QString strError = "success";

    CDBConnectionTemp * pDBConnect = MDDBConnCache::GetInst()->PullConnection(m_pDBParam);
    if(!pDBConnect)
    {
        strError = "CDBQueryCommand::callbackQueryResult:MDDBConnCache::GetInst() fail";
        return ;
    }

    if(!pDBConnect->open())
    {
        strError = "CDBQueryCommand::callbackQueryResult:db connection open fail";
        return ;
    }

    //QSqlQuery query(m_pDBConnect->currentDatabase());
    //bool success = query.exec(m_strQueryCommand);
   QSqlQuery * pQuery = pDBConnect->getQuery();
    bool success = pQuery->exec(m_strQueryCommand);

    if (!success)
    {
        strError = pQuery->lastError().text();
        return ;
    }

    if (m_pDBQueryRst)
    {
        CMDDatabaseRecordset *pDBRecSet = new CMDDatabaseRecordset();
        pDBRecSet->bindSqlQuery(pQuery);
        m_pDBQueryRst->NotityDBQueryResult(m_strQueryID, pDBRecSet);
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

void CDBQueryCommand::start()
{
    if(!isStarted())
    {
        setStarted(true);
    }
}
