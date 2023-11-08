#include "MDArchivesVariableManager.h"
#include <QtSql>
CMDArchivesVariableManager::CMDArchivesVariableManager()
{
    m_databaseConnectName = "";
    m_prjInitValueDatabaseConnectName = "";
    m_mdRTDBManager = NULL;
    m_running = false;
    m_logSwitch = NULL;
}
void CMDArchivesVariableManager::start()
{
    m_running = true;
}
void CMDArchivesVariableManager::stop()
{
    m_running = false;
}
bool CMDArchivesVariableManager::isRunning()
{
    return m_running;
}

static QAtomicInt dbConnCount;
static QAtomicInt initdbConnCount;
//!创建数据库+添加归档变量
void CMDArchivesVariableManager::initial()
{
    //创建数据库    
    if(m_databaseConnectName.isEmpty())//Y2hw
    {
        m_databaseConnectName = QString("MDArchivesVariable%1").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss_%1").arg(dbConnCount.fetchAndAddOrdered(1)+1));
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",m_databaseConnectName);
        // sql lite connectOptions
        // QSQLITE_BUSY_TIMEOUT
        // QSQLITE_OPEN_READONLY
        // QSQLITE_OPEN_URI
        // QSQLITE_ENABLE_SHARED_CACHE
        // QSQLITE_ENABLE_REGEXP
        db.setConnectOptions("QSQLITE_OPEN_URI;QSQLITE_ENABLE_SHARED_CACHE");
        //db.setDatabaseName(":memory:");     //创建数据库 sqllite3 内存模式
        db.setDatabaseName("file::memory:?cache=shared");
        //db.setDatabaseName("file:memdb1?mode=memory&cache=shared");
        if(db.open())
            db.close();
        db.open();
    }


    if(NULL==m_mdRTDBManager)
    {
        return;
    }

    if(m_prjInitValueDatabaseConnectName.isEmpty())//bosmutus
    {
        m_prjInitValueDatabaseConnectName = QString("MDPrjInitValue%1").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss_%1").arg(initdbConnCount.fetchAndAddOrdered(1)+1));
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",m_prjInitValueDatabaseConnectName);
        QString tempdbRootPath = QCoreApplication::applicationDirPath();
        QDir projectDir(m_mdRTDBManager->projectPath());
        if(projectDir.exists())
            tempdbRootPath = m_mdRTDBManager->projectPath();
        if(tempdbRootPath.right(1) != QDir::separator())
            tempdbRootPath.append("/");
        QString strPath = tempdbRootPath + "DataArchives/";

        QDir dir(strPath);
        if (!dir.exists())
        {
            if(!dir.mkdir(strPath))
            {
                //创建文件路径失败 环境出现严重问题 程序不能正常工作
                //MDLOG_ERROR_TO_LOGGER("TempDBWorker::InitTempDB:Cann't create file path.");
                return;
            }
        }
        if(!db.isValid()) //加载驱动是否成否
        {
            //MDLOG_ERROR_TO_LOGGER("TempDBWorker::InitTempDB:Load SQLite Driver is failure.");
            return;
        }
        QString tempDBName = "VarInitValue";
        //如果数据库名称相同 则临时数据库相同 程序没办法判断重名问题 调用者要保证数据库名称不能重名
        QString dbFullName = strPath + tempDBName + ".db";
        db.setDatabaseName(dbFullName);
        if(!db.open())
        {
            QSqlError err = db.lastError();
            //MDLOG_ERROR_TO_LOGGER(QString("TempDBWorker::InitTempDB: %.").arg(err.text()));
            return;
        }

        getPrjInitValuesFromDB();
    }


    //日志
    if(m_logSwitch == NULL)
    {
        m_logSwitch = CMDLogSwitchFactory::createSwitchWithLogger(m_mdRTDBManager->projectPath(), "RTDB.ArchivesVariableManager" , "归档");
        if(m_logSwitch->isOn())
        {
            m_logSwitch->LOG_INFO(m_databaseConnectName);
        }        
    }

    //添加需归档变量
    IMDVariableManager * varManager = m_mdRTDBManager->variableManager();
    if(NULL==varManager)
    {
        return;
    }
    for(int i=0;i<varManager->getVariableCount();i++)
    {
        IMDRTDBVariable*var = varManager->getVariableAt(i);
        if(NULL==var)
        {
            continue;
        }
        if(var->dataSaveRulerParameter().m_saveRuler!= MDRTDataSaveRuler::None)
        {
            addVariable(var);
        }
        if(var->initializeParameter().m_initType== MDRTInitMode::LastValue)
        {
            addPrjInitValueVariable(var);
        }
    }
}

