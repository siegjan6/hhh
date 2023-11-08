#include "MDDatabaseCheckCMD.h"
#include "MDDatabaseService.h"
#include "IMDDataSource.h"
#include "IMDDatabaseParam.h"
#include "IMDDataTable.h"
#include "IMDDataField.h"
#include <MDDBConnect.h>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

static DBSOURCE_TYPE ConvertDBType(DATABASEPARAM::DATABASE_TYPE dbType);

CMDDatabaseCheckCMD::CMDDatabaseCheckCMD(CMDDBSourceService *parent, IMDDataBaseParam * pDBConnParam)
    : m_pService(parent),
      m_pDBConnParam(pDBConnParam)
{
    MDLOG_INFO_TO_LOGGER(QString("new CMDDatabaseCheckCMD:%1").arg((long)this));
}

CMDDatabaseCheckCMD::~CMDDatabaseCheckCMD()
{
    MDLOG_INFO_TO_LOGGER(QString("delete CMDDatabaseCheckCMD:%1").arg((long)this));
}

void CMDDatabaseCheckCMD::Do()
{
    if (NULL == m_pService)
        return;

    if (NULL == m_pService->dataSource())
        return;

    // 创建正常工作数据库
    if(!checkDatabase())
    {
        MDLOG_INFO_TO_LOGGER("CMDDatabaseCheckCMD::Do:Check Database is fail");
        return ;
    }
    MDLOG_INFO_TO_LOGGER("CMDDatabaseCheckCMD::Do:Check Database is OK");

    checkTable();
}

/*!
  \section 检查表结构是否发生变化 若变化则新建表
 */
