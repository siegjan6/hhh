#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>
#include <QThread>
#include <QSqlDriver>
#include <QDebug>

#include "MDDatabaseService.h"
#include "MDNotifyParamCMD.h"
#include "IMDDataSource.h"
#include "IMDDataTable.h"
#include "IMDDataGroup.h"
#include "IMDDataGroupManager.h"
#include "IMDDataArchives.h"
#include "IMDDataField.h"
#include "tempdbworker.h"//linux下,include文件区分大小写
#include "timerthread.h"
#include <MDDBConnect.h>
#include "MDDatabaseCheckCMD.h"
//============================================================


class DBSourceTimerCallback : public ITimerCallback
{
public:
    DBSourceTimerCallback(CMDDBSourceService * pDBSource)
        :m_pDBSource(pDBSource)
    {}
    virtual void Call() Q_DECL_OVERRIDE;
private:
    CMDDBSourceService * m_pDBSource;
};

void DBSourceTimerCallback::Call()
{
    if(!m_pDBSource->dbConnectionStatus())
    {
        IMDDataSource * pDataSource = m_pDBSource->dataSource();
        if(pDataSource)
        {
            m_pDBSource->submitCommand(new CMDDatabaseCheckCMD(m_pDBSource, pDataSource->dbParam()));
            MDLOG_INFO_TO_LOGGER("DBSourceTimerCallback:Call:reconnect work db");
        }
        else
            MDLOG_INFO_TO_LOGGER("DBSourceTimerCallback:Call: data source is null");
    }
}

CMDDBSourceService::CMDDBSourceService()
    : CMDCommandQueueEngine(),
      m_pDBSource(0),
      m_mdDataArchives(0),
      m_pWorkDBConnection(0),
      m_bDBConnectionStatus(false)
{
    m_pTempDBWorker = new TempDBWorker;
    m_pTimerThread = new TimerThread;
    m_pWorkDBConnection = new MDDBConnection;
}

CMDDBSourceService::~CMDDBSourceService()
{
    release();
    m_pTempDBWorker->Stop();
    m_pTimerThread->Stop();
    delete m_pTempDBWorker;
    delete m_pTimerThread;
}

void CMDDBSourceService::onProcessCommand(CMDCommand *command)
{
    if (NULL == command)
        return;

    CMDDatabaseCommand * pCmd = dynamic_cast<CMDDatabaseCommand *>(command);
    if(pCmd)
        pCmd->Do();
}

void CMDDBSourceService::release()
{
    if (m_pWorkDBConnection)
    {
        m_pWorkDBConnection->close(); //TODO:待看源码

        delete m_pWorkDBConnection;
        m_pWorkDBConnection = NULL;
    }
}

bool CMDDBSourceService::doStart()
{
    /*
     * 加载项目时，首先需要检查数据库是否已经存在，检查表是否已经创建
     * 即创建数据库检查命令DB_CMD_CHECK_DATABASE，并提交到队列中
     * 加载项目时，才构造数据库的连接，反之卸载项目时，会关闭数据库并释放数据库连接
     */

    start(); //!父类方法 开启线程

    if(m_pDBSource)
    {
        CMDDatabaseCheckCMD * pCmd = new CMDDatabaseCheckCMD(this, m_pDBSource->dbParam()->clone());
        submitCommand(pCmd);
        return true;
    }
    else
    {
        MDLOG_ERROR_TO_LOGGER("CMDDBSourceService::doStar:data source is null");
        return false;
    }

    return false;
}

// 启动数据库服务
bool CMDDBSourceService::startService()
{
    if (MDCommandEngineStatus::starting == status())
    {
        return false;
    }

    QString projectPath = m_mdDataArchives->projectPath();
    if(!m_pTempDBWorker->Start(m_pDBSource->name(), projectPath))
    {
        //不能出错 出错意味着程序运行环境出现异常 程序已经不能正常工作 直接退出程序
       //TODO:更换log
        MDLOG_ERROR_TO_LOGGER("CMDDBSourceService::startService:It's fial that start tempDBWork");
        //abort();
        return false;
    }

    m_pTempDBWorker->ModifyConnParam(m_pDBSource->dbParam());

    DBSourceTimerCallback * pCallback = new DBSourceTimerCallback(this);
    m_pTimerThread->Start(pCallback, 15*1000); //每隔15秒检查工作数据库连接是否正常

    return doCtrlServiceCmd(DatabaseEngine::DB_CMD_START);
}

// 停止数据库服务
bool CMDDBSourceService::stopService()
{
    // 如果服务未停止
    if(MDCommandEngineStatus::starting == status())
    {
        // 停止
        if(m_pTempDBWorker)
            m_pTempDBWorker->Stop();

        if(m_pTimerThread)
            m_pTimerThread->Stop();

        return doCtrlServiceCmd(DatabaseEngine::DB_CMD_STOP);
    }

    return false;
}

