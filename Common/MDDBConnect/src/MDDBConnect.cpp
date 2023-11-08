#include "MDDBConnect.h"
#include "utils.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QAtomicInt>
#include <QMutex>

CDataBaseParam::CDataBaseParam()
{
    m_dbPassword                    = "";                               //! 数据库密码
    m_dbUser                        = "";                               //! 数据库用户名
    m_dbServerIP                    = "";                               //! 数据库服务器地址
    m_dbServerName                  = "";                               //! 数据库服务器名
    m_dbName                        = "";                               //! 数据库名
    m_dbPath                        = "";                               //! 数据库路径
    m_dbServerPort                  = -1;                               //! 数据库端口号
    m_dbType   = DBSOURCE_TYPE_UNKNOWN;   //! 数据库类型
}

CDataBaseParam::~CDataBaseParam()
{

}

/*!
\section 克隆
*/
CDataBaseParam *CDataBaseParam::clone()
{
    CDataBaseParam *pDatabaseParam = new CDataBaseParam();
    *pDatabaseParam = *this;
    return pDatabaseParam;
}
/*!
\section 赋值运算符
*/
void CDataBaseParam::operator =(const CDataBaseParam &otherObj)
{
    m_dbPassword            = otherObj.m_dbPassword;
    m_dbUser                = otherObj.m_dbUser;
    m_dbServerIP            = otherObj.m_dbServerIP;
    m_dbServerName          = otherObj.m_dbServerName;
    m_dbName                = otherObj.m_dbName;
    m_dbPath                = otherObj.m_dbPath;
    m_dbServerPort          = otherObj.m_dbServerPort;
    m_dbType                = otherObj.m_dbType;
}

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

MDDBConnection::MDDBConnection()
    :m_bOpen(false),
     //m_pQSqlQuery(0),
     m_ExtendFlag(0)
{
    //m_strConnectionName = QString("mdscada_db_connection_%1").arg(dbConnCount++);
    //Qt5.2 QAtomicInt不支持++操作符 fetchAndAddOrdered(1)+1==++
    //数据归档模块中的字符串为mdscada_db_connection
    //这里采用不同的名称 是为了防止连接名相同 导致QtSQL模块出错
    //待完全替换掉MDDBConnet 在将此类单独抽出 形成一个独立的模块

    // todo : QSqlDatabasePrivate::addDatabase: duplicate connection name 'mdscada_db_query_connection_1', old connection removed.
    m_strConnectionName = QString("mdscada_db_query_connection_%1").arg(dbConnCount.fetchAndAddOrdered(1)+1);
//    m_strConnectionName = QString("mdscada_db_query_connection_%1").arg(dbConnCount++);
}

MDDBConnection::~MDDBConnection()
{
    QSqlDatabase::removeDatabase(m_strConnectionName);
}

//MDDBConnection::MDDBConnection(const QString & connectionNameStr)
//    :m_strConnectionName(connectionNameStr),
//      m_bOpen(false),
//      m_pQSqlQuery(0)
//{
//}

bool MDDBConnection::open(DBSOURCE_TYPE  type, const QString &dbName, const QString &host,
                                                                            const QString &user, const QString &passwd, int port)
{
    //delete m_pQSqlQuery;
    //m_pQSqlQuery = 0;

    m_DBType = type;
    m_DBName = dbName;
    m_Host = host;
    m_Port = port;
    m_UserName = user;
    m_Password = passwd;

    m_bOpen = false;
    QString strType;
    if(!convertTypeToString(type, strType))
    {
        m_LastErrorText = "unknown type";
        MDDBC::WriteInfoLog("MDDBConnection::open(...):unknown type");
        return false;
    }

    QSqlDatabase dbConn;
    if(QSqlDatabase::contains(m_strConnectionName))
        dbConn = QSqlDatabase::database(m_strConnectionName);
    else
        dbConn = QSqlDatabase::addDatabase(strType, m_strConnectionName);
    if(!dbConn.isValid()) //加载驱动是否成否
    {
        m_LastErrorText = dbConn.lastError().text();
        MDDBC::WriteErrorLog(QString("MDDBConnection::open:fail, err = %1.q conn = %2.db type = %3")
                                        .arg(m_LastErrorText).arg(m_strConnectionName).arg(strType));
        //QSqlDatabase::removeDatabase(m_strConnectionName);
        return false;
    }

    setConnectionParamters(dbConn, type, dbName, host, user, passwd, port); //一定会成功
    m_bOpen = openInner(dbConn, type);
    return m_bOpen;
}

 bool MDDBConnection::open()
 {
     if(m_bOpen)
         return true;

     //delete m_pQSqlQuery;
     //m_pQSqlQuery = 0;

     m_bOpen = false;
     QString strType;
     if(!convertTypeToString(m_DBType, strType))
     {
         m_LastErrorText = "unknown type";
         MDDBC::WriteInfoLog("MDDBConnection::open:unknown type");
         return false;
     }

     QSqlDatabase dbConn;
     if(QSqlDatabase::contains(m_strConnectionName))
         dbConn = QSqlDatabase::database(m_strConnectionName);
     else
        dbConn = QSqlDatabase::addDatabase(strType, m_strConnectionName);
     if(!dbConn.isValid()) //加载驱动是否成否
     {
         m_LastErrorText = dbConn.lastError().text();
         MDDBC::WriteInfoLog(QString("MDDBConnection::open:fail, err = %1.q conn = %2").arg(m_LastErrorText).arg(m_strConnectionName));
         //QSqlDatabase::removeDatabase(m_strConnectionName);
         return false;
     }

     setConnectionParamters(dbConn, m_DBType, m_DBName, m_Host, m_UserName, m_Password, m_Port); //一定会成功
     m_bOpen = openInner(dbConn, m_DBType);
     return m_bOpen;
 }

 void MDDBConnection::close()
 {
     QSqlDatabase::database(m_strConnectionName).close();
 }