bool CMDDatabaseCheckCMD::checkTable()
{
    //L:运行时不能修改表结构

    if (NULL == m_pService)
    {
        return false;
    }

    MDDBConnection *pDBConn = m_pService->currdb(); //?有可能是临时数据库
    if (NULL == pDBConn)
    {
        return false;
    }

    bool bSuccess = false;

    int iTableCount                 = 0;
    int iTableFieldCount            = 0;
    int iFieldsCount                = 0;

    QString strTable                = "";
    QString strTime                 = "";
    QString strNewTalbe             = "";

    IMDDataTable *pTable            = NULL;

    // 获取当前数据源文件中数据库表配置链表
    iTableCount = m_pService->dataSource()->dataTableCount();
    if (0 >= iTableCount)
    {
        return true;
    }

    QSqlQuery query(QSqlDatabase::database(pDBConn->connectionName()));
    for(int i = 0; i < iTableCount; ++i)
    {
        pTable = m_pService->dataSource()->getDataTable(i);
        if(NULL == pTable)
        {
            continue;
        }

        // 数据表重建标志
        bool bTableMustCreate           = false;
        strTable = pTable->name();

        QString strSql = "";
        strSql += "select * from "; //TODO:要改进 否则数据多了时 效率低
        if(pDBConn->dbType() == DBSOURCE_TYPE::DBSOURCE_TYPE_SQL_SERVER)
        {
            strSql = "select top 1 * from "; // 改进查询表结构的效率
        }
        if (m_pService->dataSource()->isEnableDataTablePrefix()
                && !m_pService->dataSource()->dataTablePrefix().isEmpty())
        {
            strTable = m_pService->dataSource()->dataTablePrefix() + "_" + pTable->name();
        }

        strSql += strTable;
        if(pDBConn->dbType() == DBSOURCE_TYPE::DBSOURCE_TYPE_MYSQL)
        {
            strSql += " limit 1"; // 改进查询表结构的效率
        }
        strSql += ";";

        bool bResult = query.exec(strSql);
        if(bResult)
        {
            // 字段个数
            iFieldsCount = query.record().count();
            // 配置文件中表字段个数
            iTableFieldCount = pTable->dataFieldCount() - pTable->dataFieldCount("virtual");
            if(0 == iTableFieldCount && 0 == iFieldsCount)
            {
                continue;
            }

            if (iTableFieldCount != iFieldsCount)
            {
                bTableMustCreate = true;
            }
            else
            {
                for (int j = 0; j < iFieldsCount; ++j)
                {
                    QString strName = query.record().fieldName(j);
                    //if (pTable->getDataField(j)->name() != strName)
                    IMDDataField* f = pTable->getDataField(strName);
                    if (f == nullptr)
                    {
                        bTableMustCreate = true;
                        break;
                    }
                }
            }

            //表存在，但字段不相同，需要重新构建
            if (bTableMustCreate)
            {
                // 首先判断设计的数据源字段是否包含数据库表中的实际字段，查出多余出来到字段，通过alter table add column实现自动变更，补足所缺字段
                if(iTableFieldCount > iFieldsCount)
                {
                    QString Alter_sqlcmd="";
                    for (int k = 0; k < iTableFieldCount; ++k)
                    {
                        QString strName = query.record().fieldName(k);
                        //if (pTable->getDataField(j)->name() != strName)
                        IMDDataField* f = pTable->getDataField(k);
                        if (f != nullptr)
                        {
                            // 配置的数据源表中的字段实际数据表中不存在则添加
                            if(!query.record().contains(f->name()))
                            {
                                // eg: "ADD COLUMN `bm_Index1Version`  int not NULL DEFAULT 0 "
                                Alter_sqlcmd += "ADD COLUMN ";
                                Alter_sqlcmd += getFieldSqlCmdPart(f);
                                Alter_sqlcmd += ",";
                            }
                        }
                    }
                    if(Alter_sqlcmd != "")
                    {
                        Alter_sqlcmd = Alter_sqlcmd.mid(0, Alter_sqlcmd.size() - 1);
                        Alter_sqlcmd = "ALTER TABLE " + strTable + " " +Alter_sqlcmd +";";
                        bool b = query.exec(Alter_sqlcmd);
                        if(b)
                        {
                            qDebug()<<"engine--modify table add columns:" << Alter_sqlcmd;
                            return b;
                        }
                        else
                            m_pService->SaveSQLToTempDB(Alter_sqlcmd);
                    }
                }
                //将原来表中的数据导入到新表中，删除原表再重新创建
                //?
                strTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
                strNewTalbe = QString("%1_%2").arg(strTable, strTime);
                bool b = false;
                switch(pDBConn->dbType())
                {
                case DBSOURCE_TYPE::DBSOURCE_TYPE_MYSQL:
                        b = query.exec(QString("ALTER TABLE %1 RENAME TO %2;").arg(strTable).arg(strNewTalbe));
                break;
                case DBSOURCE_TYPE::DBSOURCE_TYPE_SQL_SERVER: //MS SQL SERVER
                        b = query.exec(QString("execute sp_rename '%1', '%2'").arg(strTable).arg(strNewTalbe));
                break;
                default:
                    //TODO:待添加其他数据库重命名表的方法
                    //TODO:log
                    //qDebug()<<"engine--modify table name:cann't support this database.";
                    return false;
                }
                //MDLOG_INFO_TO_LOGGER(QString("engine--modify table name '%1' as '%2', error text: %3.").arg(strTable).arg(strNewTalbe).arg(query.lastError().text()));
                if(!b)
                {
                    //qDebug()<<QString("engine--modify table name '%1' as '%2', error text: %3.").arg(strTable).arg(strNewTalbe).arg(query.lastError().text());
                    //ALTER TABLE old_tab RENAME TO new_tab
                    //TODO:log
                    return false;
                }

                bSuccess = createDbTables(pTable);
            }
        }
        else
        {
            bSuccess = createDbTables(pTable);
        }
    }

    return bSuccess;
}

