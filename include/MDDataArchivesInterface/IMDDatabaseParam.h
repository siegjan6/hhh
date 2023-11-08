#ifndef IMDDATABASEPARAM_H
#define IMDDATABASEPARAM_H

/*!

\author luochao

\section 功能
1.数据库参数接口类--抽像类。
2.数据库参数属性。

\section

\code

\endcode

*/

#include <QDataStream>

/*!
\section 数据库类型
*/
namespace DATABASEPARAM {

enum DATABASE_TYPE
{
    DB_TYPE_INVALID         = -1,   //! 无效类型
    DB_TYPE_ACCESS          = 0,    //! access类型
    DB_TYPE_SQL             = 1,    //! sql类型
    DB_TYPE_MYSQL           = 2,    //! mysql类型
    DB_TYPE_DB2             = 3,    //! db2类型
    DB_TYPE_SQLLITE         = 4,    //! sqlite类型
    DB_TYPE_ORACLE          = 5,    //! oracle类型
    DB_TYPE_KINGBASE        = 6     //! kingbase类型
};

}

class IMDDataBaseParam
{
public:
    virtual ~IMDDataBaseParam(){}
    virtual IMDDataBaseParam *clone() = 0;
    virtual void serialize(QDataStream *ar, bool bSave) = 0;
    virtual void serialize(QJsonObject& json) = 0;
    virtual void deserialize(QJsonObject json) = 0;

/*!
\section 数据库服务器
*/
public:
    //! 设置/获取数据库类型  0 -- DB_TYPE_ACCESS = 0;  1 -- DB_TYPE_SQL;	2 -- DB_TYPE_ORACLE
    virtual DATABASEPARAM::DATABASE_TYPE dbServeType() = 0;
    virtual void setdbServeType(DATABASEPARAM::DATABASE_TYPE iType) = 0;

    //! 设置/获取SQLServer连接方式  0 -- MD_NAMEPIPES = 0;	1 -- MD_TCPIP = 0;
    virtual int  dbServerConnectType() = 0;
    virtual void setdbServerConnectType(int iType) = 0;

    //! 设置/获取数据库服务器名称
    virtual QString dbServerName() = 0;
    virtual void setdbServerName(const QString &name) = 0;

    //! 设置/获取数据库服务器IP
    virtual QString dbServerIP() = 0;
    virtual void setdbServerIP(const QString &IP) = 0;

    //! 设置/获取数据库服务器服务端口
    virtual int	dbServerPort() = 0;
    virtual void setdbServerPort(int iPort) = 0;

    //! 设置/获取用户名
    virtual QString dbUser() = 0;
    virtual void setdbUser(const QString &user) = 0;

    //! 设置/获取登录密码
    virtual QString dbPassword() = 0;
    virtual void setdbPassword(const QString &password) = 0;

    //! 设置/获取命令执行超时时间(秒)
    virtual int   cmdTimeOut() = 0;
    virtual void setCmdTimeOut(int iTimeOut) = 0;

/*!
\section 数据库
*/
public:
    //! 设置/获取数据库名
    virtual QString dbName() = 0;
    virtual void setdbName(const QString &name) = 0;

    //! 设置/获取数据库路径
    virtual QString dbPath() = 0;
    virtual void setDBPath(const QString &path) = 0;

    //! 设置/获取数据库备份路径
    virtual QString	dbBackUpPath() = 0;
    virtual void setDbBackUpPath(const QString &path) = 0;
};

#endif // IMDDATABASEPARAM_H
