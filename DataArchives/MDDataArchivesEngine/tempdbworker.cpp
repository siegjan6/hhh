#include "tempdbworker.h"
#include <QCoreApplication>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <MDDataArchivesInterface/IMDDatabaseParam.h>
#include <QVariant>
#include <MDSysLogExpert/MDSysLogExpert.h>
#include <MDDBConnect.h>

static DBSOURCE_TYPE ConvertDBType(DATABASEPARAM::DATABASE_TYPE dbType);

TempDBWorker::TempDBWorker(QObject *parent) :
    QThread(parent),
    m_Stop(true),
    m_Init(false),
    m_WorkDBConnStatus(false),
    m_pTempDBInsertQuery(0),
    m_pTempDBBackupQuery(0),
    m_pTempDBCommonQuery(0),
    m_pWorkDBConnection(0)
{
}

TempDBWorker::~TempDBWorker()
{
    delete m_pTempDBInsertQuery;
    delete m_pTempDBBackupQuery;
    delete m_pTempDBCommonQuery;
	delete m_pWorkDBConnection;
    //delete m_pWorkDBQuery 引用m_pWorkDBConnection内部的query 故无需释放
    qDeleteAll(m_ConnList);
}

//TempDBWorker只能被启动一次
bool TempDBWorker::Start(const QString & tempDBName, const QString& projectPath)
{
    if(tempDBName.isEmpty())
        return false;

    if(!m_Init) //TODO:使用原子递增
    {
        m_TempDBName = tempDBName;
        m_projectPath = projectPath;

        m_Init = true;
        m_Stop = false; //还没有开启线程 不用加锁
        if(!InitTempDB(tempDBName))
            return false;

        //m_pWorkDBConnection = new MDDBConnection(QString("MD_TEMP_DB_WORK_CONN_%1").arg(m_TempDBName));
        m_pWorkDBConnection = new MDDBConnection;
        QThread::start();//启动线程
        return true;
    }

    return false;
}

void TempDBWorker::Stop()
{
	m_StopLock.lock();
	m_Stop = true;
	m_StopConditon.wakeOne();
	m_StopLock.unlock();

	MDLOG_INFO_TO_LOGGER(QString("TempDBWorker::Stop:Start conn = %1").arg(m_TempDBConnection.connectionName()));
	wait(); //等待run返回
    MDLOG_INFO_TO_LOGGER(QString("TempDBWorker::Stop:End conn = %1").arg(m_TempDBConnection.connectionName()));
    //TODO:释放m_ConnList中对象
}

void TempDBWorker::ModifyConnParam(IMDDataBaseParam * pDBConnParam)
{
    if(!pDBConnParam)
    {
        MDLOG_WARN_TO_LOGGER("TempDBWorker::ModifyConnParam:para is null");
        return ;
    }

    m_Lock.lock();
    m_ConnList.append(pDBConnParam->clone());
    m_Lock.unlock();
}

// 考虑stop时的问题 TempDBWorker对象析构问题 使用智能指针
//为数据插入的SQL语句
void TempDBWorker::SaveData(const QString & sqlStr)
{
    //向链表插入数据
    m_Lock.lock();
    m_SQLStringList.append(sqlStr);
    m_Lock.unlock();
}

void TempDBWorker::run()
{
    while(!m_Stop)
    {
        //QThread::sleep(3); //test
        //执行修改工作数据库连接命令
        m_Lock.lock();
        SetWorkDBConnInfoNolock();
        m_Lock.unlock();
        //保存数据到临时数据库
        SaveDataToTempDB();

        if(false == m_WorkDBConnStatus)
        {
			m_WorkDBConnStatus = m_pWorkDBConnection->open();
            if(!m_WorkDBConnStatus)
            {
                MDLOG_INFO_TO_LOGGER(QString("TempDBWorker::run:work db connect is fail:param:user = %1, ip = %2, port = %3, dbType = %4, dbName = %5, connection name = %6")
                                                               .arg(m_pWorkDBConnection->userName()).arg(m_pWorkDBConnection->host()).arg(m_pWorkDBConnection->port())
                                                               .arg(m_pWorkDBConnection->driverName()).arg(m_pWorkDBConnection->dbName()).arg(m_pWorkDBConnection->connectionName()));
            }
        }

        if(!m_WorkDBConnStatus) //连接失败 过x秒重连
            goto MD_WAIT;

        if(IsEmptyInTempDB()) //临时数据库中是否有数据
            goto MD_WAIT;

        //向工作数据库中导入数据
        //成功:过x秒中新循环。
        //失败:失败的的原因:
        //1、连接断开 -->动作：关闭连接.过x秒重新循环.
        //2、数据非法或工作数据库状态不对-->动作：关闭连接.过x秒重新循环.

        if(!ImportToWorkDB())
        {
            m_WorkDBConnStatus = false;
        }

MD_WAIT:
        m_StopLock.lock(); //为了调用Stop时及时退出
        if(!m_Stop)
            m_StopConditon.wait(&m_StopLock, 3000);//过x秒重试
        m_StopLock.unlock();
    } //end while

}