QString CMDDatabaseCheckCMD::getFieldSqlCmdPart(void *pF)
{
    IMDDataField *pField = (IMDDataField*)pF;
    if(nullptr == pField)
        return "";
    DATABASEPARAM::DATABASE_TYPE type = m_pDBConnParam->dbServeType();
    QString strFieldType = "";
    QString strSQLCmd = "";
    QVariant vExtType = pField->ext("type");
    if(!vExtType.isNull() && vExtType.isValid())
    {
        QString fExtType = vExtType.toString();
        if("virtual" == fExtType)
        {
            return "";
        }
    }

    // 字段大小
    int uSize = pField->size();
    // 字段类型
    MDDataType nFieldType = pField->dataType();
    switch(nFieldType)
    {
    case MDDataType::Bool:
    case MDDataType::Int16:
    case MDDataType::UInt16:
    case MDDataType::Int32:
    case MDDataType::UInt32:
    case MDDataType::Byte:
    case MDDataType::SByte:
        strFieldType = "INT";
        break;
    case MDDataType::Int64:
    case MDDataType::UInt64:
        strFieldType = "BIGINT";
        break;
    case MDDataType::Float:
    case MDDataType::Double:
        strFieldType = "DOUBLE";
        if (DATABASEPARAM::DB_TYPE_SQL == type)
        {
            strFieldType = "FLOAT";
        }
        break;
    case MDDataType::DateTime:
        strFieldType = "DATETIME";
        break;
    case MDDataType::String:
        strFieldType = QString("VARCHAR(%1)").arg(250);
        break;
    default:
        uSize = 255;
        strFieldType = QString("VARCHAR(%1)").arg(250);
        break;
    }

    if(type == DATABASEPARAM::DATABASE_TYPE ::DB_TYPE_MYSQL)
    {
        QVariant vExtMysqlType = pField->ext("mysqltype");
        if(!vExtMysqlType.isNull() && vExtMysqlType.isValid())
        {
            QString fExtType = vExtMysqlType.toString();
            if("json" == fExtType)
            {
                //从MariaDB 10.2.7开始，JSON是LONGTEXT的别名
                strFieldType = "json";
            }
        }
    }

    strSQLCmd += " ";
    strSQLCmd += pField->name();
    strSQLCmd += " ";
    strSQLCmd += strFieldType;

    if(!pField->isEnableNull())
    {
        strSQLCmd += " NOT NULL";
    }

    QVariant vExtFieldDefaultValue = pField->ext("fieldDefaultValue");
    if(!vExtFieldDefaultValue.isNull() && vExtFieldDefaultValue.isValid())
    {
        QString fExtFieldDefaultValue = vExtFieldDefaultValue.toString();
        if("" != fExtFieldDefaultValue)
        {
            //从MariaDB 10.2.7开始，JSON是LONGTEXT的别名
            strSQLCmd += " DEFAULT "+fExtFieldDefaultValue;
        }
    }
    return strSQLCmd;
}

/*!
  \section 由MindSCADA的表创建数据库中的表
 */
bool CMDDatabaseCheckCMD::createDbTables(IMDDataTable *pTable)
{

    if (NULL == m_pService)
    {
        return false;
    }

    if (NULL == m_pService->dataSource())
    {
        return false;
    }

    QString strError = "";
    bool bSuccess = createDbTables(pTable, strError);

    if (!bSuccess)
    {
        MDLOG_INFO_TO_LOGGER(QString("engine---create table %1, error state: %2").arg(pTable->name()).arg(strError));
        return false;
    }

    return true;
}

/*!
  \section 由MindSCADA的表创建数据库中的表
 */