void MDDBConnection::setDBType(DBSOURCE_TYPE type)
{
    m_DBType = type;
}

DBSOURCE_TYPE MDDBConnection::dbType()
{
    return m_DBType;
}

void MDDBConnection::setDBName(const QString & dbName)
{
    m_DBName = dbName;
}

const QString &MDDBConnection::dbName()
{
    return m_DBName;
}

void MDDBConnection::setHost(const QString & hostStr)
{
    m_Host = hostStr;
}

const QString & MDDBConnection::host()
{
    return m_Host;
}

void MDDBConnection::setPort(int port)
{
    m_Port = port;
}

int MDDBConnection::port()
{
    return m_Port;
}

void MDDBConnection::setUserName(const QString & userNameStr)
{
    m_UserName = userNameStr;
}

const QString &MDDBConnection::userName()
{
    return m_UserName;
}

void MDDBConnection::setPassword(const QString & passwordStr)
{
    m_Password = passwordStr;
}

const QString &MDDBConnection::password()
{
    return m_Password;
}

void MDDBConnection::setParameters(CDataBaseParam * pParam)
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

const QString &MDDBConnection::connectionName()
{
    return m_strConnectionName;
}

QString MDDBConnection::driverName()
{
    return QSqlDatabase::database(m_strConnectionName).driverName();
}

//QSqlDatabase MDDBConnection::getDBConn()
//{
//    return QSqlDatabase::database(m_strConnectionName);
//}

QString MDDBConnection::getLastErrorText()
{
    return m_LastErrorText;
}

bool MDDBConnection::executeSQL(const QString & sql)
{
    QSqlQuery query(QSqlDatabase::database(m_strConnectionName));
    mySQLLockTemp.lock();
    if(!query.exec(sql))
    {
        mySQLLockTemp.unlock();
        m_LastErrorText = query.lastError().text();
        MDDBC::WriteErrorLog(QString("MDDBConnection::executeSQL:errmsg(drivername=%1, err=%2), sql=%3").arg(driverName()).arg(m_LastErrorText).arg(sql));
        return false;
    }
    mySQLLockTemp.unlock();

    m_LastErrorText.clear();
    return true;
}

//QSqlQuery * MDDBConnection::getQuery()
//{
//    return m_pQSqlQuery;
//}

bool MDDBConnection::isOpen()
{
    return m_bOpen;
}

bool MDDBConnection::compareConnectionParam(CDataBaseParam * pParam)
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

void MDDBConnection::setExtendFlag(int flag)
{
    m_ExtendFlag = flag;
}

int MDDBConnection::getExtendFlag()
{
    return m_ExtendFlag;
}

