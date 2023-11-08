/*
#ifndef MDDATABASECONNECTION_H
#define MDDATABASECONNECTION_H

#include "nssqldatabaseproxy_global.h"
#include <QVariant>
class CMDDatabaseConnectionPrivate;
class MDSQLDATABASEPROXYSHARED_EXPORT CMDDatabaseConnection
{
public:
    CMDDatabaseConnection();
    ~CMDDatabaseConnection();
public:
    QString name();
    void setName(const QString & name);
    QString databaseParameter();
    void setDatabaseParameter(const QString & dbParameter);
    void  connect();
    void disConnect();
    //QSqlDatabase
     void * sqlDatabase();
private:
     CMDDatabaseConnectionPrivate *m_dbConcectionPrivate;
};

#endif // MDDATABASECONNECTION_H
*/