QString CMDArchivesVariableManager::getPrjInitValue(const QString& varName)
{
    return m_prjInitValueMap[varName];
}

bool CMDArchivesVariableManager::getPrjInitValuesFromDB()
{
    if(!m_prjInitValueDatabaseConnectName.isEmpty())
    {
        QSqlDatabase db = QSqlDatabase::database(m_prjInitValueDatabaseConnectName);
        if(db.isValid() && db.isOpen())
        {
            QString sqlCmd = "SELECT VARNAME, Value FROM VarInitValue";
            QSqlQuery m_prjInitValueQuery(db);
            bool ret = m_prjInitValueQuery.exec(sqlCmd);
            if(!ret)
            {
                qDebug() << "sql cmd error: " << m_prjInitValueQuery.lastError() << "cmd: " << sqlCmd;
                return false;
            }
            m_prjInitValueQuery.seek(-1);
            while(m_prjInitValueQuery.next())
            {
                QString k = m_prjInitValueQuery.value(0).toString();
                QString v = m_prjInitValueQuery.value(1).toString();
                m_prjInitValueMap[k] = v;
                qDebug() << "k="<<k<<" v=" <<v;
            }
            m_prjInitValueQuery.clear();
        }
    }
    return true;
}
//!删除数据库+变量
void CMDArchivesVariableManager::release()
{
//    if(!m_databaseConnectName.isEmpty())
//    {
//        QSqlDatabase::removeDatabase(m_databaseConnectName);
//    }
    if(!m_prjInitValueDatabaseConnectName.isEmpty())
    {
        if(QSqlDatabase::contains(m_prjInitValueDatabaseConnectName)){
            QSqlDatabase db = QSqlDatabase::database(m_prjInitValueDatabaseConnectName);
            db.close();
        }
        QSqlDatabase::removeDatabase(m_prjInitValueDatabaseConnectName);
    }

    removeAllVariable();
    removeAllArchivesDataCmd();//Y2hw
    if(m_logSwitch != NULL && m_logSwitch->isOn())
        m_logSwitch->LOG_INFO("Release");

    m_logSwitch = NULL;
}
void CMDArchivesVariableManager::setRTDBManager(IMDRTDBManager *nsRTDBManager)
{
    m_mdRTDBManager = nsRTDBManager;
}
/*!
\section 变量操作
*/
bool CMDArchivesVariableManager::addVariable(IMDRTDBVariable *var)
{
    bool ret = false;
    if(var== NULL)
    {
        return ret;
    }
    if(getVariable(var->name())!= NULL)
    {
        return ret;
    }
    QMutexLocker locker(&m_archivesVariableListMutex);
    CMDArchivesVariable*  archivesVariable =new CMDArchivesVariable(var);
    archivesVariable->setDatabaseConnectName(m_databaseConnectName);
    archivesVariable->initial();
    m_archivesVariableList.append(archivesVariable);
    return true;
}

bool CMDArchivesVariableManager::deleteVariable(IMDRTDBVariable *var)
{
    bool ret = false;
    if(var== NULL)
    {
        return ret;
    }
    QMutexLocker locker(&m_archivesVariableListMutex);
    QString  varNameTemp= var->name();
    varNameTemp=varNameTemp.replace(".","_");
    for(int i=0;i<m_archivesVariableList.count();i++)
    {
        if(0==QString::compare(m_archivesVariableList[i]->name(), varNameTemp,Qt::CaseInsensitive))
        {
            delete m_archivesVariableList[i];
            m_archivesVariableList.removeAt(i);
            break;
        }
    }
    return true;
}
CMDArchivesVariable*  CMDArchivesVariableManager::getVariable(const QString & varName)
{
    QMutexLocker locker(&m_archivesVariableListMutex);
    QString  varNameTemp= varName;
    varNameTemp=varNameTemp.replace(".","_");
    for(int i=0;i<m_archivesVariableList.count();i++)
    {
        if(0==QString::compare(m_archivesVariableList[i]->name(), varNameTemp,Qt::CaseInsensitive))
        {
            return  m_archivesVariableList[i];
        }
    }
    return NULL;
}
CMDArchivesVariable* CMDArchivesVariableManager::getVariableAt(int index)
{
    QMutexLocker locker(&m_archivesVariableListMutex);
    if(index<m_archivesVariableList.count() && index>=0)
    {
        return  m_archivesVariableList[index];
    }
    return NULL;
}
int CMDArchivesVariableManager::getVariableCount()
{
    QMutexLocker locker(&m_archivesVariableListMutex);
    return m_archivesVariableList.count();
}
void CMDArchivesVariableManager::removeAllVariable()
{
    QMutexLocker locker(&m_archivesVariableListMutex);
    for(int i=0;i<m_archivesVariableList.count();i++)
    {
         delete m_archivesVariableList[i];
    }
    m_archivesVariableList.clear();
}