//频繁调用数据库连接函数是不正常的行为
bool MDDBConnection::openInner(QSqlDatabase & dbConn,DBSOURCE_TYPE type)
{
    if(DBSOURCE_TYPE_MYSQL == type)
    {
        //c2pj 2016.08.04
        //mysql驱动使用了MySQLConnector.C，即libmysql.dll/so，来连接MySQL数据库。
        //其中mysql_init函数不是线程安全函数。如果多个线程，其中的每一个线程，有自己的CDBConnectionTemp,
        //那么在并发连接时，可能会连接失败。失败原因：open函数，会调用mysql_init函数。它又不是线程安全函数。故导致错误。
        //所以，这里先加一把锁。因为open时，还会连接TCP连接。所以，这段代码，在网络不正常的时候，会有些延时。
        //可能会阻塞其它调用此函数的线程。
        //有两种解决方案:一种是修改MySQLConnector.C的代码。加锁。
        //第二种方案是使用ODBC连接MySQL数据库。已测试使用ODBC连接MySQL数据库不存在此问题。
        //使用了锁 加上日志 记录锁的争用情况及连接耗时
        MDDBC::WriteInfoLog(QString("MDDBConnection::openInner:mysql before open begin connect.id=%1").arg(m_strConnectionName));
        mySQLLockTemp.lock();
        bool b = dbConn.open();
        mySQLLockTemp.unlock();
        MDDBC::WriteInfoLog(QString("MDDBConnection::openInner:mysql after open end connect.id=%1, result=%2").arg(m_strConnectionName).arg(b));
        if(!b)
        {
            //不记录密码
            QString connInfoStr =QString("dbName = %1, host = %2, port = %3, user = %4, q_conn = %5")
                                                                .arg(dbConn.databaseName()).arg(dbConn.hostName()).arg(dbConn.port()).arg(dbConn.userName()).arg(m_strConnectionName);

            MDDBC::WriteInfoLog(QString("MDDBConnection::open:fail, err = %1; = %2").arg(dbConn.lastError().text()).arg(connInfoStr));
            dbConn.close();
            return false;
        }

        //m_pQSqlQuery = new QSqlQuery(dbConn);
        return true;
    }

    //TODO:特殊处理SQLite数据库
    MDDBC::WriteInfoLog(QString("MDDBConnection::openInner:%1.open begin connect.id = %2")
                                        .arg(dbConn.driverName()).arg(m_strConnectionName));
    mySQLLockTemp.lock();
    bool b= dbConn.open();
    mySQLLockTemp.unlock();
    MDDBC::WriteInfoLog(QString("MDDBConnection::openInner:%1.open end connect.id = %2, %3" )
                                        .arg(dbConn.driverName()).arg(m_strConnectionName).arg(b));
    if(!b)
    {
        //不记录密码
        QString connInfoStr =QString("dbName = %1, host = %2, port = %3, user = %4, q_conn = %5")
                                                            .arg(dbConn.databaseName()).arg(dbConn.hostName()).arg(dbConn.port()).arg(dbConn.userName()).arg(m_strConnectionName);

        MDDBC::WriteInfoLog(QString("MDDBConnection::open:fail, err = %1; = %2").arg(dbConn.lastError().text()).arg(connInfoStr));
        dbConn.close();
        return false;
    }

    //m_pQSqlQuery = new QSqlQuery(dbConn);
    return true;
}

void MDDBConnection::setConnectionParamters(QSqlDatabase & dbConn, DBSOURCE_TYPE type, const QString & dbName, const QString & host,
                                                                                        const QString & user, const QString & passwd, int port)
{
    if(DBSOURCE_TYPE_SQL_SERVER == type) //MS SQL SERVER使用ODBC连接
    {
        //当前模式为工作数据库且插入的数据库为sqlserver数据库的处理方式，采用数据源的方式连接数据库，这样即不用再配置数据源
        QString strDsn = QString("DRIVER={SQL SERVER};SERVER=%1,%2;DATABASE=%3;uid=%4;pwd=%5;")\
                                                .arg(host).arg(port).arg(dbName).arg(user).arg(passwd);

        dbConn.setDatabaseName(strDsn);
        return ;
    }

    //if(DATABASEPARAM::DB_TYPE_SQLLITE == type)
    //{
      //  //TODO:待完善
       // abort();
        //return ;
    //}

    if(DBSOURCE_TYPE_MYSQL == type
        || DBSOURCE_TYPE_ORACLE == type
        || DBSOURCE_TYPE_KINGBASE == type) //support kingbase 2016.08.24
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
    //abort();
    MDDBC::WriteErrorLog(QString("MDDBConnection::setConnectionParamters:cann't support this db:type = %1").arg(type));
    return ;
}

bool MDDBConnection::convertTypeToString(DBSOURCE_TYPE type, QString & strType)
{
    switch(type)
    {
    case DBSOURCE_TYPE_SQL_SERVER: //MS SQL SERVER
        strType = "QODBC";
        break;
    case DBSOURCE_TYPE_MYSQL:
        strType = "QMYSQL";
        break;
    case DBSOURCE_TYPE_ORACLE:
        strType = "QODBC";
        break;
    case DBSOURCE_TYPE_KINGBASE:
        strType = "QKINGBASE"; //provided by KingBase Team
        break;
    default:
        MDDBC::WriteInfoLog("MDDBConnection::convertTypeToString:unknown type");
        return false;
    }

    return true;
}