bool TempDBWorker::InitTempDB(const QString & tempDBName)
{
    //临时数据库为sqlite，它的数据库名是一个文件，要进行拼接strDbName
    //设定数据库名为一个文件名 Qt的SQL模块才能创建SQLite数据库

    QString tempdbRootPath = QCoreApplication::applicationDirPath();
    QDir projectDir(m_projectPath);
    if(projectDir.exists())
        tempdbRootPath = m_projectPath;
    if(tempdbRootPath.right(1) != QDir::separator())
        tempdbRootPath.append("/");
    QString strPath = tempdbRootPath + "TempDatabase/";

    QDir dir(strPath);
    if (!dir.exists())
    {
        if(!dir.mkdir(strPath))
        {
            //创建文件路径失败 环境出现严重问题 程序不能正常工作
            MDLOG_ERROR_TO_LOGGER("TempDBWorker::InitTempDB:Cann't create file path.");
            return false;
        }
    }

    //加载sqlite驱动有可能失败 若Qt找不到sqlite的驱动动态库 就会失败
    m_TempDBConnection = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"), QString("MD_TEMP_DB_TEMP_CONN_%1").arg(tempDBName));

    if(!m_TempDBConnection.isValid()) //加载驱动是否成否
    {
        MDLOG_ERROR_TO_LOGGER("TempDBWorker::InitTempDB:Load SQLite Driver is failure.");
        return false;
    }
    //如果数据库名称相同 则临时数据库相同 程序没办法判断重名问题 调用者要保证数据库名称不能重名
    QString dbFullName = strPath + tempDBName + ".db";
    m_TempDBConnection.setDatabaseName(dbFullName);
    if(!m_TempDBConnection.open())
    {
        QSqlError err = m_TempDBConnection.lastError();
        MDLOG_ERROR_TO_LOGGER(QString("TempDBWorker::InitTempDB: %.").arg(err.text()));
        return false;
    }

    m_pTempDBInsertQuery = new QSqlQuery(m_TempDBConnection);
    bool b = m_pTempDBInsertQuery->exec(QLatin1String("CREATE TABLE IF NOT EXISTS smTb(id INTEGER PRIMARY KEY AUTOINCREMENT, content TEXT, count INT DEFAULT 0)"));
    if(!b) //不可能失败
    {
        MDLOG_ERROR_TO_LOGGER(QString("TempDBWorker::InitTempDB:create table fail.error txt = %1").arg(m_pTempDBInsertQuery->lastError().text()));
        return false;
    }

    b = m_pTempDBInsertQuery->prepare("insert into smTb(content) values(?)");
    if(!b) //不可能失败
    {
        MDLOG_ERROR_TO_LOGGER(QString("TempDBWorker::InitTempDB:prepare fail.error txt = %1").arg(m_pTempDBInsertQuery->lastError().text()));
        return false;
    }

    m_pTempDBCommonQuery = new QSqlQuery(m_TempDBConnection);
    m_pTempDBBackupQuery = new QSqlQuery(m_TempDBConnection);

    b = m_pTempDBBackupQuery->exec(QLatin1String("CREATE TABLE IF NOT EXISTS smTbBackup(id INTEGER PRIMARY KEY AUTOINCREMENT, sql TEXT, time TEXT)"));
    if(!b) //不可能失败
    {
       MDLOG_ERROR_TO_LOGGER(QString("TempDBWorker::InitTempDB:create backup table fail.error txt = %1").arg(m_pTempDBBackupQuery->lastError().text()));
        return false;
    }

    b = m_pTempDBBackupQuery->prepare("insert into smTbBackup(sql, time) values(?, ?)");
    if(!b) //不可能失败
    {
        MDLOG_ERROR_TO_LOGGER(QString("TempDBWorker::InitTempDB:prepare bakcup sql fail.error txt = %1")
                                                                .arg(m_pTempDBBackupQuery->lastError().text()));
        return false;
    }

    return true;
}

