#include "MDDatabaseRecordsetPrivate.h"
#include <QtSql>
CMDDatabaseRecordsetPrivate::CMDDatabaseRecordsetPrivate()
{
    m_isValid = false;
    m_rowCount=0;
}

CMDDatabaseRecordsetPrivate::~CMDDatabaseRecordsetPrivate()
{
    clear();
}

bool CMDDatabaseRecordsetPrivate::isValid()
{
    return m_isValid;
}

QString CMDDatabaseRecordsetPrivate::name()
{
    return m_name;
}

void CMDDatabaseRecordsetPrivate::setName(const QString & rsName)
{
    m_name = rsName;
}

QString CMDDatabaseRecordsetPrivate::id()
{
    return m_id;
}

void CMDDatabaseRecordsetPrivate::setID(const QString & rsID)
{
    m_id = rsID;
}

QString CMDDatabaseRecordsetPrivate::getFiledName(int index)
{
    return m_mdFields.getFiledName(index);
}
int CMDDatabaseRecordsetPrivate::getFiledIndex(const QString &name)
{
    return m_mdFields.getFiledIndex(name);
}

void CMDDatabaseRecordsetPrivate::bindFileds(const QStringList &fieldNameList)
{
    clear();
    int filedCount = fieldNameList.count();
    for(int i=0;i<filedCount;i++)
    {
       m_mdFields.addFiled(fieldNameList[i]);
    }
    if(m_mdFields.size()<=0 ||m_mdFields.size()!= filedCount)
    {
        return;
    }
    m_isValid = true;
    m_columnCount = filedCount;

}
int CMDDatabaseRecordsetPrivate::addRow()
{
    CMDRowData* rowData = new CMDRowData(m_columnCount);
    m_rowDataList.append(rowData);
    m_rowCount = m_rowCount+1;
    return m_rowDataList.count()-1;
}
bool CMDDatabaseRecordsetPrivate::deleteRow(int index)
{
    if(index<m_rowCount && index>=0)
    {
        CMDRowData* rowData = m_rowDataList[index];
        m_rowDataList.removeAt(index);
        delete rowData;
        m_rowCount = m_rowCount-1;
        return true;
    }
    return false;
}
bool CMDDatabaseRecordsetPrivate::setValue(int row,int col,const QVariant &data)
{
    if(row<0||row>=m_rowCount||
       col<0||col>=m_columnCount)
    {        
        return false;
    }

    m_rowDataList[row]->setValue(col,data);
    return true;
}
bool CMDDatabaseRecordsetPrivate::setValue(int row,const QString &fieldName,const QVariant &data)
{
    int col = m_mdFields.getFiledIndex(fieldName);

    if(row<0||row>=m_rowCount||
       col<0||col>=m_columnCount)
    {        
        return false;
    }

    m_rowDataList[row]->setValue(col,data);
    return true;
}

int CMDDatabaseRecordsetPrivate::rowCount()
{
   return m_rowCount;
}
int CMDDatabaseRecordsetPrivate::columnCount()
{
   return m_columnCount;
}
void CMDDatabaseRecordsetPrivate::moveTo(int index)
{
    if(index<m_rowCount && index>=0)
    {
        m_curPos = index;
    }
}
void CMDDatabaseRecordsetPrivate::moveNext()
{
    if(m_curPos<m_rowCount-1 && m_curPos>=0)
    {
        m_curPos = m_curPos+1;
    }
}
void CMDDatabaseRecordsetPrivate::movePrevious()
{
    if(m_curPos<m_rowCount && m_curPos>0)
    {
        m_curPos = m_curPos-1;
    }
}
void CMDDatabaseRecordsetPrivate::moveFirst()
{
    m_curPos = 0;
}
void CMDDatabaseRecordsetPrivate::moveLast()
{
    if(m_rowCount>0)
    {
       m_curPos =m_rowCount -1;
    }
}
bool CMDDatabaseRecordsetPrivate::isFirst()
{
    if(m_curPos<=0)
    {
       return true;
    }
    return false;
}
bool CMDDatabaseRecordsetPrivate::isLast()
{
    if(m_curPos>=0 && m_curPos==m_rowCount-1)
    {
       return true;
    }
    return false;
}
void CMDDatabaseRecordsetPrivate::clear()
{
    for(int i=0;i<m_rowCount;i++)
    {
        delete m_rowDataList[i];
    }
    m_rowDataList.clear();
    m_mdFields.deleteAllFiled();
    m_curPos = 0;
    m_rowCount = 0;
    m_columnCount = 0;
    m_isValid = false;
}
QVariant CMDDatabaseRecordsetPrivate::value(int row,int col)
{
    return m_rowDataList[row]->valueEx(col);
    /*/
    if(row<0||row>=m_rowCount||
       col<0||col>=m_columnCount)
    {
        return m_rowDataList[row]->valueEx(col);
    }

   return QVariant();
   /*/
}
QVariant CMDDatabaseRecordsetPrivate::value(int row,const QString &fieldName)
{
    int col = m_mdFields.getFiledIndex(fieldName);
    return m_rowDataList[row]->valueEx(col);
    /*/
    if(row<0||row>=m_rowCount||
       col<0||col>=m_columnCount)
    {
        return m_rowDataList[row]->valueEx(col);
    }

    return QVariant();
    /*/
}
QVariant CMDDatabaseRecordsetPrivate::value(int col)
{
    return m_rowDataList[m_curPos]->valueEx(col);
    /*/
    if(m_curPos<0||m_curPos>=m_rowCount||
       col<0||col>=m_columnCount)
    {
        return m_rowDataList[m_curPos]->valueEx(col);
    }
    return QVariant();
     /*/
}
QVariant CMDDatabaseRecordsetPrivate::value(const QString &fieldName)
{
    int col = m_mdFields.getFiledIndex(fieldName);
    return m_rowDataList[m_curPos]->valueEx(col);
    /*/
    if(m_curPos<0||m_curPos>=m_rowCount||
       col<0||col>=m_columnCount)
    {
        return m_rowDataList[row]->valueEx(col);
    }

   return QVariant();
   /*/
}
void CMDDatabaseRecordsetPrivate::bindSqlQuery(void *sqlQuery)
{
    clear();
    if(sqlQuery == NULL)
    {
        return;
    }
    QSqlQuery *query = (QSqlQuery *)sqlQuery;
    QSqlRecord sqlRecord = query->record();
    int filedCount = sqlRecord.count();
    for(int i=0;i<filedCount;i++)
    {
       QString fieldName= sqlRecord.fieldName(i);
       m_mdFields.addFiled(fieldName);
    }
    if(m_mdFields.size()<=0 ||m_mdFields.size()!= filedCount)
    {
        return;
    }
    query->seek(-1);
    while(query->next())
    {
        CMDRowData* rowData = new CMDRowData(filedCount);
        for(int n=0;n<filedCount;n++)
        {
           rowData->setValue(n,query->value(n));
        }
        m_rowDataList.append(rowData);
    }
    m_isValid = true;
    m_rowCount = m_rowDataList.count();
    m_columnCount = filedCount;
}
