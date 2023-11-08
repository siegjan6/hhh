/*!

\author bosmutus

\section 功能

自定义查询专家实现类

*/
#ifndef DBQUERYEXPERT_H
#define DBQUERYEXPERT_H

#include <QVariant>
#include "mddbqueryexpert_global.h"

class CMDDBQueryExpertImp;
class MDDBQUERYEXPERTSHARED_EXPORT CMDDBQueryExpert
{
public:
    CMDDBQueryExpert();
    virtual ~CMDDBQueryExpert();

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
    QVariant getRecordSetValue(const QString& recordSetName, int row, const QString&  fieldName);
    int deleteRecordSet(const QString& recordSetName);

protected:
    CMDDBQueryExpertImp* m_imp;
};

#endif // DBQUERYEXPERT_H
