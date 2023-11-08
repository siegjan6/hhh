#ifndef MDDBCONNECTION_H_2017_2_7
#define MDDBCONNECTION_H_2017_2_7
#include <QtCore/qglobal.h>
#include <QString>
#include <MDDBConnect/MDDBCommonDef.h>

#if defined(MDDBCONNECT_LIBRARY)
#  define MDDBCONNECTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDBCONNECTSHARED_EXPORT Q_DECL_IMPORT
#endif

/*!
\author c2pj

\section 功能
1.封装打开数据库连接功能。
2.测试数据库是否连接通常

\note 线程不安全类 只能在一个线程中使用
*/

class MDDBCONNECTSHARED_EXPORT CDataBaseParam //TODO:去掉重名类
{
public:
    CDataBaseParam();
    virtual ~CDataBaseParam();
    //! 克隆
    virtual CDataBaseParam *clone();
    //! 赋值运算符
    void operator =(const CDataBaseParam &otherObj);

/*!
\section 数据库服务器
*/
public:
    //! 设置/获取数据库类型
    inline DBSOURCE_TYPE dbServeType(){return m_dbType;}
    inline void setdbServeType(DBSOURCE_TYPE iType){m_dbType = iType;}

    //! 设置/获取数据库服务器名称
    inline QString dbServerName(){return m_dbServerName;}
    inline void setdbServerName(const QString &name){m_dbServerName = name;}

    //! 设置/获取数据库服务器IP
    inline QString dbServerIP(){return m_dbServerIP;}
    inline void setdbServerIP(const QString &ip){m_dbServerIP = ip;}

    //! 设置/获取数据库服务器服务端口
    inline int	dbServerPort(){return m_dbServerPort;}
    inline void setdbServerPort(int iPort){m_dbServerPort = iPort;}

    //! 设置/获取用户名
    inline QString dbUser(){return m_dbUser;}
    inline void setdbUser(const QString &user){m_dbUser = user;}

    //! 设置/获取登录密码
    inline QString dbPassword(){return m_dbPassword;}
    inline void setdbPassword(const QString &password){m_dbPassword = password;}

/*!
\section 数据库
*/
public:
    //! 设置/获取数据库名
    inline QString dbName(){return m_dbName;}
    inline void setdbName(const QString &name){m_dbName = name;}

    //! 设置/获取数据库路径
    inline QString dbPath(){return m_dbPath;}
    inline void setDBPath(const QString &path){m_dbPath = path;}
private:
    QString m_dbPassword;                                      //! 数据库密码
    QString m_dbUser;                                              //! 数据库用户名
    QString m_dbServerIP;                                        //! 数据库服务器地址
    QString m_dbServerName;                                  //! 数据库服务器名
    QString m_dbName;                                            //! 数据库名
    QString m_dbPath;                                              //! 数据库路径
    int m_dbServerPort;                                             //! 数据库端口号
    DBSOURCE_TYPE m_dbType;                          //! 数据库类型
};

class QSqlDatabase;
class MDDBCONNECTSHARED_EXPORT MDDBConnection
{
public:
    MDDBConnection();
    virtual ~MDDBConnection();
    bool open(DBSOURCE_TYPE type, const QString &dbName,
                    const QString &host,const QString &user,
                    const QString &passwd,int port);
    bool open();
    void close();
    void setDBType(DBSOURCE_TYPE type);
    DBSOURCE_TYPE dbType();
    void setDBName(const QString & dbName);
    const QString & dbName();
    void setHost(const QString & hostStr);
    const QString & host();
    void setPort(int port);
    int port();
    void setUserName(const QString & userNameStr);
    const QString & userName();
    void setPassword(const QString & passwordStr);
    const QString & password();
    void setParameters(CDataBaseParam * pParam);//新增
    const QString & connectionName();
    QString driverName();
    bool executeSQL(const QString & sql);
    QString getLastErrorText();
    bool isOpen();
    bool compareConnectionParam(CDataBaseParam * pParam);//!对比连接参数 相同为true 新增 2016.09.22
    void setExtendFlag(int flag);
    int getExtendFlag();
private:
    bool openInner(QSqlDatabase & dbConn, DBSOURCE_TYPE type);
    bool convertTypeToString(DBSOURCE_TYPE type, QString & strType);
    void setConnectionParamters(QSqlDatabase & dbConn, DBSOURCE_TYPE type,
                                                    const QString & dbName, const QString & host,
                                                    const QString & user, const QString & passwd, int port);
private:
    QString m_strConnectionName;
    DBSOURCE_TYPE m_DBType;
    QString m_DBName;
    QString m_Host;
    int m_Port;
    QString m_UserName;
    QString m_Password;
    bool m_bOpen;
    QString m_LastErrorText;
    volatile int m_ExtendFlag; //扩展标识符 供扩展使用
};
#endif // MDDBCONNECTION_H_2017_2_7
