#include "MDArchivesVariable.h"

CMDArchivesVariable::CMDArchivesVariable()
{
    m_databaseConnectName = "";
    m_initVarValueDatabaseConnectName = "";
    m_name = "";
    m_dataType=MDDataType::Float;
    m_mdVariable = NULL;
}
//!变量表名：Group_xx_yy_变量名
CMDArchivesVariable::CMDArchivesVariable(IMDRTDBVariable *var)
{
    m_mdVariable =var;
    m_databaseConnectName = "";
    m_initVarValueDatabaseConnectName = "";
    m_name = m_mdVariable->name();
    m_name = m_name.replace(".","_");
    m_dataType=m_mdVariable->dataType();
    m_dataSaveRulerParameter = m_mdVariable->dataSaveRulerParameter();
    m_initType = m_mdVariable->initializeParameter().m_initType;
}
CMDArchivesVariable::~CMDArchivesVariable()
{
       release();
}

void CMDArchivesVariable::setVariable(IMDRTDBVariable *var)
{
    m_mdVariable =var;
}

//!名称
QString CMDArchivesVariable::name()
{
    return m_name;
}

void CMDArchivesVariable::setName(const QString & name)
{
    m_name = name;
    m_name = m_name.trimmed();
    m_name = m_name.replace(".","_");
}
//!创建变量表名
void CMDArchivesVariable::initial()
{
     QMutexLocker locker(&m_objMutex);
    if(m_databaseConnectName.isEmpty()||m_name.isEmpty())
    {
          return;
    }
    m_db = QSqlDatabase::database(m_databaseConnectName);
    if(!m_db.isValid()||!m_db.isOpen())
        return;
     QString createTableCmd= "CREATE TABLE IF NOT EXISTS ";
     createTableCmd =createTableCmd+  m_name ;
     switch (m_dataType) {
     case MDDataType::DateTime:
         createTableCmd =createTableCmd+ " (RecordTime DATETIME PRIMARY KEY,TimeStamp DATETIME,Quality INTEGER,Value DATETIME)";
         break;
     case MDDataType::String:
         createTableCmd =createTableCmd+ " (RecordTime DATETIME PRIMARY KEY,TimeStamp DATETIME,Quality INTEGER,Value TEXT)";
         break;
     case MDDataType::Float:
     case MDDataType::Double:
         createTableCmd =createTableCmd+ " (RecordTime DATETIME PRIMARY KEY,TimeStamp DATETIME,Quality INTEGER,Value REAL)";
         break;
    default:
         createTableCmd =createTableCmd+ " (RecordTime DATETIME PRIMARY KEY,TimeStamp DATETIME,Quality INTEGER,Value INTEGER)";
         break;
     }
     QSqlQuery query(m_db);
     bool bret =query.exec(createTableCmd);

     m_time.start();
}

void CMDArchivesVariable::initVarChangeStorage()
{
    QMutexLocker locker(&m_objMutex);
    if(m_initVarValueDatabaseConnectName.isEmpty())
    {
         return;
    }
    m_varChangeInitValueDb = QSqlDatabase::database(m_initVarValueDatabaseConnectName);
    if(!m_varChangeInitValueDb.isValid()||!m_varChangeInitValueDb.isOpen())
        return;

    QSqlQuery query(m_varChangeInitValueDb);
    bool b = query.exec(QLatin1String("CREATE TABLE IF NOT EXISTS VarInitValue(VarName Text PRIMARY KEY, Value TEXT, RecordTime DATETIME)"));
    if(!b) //不可能失败
    {
        //MDLOG_ERROR_TO_LOGGER(QString("TempDBWorker::InitTempDB:create table fail.error txt = %1").arg(m_pTempDBInsertQuery->lastError().text()));
        return;
    }

//    b = m_pTempDBInsertQuery->prepare("insert into VarInitValue(VarName, Value, RecordTime) values(?,?,?)");
//    if(!b) //不可能失败
//    {
//        //MDLOG_ERROR_TO_LOGGER(QString("TempDBWorker::InitTempDB:prepare fail.error txt = %1").arg(m_pTempDBInsertQuery->lastError().text()));
//        return;
//    }
}

void CMDArchivesVariable::release()
{
    QMutexLocker locker(&m_objMutex);
    if(!m_db.isValid()||!m_db.isOpen())
        return;
     QString dropTableCmd = "DROP TABLE  ";
     dropTableCmd =dropTableCmd+  m_name ;
     QSqlQuery query(m_db);
     query.exec(dropTableCmd);
}
//!数据库连接名
void CMDArchivesVariable::setDatabaseConnectName(const QString &databaseConnectName)
{
    m_databaseConnectName = databaseConnectName;
}
void CMDArchivesVariable::setInitVarValueDatabaseConnectName(const QString &databaseConnectName)
{
    m_initVarValueDatabaseConnectName = databaseConnectName;
}

