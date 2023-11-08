#ifndef IMDVARIABLEARCHIVESMANAGER_H
#define IMDVARIABLEARCHIVESMANAGER_H

#include "../../include/MDRTDBInterfaces/IMDVariable.h"
class IMDRTDBManager;
class IMDRTDBVariable;
class CMDDatabaseRecordset;
class IMDVariableArchivesManager
{
public:
    virtual ~IMDVariableArchivesManager(){}
public:
    virtual void setRTDBManager(IMDRTDBManager *nsRTDBManager)= 0;
    virtual void start()= 0;
    virtual void stop()= 0;
    //存储变量数据
    virtual void triggerChangeDataArchives(const QString & varName,const QVariant &value,const MDVariableDataQuality &quality,const QDateTime &timeStamp, MDRTInitMode initMode, MDRTDataSaveRuler saveRuler)= 0;
    //获取变量数据： dataRecordset包含两个字：RecordTime、Value
    virtual bool getData(const QString & varName,const QDateTime &startTime,const QDateTime &endTime,CMDDatabaseRecordset *dataRecordset)= 0;
    virtual bool getAllData(const QString & varName,CMDDatabaseRecordset  *dataRecordset)= 0;
    //删除变量数据
    virtual void deleteData(const QString & varName,const QDateTime &startTime)= 0;
    virtual void deleteData(const QString & varName,const QDateTime &startTime,const QDateTime &endTime)= 0;
    virtual void deleteAllData(const QString & varName)= 0;

    virtual QString getPrjInitValue(const QString & varName) = 0;
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
    virtual void dynamicAddVariable(IMDRTDBVariable *var)= 0;
    /*!
     动态删除归档变量
     \param var 归档变量
     \returns 无。
     \note
     */
    virtual void dynamicDeleteVariable(IMDRTDBVariable *var)= 0;

};
#endif // IMDVARIABLEARCHIVESMANAGER_H
