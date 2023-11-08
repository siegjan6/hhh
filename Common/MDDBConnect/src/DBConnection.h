//#ifndef DBCONNECTION_H
//#define DBCONNECTION_H

///*!

//\author bHVvY2hhbw==

//\section 功能
//1.数据库连接内部实现类。
//2.数据库连接--查询连接与操作连接，分两种为防止在查询数据库时，误断开操作数据库的连接。

//*/

//#include <QtCore/qglobal.h>
//#include <QObject>
//#include <QSqlDatabase>
//#include <QMap>

//#define SCADA_DB_TYPE_ACCESS    "SCADA_DB_TYPE_ACCESS"
//#define SCADA_DB_TYPE_SQL       "SCADA_DB_TYPE_SQL"
//#define SCADA_DB_TYPE_MYSQL     "SCADA_DB_TYPE_MYSQL"
//#define SCADA_DB_TYPE_DB2       "SCADA_DB_TYPE_DB2"
//#define SCADA_DB_TYPE_SQLLITE   "SCADA_DB_TYPE_SQLLITE"
//#define SCADA_DB_TYPE_ORACLE    "SCADA_DB_TYPE_ORACLE"
//#define SCADA_DB_TYPE_KINGBASE  "SCADA_DB_TYPE_KINGBASE"

//class CDBConnection
//{
//public:
//    explicit CDBConnection();
//    ~CDBConnection();

//    /*!
//    \section 连接数据库--查询数据库的连接与插入、删除、更新数据库的连接
//    */
//public:

//    //! 连接不同的数据库，用于操作数据库的连接
//    //! type: 0-- access 1--sqlserver 2--mysql 3--db2 4--sqlite 5--oracle 6--kingbase
//    bool connectDatabase(int type,
//                         const QString &dbName,
//                         const QString &host,
//                         const QString &user,
//                         const QString &passwd,
//                         int port,
//                         QString &strError);

//    //! 连接不同的数据库，用于操作数据库的连接
//    //! type: 0-- access 1--sqlserver 2--mysql 3--db2 4--sqlite 5--oracle 6--kingbase
//    bool connectDatabase(int type,
//                         const QString &strDsn,
//                         QString &strError);

//    //! 连接不同的数据库，用于查询数据库的连接
//    //! type: 0-- access 1--sqlserver 2--mysql 3--db2 4--sqlite 5--oracle 6--kingbase
//    bool queryConnectDatabase(int type,
//                         const QString &dbName,
//                         const QString &host,
//                         const QString &user,
//                         const QString &passwd,
//                         int port,
//                         QString &strError);

//    /*!
//    \section 功能--数据库状态、回滚、连接、执行
//    */
//public:
//    QSqlDatabase currentDatabase();
//    void close();
//    bool isConnect();
//    bool exec(const QString &sqlQuery, QString &strError, int &iError);
//    bool beginTrans();
//    bool commit();
//    bool roolBack();
//    QString lastError();
//    int connectionCount();

//    /*!
//    \section 功能--查找连接名
//    */
//private:
//    QString findConnectName(int type);
//    void intChangeConnectType(int type);

//private:
//    QString m_strConnectionName       ; //! 连接名
//    QString m_strType                 ; //! 连接类型
//    QMap<int, QString> m_mapConnName;        //! 连接名map
//    QSqlDatabase *m_database;                 //! 数据库连接对象
//};

//#endif // DBCONNECTION_H
