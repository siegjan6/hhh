#include "MDVariableArchivesManager.h"


CMDVariableArchivesManager::CMDVariableArchivesManager()
{
}
CMDVariableArchivesManager::~CMDVariableArchivesManager()
{
    m_archivesVariableManager.release();
}
void CMDVariableArchivesManager::setRTDBManager(IMDRTDBManager *nsRTDBManager)
{
    m_archivesVariableManager.setRTDBManager (nsRTDBManager);
}
void CMDVariableArchivesManager::start()
{
    if(m_archivesVariableManager.isRunning())//Y2hw
        return;

    m_archivesVariableManager.initial();
    m_archivesVariableManager.start();
    m_variableArchivesEngine.start(&m_archivesVariableManager);
}
void CMDVariableArchivesManager::stop()
{
    m_archivesVariableManager.stop();
    m_variableArchivesEngine.stop();
    m_archivesVariableManager.release();
}
void CMDVariableArchivesManager::dynamicAddVariable(IMDRTDBVariable *var)
{
    if(m_variableArchivesEngine.isRunning())
    {
         m_variableArchivesEngine.stop();
         m_archivesVariableManager.addVariable (var);
         m_variableArchivesEngine.start(&m_archivesVariableManager);
    }
    else
    {
        m_archivesVariableManager.addVariable (var);
    }
}

void CMDVariableArchivesManager::dynamicDeleteVariable(IMDRTDBVariable *var)
{
    if(m_variableArchivesEngine.isRunning())
    {
         m_variableArchivesEngine.stop();
         m_archivesVariableManager.deleteVariable (var);
         m_variableArchivesEngine.start(&m_archivesVariableManager);
    }
    else
    {
        m_archivesVariableManager.deleteVariable (var);
    }
}


void CMDVariableArchivesManager::triggerChangeDataArchives(const QString & varName,const QVariant &value,const MDVariableDataQuality &quality,const QDateTime &timeStamp, MDRTInitMode initMode, MDRTDataSaveRuler saveRuler)
{
    m_archivesVariableManager.triggerChangeDataArchives(varName,value,quality,timeStamp,initMode,saveRuler);
}

  //获取数据
bool CMDVariableArchivesManager::getData(const QString & varName,const QDateTime &startTime,const QDateTime &endTime,CMDDatabaseRecordset  *dataRecordset)
{
       return m_archivesVariableManager.getData(varName,startTime,endTime,dataRecordset);
}
bool CMDVariableArchivesManager::getAllData(const QString & varName,CMDDatabaseRecordset  *dataRecordset)
{
    return m_archivesVariableManager.getAllData(varName,dataRecordset);
}
//删除数据
void CMDVariableArchivesManager::deleteData(const QString & varName,const QDateTime &startTime)
{
    m_archivesVariableManager.deleteData(varName,startTime);
}
void CMDVariableArchivesManager::deleteData(const QString & varName,const QDateTime &startTime,const QDateTime &endTime)
{
     m_archivesVariableManager.deleteData(varName,startTime,endTime);
}
void CMDVariableArchivesManager::deleteAllData(const QString & varName)
{
    m_archivesVariableManager.deleteAllData(varName);
}

QString CMDVariableArchivesManager::getPrjInitValue(const QString& varName)
{
    return m_archivesVariableManager.getPrjInitValue(varName);
}
