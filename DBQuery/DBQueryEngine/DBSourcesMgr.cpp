#include "DBSourcesMgr.h"
#include "DatabaseParam.h"
#include "utils.h"

namespace DBQE
{
bool DBSourcesMgr::Init(const QString & xmlFileName)
{
    if(m_DBSrcsMap.size() != 0)
    {
        DBQE::WriteInfoLog(QString("DBSourcesMgr::Init:%1 already initialized").arg(xmlFileName));
        return false;
    }

    m_xmlFileName = xmlFileName;
    QList<DBQ::DBSource *> dbSourcesList;
    QString errStr;
    if(!DBQ::DataSource_Load(m_xmlFileName, DBQ::FILE_TYPE_JSON, dbSourcesList, errStr))
    {
        DBQE::WriteInfoLog(QString("DBSourcesMgr::Init:load %1 file fail,error=%2").arg(xmlFileName).arg(errStr));
        return false;
    }

    foreach(DBQ::DBSource * pDBSrc, dbSourcesList)
    {
        if(!CheckDBSource(pDBSrc))
        {
            DBQE::WriteInfoLog(QString("DBSourcesMgr::Init:CheckDBSource fail, file=%1").arg(xmlFileName));
            return false;
        }

        m_DBSrcsMap[pDBSrc->dbSrcName] = pDBSrc;
    }

    return true;
}

//获得一个数据源连接 如果数据源中没有数据库 则数据库名为空
//如果有多个数据库 选择默认数据库.如果没有默认数据库 则数据库名为空
CDataBaseParam * DBSourcesMgr::GetDBConnectionParam(const QString & dbSourceName)
{
    DBQ::DBSource * pDBSrc = m_DBSrcsMap[dbSourceName];
    if(!pDBSrc)
    {
        DBQE::WriteInfoLog(QString("DBSourcesMgr::GetDBConnectionParam:cann't find connection with %1").arg(dbSourceName));
        return 0;
    }

    CDataBaseParam * pParam = new CDataBaseParam;
    QString dbName;
    int size = pDBSrc->dbs.size();
    for(int i=0; i<size;i++)
    {
        if(pDBSrc->dbs[i].bDefault)
        {
            dbName = pDBSrc->dbs[i].dbName;
            break;
        }
    }

    pParam->setdbServerName(pDBSrc->dbSrcName);
    pParam->setdbServeType(pDBSrc->dbType);
    pParam->setdbServerIP(pDBSrc->host);
    pParam->setdbServerPort(pDBSrc->port);
    pParam->setdbName(dbName);
    pParam->setdbUser(pDBSrc->userName);
    pParam->setdbPassword(pDBSrc->password);

    return pParam;
}

DBQ::DBSource * DBSourcesMgr::GetDefaultDBSource()
{
    QMapIterator<QString, DBQ::DBSource *> ite(m_DBSrcsMap);
    while(ite.hasNext())
    {
        ite.next();
        if(ite.value()->bDefault) //默认数据源
            return new DBQ::DBSource(*ite.value());
    }

    return 0;
}

QString DBSourcesMgr::GetTablePrefix(const QString & dbsName)
{
    DBQ::DBSource * pDBSrc = m_DBSrcsMap[dbsName];
    if(!pDBSrc)
    {
        return "";
    }
    return pDBSrc->tablePrefix;
}

bool DBSourcesMgr::CheckDBSource(DBQ::DBSource * pDBSrc)
{
    if(!pDBSrc)
        return false;
    if(pDBSrc->dbSrcName.isEmpty())
        return false ;
    if(m_DBSrcsMap.contains(pDBSrc->dbSrcName))
        return false ;

    return true;
}
}
