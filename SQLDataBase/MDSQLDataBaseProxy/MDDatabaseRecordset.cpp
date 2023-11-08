#include "MDDatabaseRecordset.h"
#include <QtSql>
#include "MDDatabaseRecordsetPrivate.h"
CMDDatabaseRecordset::CMDDatabaseRecordset()
{
    m_recordsetPrivate = new CMDDatabaseRecordsetPrivate();
}

CMDDatabaseRecordset::~CMDDatabaseRecordset()
{
    if(NULL != m_recordsetPrivate)
    {
//        m_recordsetPrivate->clear(); 析构中已经有clear的调用
        delete m_recordsetPrivate;
        m_recordsetPrivate = NULL;
    }
}

QString CMDDatabaseRecordset::name()
{
    return m_recordsetPrivate->name();
}

void CMDDatabaseRecordset::setName(const QString & rsName)
{
    m_recordsetPrivate->setName(rsName);
}

QString CMDDatabaseRecordset::id()
{
    return m_recordsetPrivate->id();
}

void CMDDatabaseRecordset::setID(const QString & rsID)
{
    m_recordsetPrivate->setID(rsID);
}

void CMDDatabaseRecordset::bindFileds(const QStringList &fieldNameList)
{
   return m_recordsetPrivate->bindFileds(fieldNameList);
}
int CMDDatabaseRecordset::addRow()
{
   return m_recordsetPrivate->addRow();
}
bool CMDDatabaseRecordset::deleteRow(int rowIndex)
{
   return m_recordsetPrivate->deleteRow(rowIndex);
}
bool CMDDatabaseRecordset::setValue(int row,int col,const QVariant &data)
{
   return m_recordsetPrivate->setValue(row,col,data);
}
bool CMDDatabaseRecordset::setValue(int row,const QString &fieldName,const QVariant &data)
{
   return m_recordsetPrivate->setValue(row,fieldName,data);
}

QString CMDDatabaseRecordset::getFiledName(int index)
{
    return m_recordsetPrivate->getFiledName(index);
}
int CMDDatabaseRecordset::getFiledIndex(const QString &name)
{
    return m_recordsetPrivate->getFiledIndex(name);
}
int CMDDatabaseRecordset::rowCount()
{
   return m_recordsetPrivate->rowCount();
}
int CMDDatabaseRecordset::columnCount()
{
   return m_recordsetPrivate->columnCount();
}
void CMDDatabaseRecordset::moveTo(int index)
{
    m_recordsetPrivate->moveTo(index);
}
void CMDDatabaseRecordset::moveNext()
{
   m_recordsetPrivate->moveNext();
}
void CMDDatabaseRecordset::movePrevious()
{
     m_recordsetPrivate->movePrevious();
}
void CMDDatabaseRecordset::moveFirst()
{
     m_recordsetPrivate->moveFirst();
}
void CMDDatabaseRecordset::moveLast()
{
     m_recordsetPrivate->moveLast();
}
bool CMDDatabaseRecordset::isFirst()
{
    return  m_recordsetPrivate->isFirst();
}
bool CMDDatabaseRecordset::isLast()
{
    return  m_recordsetPrivate->isLast();
}
void CMDDatabaseRecordset::clear()
{
    m_recordsetPrivate->clear();
}
QVariant CMDDatabaseRecordset::value(int row,int col)
{

   return m_recordsetPrivate->value(row,col);
}
QVariant CMDDatabaseRecordset::value(int row,const QString &fieldName)
{
   return m_recordsetPrivate->value(row,fieldName);
}
QVariant CMDDatabaseRecordset::value(int col)
{
    return m_recordsetPrivate->value(col);
}
QVariant CMDDatabaseRecordset::value(const QString &fieldName)
{
   return m_recordsetPrivate->value(fieldName);
}
void CMDDatabaseRecordset::bindSqlQuery(void *sqlQuery)
{
    m_recordsetPrivate->bindSqlQuery(sqlQuery);
}
