#include "dbqueryexpert.h"
#include "dbqueryexpertimp.h"

CMDDBQueryExpert::CMDDBQueryExpert()
{
    m_imp = new CMDDBQueryExpertImp;
}

 CMDDBQueryExpert::~CMDDBQueryExpert()
 {
     if(m_imp)
         delete m_imp;
 }

void CMDDBQueryExpert::addDataBaseConnection(const QString& dbConName)
{
    if(m_imp)
        m_imp->addDataBaseConnection(dbConName);
}
int CMDDBQueryExpert::connectDataBase(const QString& dbConName, const QVariant& serverProperty, const QString& dbName)
{
    return ( m_imp ? m_imp->connectDataBase(dbConName, serverProperty, dbName) : -1);
}
int CMDDBQueryExpert::disconnectDataBase(const QString& dbConName)
{
    return m_imp ? m_imp->disconnectDataBase(dbConName) : -1;
}
int CMDDBQueryExpert::deleteDataBaseConnection(const QString& dbConName)
{
    return m_imp ? m_imp->deleteDataBaseConnection(dbConName) : -1;
}
int CMDDBQueryExpert::excuteSQLCommand(const QString& dbConName, const QString& sqlCmd)
{
    return m_imp ? m_imp->excuteSQLCommand(dbConName, sqlCmd) : -1;
}
QString CMDDBQueryExpert::getLastError(const QString& dbConName)
{
    return m_imp ? m_imp->getLastError(dbConName) : "";
}
void CMDDBQueryExpert::addRecordSet(const QString& recordSetName)
{
    m_imp->addRecordSet(recordSetName);
}
int CMDDBQueryExpert::updateRecordSetData(const QString& dbConName, const QString& recordSetName, const QString& sqlCmd)
{
    return m_imp ? m_imp->updateRecordSetData(dbConName, recordSetName, sqlCmd) : -1;
}
int CMDDBQueryExpert::getRecordSetRowCount(const QString& recordSetName)
{
    return m_imp ? m_imp->getRecordSetRowCount(recordSetName) : -1;
}
int CMDDBQueryExpert::getRecordSetColumnCount(const QString& recordSetName)
{
    return m_imp ? m_imp->getRecordSetColumnCount(recordSetName) : -1;
}
QVariant CMDDBQueryExpert::getRecordSetValue(const QString& recordSetName, int row, int col)
{
    return m_imp ? m_imp->getRecordSetValue(recordSetName, row, col) : NULL;
}
QVariant CMDDBQueryExpert::getRecordSetValue(const QString& recordSetName, int row, const QString&  fieldName)
{
    return m_imp ? m_imp->getRecordSetValue(recordSetName, row, fieldName) : NULL;
}
int CMDDBQueryExpert::deleteRecordSet(const QString& recordSetName)
{
    return m_imp ? m_imp->deleteRecordSet(recordSetName) : -1;
}