bool CMDArchivesVariableManager::addPrjInitValueVariable(IMDRTDBVariable *var)
{
    bool ret = false;
    if(var== NULL)
    {
        return ret;
    }
    if(getPrjInitValueVariable(var->name())!= NULL)
    {
        return ret;
    }
    QMutexLocker locker(&m_prjInitValueVariableListMutex);
    CMDArchivesVariable*  archivesVariable =new CMDArchivesVariable(var);
    archivesVariable->setInitVarValueDatabaseConnectName(m_prjInitValueDatabaseConnectName);
    archivesVariable->initVarChangeStorage();
    m_prjInitValueVariableList.append(archivesVariable);
    return true;
}

bool  CMDArchivesVariableManager::deletePrjInitValueVariable(IMDRTDBVariable *var)
{
    bool ret = false;
    if(var== NULL)
    {
        return ret;
    }
    QMutexLocker locker(&m_prjInitValueVariableListMutex);
    QString  varNameTemp= var->name();
    varNameTemp=varNameTemp.replace(".","_");
    for(int i=0;i<m_prjInitValueVariableList.count();i++)
    {
        if(0==QString::compare(m_prjInitValueVariableList[i]->name(), varNameTemp,Qt::CaseInsensitive))
        {
            delete m_prjInitValueVariableList[i];
            m_prjInitValueVariableList.removeAt(i);
            break;
        }
    }
    return true;
}
CMDArchivesVariable*  CMDArchivesVariableManager::getPrjInitValueVariable(const QString & varName)
{
    QMutexLocker locker(&m_prjInitValueVariableListMutex);
    QString  varNameTemp= varName;
    varNameTemp=varNameTemp.replace(".","_");
    for(int i=0;i<m_prjInitValueVariableList.count();i++)
    {
        if(0==QString::compare(m_prjInitValueVariableList[i]->name(), varNameTemp,Qt::CaseInsensitive))
        {
            return  m_prjInitValueVariableList[i];
        }
    }
    return NULL;
}
CMDArchivesVariable* CMDArchivesVariableManager::getPrjInitValueVariableAt(int index)
{
    QMutexLocker locker(&m_prjInitValueVariableListMutex);
    if(index<m_prjInitValueVariableList.count() && index>=0)
    {
        return  m_prjInitValueVariableList[index];
    }
    return NULL;
}
int CMDArchivesVariableManager::getPrjInitValueVariableCount()
{
    QMutexLocker locker(&m_prjInitValueVariableListMutex);
    return m_prjInitValueVariableList.count();
}
void CMDArchivesVariableManager::removeAllPrjInitValueVariable()
{
    QMutexLocker locker(&m_prjInitValueVariableListMutex);
    for(int i=0;i<m_prjInitValueVariableList.count();i++)
    {
         delete m_prjInitValueVariableList[i];
    }
    m_prjInitValueVariableList.clear();
}


