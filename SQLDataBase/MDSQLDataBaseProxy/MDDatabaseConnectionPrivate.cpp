/*
#include "MDDatabaseConnectionPrivate.h"
#include <QtSql>
CMDDatabaseConnectionPrivate::CMDDatabaseConnectionPrivate()
{
        m_name = "";
        m_databaseParameter= "";
}

CMDDatabaseConnectionPrivate::~CMDDatabaseConnectionPrivate()
{
     disConnect();
}
void * CMDDatabaseConnectionPrivate::sqlDatabase()
{
        QSqlDatabase db = QSqlDatabase::database(m_name);
        if(db.isValid())
        {
             return &db;
        }
        return NULL;
}
QString CMDDatabaseConnectionPrivate::name()
{
     return m_name;
}
void CMDDatabaseConnectionPrivate::setName(const QString & name)
{
      m_name = name;
      m_name.trimmed();
}
QString CMDDatabaseConnectionPrivate::databaseParameter()
{
     return m_databaseParameter;
}
void CMDDatabaseConnectionPrivate::setDatabaseParameter(const QString & dbParameter)
{
       m_databaseParameter = dbParameter;
       m_databaseParameter.trimmed();
}
void CMDDatabaseConnectionPrivate::disConnect()
{
     bool bDisConnect = false;
     {
         QSqlDatabase db = QSqlDatabase::database(m_name);
         if(db.isValid())
         {
             if(db.isOpen())
             {
                 db.close();
             }
         }
         bDisConnect =true;
     }
     QSqlDatabase::removeDatabase(m_name);
}
void CMDDatabaseConnectionPrivate::connect()
{
    if(m_name.isEmpty() ||m_name.isNull())
    {
        return;
    }
   // "DBType = QSQLITE;"

   //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",m_name);
   //db.setDatabaseName(":memory:");     //创建数据库
   //if(!db.open())
}
*/