//!处理变化存储
void CMDArchivesVariable::changeDataArchives(CMDArchivesDataCmd* cmd)
{
    if(cmd == NULL)
    {
        return;
    }
    if(cmd->initMode() == MDRTInitMode::LastValue)
    {
        addVarChangeInitValueData(cmd->value(),cmd->quality(),cmd->timeStamp());
    }
    if(m_dataSaveRulerParameter.m_saveRuler== MDRTDataSaveRuler::DataChanged)
    {
         addData(cmd->value(),cmd->quality(),cmd->timeStamp());
         QDateTime now = QDateTime::currentDateTime();
         if(!m_startDeleteDateTime.isValid())
         {
             m_startDeleteDateTime= now.addSecs(m_dataSaveRulerParameter.m_timeRange*1.5);
         }
         if(now>=m_startDeleteDateTime)
         {
             QDateTime startTime = now.addSecs((-1)*m_dataSaveRulerParameter.m_timeRange);
             deleteData(startTime);
             m_startDeleteDateTime = now.addSecs(m_dataSaveRulerParameter.m_timeRange*1.5);
         }
    }
}
//!处理周期存储，利用m_time.restart()判断记录周期
void CMDArchivesVariable::cycleDataArchives()
{
    if(m_mdVariable == NULL)
    {
        return;
    }
    if(m_dataSaveRulerParameter.m_saveRuler== MDRTDataSaveRuler::Cycle &&
        m_time.elapsed()>=m_dataSaveRulerParameter.m_saveCycle*1000)
    {
         addData(m_mdVariable->currentValue(),m_mdVariable->currentQuality(),m_mdVariable->timeStamp());
         m_time.restart();
         QDateTime now = QDateTime::currentDateTime();
         if(!m_startDeleteDateTime.isValid())
         {
             m_startDeleteDateTime= now.addSecs(m_dataSaveRulerParameter.m_timeRange*1.5);
         }
         if(now>=m_startDeleteDateTime)
         {
             QDateTime startTime = now.addSecs((-1)*m_dataSaveRulerParameter.m_timeRange);
             deleteData(startTime);
             m_startDeleteDateTime = now.addSecs(m_dataSaveRulerParameter.m_timeRange*1.5);
         }
    }
}
/*!
\section 增加或删除变量
*/
//添加数据
bool CMDArchivesVariable::addData(const QVariant &value,const MDVariableDataQuality &quality,const QDateTime &timeStamp)
{
    QMutexLocker locker(&m_objMutex);

    if(value.isNull())
        return false;
    if(!value.isValid())
        return false;

    if(!m_db.isValid()||!m_db.isOpen())
        return false;

    if(m_databaseConnectName == "")
        return false;

    if(!QSqlDatabase::isDriverAvailable("QSQLITE"))
        return false;

    if(!QSqlDatabase::contains(m_databaseConnectName))
        return false;

    const uint threadId = quintptr(QThread::currentThreadId());
    QString connectionName = QString("addData_%1").arg(threadId);
    QSqlDatabase addDataDb = QSqlDatabase::database(m_databaseConnectName);
    if(!addDataDb.isValid() || !addDataDb.isOpenError()){
        addDataDb = QSqlDatabase::cloneDatabase(m_db, connectionName);
        if (!addDataDb.open()) {
            qDebug() << "DB" << connectionName << "was not opened to addData!";
            return false;
        }
    }
    if(!addDataDb.isValid()||!addDataDb.isOpen())
       return false;

    QString insertDataCmd = "INSERT INTO ";
    insertDataCmd =insertDataCmd+  m_name ;
    insertDataCmd =insertDataCmd+  " VALUES(" ;
    switch (m_dataType) {
        case MDDataType::DateTime:
        insertDataCmd =insertDataCmd+ QString("'%1','%2'',%3,'%4'')") .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
                                                                                                .arg(timeStamp.toString("yyyy-MM-dd hh:mm:ss.zzz"))
                                                                                                .arg((int)quality)
                                                                                                .arg(value.toDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
        break;
        case MDDataType::String:
        insertDataCmd =insertDataCmd+ QString("'%1','%2'',%3,'%4'')") .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
                                                                                                .arg(timeStamp.toString("yyyy-MM-dd hh:mm:ss.zzz"))
                                                                                                .arg((int)quality)
                                                                                                .arg(value.toString());
        break;
        case MDDataType::Bool:
        insertDataCmd =insertDataCmd+ QString("'%1','%2',%3,%4)") .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
                                                                                                .arg(timeStamp.toString("yyyy-MM-dd hh:mm:ss.zzz"))
                                                                                                .arg((int)quality)
                                                                                                .arg(value.toBool()?1:0);
        break;
        case MDDataType::Double:
        case MDDataType::Float:
        insertDataCmd =insertDataCmd+ QString("'%1','%2',%3,%4)") .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
                                                                                                .arg(timeStamp.toString("yyyy-MM-dd hh:mm:ss.zzz"))
                                                                                                .arg((int)quality)
                                                                                                .arg(value.toDouble());
        break;
        default:
        insertDataCmd =insertDataCmd+ QString("'%1','%2',%3,%4)") .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
                                                                                                .arg(timeStamp.toString("yyyy-MM-dd hh:mm:ss.zzz"))
                                                                                                .arg((int)quality)
                                                                                                .arg(value.toString());
        break;
    }
    QSqlQuery query(addDataDb);
    bool bret = false;
    //qDebug() << "sqlite cmd: " << insertDataCmd;
    bret = query.exec(insertDataCmd);
    if(query.lastError().type() != QSqlError::ErrorType::NoError)
    {
        qDebug() << "sqlite cmd: " << insertDataCmd;
        qDebug() << "sqlite cmd ret : " << bret << " " << query.lastError();
    }

    return bret;
}

bool CMDArchivesVariable::addVarChangeInitValueData(const QVariant &value,const MDVariableDataQuality &quality,const QDateTime &timeStamp)
{
    QMutexLocker locker(&m_objMutex);

    if(!m_varChangeInitValueDb.isValid()||!m_varChangeInitValueDb.isOpen())
            return false;

    QString upsertDataCmd1 =
            "UPDATE VarInitValue SET Value='%1', RecordTime='%2' WHERE VarName='%3';";
    QString upsertDataCmd2 =
            "INSERT INTO VarInitValue(VarName, Value, RecordTime) SELECT '%3', '%1', '%2' WHERE changes()=0;"
            ;
//    "BEGIN;"
//    "UPDATE VarInitValue SET Value='%1', RecordTime='%2' WHERE VarName='%3';"
//    "INSERT INTO VarInitValue(VarName, Value, RecordTime) SELECT '%3', '%1', '%2' WHERE changes()=0;"
//    "COMMIT;";
    upsertDataCmd1 = upsertDataCmd1.arg(value.toString()).arg(timeStamp.toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(m_mdVariable->name());
    upsertDataCmd2 = upsertDataCmd2.arg(value.toString()).arg(timeStamp.toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(m_mdVariable->name());

    QSqlQuery query(m_varChangeInitValueDb);
    bool bret = false;
    m_varChangeInitValueDb.transaction();
    bret = query.exec(upsertDataCmd1);
    bret = query.exec(upsertDataCmd2);
    m_varChangeInitValueDb.commit();
    if(query.lastError().type() != QSqlError::ErrorType::NoError)
    {
        qDebug() << "sqlite cmd: " << upsertDataCmd1;
        qDebug() << "sqlite cmd: " << upsertDataCmd2;
        qDebug() << "sqlite cmd ret : " << bret << " " << query.lastError();
    }

    return bret;
}

//删除数据
void CMDArchivesVariable::deleteData(const QDateTime &startTime)
{

    QMutexLocker locker(&m_objMutex);
    const uint threadId = quintptr(QThread::currentThreadId());
    QString connectionName = QString("deleteData_%1").arg(threadId);
    QSqlDatabase deleteDataDb = QSqlDatabase::database(connectionName);
    if(!deleteDataDb.isValid()){
        deleteDataDb = QSqlDatabase::cloneDatabase(m_db, connectionName);
        if (!deleteDataDb.open()) {
            qDebug() << "DB" << connectionName << "was not opened to deleteData!";
            return;
        }
    }
    if(!deleteDataDb.isValid()||!deleteDataDb.isOpen())
       return;

     QString deleteDataCmd = "DELETE FROM ";
     deleteDataCmd =deleteDataCmd+  m_name ;
     deleteDataCmd =deleteDataCmd+  "  where RecordTime<=" ;
     deleteDataCmd =deleteDataCmd+  QString("'%1'") .arg(startTime.toString("yyyy-MM-dd hh:mm:ss.zzz"));
     QSqlQuery query(deleteDataDb);
     bool bret =query.exec(deleteDataCmd);
}
void CMDArchivesVariable::deleteData(const QDateTime &startTime,const QDateTime &endTime)
{
    QMutexLocker locker(&m_objMutex);
    const uint threadId = quintptr(QThread::currentThreadId());
    QString connectionName = QString("deleteData_%1").arg(threadId);
    QSqlDatabase deleteDataDb = QSqlDatabase::database(connectionName);
    if(!deleteDataDb.isValid()){
        deleteDataDb = QSqlDatabase::cloneDatabase(m_db, connectionName);
        if (!deleteDataDb.open()) {
            qDebug() << "DB" << connectionName << "was not opened to deleteData!";
            return;
        }
    }
    if(!deleteDataDb.isValid()||!deleteDataDb.isOpen())
       return;

     QString deleteDataCmd = "DELETE FROM ";
     deleteDataCmd =deleteDataCmd+  m_name ;
     deleteDataCmd =deleteDataCmd+  "  where RecordTime>=" ;
     deleteDataCmd =deleteDataCmd+  QString("'%1'") .arg(startTime.toString("yyyy-MM-dd hh:mm:ss.zzz"));
     deleteDataCmd =deleteDataCmd+  " and RecordTime<= ";
     deleteDataCmd =deleteDataCmd+  QString("'%1'") .arg(endTime.toString("yyyy-MM-dd hh:mm:ss.zzz"));
     QSqlQuery query(deleteDataDb);
     bool bret =query.exec(deleteDataCmd);
}
void CMDArchivesVariable::deleteAllData()
{
    QMutexLocker locker(&m_objMutex);
    const uint threadId = quintptr(QThread::currentThreadId());
    QString connectionName = QString("deleteAllData_%1").arg(threadId);
    QSqlDatabase deleteAllDataDb = QSqlDatabase::database(connectionName);
    if(!deleteAllDataDb.isValid()){
        deleteAllDataDb = QSqlDatabase::cloneDatabase(m_db, connectionName);
        if (!deleteAllDataDb.open()) {
            qDebug() << "DB" << connectionName << "was not opened to deleteAllData_!";
            return;
        }
    }
    if(!deleteAllDataDb.isValid()||!deleteAllDataDb.isOpen())
       return;

     QString deleteDataCmd = "DELETE FROM ";
     deleteDataCmd =deleteDataCmd+  m_name ;
     QSqlQuery query(deleteAllDataDb);
     bool bret =query.exec(deleteDataCmd);
}
//获取数据
bool CMDArchivesVariable::getData(const QDateTime &startTime,const QDateTime &endTime,CMDDatabaseRecordset *dataRecordset)
{
     if(dataRecordset == NULL)
     {
         return false;
     }

    QMutexLocker locker(&m_objMutex);
    const uint threadId = quintptr(QThread::currentThreadId());
    QString connectionName = QString("getData_%1").arg(threadId);
    QSqlDatabase getDataDb = QSqlDatabase::database(connectionName);
    if(!getDataDb.isValid()){
        getDataDb = QSqlDatabase::cloneDatabase(m_db, connectionName);
        if (!getDataDb.open()) {
            qDebug() << "DB" << connectionName << "was not opened to getData!";
            return false;
        }
    }

    if(!getDataDb.isValid()||!getDataDb.isOpen())
       return false;
     QString slelectDataCmd= "SELECT  RecordTime,Value FROM ";
     slelectDataCmd =slelectDataCmd+  m_name ;
     slelectDataCmd =slelectDataCmd+  "  where RecordTime>=" ;
     slelectDataCmd =slelectDataCmd+  QString("'%1'") .arg(startTime.toString("yyyy-MM-dd hh:mm:ss.zzz"));
     slelectDataCmd =slelectDataCmd+  " and RecordTime<= ";
     slelectDataCmd =slelectDataCmd+  QString("'%1'") .arg(endTime.toString("yyyy-MM-dd hh:mm:ss.zzz"));
     slelectDataCmd =slelectDataCmd+  " ORDER BY RecordTime";
     qDebug() << slelectDataCmd;
     QSqlQuery query(getDataDb);
     if(query.exec(slelectDataCmd))
     {
         dataRecordset->bindSqlQuery(&query);
         return true;
     }
     return false;
}
bool CMDArchivesVariable::getAllData(CMDDatabaseRecordset *dataRecordset)
{
    if(dataRecordset == NULL)
    {
        return false;
    }
    QMutexLocker locker(&m_objMutex);
    const uint threadId = quintptr(QThread::currentThreadId());
    QString connectionName = QString("getAllData_%1").arg(threadId);
    QSqlDatabase getDataDb = QSqlDatabase::database(connectionName);
    if(!getDataDb.isValid()){
        getDataDb = QSqlDatabase::cloneDatabase(m_db, connectionName);
        if (!getDataDb.open()) {
            qDebug() << "DB" << connectionName << "was not opened to getAllData!";
            return false;
        }
    }
    if(!getDataDb.isValid()||!getDataDb.isOpen())
       return false;
     QString slelectDataCmd = "SELECT  RecordTime,Value FROM ";
     slelectDataCmd =slelectDataCmd+  m_name ;
     slelectDataCmd =slelectDataCmd+  "  ORDER BY RecordTime" ;
     QSqlQuery query(getDataDb);
     if(query.exec(slelectDataCmd))
     {
         dataRecordset->bindSqlQuery(&query);
         return true;
     }
     return false;
}