void TempDBWorker::SetWorkDBConnInfoNolock()
{
    if(!m_ConnList.size())
        return;

    if(m_TempDBName.isEmpty())
        return ;

    m_WorkDBConnStatus = false;

    IMDDataBaseParam * pIMDDBParam = m_ConnList.takeFirst();

    m_pWorkDBConnection->setDBType(ConvertDBType(pIMDDBParam->dbServeType()));
	m_pWorkDBConnection->setDBName(pIMDDBParam->dbName());
	m_pWorkDBConnection->setHost(pIMDDBParam->dbServerIP());
	m_pWorkDBConnection->setPort(pIMDDBParam->dbServerPort());
	m_pWorkDBConnection->setUserName(pIMDDBParam->dbUser());
	m_pWorkDBConnection->setPassword(pIMDDBParam->dbPassword());

    delete pIMDDBParam;
}

void TempDBWorker::SaveDataToTempDB()
{
    //在线程开始之间 已经保证m_TempDBConnection有效
    //因为是SQLite数据库 所以正常情况下m_TempDBConnection会一直有效

    //不对所有插入语句使用一个事务 因为如果其中的某条SQL出错 其他SQL就运行不了
    //再则 针对此种场景 也无需使用事务

    m_Lock.lock();
    if(m_SQLStringList.isEmpty())
    {
        m_Lock.unlock();
        return ;
    }
    m_Lock.unlock();

    if(!m_pTempDBInsertQuery) //不可能为空
    {
        MDLOG_ERROR_TO_LOGGER("TempDBWorker::SaveDataToTempDBNolock:tempQurey is null");
        abort(); //终止程序
    }

    QString contentStr;

    //插入SQLite数据库 很快
    while(true)
    {
        m_Lock.lock();
        if(m_SQLStringList.isEmpty())
        {
            m_Lock.unlock();
            break;
        }

        //使用预编译语句 防止拼SQL时 由于插入内容导致出错
        contentStr = m_SQLStringList.takeFirst();
        m_Lock.unlock();

        m_pTempDBInsertQuery->bindValue(0, contentStr);
        if(!m_pTempDBInsertQuery->exec())
        {
            //不可能失败 失败则为异常
            MDLOG_ERROR_TO_LOGGER(QString("TempDBWorker::SaveDataToTempDBNolock:sava to sqlite fail.conntent = %1").arg(contentStr));
            abort(); //终止程序
        }
    }
}

bool TempDBWorker::IsEmptyInTempDB()
{
    if(!m_pTempDBCommonQuery) //不可能出现的情况
    {
        MDLOG_ERROR_TO_LOGGER("TempDBWorker::IsEmptyInTempDB:m_pTempDBCommonQuery is null");
        abort(); //终止程序
        return false;
    }

    if(!m_pTempDBCommonQuery->exec("select count(*) from smTb")) //不可能出现的情况
    {
        MDLOG_ERROR_TO_LOGGER(QString("TempDBWorker::IsEmptyInTempDB:select is fail.error text = %1").arg(m_pTempDBCommonQuery->lastError().text()));
        abort(); //终止程序
        return false;
    }

    m_pTempDBCommonQuery->next();
    int num = m_pTempDBCommonQuery->value(0).toInt();
    return 0 == num;
}

class WorkDBImporter
{
public:
    WorkDBImporter(QSqlQuery * pTempDBCommonQuery, QSqlQuery * pTempDBBackupQuery, MDDBConnection * pWorkDBConnection)
        :m_pTempDBCommonQuery(pTempDBCommonQuery),
         m_pTempDBBackupQuery(pTempDBBackupQuery),
         m_pWorkDBConnection(pWorkDBConnection)
    {}
    bool Import();
private:
    //导出到工作数据库成功后 为了方便Debug 为了进一步防止丢失数据
    //将此条记录导入到临时数据库的备份表中 备份表的数据超过一定数量 在清空此表
    void InsertToTempDBBackupTable(const QString & contenStr);
    void DeleteItemInTempDB(int id);
    void MarkSQLErrorItem(int id);
private:
    QSqlQuery * m_pTempDBCommonQuery;
    QSqlQuery * m_pTempDBBackupQuery;
    //QSqlQuery * m_pWorkDBQuery;
    MDDBConnection * m_pWorkDBConnection;
};