bool CMDDatabaseCheckCMD::createDbTables(IMDDataTable *pTable, QString &strError)
{
    bool bSuccess = false;
    if (NULL == m_pService)
    {
        return bSuccess;
    }

    MDDBConnection *pDBConn = m_pService->currdb();
    if (NULL == pDBConn)
    {
        return bSuccess;
    }

    if ((NULL == m_pService->dataSource()) || (NULL == pTable))
    {
        return bSuccess;
    }


    IMDDataField *pField = NULL;
    QString strSQLCmd = "";

    DATABASEPARAM::DATABASE_TYPE type = m_pDBConnParam->dbServeType();

    // 主键链表
    QString strPrimary = " primary key";

    int iTableFieldCount = pTable->dataFieldCount();
    if(iTableFieldCount == 0)
    {
        return bSuccess;
    }

    strSQLCmd = "CREATE TABLE ";
    QString strTab = pTable->name();
    if (m_pService->dataSource()->isEnableDataTablePrefix()
            && !m_pService->dataSource()->dataTablePrefix().isEmpty())
    {
        strTab = m_pService->dataSource()->dataTablePrefix() + "_" + pTable->name();
    }

    strSQLCmd += strTab;
    strSQLCmd += " (";

    QString strFieldType = "";
    QStringList strPrimarySqlList;
    int uSize = 0;
    Q_UNUSED(uSize)
    for(int i = 0; i < iTableFieldCount; i++)
    {
        pField = pTable->getDataField(i);
        if(nullptr == pField)
        {
            continue;
        }

        QVariant vExtType = pField->ext("type");
        if(!vExtType.isNull() && vExtType.isValid())
        {
            QString fExtType = vExtType.toString();
            if("virtual" == fExtType)
            {
                continue;
            }
        }

        // 字段大小
        uSize = pField->size();
        // 字段类型
        MDDataType nFieldType = pField->dataType();
        switch(nFieldType)
        {
        case MDDataType::Bool:
        case MDDataType::Int16:
        case MDDataType::UInt16:
        case MDDataType::Int32:
        case MDDataType::UInt32:
        case MDDataType::Byte:
        case MDDataType::SByte:
            strFieldType = "INT";
            break;
        case MDDataType::Int64:
        case MDDataType::UInt64:
            strFieldType = "BIGINT";
            break;
        case MDDataType::Float:
        case MDDataType::Double:
            strFieldType = "DOUBLE";
            if (DATABASEPARAM::DB_TYPE_SQL == type)
            {
                strFieldType = "FLOAT";
            }
            break;
        case MDDataType::DateTime:
            strFieldType = "DATETIME";
            break;
        case MDDataType::String:
            strFieldType = QString("VARCHAR(%1)").arg(250);
            break;
        default:
            uSize = 255;
            strFieldType = QString("VARCHAR(%1)").arg(250);
            break;
        }

        if(type == DATABASEPARAM::DATABASE_TYPE ::DB_TYPE_MYSQL)
        {
            QVariant vExtMysqlType = pField->ext("mysqltype");
            if(!vExtMysqlType.isNull() && vExtMysqlType.isValid())
            {
                QString fExtType = vExtMysqlType.toString();
                if("json" == fExtType)
                {
                    //从MariaDB 10.2.7开始，JSON是LONGTEXT的别名
                    strFieldType = "json";
                }
            }
        }

        strSQLCmd += " ";
        strSQLCmd += pField->name();
        strSQLCmd += " ";
        strSQLCmd += strFieldType;

        if(!pField->isEnableNull())
        {
            strSQLCmd += " NOT NULL";
        }

        if(pField->isPrimarykey())
        {// 如果设置为主键，加入主键队列
            //strSQLCmd += " NOT NULL";
            //strSQLCmd += strPrimary;
            strPrimarySqlList.append(pField->name());
        }

        strSQLCmd += ",";
    }

    if(strPrimarySqlList.length() >0)
        strSQLCmd += strPrimary + " (" + strPrimarySqlList.join(',') + ")";
    else
        strSQLCmd = strSQLCmd.mid(0, strSQLCmd.size() - 1);
    strSQLCmd += ");";
    qDebug() << strSQLCmd;

    QSqlQuery query(QSqlDatabase::database(pDBConn->connectionName()));
    bSuccess = query.exec(strSQLCmd);
    if(!bSuccess)
    {
        strError = query.lastError().text();
        MDLOG_INFO_TO_LOGGER(QString("engine---create table sql: %1, error text: %2, database type: %3").arg(strSQLCmd).arg(strError).arg(type));
        m_pService->SaveSQLToTempDB(strSQLCmd);
    }

    return bSuccess;
}

//!创建数据库
bool CMDDatabaseCheckCMD::checkDatabase()
{
    if (NULL == m_pService)
    {
        return false;
    }

    if (NULL == m_pService->dataSource())
    {
        return false;
    }

    // 获取数据库参数
    if(NULL == m_pDBConnParam)
    {
        return false;
    }

    bool bSuccess = createDatabaseAndConnect();

    m_pService->setDBConnectionStatus(bSuccess);
    return bSuccess;
}