bool CMDDBSourceService::doCtrlServiceCmd(DatabaseEngine::DB_CMD_TYPE type)
{
    if(NULL == m_pDBSource)
    {
        return false;
    }

    switch(type)
    {
    case DatabaseEngine::DB_CMD_START:
        // 开始
        return doStart();
    case DatabaseEngine::DB_CMD_STOP:
        // 停止
        return doStop();
    default:
        //TODO:log
        break;
    }

    return false;
}

//TODO:停止后 再启动会崩溃
bool CMDDBSourceService::doStop()
{
    cancelAllWaiting();
    QThread::msleep(THREAD_QUIT_SLEEP_MS);
    stop();
    release();
    return true;
}

bool CMDDBSourceService::asyncCallDataArchives(DataArchives::DB_NOTIFY_TYPE nMSGType, CMDDataArchivesNotifyParam *pParam)
{
    CMDNotifyParamCMD * pNotifyCMD = new CMDNotifyParamCMD(m_pDBSource->dbParam()->dbServeType(), nMSGType, pParam, this);
    submitCommand(pNotifyCMD);
    return true;
}

MDDBConnection *CMDDBSourceService::currdb()
{
    return m_pWorkDBConnection;
}

bool CMDDBSourceService::executeInsertSQL(const QString &strSQLCmd)
{
    if (!m_pWorkDBConnection)
    {
        //TODO:log
        return false;
    }

    if(!m_pWorkDBConnection->isOpen())
    {

        MDLOG_INFO_TO_LOGGER(QString("CMDDBSourceService::executeInsertSQL: %1, error msg: %2")
                                                            .arg(strSQLCmd).arg(m_pWorkDBConnection->getLastErrorText()));
        return false;
    }

    //QSqlQuery * pWorkSqlQuery =  m_pWorkDBConnection->getQuery();
    //if (pWorkSqlQuery->exec(strSQLCmd))
    if(m_pWorkDBConnection->executeSQL(strSQLCmd))
    {
        //m_pWorkSqluery->clear(); //TODO:不需要调用吧 待开源码
        return true;
    }

    //fail
    MDLOG_INFO_TO_LOGGER(QString("CMDDBSourceService::executeInsertSQL: sql = %1, error = %2")
                                                        .arg(strSQLCmd).arg(m_pWorkDBConnection->getLastErrorText()));
    //当数据库异常终止后,数据库的状态还是打开状态.只能通过SQL执行出错判断出TCP连接可能出错.
    //所以，为了处理可能的TCP出错这种情况。所以要断开数据库。之后程序会自动重连.
    //m_pWorkDBConnection->getDBConn().close();
    m_pWorkDBConnection->close();
    m_bDBConnectionStatus = false;

    return false;

    /*LD 防止数据库执行exec方法的内存泄漏，当数据库插入的条数达到SQL_EXEC_SUM值时，
     * 会释放QSqlQuery对象，并关闭数数据库。
     *
    */
    /*
    if (++offset > SQL_EXEC_SUM) //TODO:有待看Qt源码
    {
        //TODO:Log
        offset = 0;
        m_pWorkSqluery->clear();
        delete m_pWorkSqluery;
        m_pWorkSqluery = NULL;
        m_pWorkDBConnection->currentDatabase().close();//TODO :close 会引发什么内部变化
        if (m_pWorkDBConnection->currentDatabase().open() && NULL == m_pWorkSqluery)
        {
            m_pWorkSqluery = new QSqlQuery(m_pWorkDBConnection->currentDatabase());
        }
    }
    */
}

bool CMDDBSourceService::executeCommonSQL(const QString &strSQLCmd)
{
    if (!m_pWorkDBConnection)
    {
        //TODO:log
        return false;
    }

    if(!m_pWorkDBConnection->isOpen())
    {

        MDLOG_INFO_TO_LOGGER(QString("CMDDBSourceService::executeNonInsertSQL: %1, error msg: %2")
                                                          .arg(strSQLCmd).arg(m_pWorkDBConnection->getLastErrorText()));
        return false;
    }

    //QSqlQuery * pWorkSqlQuery =  m_pWorkDBConnection->getQuery();
    //if (pWorkSqlQuery->exec(strSQLCmd))
    if(m_pWorkDBConnection->executeSQL(strSQLCmd))
    {
        //m_pWorkSqluery->clear(); //TODO:不需要调用吧 待开源码
        return true;
    }

    return false;
}

QString CMDDBSourceService::getLastError()
{
    if(m_pWorkDBConnection)
    {
        return m_pWorkDBConnection->getLastErrorText();
    }
    return "";
}

void CMDDBSourceService::SaveSQLToTempDB(const QString & strSQLCmd)
{
    m_pTempDBWorker->SaveData(strSQLCmd);
}

void CMDDBSourceService::setDBConnectionStatus(bool bStatus)
{
    m_bDBConnectionStatus = bStatus;
}

bool CMDDBSourceService::dbConnectionStatus()
{
    return m_bDBConnectionStatus;
}

