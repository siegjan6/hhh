#include "DBConnectionTemp.h"
#include <QAtomicInt>
#include <QMutex>
#include <QSqlError>
#include <stdlib.h>
#include <MDSysLogExpert/MDSysLogExpert.h>

//<c2pj 2016.07.21>
//rand()在多线程时会输出一样的值
//证据:
//CDBConnection::connectDatabase:connection name = SCADA_DB_TYPE_MYSQL_41.DB Name = mindscadatestdb"
//CDBConnection::connectDatabase:connection name = SCADA_DB_TYPE_MYSQL_41.DB Name = mindsadadb
//通过测试代码，也验证了情况。导致的问题是db的connection name相同。
//导致QSqlDatabase报错:
//QSqlDatabasePrivate::removeDatabase: connection xxx... is still in use, all queries will cease to work.
//导致QSql模块释放driver内存，导致程序崩溃.
//</c2pj 2016.07.21>

static QAtomicInt dbConnCount;
static QMutex mySQLLockTemp;

#define SCADA_DB_TYPE_ACCESS    "QSCADA_DB_TYPE_ACCESS"
#define SCADA_DB_TYPE_SQL       "QSCADA_DB_TYPE_SQL"
#define SCADA_DB_TYPE_MYSQL     "QSCADA_DB_TYPE_MYSQL"
#define SCADA_DB_TYPE_DB2       "QSCADA_DB_TYPE_DB2"
#define SCADA_DB_TYPE_SQLLITE   "QSCADA_DB_TYPE_SQLLITE"
#define SCADA_DB_TYPE_ORACLE    "QSCADA_DB_TYPE_ORACLE"
#define SCADA_DB_TYPE_KINGBASE  "QSCADA_DB_TYPE_KINGBASE"


CDBConnectionTemp::CDBConnectionTemp()
    :m_bOpen(false),
     m_pQSqlQuery(0),
     m_ExtendFlag(0)
{
    m_lastError = "";
    //m_strConnectionName = QString("mdscada_db_connection_%1").arg(dbConnCount++);
    //Qt5.2 QAtomicInt不支持++操作符 fetchAndAddOrdered(1)+1==++
    //数据归档模块中的字符串为mdscada_db_connection
    //这里采用不同的名称 是为了防止连接名相同 导致QtSQL模块出错
    //待完全替换掉MDDBConnet 在将此类单独抽出 形成一个独立的模块

    m_strConnectionName = QString("mdscada_db_query_connection_temp_%1").arg(dbConnCount.fetchAndAddOrdered(1)+1);
}

CDBConnectionTemp::CDBConnectionTemp(const QString & connectionNameStr)
    :m_bOpen(false),
      m_strConnectionName(connectionNameStr),
      m_pQSqlQuery(0)
{
    m_lastError = "";
}

CDBConnectionTemp::~CDBConnectionTemp()
{
    QSqlDatabase::removeDatabase(m_strConnectionName);
}

bool CDBConnectionTemp::open(DATABASEPARAM:: DATABASE_TYPE  type, const QString &dbName, const QString &host,
                                                                            const QString &user, const QString &passwd, int port)
{
    delete m_pQSqlQuery;
    m_pQSqlQuery = 0;

    m_bOpen = false;
    QString strType;
    if(!convertTypeToString(type, strType))
    {
        //TODO:log
        return false;
    }

    QSqlDatabase dbConn = QSqlDatabase::addDatabase(strType, m_strConnectionName);
    if(!dbConn.isValid()) //加载驱动是否成否
    {
        m_lastError = QString("CDBConnectionTemp::open:fail, err = %1.q conn = %2").arg(dbConn.lastError().text()).arg(m_strConnectionName);
        MDLOG_ERROR_TO_LOGGER(m_lastError);
        return false;
    }

    setConnectionParamters(dbConn, type, dbName, host, user, passwd, port); //一定会成功
    m_bOpen = openInner(dbConn, type);
    return m_bOpen;
}

 bool CDBConnectionTemp::open()
 {
     if(m_bOpen)
         return true;

     delete m_pQSqlQuery;
     m_pQSqlQuery = 0;

     m_bOpen = false;
     QString strType;
     if(!convertTypeToString(m_DBType, strType))
     {
         //TODO:log
         return false;
     }

     QSqlDatabase dbConn;
     if(QSqlDatabase::contains(m_strConnectionName))
         dbConn = QSqlDatabase::database(m_strConnectionName);
     else
        dbConn = QSqlDatabase::addDatabase(strType, m_strConnectionName);
     if(!dbConn.isValid()) //加载驱动是否成否
     {
        m_lastError = QString("CDBConnectionTemp::open:fail, err = %1.q conn = %2").arg(dbConn.lastError().text()).arg(m_strConnectionName);
         MDLOG_ERROR_TO_LOGGER(m_lastError);
         return false;
     }

     setConnectionParamters(dbConn, m_DBType, m_DBName, m_Host, m_UserName, m_Password, m_Port); //一定会成功
     m_bOpen = openInner(dbConn, m_DBType);
     return m_bOpen;
 }

 void CDBConnectionTemp::close()
 {
     QSqlDatabase::database(m_strConnectionName).close();
 }

