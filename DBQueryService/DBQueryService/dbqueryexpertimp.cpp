#include "dbqueryexpertimp.h"
#include "../../include/MDDataSource/MDDataSourceFactory.h"
#include "MDDBConnCache.h"
#include "DBConnectionTemp.h"
#include "MDDatabaseRecordset.h"
#include <QJsonObject>
#include <QSqlQuery>
#include <QSqlError>

CMDDBQueryExpertImp::CMDDBQueryExpertImp()
{
    m_strLastError = "";
}

CMDDBQueryExpertImp::~CMDDBQueryExpertImp()
{

}

CDBConnectionTemp *CMDDBQueryExpertImp::containsConnection(const QString& dbConName)
{
    ConnMap::iterator iter = m_connMap.find(dbConName);
    if( iter != m_connMap.end() )
     return iter->second;

    return nullptr;
}

CMDDatabaseRecordset *CMDDBQueryExpertImp::containsRecordset(const QString& recordSetName)
{
    RecordsetMap::iterator iter = m_recordsetMap.find(recordSetName);
    if( iter != m_recordsetMap.end() )
     return iter->second;

    return nullptr;
}

void CMDDBQueryExpertImp::addDataBaseConnection(const QString& dbConName)
{
    if(!containsConnection(dbConName))
    {
        CDBConnectionTemp *pCon = new CDBConnectionTemp;
        m_connMap[dbConName] = pCon;
    }
}
int CMDDBQueryExpertImp::connectDataBase(const QString& dbConName, const QVariant& serverProperty, const QString& dbName)
{
    if(containsConnection(dbConName))
    {
        CDBConnectionTemp *pCon = m_connMap[dbConName];
        if(pCon)
        {
//            var severProperty=
//            {
//                dbType:"MYSQL",
//                host:ip,
//                port:port,
//                user:user,
//                psw:psw
//            };
            QJsonObject obj = serverProperty.toJsonObject();
            DATABASEPARAM::DATABASE_TYPE tp = (DATABASEPARAM::DATABASE_TYPE)DBTye_FromString(obj["dbType"].toString());

            IMDDataBaseParam * pDBConnParam = CMDDataSourceFactory::createDatabasePaream();
            pDBConnParam->setdbServeType(tp);
            pDBConnParam->setdbServerIP(obj["host"].toString());
            pDBConnParam->setdbServerPort(obj["port"].toInt());
            pDBConnParam->setdbName(dbName);
            pDBConnParam->setdbUser(obj["user"].toString());
            pDBConnParam->setdbPassword(obj["psw"].toString());
            pCon->setParameters(pDBConnParam);

            if(pCon->open())
                return 0;
        }
    }
    return -1;
}
int CMDDBQueryExpertImp::disconnectDataBase(const QString& dbConName)
{
    CDBConnectionTemp *pCon = containsConnection(dbConName);
    if(pCon && pCon->isOpen())
    {
        pCon->close();
        return 0;
    }
    return -1;
}
int CMDDBQueryExpertImp::deleteDataBaseConnection(const QString& dbConName)
{
    CDBConnectionTemp *pCon = containsConnection(dbConName);
    if(pCon)
    {
        if(m_connMap.erase(dbConName) >0)
        {
            delete pCon;
            return 0;
        }
    }
    return -1;
}
int CMDDBQueryExpertImp::excuteSQLCommand(const QString& dbConName, const QString& sqlCmd)
{
    CDBConnectionTemp *pCon = containsConnection(dbConName);
    if(pCon)
    {
        return pCon->exec(sqlCmd);
    }
    return -1;
}
QString CMDDBQueryExpertImp::getLastError(const QString& dbConName)
{
    CDBConnectionTemp *pCon = containsConnection(dbConName);
    if(pCon)
    {
        return pCon->getLastError();
    }
    return "";
}
void CMDDBQueryExpertImp::addRecordSet(const QString& recordSetName)
{
    if(!containsRecordset(recordSetName))
    {
        CMDDatabaseRecordset *pRst = new CMDDatabaseRecordset;
        m_recordsetMap[recordSetName] = pRst;
    }
}
int CMDDBQueryExpertImp::updateRecordSetData(const QString& dbConName, const QString& recordSetName, const QString& sqlCmd)
{
    if(containsConnection(dbConName))
    {
        CDBConnectionTemp *pCon = m_connMap[dbConName];
        if(pCon)
        {
            QSqlQuery *pQuery = pCon->getQuery();
            bool success = pQuery->exec(sqlCmd);
            if (!success)
            {
                 m_strLastError = pQuery->lastError().text();
                 return -1;
            }

            CMDDatabaseRecordset *pRst = containsRecordset(recordSetName);
            if(pRst)
            {
                pRst->bindSqlQuery(pQuery);
                return 0;
            }
        }
    }

    return -1;
}
int CMDDBQueryExpertImp::getRecordSetRowCount(const QString& recordSetName)
{
    CMDDatabaseRecordset *pRst = containsRecordset(recordSetName);
    if(pRst)
    {
        return pRst->rowCount();
    }
    return -1;
}
int CMDDBQueryExpertImp::getRecordSetColumnCount(const QString& recordSetName)
{
    CMDDatabaseRecordset *pRst = containsRecordset(recordSetName);
    if(pRst)
    {
        return pRst->columnCount();
    }
    return -1;
}
QVariant CMDDBQueryExpertImp::getRecordSetValue(const QString& recordSetName, int row, int col)
{
    CMDDatabaseRecordset *pRst = containsRecordset(recordSetName);
    if(pRst)
    {
        return pRst->value(row,col);
    }
    return NULL;
}
QVariant CMDDBQueryExpertImp::getRecordSetValue(const QString& recordSetName, int row, const QString& fieldName)
{
    CMDDatabaseRecordset *pRst = containsRecordset(recordSetName);
    if(pRst)
    {
        return pRst->value(row,fieldName);
    }
    return NULL;
}
int CMDDBQueryExpertImp::deleteRecordSet(const QString& recordSetName)
{
    CMDDatabaseRecordset *pRst = containsRecordset(recordSetName);
    if(pRst)
    {
        if(m_recordsetMap.erase(recordSetName) >0)
        {
            delete pRst;
            return 0;
        }
    }
    return -1;
}

int CMDDBQueryExpertImp::DBTye_FromString(const QString & DBTypeString)
{
    int nType = -1;
    if (0 == DBTypeString.compare(QString("MYSQL"), Qt::CaseInsensitive)){
        nType = DATABASEPARAM::DB_TYPE_MYSQL;
    } else if (0 == DBTypeString.compare(QString("ORACLE"), Qt::CaseInsensitive)){
        nType = DATABASEPARAM::DB_TYPE_ORACLE;
    } else if (0 == DBTypeString.compare(QString("KINGBASE"), Qt::CaseInsensitive)){
        nType = DATABASEPARAM::DB_TYPE_KINGBASE;
    } else if (0 == DBTypeString.compare(QString("DB2"), Qt::CaseInsensitive)){
        nType = DATABASEPARAM::DB_TYPE_DB2;
    } else if (0 == DBTypeString.compare(QString("SQLITE"), Qt::CaseInsensitive)){
        nType = DATABASEPARAM::DB_TYPE_SQLLITE;
    } else if (0 == DBTypeString.compare(QString("SQLSERVER"), Qt::CaseInsensitive)){
        nType = DATABASEPARAM::DB_TYPE_SQL;
    }
    return nType;
}