/*!
\section 变量存储触发
*/
bool CMDArchivesVariableManager::addArchivesDataCmd(CMDArchivesDataCmd* archivesDataCmd)
{
    bool ret = false;
    if(archivesDataCmd== NULL)
    {
        return ret;
    }
    QMutexLocker locker(&m_archivesDataCmdListMutex);
     m_archivesDataCmdList.append(archivesDataCmd);
     return true;
}
CMDArchivesDataCmd* CMDArchivesVariableManager::removeHeadArchivesDataCmd(int dataAchiveOrInit/* = 0*/)
{
    CMDArchivesDataCmd* archivesDataCmd = NULL;
    QMutexLocker locker(&m_archivesDataCmdListMutex);
    int index = 0;
    while(index < m_archivesDataCmdList.count())
    {
           archivesDataCmd = m_archivesDataCmdList[index];
           if(dataAchiveOrInit == 0)
           {
               // dataChangeAchive
               if(archivesDataCmd->saveRuler() == MDRTDataSaveRuler::DataChanged)
               {
                   if(archivesDataCmd->initMode() != MDRTInitMode::LastValue)
                       m_archivesDataCmdList.removeAt(index);
                   return archivesDataCmd;
               }
           }
           else if(dataAchiveOrInit == 1)
           {
               // prjInitValue
               if(archivesDataCmd->initMode() == MDRTInitMode::LastValue)
               {
                   if(archivesDataCmd->saveRuler() != MDRTDataSaveRuler::DataChanged)
                       m_archivesDataCmdList.removeAt(index);
                   return archivesDataCmd;
               }
           }
           index++;

    }
    return NULL;
}
void CMDArchivesVariableManager::removeAllArchivesDataCmd()
{
    QMutexLocker locker(&m_archivesDataCmdListMutex);
    for(int i=0;i<m_archivesDataCmdList.count();i++)
    {
         delete m_archivesDataCmdList[i];
    }
    m_archivesDataCmdList.clear();
}
void CMDArchivesVariableManager::triggerChangeDataArchives(const QString & varName,const QVariant &value,const MDVariableDataQuality &quality,const QDateTime &timeStamp, MDRTInitMode initMode, MDRTDataSaveRuler saveRuler)
{
    CMDArchivesDataCmd* archivesDataCmd =  new CMDArchivesDataCmd();
    archivesDataCmd->setVarName(varName);
    archivesDataCmd->setValue(value);
    archivesDataCmd->setTimeStamp(timeStamp);
    archivesDataCmd->setQuality(quality);
    archivesDataCmd->setInitMode(initMode);
    archivesDataCmd->setSaveRuler(saveRuler);
    addArchivesDataCmd(archivesDataCmd);
}

void CMDArchivesVariableManager::doChangeDataArchives()
{
    while(m_running)
    {
        CMDArchivesDataCmd* cmd = removeHeadArchivesDataCmd(0);
        if(cmd!=NULL)
        {
            CMDArchivesVariable* var = getVariable(cmd->varName());
            if(var!=NULL)
            {
                var->changeDataArchives(cmd);
                cmd->setSaveRuler(MDRTDataSaveRuler::None);
            }
            if(cmd->initMode() == MDRTInitMode::None && cmd->saveRuler() == MDRTDataSaveRuler::None)
                delete cmd;
        }
        else
        {
            break;
        }
        QThread::msleep(1);//Y2hw
    }
}
void CMDArchivesVariableManager::doCycleDataArchives()
{
    for(int i=0;i<getVariableCount();i++)
    {
        if(!m_running)
        {
            break;
        }
        CMDArchivesVariable* var = getVariableAt(i);
        if(var!=NULL)
        {
            var->cycleDataArchives();
        }
    }
}
void CMDArchivesVariableManager::doChangeDataPrjInitValueArchives()
{
    while(m_running)
    {
        CMDArchivesDataCmd* cmd = removeHeadArchivesDataCmd(1);
        if(cmd!=NULL)
        {
            CMDArchivesVariable* var = getPrjInitValueVariable(cmd->varName());
            if(var!=NULL)
            {
                var->changeDataArchives(cmd);
                cmd->setInitMode(MDRTInitMode::None);
            }
            if(cmd->initMode() == MDRTInitMode::None && cmd->saveRuler() == MDRTDataSaveRuler::None)
                delete cmd;
        }
        else
        {
            break;
        }
        QThread::msleep(1);
    }
}
/*!
\section 变量值操作
*/
//!获取数据
bool CMDArchivesVariableManager::getData(const QString & varName,const QDateTime &startTime,const QDateTime &endTime,CMDDatabaseRecordset *dataRecordset)
{
    CMDArchivesVariable*  var = getVariable(varName);
    if(var == NULL)
    {
        return false;
    }
    return var->getData(startTime,endTime,dataRecordset);
}
bool CMDArchivesVariableManager::getAllData(const QString & varName,CMDDatabaseRecordset *dataRecordset)
 {
    CMDArchivesVariable*  var = getVariable(varName);
    if(var == NULL)
    {
        return false;
    }
    return var->getAllData(dataRecordset);
}
//!删除数据
void CMDArchivesVariableManager::deleteData(const QString & varName,const QDateTime &startTime)
{
    CMDArchivesVariable*  var = getVariable(varName);
    if(var == NULL)
    {
        return;
    }
    var->deleteData(startTime);
}
void CMDArchivesVariableManager::deleteData(const QString & varName,const QDateTime &startTime,const QDateTime &endTime)
{
    CMDArchivesVariable*  var = getVariable(varName);
    if(var == NULL)
    {
        return;
    }
    var->deleteData(startTime,endTime);
}
void CMDArchivesVariableManager::deleteAllData(const QString & varName)
{
    CMDArchivesVariable*  var = getVariable(varName);
    if(var == NULL)
    {
        return;
    }
    var->deleteAllData();
}