bool CMDDatabaseCheckCMD::createDatabaseAndConnect()
{

    if (NULL == m_pService)
    {
        return false;
    }

    if (NULL == m_pService->dataSource())
    {
        return false;
    }

    MDDBConnection *pDBConnect = m_pService->currdb();//?有可能是临时数据库
    if (NULL == pDBConnect)
    {
        return false;
    }

    QString strPassword = m_pDBConnParam->dbPassword();
    QString strDbName	 = m_pDBConnParam->dbName();
    QString strUser = m_pDBConnParam->dbUser();
    int port = m_pDBConnParam->dbServerPort();
    QString strDBIP = m_pDBConnParam->dbServerIP();
    DATABASEPARAM::DATABASE_TYPE type = m_pDBConnParam->dbServeType();

    //当程序第一次运行时,数据库中可能没有对应的数据库实例。
    //例如，数据库实例名为mtDB。当连接mtDB时就会出错。
    //出错有两种可能:1、连接不通 2、数据库中不存在mtDB实例
    //所以,以下代码先以空数据库实例(金仓数据库为SAMPLES)测试连接是否通畅
    //如果通暢，则创建mtDB数据库实例

    QString connInfoStr = QString("dbName:%1, ip: %2, user: %3, port: %4").arg(strDbName).arg(strDBIP).arg(strUser).arg(port);

    bool b = pDBConnect->open(ConvertDBType(type), strDbName, strDBIP, strUser, strPassword, port);
    if(b)
    {
        MDLOG_INFO_TO_LOGGER(QString("CMDDatabaseCheckCMD::createDatabase:OK: %1").arg(connInfoStr));
        return true;
    }

    if(DATABASEPARAM::DB_TYPE_KINGBASE ==  type)
        b = pDBConnect->open(ConvertDBType(type), "SAMPLES", strDBIP, strUser, strPassword, port);
    else
        b = pDBConnect->open(ConvertDBType(type), "", strDBIP, strUser, strPassword, port);

    if(!b) //真连接不上了
    {
        MDLOG_INFO_TO_LOGGER(QString("CMDDatabaseCheckCMD::createDatabase:fail: %1, err=%2")
                                                            .arg(connInfoStr).arg(pDBConnect->getLastErrorText()));
        return false;
    }

    //说明不存在指定的数据库
    //创建数据库
    QString strCreateDBSQL = QString("create database %1;").arg(strDbName);
    {//必须要加上{},将query的生命期限制在一次exec中.
        //原因:之后会执行pDBConnect->open操作。会删除内部的query，会释放query
        //内部的一些资源A.下面的代码 将query又赋给了新的query变量。当新的query变量
        //析构时，又释放了一资源A.导致崩溃
        //TODO:不通过QSqlQuery查询.
        //QSqlQuery query(pDBConnect->getDBConn());

        //if(!query.exec(strCreateDBSQL)) //创建数据库失败
        if(!pDBConnect->executeSQL(strCreateDBSQL)) //创建数据库失败
        {
            MDLOG_INFO_TO_LOGGER(QString("CMDDatabaseCheckCMD::createDatabase:fail 2: %1, err=%2")
                                                                .arg(connInfoStr).arg(pDBConnect->getLastErrorText()));
            return false;
        }
    }

    //再连接指定的数据库
    b = pDBConnect->open(ConvertDBType(type), strDbName, strDBIP, strUser, strPassword, port);
    if(b)
    {
        MDLOG_INFO_TO_LOGGER(QString("CMDDatabaseCheckCMD::createDatabase:OK 2: %1, ").arg(connInfoStr));
        return true;
    }

    MDLOG_INFO_TO_LOGGER(QString("CMDDatabaseCheckCMD::createDatabase:fail 3: %1, err=%2")
                                                        .arg(connInfoStr).arg(pDBConnect->getLastErrorText()));
    return false;
}

static DBSOURCE_TYPE ConvertDBType(DATABASEPARAM::DATABASE_TYPE dbType)
{
    switch(dbType)
    {
    case DATABASEPARAM::DB_TYPE_INVALID: return DBSOURCE_TYPE_UNKNOWN;
    case DATABASEPARAM::DB_TYPE_ACCESS: return DBSOURCE_TYPE_UNKNOWN;
    case DATABASEPARAM::DB_TYPE_SQL: return DBSOURCE_TYPE_SQL_SERVER;
    case DATABASEPARAM::DB_TYPE_MYSQL: return DBSOURCE_TYPE_MYSQL;
    case DATABASEPARAM::DB_TYPE_DB2: return DBSOURCE_TYPE_UNKNOWN;
    case DATABASEPARAM::DB_TYPE_SQLLITE: return DBSOURCE_TYPE_UNKNOWN;
    case DATABASEPARAM::DB_TYPE_ORACLE: return DBSOURCE_TYPE_ORACLE;
    case DATABASEPARAM::DB_TYPE_KINGBASE: return DBSOURCE_TYPE_KINGBASE;
    default:
        return DBSOURCE_TYPE_UNKNOWN;
    }

    return DBSOURCE_TYPE_UNKNOWN;
}