bool TempDBWorker::ImportToWorkDB()
{
    if(!m_pWorkDBConnection) //不可能出现的情况
    {
        MDLOG_ERROR_TO_LOGGER("TempDBWorker::ImportToWorkDB:db conn is null");
        //abort();
        return false;
    }

    WorkDBImporter importer(m_pTempDBCommonQuery, m_pTempDBBackupQuery, m_pWorkDBConnection);
    return importer.Import();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool WorkDBImporter::Import()
{
    const static int PAGE_SIZE = 100;
    const static int MAX_READ_COUNT = 100000000;
    QString selectSQLStr;

    QList<int> idList;
    QList<int> idErrList;

    for(int i=0; i<MAX_READ_COUNT; i++)
    {
        //分页查询
        selectSQLStr = QString("select * from smTb order by id limit %1 offset %2").arg(PAGE_SIZE).arg(i*PAGE_SIZE);
        if(!m_pTempDBCommonQuery->exec(selectSQLStr))
        {
            //不可能出现的情况
            MDLOG_ERROR_TO_LOGGER(QString("DB_ERROR TempDBWorker::ImportToWorkDB:TempDB SQL error:sql = %1, err = %2").
                                                                    arg(selectSQLStr).arg(m_pTempDBCommonQuery->lastError().text()));
            //abort();
            return false;
        }

        //SQLite不支持QuerySize特性 所以不能使用size()函数
        //if(m_pTempDBCommonQuery->size() <1) //没有数据了
            //break;

        QString sqlStr;
        int importCount= 0;

        QSqlQuery query(QSqlDatabase::database(m_pWorkDBConnection->connectionName()));
        while(m_pTempDBCommonQuery->next())
        {
            //while内函数不能使用m_pTempDBCommonQuery
            importCount++;
            sqlStr = m_pTempDBCommonQuery->value(1).toString(); //表的第二项保存着SQL语句
            if(query.exec(sqlStr))
            {
                //导出到工作数据库成功后 为了方便Debug 为了进一步防止丢失数据
                //将此条记录导入到临时数据库的备份表中 备份表的数据超过一定数量 在清空此表
                InsertToTempDBBackupTable(sqlStr);
                //DeleteItemInTempDB(m_pTempDBCommonQuery->value(0).toInt());
                idList.append(m_pTempDBCommonQuery->value(0).toInt());
            }
            else
            {
                MDLOG_ERROR_TO_LOGGER(QString("WorkDBImporter::Import:work db error:sql = %1, err txt = %2 ")
                                                                        .arg(sqlStr).arg(query.lastError().text()));

                idErrList.append(m_pTempDBCommonQuery->value(0).toInt());
                //return false; //不马上return false 防止一条SQL错误 导致后面正确的SQL都无法执行
            }
        }//end while

        if(!importCount) //一旦进入for循环 必然从这里退出
        {
            while(!idList.isEmpty()) //同时清空idList
                DeleteItemInTempDB(idList.takeFirst());

            bool b = idErrList.isEmpty();
            while(!idErrList.isEmpty())
                MarkSQLErrorItem(idErrList.takeFirst());

            return b; //idErrList空 说明所有SQL都导出成功 所以返回true
        }

    }//end for

    //临时数据库为空 返回true
    return true;
}

void WorkDBImporter::InsertToTempDBBackupTable(const QString & contentStr)
{
    m_pTempDBBackupQuery->bindValue(0, contentStr);
    m_pTempDBBackupQuery->bindValue(1, QDateTime::currentDateTime().toString(Qt::ISODate)); // 日期精确到秒 Qt::ISODate防止出现中文
    if(!m_pTempDBBackupQuery->exec()) //不可能失败
    {
        MDLOG_ERROR_TO_LOGGER(QString("WorkDBImporter::InsertToTempDBBackupTable:conten = %1.err text = %2")
                                                                .arg(contentStr).arg(m_pTempDBBackupQuery->lastError().text()));
    }
}

void WorkDBImporter::DeleteItemInTempDB(int id)
{
    QString deleteSQLStr = QString("delete from smTb where id = %1").arg(id);
    if(!m_pTempDBCommonQuery->exec(deleteSQLStr))
    {
        MDLOG_ERROR_TO_LOGGER(QString("WorkDBImporter::DeleteItemInTempDB:sql = %1.err text = %2")
                                                                .arg(deleteSQLStr).arg(m_pTempDBCommonQuery->lastError().text()));
    }

}

//标记执行出错的SQL语句.将计数加1
void WorkDBImporter::MarkSQLErrorItem(int id)
{
    QString sqlStr = QString("update smTb set count=count+1 where id=%1").arg(id);
    if(!m_pTempDBCommonQuery->exec(sqlStr))
    {
        MDLOG_ERROR_TO_LOGGER(QString("WorkDBImporter::MarkSQLErrorItem:sql = %1.err text = %2")
                                                                .arg(sqlStr).arg(m_pTempDBCommonQuery->lastError().text()));
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
