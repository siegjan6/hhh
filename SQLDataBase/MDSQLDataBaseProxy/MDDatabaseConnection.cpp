/*
#include "MDDatabaseConnection.h"
#include "MDDatabaseConnectionPrivate.h"
CMDDatabaseConnection::CMDDatabaseConnection()
{
    m_dbConcectionPrivate = new CMDDatabaseConnectionPrivate();
}
CMDDatabaseConnection::~CMDDatabaseConnection()
{
    disConnect();
    delete m_dbConcectionPrivate;
}
 void * CMDDatabaseConnection::sqlDatabase()
 {
      return m_dbConcectionPrivate->sqlDatabase();
 }
 QString CMDDatabaseConnection::name()
 {
      return m_dbConcectionPrivate->name();
 }
 void CMDDatabaseConnection::setName(const QString & name)
 {
       m_dbConcectionPrivate->setName(name);
 }
 QString CMDDatabaseConnection::databaseParameter()
 {
      return m_dbConcectionPrivate->databaseParameter();
 }
 void CMDDatabaseConnection::setDatabaseParameter(const QString & dbParameter)
 {
       m_dbConcectionPrivate->setDatabaseParameter(dbParameter);
 }
 void CMDDatabaseConnection::disConnect()
 {
      m_dbConcectionPrivate->disConnect();
 }
 void CMDDatabaseConnection::connect()
 {
      m_dbConcectionPrivate->connect();
 }
*/
