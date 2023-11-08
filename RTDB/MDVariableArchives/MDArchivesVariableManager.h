/*!
\author enli 2015.01.08
\section 归档变量管理类
*/
#ifndef MDARCHIVESVARIABLEMANAGER_H
#define MDARCHIVESVARIABLEMANAGER_H
#include <QDateTime>
#include <QMutex>
#include "IMDVariable.h"
#include "MDArchivesVariable.h"
#include "IMDRTDBManager.h"
#include "MDArchivesDataCmd.h"
#include "MDSwitchFactory/MDFuncSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"
class CMDDatabaseRecordset;
class CMDArchivesVariableManager
{
public:
    CMDArchivesVariableManager();
public:
    void setRTDBManager(IMDRTDBManager *nsRTDBManager);
    //!变化存储数据
    void triggerChangeDataArchives(const QString & varName,const QVariant &value,const MDVariableDataQuality &quality,const QDateTime &timeStamp, MDRTInitMode initMode, MDRTDataSaveRuler saveRuler);
    //!获取变量数据： dataRecordset包含两个字：RecordTime、Value
    bool getData(const QString & varName,const QDateTime &startTime,const QDateTime &endTime,CMDDatabaseRecordset *dataRecordset);
    bool getAllData(const QString & varName,CMDDatabaseRecordset *dataRecordset);
    //!删除数据
    void deleteData(const QString & varName,const QDateTime &startTime);
    void deleteData(const QString & varName,const QDateTime &startTime,const QDateTime &endTime);
    void deleteAllData(const QString & varName);

    QString getPrjInitValue(const QString& varName);
private:
    IMDRTDBManager *m_mdRTDBManager;
public:
    //!创建数据库+从RTDBManager获取归档变量
    void initial();
    void release();
    void start();
    void stop();
    bool isRunning();
    //!处理每个变量的周期或变化存储
    void doCycleDataArchives();
    void doChangeDataArchives();
    //!变量项目保存值存储
    void doChangeDataPrjInitValueArchives();

    bool addVariable(IMDRTDBVariable *var);
    bool deleteVariable(IMDRTDBVariable *var);
    CMDArchivesVariable* getVariable(const QString & varName);
    CMDArchivesVariable* getVariableAt(int index);
    int getVariableCount();
    void removeAllVariable();

    bool addPrjInitValueVariable(IMDRTDBVariable *var);
    bool deletePrjInitValueVariable(IMDRTDBVariable *var);
    CMDArchivesVariable* getPrjInitValueVariable(const QString & varName);
    CMDArchivesVariable* getPrjInitValueVariableAt(int index);
    int getPrjInitValueVariableCount();
    void removeAllPrjInitValueVariable();

    /*!
    \section 添加
    */
    bool addArchivesDataCmd(CMDArchivesDataCmd* archivesDataCmd);
    // dataAchiveOrInit
    // 0 - DataAchive
    // 1 - init
    CMDArchivesDataCmd* removeHeadArchivesDataCmd(int dataAchiveOrInit = 0);
    void removeAllArchivesDataCmd();

    /*!
    \section 日志
    */
    CMDLogSwitch* m_logSwitch;
private:
    //!归档变量链表
    QList<CMDArchivesVariable*> m_archivesVariableList;
    QMutex m_archivesVariableListMutex;
    //!变量数据变化归档命令链表
    QList<CMDArchivesDataCmd*> m_archivesDataCmdList;
    QMutex m_archivesDataCmdListMutex;

    QString  m_databaseConnectName;
    bool m_running;

    QList<CMDArchivesVariable*> m_prjInitValueVariableList;
    QMutex m_prjInitValueVariableListMutex;
    QString  m_prjInitValueDatabaseConnectName;
    QMap<QString,QString> m_prjInitValueMap;

    bool getPrjInitValuesFromDB();

};

#endif // MDARCHIVESVARIABLEMANAGER_H
