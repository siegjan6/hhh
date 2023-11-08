#ifndef MDDATABASEPARAM_H
#define MDDATABASEPARAM_H

/*!

\author bHVvY2hhbw==

\section 功能
1.IMDDataBaseParam实现类。
2.数据库参数属性。

*/

#include"IMDDatabaseParam.h"

class CMDDataBaseParam: public IMDDataBaseParam
{
public:
    CMDDataBaseParam();
    virtual ~CMDDataBaseParam();
    //! 克隆
    virtual IMDDataBaseParam *clone();
    //! 赋值运算符
    void operator =(const CMDDataBaseParam &otherObj);
    //! 序列化
    virtual void serialize(QDataStream *ar, bool bSave);
    virtual void serialize(QJsonObject& json);
    virtual void deserialize(QJsonObject json);

/*!
\section 数据库服务器
*/
public:
    //! 设置/获取数据库类型  0 -- DB_TYPE_ACCESS;  1 -- DB_TYPE_SQL;	2 -- DB_TYPE_ORACLE
    inline DATABASEPARAM::DATABASE_TYPE dbServeType(){return m_dbType;}
    inline void setdbServeType(DATABASEPARAM::DATABASE_TYPE iType){m_dbType = iType;}

    //! 设置/获取SQLServer连接方式  0 -- MD50_NAMEPIPES;	1 -- MD50_TCPIP;
    inline int  dbServerConnectType(){return m_dbServerConnectType;}
    inline void setdbServerConnectType(int iType){m_dbServerConnectType = iType;}

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

    //! 设置/获取命令执行超时时间(秒)
    inline int   cmdTimeOut(){return m_cmdTimeOut;}
    inline void setCmdTimeOut(int iTimeOut){m_cmdTimeOut = iTimeOut;}

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

    //! 设置/获取数据库备份路径
    inline QString	dbBackUpPath(){return m_dbBackUpPath;}
    inline void setDbBackUpPath(const QString &path){m_dbBackUpPath = path;}

private:
    QString m_dbPassword                    ;                               //! 数据库密码
    QString m_dbUser                        ;                               //! 数据库用户名
    QString m_dbServerIP                    ;                               //! 数据库服务器地址
    QString m_dbServerName                  ;                               //! 数据库服务器名
    QString m_dbName                        ;                               //! 数据库名
    QString m_dbPath                        ;                               //! 数据库路径
    QString m_dbBackUpPath                  ;                               //! 数据库备份路径
    int m_dbServerPort                      ;                               //! 数据库端口号
    int m_dbServerConnectType               ;                               //! 数据库连接类型
    int m_cmdTimeOut                        ;                               //! 数据库命令超时
    DATABASEPARAM::DATABASE_TYPE m_dbType   ;                               //! 数据库类型
};


#endif // MDDATABASEPARAM_H
