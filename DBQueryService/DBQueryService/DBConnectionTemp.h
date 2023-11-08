#ifndef DBCONNECTIONTEMP_H
#define DBCONNECTIONTEMP_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <MDDataArchivesInterface/IMDDatabaseParam.h>

/*!

\author c2pj

\section 功能
1.封装打开数据库连接功能。
2.测试数据库是否连接通常

\note 此类为临时类。待此类稳定后，可替换MDDBConnect 类
         线程不安全类 只能在一个线程中使用
*/
class CDBConnectionTemp
{
public:
    CDBConnectionTemp();
    CDBConnectionTemp(const QString & connectionNameStr);
    virtual ~CDBConnectionTemp();
    bool open(DATABASEPARAM:: DATABASE_TYPE type, const QString &dbName,
                        const QString &host,const QString &user,
                        const QString &passwd,int port);
    bool open();
    void close();
    void setDBType(DATABASEPARAM:: DATABASE_TYPE type);
    void setDBName(const QString & dbName);
    void setHost(const QString & hostStr);
    void setPort(int port);
    void setUserName(const QString & userNameStr);
    void setPassword(const QString & passwordStr);
    void setParameters(IMDDataBaseParam * pParam);//新增
    QSqlDatabase getDBConn();
    QSqlQuery * getQuery();
    bool isOpen();
    bool compareConnectionParam(IMDDataBaseParam * pParam);//!对比连接参数 相同为true 新增 2016.09.22
    void setExtendFlag(int flag);
    int getExtendFlag();
    int exec(const QString& sqlcmd);

    QString getLastError();
private:
    bool openInner(QSqlDatabase & dbConn, DATABASEPARAM:: DATABASE_TYPE type);
    bool convertTypeToString(DATABASEPARAM:: DATABASE_TYPE type, QString & strType);
    int DBTye_FromString(const QString & DBTypeString);
    void setConnectionParamters(QSqlDatabase & dbConn, DATABASEPARAM::DATABASE_TYPE type,
                                                    const QString & dbName, const QString & host,
                                                    const QString & user, const QString & passwd, int port);
private:
    QString m_strConnectionName;
    DATABASEPARAM::DATABASE_TYPE m_DBType;
    QString m_DBName;
    QString m_Host;
    int m_Port;
    QString m_UserName;
    QString m_Password;
    bool m_bOpen;
    QSqlQuery * m_pQSqlQuery;
    volatile int m_ExtendFlag; //扩展标识符 供扩展使用
    QString m_lastError;
};

#endif // DBCONNECTIONTEMP_H
