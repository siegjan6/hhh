#include "TSQLExecutor.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

//static const char * DBTypeConvert(DATABASEPARAM::DATABASE_TYPE type);

TSQLExecutor::TSQLExecutor()
{
}

int TSQLExecutor::queryCount(const QString & dbSourceName, JSDBParameters * pDBParam, const QString & sqlStr)
{
    //获得数据源的配置信息:IMDDataSource 数据源连接信息   表信息

    QSqlDatabase db = QSqlDatabase::addDatabase("Q"+pDBParam->dbTypeStr, dbSourceName+"_T");
    db.setDatabaseName(pDBParam->databaseName);
    db.setHostName(pDBParam->hostStr);
    db.setPort(pDBParam->portStr.toInt());//3306 MySQL
    db.setUserName(pDBParam->databaseUser);
    db.setPassword(pDBParam->databasePassword);
    if(!db.open())
    {
        qDebug()<<"TSQLExecutor::queryCount:"<<db.lastError().text();
        return -1;
    }

    QSqlQuery query(db);
    if(!query.exec(sqlStr))
    {
        qDebug()<<query.lastError().text();
        return -1;
    }

#ifdef DEBUG
    qDebug()<<"select row = "<<query.size();
#endif

    //int count = 0;
    //while(query.next()) count++;

    return query.size();
}

/*
static const char * DBTypeConvert(DATABASEPARAM::DATABASE_TYPE type)
{
    switch(type)
    {
    case DATABASEPARAM::DB_TYPE_MYSQL:
        return "QMYSQL";
    case  DATABASEPARAM::DB_TYPE_ORACLE:
        return "QORACLE";
    case DATABASEPARAM::DB_TYPE_KINGBASE:
        return "QKINGBASE";
    case DATABASEPARAM::DB_TYPE_DB2:
        return "QDB2";
    case DATABASEPARAM::DB_TYPE_SQLLITE:
        return "QSQLITE";
    case DATABASEPARAM::DB_TYPE_SQL:
        return "QSQLSERVER";
    default:
        return "INVALID";
    }
    return "INVALID";
}
*/
