/*!

\author bosmutus

\section 功能

自定义数据查询专家实现类

*/
#ifndef DBQUERYEXPERTIMP_H
#define DBQUERYEXPERTIMP_H

#include <map>
#include <QVariant>
#include "mddbqueryexpert_global.h"

class CDBConnectionTemp;
class CMDDatabaseRecordset;
class CMDDBQueryExpertImp
{
public:
    typedef std::map<QString, CDBConnectionTemp *> ConnMap;
    typedef std::map<QString, CMDDatabaseRecordset *> RecordsetMap;
public:
    CMDDBQueryExpertImp();
    virtual ~CMDDBQueryExpertImp();

public:
    void addDataBaseConnection(const QString& dbConName);
    int connectDataBase(const QString& dbConName, const QVariant& serverProperty, const QString& dbName);
    int disconnectDataBase(const QString& dbConName);
    int deleteDataBaseConnection(const QString& dbConName);

    int excuteSQLCommand(const QString& dbConName, const QString& sqlCmd);
    QString getLastError(const QString& dbConName);
    void addRecordSet(const QString& recordSetName);
    int updateRecordSetData(const QString& dbConName, const QString& recordSetName, const QString& sqlCmd);
    int getRecordSetRowCount(const QString& recordSetName);
    int getRecordSetColumnCount(const QString& recordSetName);
    QVariant getRecordSetValue(const QString& recordSetName, int row, int col);
    QVariant getRecordSetValue(const QString& recordSetName, int row, const QString& fieldName);
    int deleteRecordSet(const QString& recordSetName);

protected:
    ConnMap m_connMap;
    RecordsetMap m_recordsetMap;

private:
    QString m_strLastError;
    CDBConnectionTemp *containsConnection(const QString& dbConName);
    CMDDatabaseRecordset *containsRecordset(const QString& recordSetName);
    int DBTye_FromString(const QString & DBTypeString);
};

#endif // DBQUERYEXPERTIMP_H