bool CMDDBSourceService::notifyModifyDatabaseParam(IMDDataSource *pDataSrc)
{

    if (NULL == m_pDBSource || NULL == pDataSrc)
    {
        MDLOG_INFO_TO_LOGGER("CMDDBSourceService::notifyModifyDatabaseParam:modify err1");
        return false;
    }

    m_pDBSource->dbParam()->setdbServeType(pDataSrc->dbParam()->dbServeType());
    m_pDBSource->dbParam()->setdbServerIP(pDataSrc->dbParam()->dbServerIP());
    m_pDBSource->dbParam()->setdbServerPort(pDataSrc->dbParam()->dbServerPort());
    m_pDBSource->dbParam()->setdbName(pDataSrc->dbParam()->dbName());
    m_pDBSource->dbParam()->setdbUser(pDataSrc->dbParam()->dbUser());
    m_pDBSource->dbParam()->setdbPassword(pDataSrc->dbParam()->dbPassword());

    CMDDatabaseCheckCMD * pCmd = new CMDDatabaseCheckCMD(this, pDataSrc->dbParam()->clone());
    submitCommand(pCmd);

    m_pTempDBWorker->ModifyConnParam(pDataSrc->dbParam());

    delete pDataSrc;
    return true;
}

/*
 //处理金仓数据库预编译语句的技巧 保留此段注释
bool CMDDBSourceService::sqlBindValueExecute(QSqlQuery &query, QList<QVariant> &varValueList)
{
    bool bSuccess       = false;
    if (NULL == m_pDBSource)
    {
        return false;
    }

    if (DATABASEPARAM::DB_TYPE_KINGBASE == m_pDBSource->dbParam()->dbServeType())
    {
        //为了解决金仓数据库占位符方式插入数据库失败的处理方式，对空字串，空时间类型的value，要重新构造，否则会插入失败
        for (int i  = 0; i < varValueList.count(); ++i) //TODO:可完善
        {
            QVariant var = varValueList.at(i);
            switch (var.type())
            {
            case QVariant::Int:
//                MDLOG_INFO_TO_LOGGER(QString("engine---type: %1, index: %2, value: %3...").arg(var.type()).arg(i).arg(varValueList.at(i).toInt()));
                break;
            case QVariant::Double:
//                MDLOG_INFO_TO_LOGGER(QString("engine---type: %1, index: %2, value: %3...").arg(var.type()).arg(i).arg(varValueList.at(i).toDouble()));
                break;
            case QVariant::String:
                var.toString().isEmpty() ? var = QString() : var = QString(varValueList.at(i).toString());
//                MDLOG_INFO_TO_LOGGER(QString("engine---type: %1, index: %2, value: %3...").arg(var.type()).arg(i).arg(varValueList.at(i).toString()));
                break;
            case QVariant::DateTime:
                var.toDateTime().isNull() ? var = QDateTime() : var = QVariant(QDateTime::fromString(varValueList.at(i).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
//                MDLOG_INFO_TO_LOGGER(QString("engine---type: %1, index: %2, value: %3...").arg(var.type()).arg(i).arg(varValueList.at(i).toDateTime().toString("YYYY-mm-dd hh:mm:ss")));
                break;
            default:
                var.toString().isEmpty() ? var = QString() : var = QVariant((varValueList.at(i).toString()));
//                MDLOG_INFO_TO_LOGGER(QString("engine---type: %1, index: %2, value: %3...").arg(var.type()).arg(i).arg(varValueList.at(i).toString()));
                break;
            }
            query.bindValue(i, var);
        }

        bSuccess = query.exec();
    }
    else
    {
        for (int i  = 0; i < varValueList.count(); ++i)
        {
            QVariant var = varValueList.at(i);
            query.bindValue(i, var);
        }

        bSuccess = query.exec();
    }

    if (!bSuccess)
    {
        MDLOG_INFO_TO_LOGGER(QString("CMDDBSourceService::sqlBindValueExecute:exec sql error msg: %1").arg(query.lastError().text()));
    }

    return bSuccess;
}
*/

//TODO:换成静态函数
QString CMDDBSourceService::variantDataToString(MDDataType type, const QVariant &data)
{
    //根据type跟data换成拼接成字符串，用于插入数据库
    QString strData = "";
    QDateTime dateTime;
    switch(type)
    {
    case MDDataType::Bool:
    case MDDataType::Int16:
    case MDDataType::UInt16:
    case MDDataType::Int32:
    case MDDataType::UInt32:
    case MDDataType::Int64:
    case MDDataType::UInt64:
    case MDDataType::SByte:
    case MDDataType::Byte:
        strData = QString("%1").arg(data.toLongLong());
        strData += ", ";
        break;
    case MDDataType::Float:
    case MDDataType::Double:
        strData = QString("%1").arg(data.toDouble());
        strData += ", ";
        break;
    case MDDataType::DateTime:
        dateTime = data.toDateTime();
        if (dateTime.isNull() || !dateTime.isValid())
        {
            strData += "null";
            strData += ", ";
        }
        else
        {
            QString strTime = dateTime.toString("yyyy-MM-dd hh:mm:ss");
            strData += "'";
            strData += strTime;
            strData += "', ";
        }
        break;
    default:
        strData += "'";
        strData += QString("%1").arg(data.toString());
        strData += "', ";
        break;
    }

    return strData;
}