void CDBConnectionTemp::setDBType(DATABASEPARAM:: DATABASE_TYPE type)
{
    m_DBType = type;
}

void CDBConnectionTemp::setDBName(const QString & dbName)
{
    m_DBName = dbName;
}

void CDBConnectionTemp::setHost(const QString & hostStr)
{
    m_Host = hostStr;
}

void CDBConnectionTemp::setPort(int port)
{
    m_Port = port;
}

void CDBConnectionTemp::setUserName(const QString & userNameStr)
{
    m_UserName = userNameStr;
}

void CDBConnectionTemp::setPassword(const QString & passwordStr)
{
    m_Password = passwordStr;
}

void CDBConnectionTemp::setParameters(IMDDataBaseParam * pParam)
{
    if(!pParam)
        return ;

    m_DBType = pParam->dbServeType();
    m_DBName = pParam->dbName();
    m_Host = pParam->dbServerIP();
    m_Port = pParam->dbServerPort();
    m_UserName = pParam->dbUser();
    m_Password = pParam->dbPassword();

}

QSqlDatabase CDBConnectionTemp::getDBConn()
{
    return QSqlDatabase::database(m_strConnectionName);
}

QSqlQuery * CDBConnectionTemp::getQuery()
{
    return m_pQSqlQuery;
}

bool CDBConnectionTemp::isOpen()
{
    return m_bOpen;
}

bool CDBConnectionTemp::compareConnectionParam(IMDDataBaseParam * pParam)
{
    if(!pParam)
        return false;

    if(m_DBType == pParam->dbServeType()
        &&m_DBName == pParam->dbName()
        &&m_Host == pParam->dbServerIP()
        &&m_Port == pParam->dbServerPort()
        &&m_UserName == pParam->dbUser()
        &&m_Password == pParam->dbPassword())
    {
        return true;
    }

    return false;
}

void CDBConnectionTemp::setExtendFlag(int flag)
{
    m_ExtendFlag = flag;
}

int CDBConnectionTemp::getExtendFlag()
{
    return m_ExtendFlag;
}

int CDBConnectionTemp::exec(const QString& sqlcmd)
{
    QSqlQuery* q = getQuery();
    if(q)
    {
        bool result = q->exec(sqlcmd);
        if(!result)
        {
            m_lastError = q->lastError().text();
            return (int)(q->lastError().type());
        }
    }
    return -1;
}

