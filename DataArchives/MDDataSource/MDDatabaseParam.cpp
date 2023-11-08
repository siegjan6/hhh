#include "MDDatabaseParam.h"
#include <QJsonObject>

CMDDataBaseParam::CMDDataBaseParam()
{
    m_dbPassword                    = "";                               //! 数据库密码
    m_dbUser                        = "";                               //! 数据库用户名
    m_dbServerIP                    = "";                               //! 数据库服务器地址
    m_dbServerName                  = "";                               //! 数据库服务器名
    m_dbName                        = "";                               //! 数据库名
    m_dbPath                        = "";                               //! 数据库路径
    m_dbBackUpPath                  = "";                               //! 数据库备份路径
    m_dbServerPort                  = -1;                               //! 数据库端口号
    m_dbServerConnectType           = -1;                               //! 数据库连接类型
    m_cmdTimeOut                    = -1;                               //! 数据库命令超时
    m_dbType   = DATABASEPARAM::DB_TYPE_INVALID;   //! 数据库类型
}

CMDDataBaseParam::~CMDDataBaseParam()
{

}

/*!
\section 克隆
*/
IMDDataBaseParam *CMDDataBaseParam::clone()
{
    CMDDataBaseParam *pDatabaseParam = new CMDDataBaseParam();
    *pDatabaseParam = *this;
    return pDatabaseParam;
}
/*!
\section 赋值运算符
*/
void CMDDataBaseParam::operator =(const CMDDataBaseParam &otherObj)
{  
    m_dbPassword            = otherObj.m_dbPassword;
    m_dbUser                = otherObj.m_dbUser;
    m_dbServerIP            = otherObj.m_dbServerIP;
    m_dbServerName          = otherObj.m_dbServerName;
    m_dbName                = otherObj.m_dbName;
    m_dbPath                = otherObj.m_dbPath;
    m_dbBackUpPath          = otherObj.m_dbBackUpPath;
    m_dbServerPort          = otherObj.m_dbServerPort;
    m_dbServerConnectType   = otherObj.m_dbServerConnectType;
    m_cmdTimeOut            = otherObj.m_cmdTimeOut;
    m_dbType                = otherObj.m_dbType;
}

/*!
\section 序列化
*/
void CMDDataBaseParam::serialize(QDataStream *ar, bool bSave)
{
    if(ar)
    {
        if(bSave)
        {
            quint32 uVersion = 1;
            *ar << uVersion;
            if(uVersion >= 1)
            {
                int ndbType = m_dbType;
                *ar << ndbType;
                *ar << m_dbPassword;
                *ar << m_dbUser;
                *ar << m_dbServerIP;
                *ar << m_dbServerName;
                *ar << m_dbName;
                *ar << m_dbPath;
                *ar << m_dbBackUpPath;
                *ar << m_dbServerPort;
                *ar << m_dbServerConnectType;
                *ar << m_cmdTimeOut;
//                *ar << m_dbType;
            }
        }
        else
        {
            quint32 uVersion = 0;

            *ar >> uVersion;
            if(uVersion >= 1)
            {
                int ndbType = 0;
                *ar >> ndbType;
                m_dbType = (DATABASEPARAM::DATABASE_TYPE)ndbType;
                *ar >> m_dbPassword;
                *ar >> m_dbUser;
                *ar >> m_dbServerIP;
                *ar >> m_dbServerName;
                *ar >> m_dbName;
                *ar >> m_dbPath;
                *ar >> m_dbBackUpPath;
                *ar >> m_dbServerPort;
                *ar >> m_dbServerConnectType;
                *ar >> m_cmdTimeOut;
//                *ar >> m_dbType;
            }
        }
    }
}

void CMDDataBaseParam::serialize(QJsonObject& json)
{
    int ndbType = m_dbType;
    json.insert("Type", ndbType);
    json.insert("Password", m_dbPassword);
    json.insert("User", m_dbUser);
    json.insert("Host", m_dbServerIP);
    json.insert("ServerName", m_dbServerName);
    json.insert("Name", m_dbName);
    json.insert("Path", m_dbPath);
    json.insert("BackUpPath", m_dbBackUpPath);
    json.insert("ServerPort", m_dbServerPort);
    json.insert("ConnectType", m_dbServerConnectType);
    json.insert("CmdTimeOut", m_cmdTimeOut);
}

void CMDDataBaseParam::deserialize(QJsonObject json)
{
    int ndbType = json.value("Type").toInt();
    m_dbType = (DATABASEPARAM::DATABASE_TYPE)ndbType;
    m_dbPassword = json.value("Password").toString();
    m_dbUser = json.value("User").toString();
    m_dbServerIP = json.value("Host").toString();
    m_dbServerName = json.value("ServerName").toString();
    m_dbName = json.value("Name").toString();
    m_dbPath = json.value("Path").toString();
    m_dbBackUpPath = json.value("BackUpPath").toString();
    m_dbServerPort = json.value("ServerPort").toInt();
    m_dbServerConnectType = json.value("ConnectType").toInt();
    m_cmdTimeOut = json.value("CmdTimeOut").toInt();
}
