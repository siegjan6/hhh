/*!
\author enli 2015.01.08
\section 归档变量类
\创建步骤：
\非动态：1.CMDArchivesVariableManager::initial()
\或
\动态。目前不支持，直接存储到Redis：1.CMDRTDBManager::dynamicAddDeviceVariable
\2.CMDVariableArchivesManager::dynamicAddVariable
\3.CMDArchivesVariableManager::addVariable(IMDRTDBVariable *var)

\存储：
\1.CMDVariableArchivesManager->CMDVariableArchivesEngine->CMDVariableArchivesThread:
            \m_archivesVariableManager->doCycleDataArchives();
            \m_archivesVariableManager->doChangeDataArchives()：
                \CMDRTDBVariable::refreshData->triggerChangeDataArchives->添加到列表
*/
#ifndef MDARCHIVESVARIABLE_H
#define MDARCHIVESVARIABLE_H

#include <QDateTime>
#include <qvariant.h>
#include <QtSql>
#include "IMDDataType.h"
#include "IMDRTDBVariable.h"
#include "MDArchivesDataCmd.h"
#include "MDDatabaseRecordset.h"
class CMDArchivesVariable
{
public:
    CMDArchivesVariable();
    CMDArchivesVariable(IMDRTDBVariable *var);
    ~CMDArchivesVariable();
public:
    void setVariable(IMDRTDBVariable *var);
    //!变量名称(全名）：GroupName.LocalName
    QString name();
    void setName(const QString &name);
    //!数据类型
    MDDataType dataType();
    void setDataType(MDDataType dataType);
    //!数据存储规则
    MDRTDataSaveRulerParameter dataSaveRulerParameter();
    void setDataSaveRulerParameter(MDRTDataSaveRulerParameter parameter);

private:
    QString m_name;
    MDDataType m_dataType;
    MDRTDataSaveRulerParameter m_dataSaveRulerParameter;
    MDRTInitMode m_initType;
    IMDRTDBVariable *m_mdVariable;
public:
    //!创建变量表
    void initial();
    //! 初始化变量变化存储sqllite数据库
    void initVarChangeStorage();
    void release();
    //!数据库连接名
    void setDatabaseConnectName(const QString &databaseConnectName);
    void setInitVarValueDatabaseConnectName(const QString &databaseConnectName);
    //!周期存储
    void cycleDataArchives();
    //!变化存储
    void changeDataArchives(CMDArchivesDataCmd* cmd);
private:
    QTime m_time;
    QDateTime m_startDeleteDateTime;
    //!数据库连接名: MDArchivesVariable+年月日时分秒
    QString m_databaseConnectName;
    //!数据库连接名: MDPrjInitValue+年月日时分秒
    QString m_initVarValueDatabaseConnectName;
    //!数据库
    QSqlDatabase m_db ;
    QMutex m_objMutex;

    //! 变量变化存储sqllite数据库connection
    QSqlDatabase m_varChangeInitValueDb;

public:
    //!添加数据
    bool addData(const QVariant &value,const MDVariableDataQuality &quality,const QDateTime &timeStamp);
    bool addVarChangeInitValueData(const QVariant &value,const MDVariableDataQuality &quality,const QDateTime &timeStamp);
    //!删除数据
    void deleteData(const QDateTime &startTime);
    void deleteData(const QDateTime &startTime,const QDateTime &endTime);
    void deleteAllData();
    //获取数据
    bool getData(const QDateTime &startTime,const QDateTime &endTime,CMDDatabaseRecordset *dataRecordset);
    bool getAllData(CMDDatabaseRecordset *dataRecordset);
};

#endif // MDARCHIVESVARIABLE_H
