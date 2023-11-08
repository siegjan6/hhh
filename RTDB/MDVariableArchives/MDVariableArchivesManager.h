/*!
\author enli 2015.01.08
\section 整个数据归档管理类
*/
#ifndef MDVARIABLEARCHIVESMANAGER_H
#define MDVARIABLEARCHIVESMANAGER_H

#include "mdvariablearchives_global.h"

#include "IMDVariableArchivesManager.h"
#include "MDVariableArchivesEngine.h"
#include "MDArchivesVariableManager.h"

class CMDVariableArchivesManager:public IMDVariableArchivesManager
{
public:
    CMDVariableArchivesManager();
    ~CMDVariableArchivesManager();
public:
    void setRTDBManager(IMDRTDBManager *nsRTDBManager);
    /*!
    \section 数据归档开始/结束（停止线程）
    */
    void start();
    void stop();
public:
    //!存储数据
    void triggerChangeDataArchives(const QString & varName,const QVariant &value,const MDVariableDataQuality &quality,const QDateTime &timeStamp, MDRTInitMode initMode, MDRTDataSaveRuler saveRuler);
    //!获取变量数据： dataRecordset包含两个字：RecordTime、Value
    bool getData(const QString & varName,const QDateTime &startTime,const QDateTime &endTime,CMDDatabaseRecordset *dataRecordset);
    bool getAllData(const QString & varName,CMDDatabaseRecordset*dataRecordset);
    //!删除数据
    void deleteData(const QString & varName,const QDateTime &startTime);
    void deleteData(const QString & varName,const QDateTime &startTime,const QDateTime &endTime);
    void deleteAllData(const QString & varName);

    QString getPrjInitValue(const QString& varName);
private:
    CMDVariableArchivesEngine m_variableArchivesEngine;
    CMDArchivesVariableManager  m_archivesVariableManager ;

/*!
\section 运行时动态添加设备
*/
public:
    /*!
     动态添加归档变量
     \param var 归档变量
     \returns 无。
     \note
     */
    void dynamicAddVariable(IMDRTDBVariable *var);
    /*!
     动态删除归档变量
     \param var 归档变量
     \returns 无。
     \note
     */
    void dynamicDeleteVariable(IMDRTDBVariable *var);
};

#endif // MDVARIABLEARCHIVESMANAGER_H