bool CDBConnectionTemp::openInner(QSqlDatabase & dbConn, DATABASEPARAM::DATABASE_TYPE type)
{
    if(DATABASEPARAM::DB_TYPE_MYSQL == type)
    {
        //c2pj 2016.08.04
        //mysql驱动使用了MySQLConnector.C，即libmysql.dll/so，来连接MySQL数据库。
        //其中mysql_init函数不是线程安全函数。如果多个线程，其中的每一个线程，有自己的CDBConnectionTemp,
        //那么在并发连接时，可能会连接失败。失败原因：open函数，会调用mysql_init函数。它又不是线程安全函数。故导致错误。
        //所以，这里先加一把锁。因为open时，还会连接TCP连接。所以，这段代码，在网络不正常的时候，会有些延时。
        //可能会阻塞其它调用此函数的线程。
        //有两种解决方案:一种是修改MySQLConnector.C的代码。加锁。
        //第二种方案是使用ODBC连接MySQL数据库。已测试使用ODBC连接MySQL数据库不存在此问题。
        mySQLLockTemp.lock();
        bool b = dbConn.open();
        mySQLLockTemp.unlock();
        if(!b)
        {
            //不记录密码
            QString connInfoStr =QString("dbName = %1, host = %2, port = %3, user = %4, q_conn = %5")
                                                                .arg(dbConn.databaseName()).arg(dbConn.hostName()).arg(dbConn.port()).arg(dbConn.userName()).arg(m_strConnectionName);

            m_lastError = QString("CDBConnectionTemp::open:fail, err = %1; = %2").arg(dbConn.lastError().text()).arg(connInfoStr);
            MDLOG_ERROR_TO_LOGGER(m_lastError);
            dbConn.close();
            return false;
        }

        m_pQSqlQuery = new QSqlQuery(dbConn);
        return true;
    }

    //TODO:特殊处理SQLite数据库
    mySQLLockTemp.lock();
    bool b= dbConn.open();
    mySQLLockTemp.unlock();
    if(!b)
    {
        //不记录密码
        QString connInfoStr =QString("dbName = %1, host = %2, port = %3, user = %4, q_conn = %5")
                                                            .arg(dbConn.databaseName()).arg(dbConn.hostName()).arg(dbConn.port()).arg(dbConn.userName()).arg(m_strConnectionName);

        m_lastError = QString("CDBConnectionTemp::open:fail, err = %1; = %2").arg(dbConn.lastError().text()).arg(connInfoStr);
        MDLOG_ERROR_TO_LOGGER(m_lastError);
        dbConn.close();
        return false;
    }

    m_pQSqlQuery = new QSqlQuery(dbConn);
    return true;
}

void CDBConnectionTemp::setConnectionParamters(QSqlDatabase & dbConn, DATABASEPARAM::DATABASE_TYPE type, const QString & dbName, const QString & host,
                                                                                        const QString & user, const QString & passwd, int port)
{
    if(DATABASEPARAM::DB_TYPE_SQL == type) //MS SQL SERVER使用ODBC连接
    {
        //当前模式为工作数据库且插入的数据库为sqlserver数据库的处理方式，采用数据源的方式连接数据库，这样即不用再配置数据源
        QString strDsn = QString("DRIVER={SQL SERVER};SERVER=%1,%2;DATABASE=%3;uid=%4;pwd=%5;")\
                                                .arg(host).arg(port).arg(dbName).arg(user).arg(passwd);

        dbConn.setDatabaseName(strDsn);
        return ;
    }

    if(DATABASEPARAM::DB_TYPE_SQLLITE == type)
    {
        //TODO:待完善
        abort();
        return ;
    }

    if(DATABASEPARAM::DB_TYPE_MYSQL == type
        || DATABASEPARAM::DB_TYPE_ORACLE == type
        || DATABASEPARAM::DB_TYPE_KINGBASE == type) //support kingbase 2016.08.24
    {
        if(!dbName.isEmpty())
            dbConn.setDatabaseName(dbName);

        dbConn.setHostName(host);
        //dbConn.setDatabaseName(dbName);
        dbConn.setUserName(user);
        dbConn.setPassword(passwd);
        dbConn.setPort(port);
        return ;
    }

    //TODO:其他数据库 还未经过测试 待完善
    abort();
    return ;
}

bool CDBConnectionTemp::convertTypeToString(DATABASEPARAM::DATABASE_TYPE type, QString & strType)
{
    switch(type)
    {
    case DATABASEPARAM::DB_TYPE_ACCESS:
        strType = "QODBC";
        break;
    case DATABASEPARAM::DB_TYPE_SQL: //MS SQL SERVER
        strType = "QODBC";
        break;
    case DATABASEPARAM::DB_TYPE_MYSQL:
        strType = "QMYSQL";
        break;
    case DATABASEPARAM::DB_TYPE_DB2:
        strType = "QDB2";
        break;
    case DATABASEPARAM::DB_TYPE_SQLLITE:
        strType = "QSQLITE";
        break;
    case DATABASEPARAM::DB_TYPE_ORACLE:
        strType = "QODBC";
        break;
    case DATABASEPARAM::DB_TYPE_KINGBASE:
        strType = "QKINGBASE"; //provided by KingBase Team
        break;
    default:
        //TODO:log
        return false;
    }

    return true;
}


QString CDBConnectionTemp::getLastError()
{
    return m_lastError;
}
