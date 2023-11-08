#include "XMLDataSourcesStorage.h"
#include "JsonDataSourcesStorage.h"
#include "utils.h"
#include <QMutex>

static QMutex localMutex;

namespace DBQ
{
DBQUERYCONFIGURE_EXPORT bool DataSource_Load(const QString & fileName, FILE_TYPE fileType, QList<DBSource *> & dbSourcesOut, QString & errOut)
{
    QMutexLocker locker(&localMutex);

    if(FILE_TYPE_XML == fileType)
    {
        XMLDataSourcesStorage storage;
        return storage.Load(fileName, dbSourcesOut, errOut);
    }else if(FILE_TYPE_JSON == fileType)
    {
        JsonDataSourcesStorage storage;
        return storage.Load(fileName, dbSourcesOut, errOut);
    }

    WriteInfoLog("LoadDataSources:fileType error");
    return false;
}

DBQUERYCONFIGURE_EXPORT bool DataSource_Write(const QString & fileName, FILE_TYPE fileType, QList<DBSource *> & dbSrcsIn, QString & errStrOut)
{
    QMutexLocker locker(&localMutex);

    if(FILE_TYPE_XML == fileType)
    {
        XMLDataSourcesStorage storage;
        return storage.Save(fileName, dbSrcsIn, errStrOut);
    }else if(FILE_TYPE_JSON == fileType)
    {
        JsonDataSourcesStorage storage;
        return storage.Save(fileName, dbSrcsIn, errStrOut);
    }

    WriteInfoLog("WriteDataSourcesToFile:fileType error");
    return false;
}

DBQUERYCONFIGURE_EXPORT DBSOURCE_TYPE DataSource_GetDBType(const QString & dbTypeStr)
{
    if(dbTypeStr.compare("MYSQL", Qt::CaseInsensitive) == 0)
        return  DBSOURCE_TYPE_MYSQL;
    else if(dbTypeStr.compare("SQLSERVER", Qt::CaseInsensitive) == 0)
        return  DBSOURCE_TYPE_SQL_SERVER;
    else if(dbTypeStr.compare("ORACLE", Qt::CaseInsensitive) == 0)
        return  DBSOURCE_TYPE_ORACLE;
    else if(dbTypeStr.compare("KINGBASE", Qt::CaseInsensitive) == 0)
        return  DBSOURCE_TYPE_KINGBASE;

    return DBSOURCE_TYPE_UNKNOWN;
}

DBQUERYCONFIGURE_EXPORT bool DBSource_GetNames(const QString & viewPath, QStringList & dbsourceNamesOut)
{
    //不能加锁QMutexLocker locker(&localMutex) 因为LoadDataSources已加锁。如果加锁，在Linux平台下会死锁。
    QString errStr;
    QList<DBQ::DBSource*> dbsourcesList;
    QString path = viewPath;
    bool ok = DataSource_Load(path+"/global_dbsources.json", DBQ::FILE_TYPE_JSON, dbsourcesList, errStr);
    if(!ok)
        return false;

    int size = dbsourcesList.size();
    for(int i = 0 ; i < size; i++)
        dbsourceNamesOut.append(dbsourcesList.at(i)->dbSrcName);

    qDeleteAll(dbsourcesList);
    return true;
}
} //end namespace
