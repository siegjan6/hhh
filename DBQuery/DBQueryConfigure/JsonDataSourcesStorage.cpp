#include "JsonDataSourcesStorage.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "utils.h"

namespace DBQ
{
JsonDataSourcesStorage::JsonDataSourcesStorage()
{
}

//函数功能:从Json文件中加载数据源
//dbSourcesOut必须为空
bool JsonDataSourcesStorage::Load(const QString & fileName, QList<DBSource *> & dbSourcesOut, QString & errOut)
{
    //<加载文件>
    QByteArray fileContent;
    if(!GetFileContentToByteArray(fileName, fileContent))
    {
        errOut = QString("open file:%1 fail").arg(fileName);
        WriteInfoLog("JsonDataSourcesStorage::Load:"+errOut);
        return false;
    }

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileContent, &error);
    if(error.error != QJsonParseError::NoError)
    {
        errOut = QString("Json Error:cann't parse %1 file").arg(fileName);
        WriteInfoLog("JsonDataSourcesStorage::Load:"+errOut);
         return false;
    }
    //</加载文件>

    QJsonObject rootObj = jsonDoc.object();
    QJsonValue dbsourcesObj = rootObj.value("dbsources");
    if(!dbsourcesObj.isArray())
    {
        errOut = "dbsources node error";
        WriteInfoLog("JsonDataSourcesStorage::Load:"+errOut);
        return false;
    }

    QString dbType;
    QJsonValue v;
    QJsonArray dbsourcesArray = dbsourcesObj.toArray();
    QJsonArray dbArray;
    int size = dbsourcesArray.size(); //数据源个数
    for(int i=0; i<size; i++)
    {
        //如果出错异常 在RELASE_DB_SOURCE:中释放所有new出的DBSource
        DBSource * pDBSrc = new DBSource;
        dbSourcesOut.push_back(pDBSrc);

       QJsonObject dbsourceObj = dbsourcesArray[i].toObject();
       if(dbsourceObj.isEmpty()) //数组元素不是对象 出错
       {
           errOut = "dbsource is not object";
           WriteInfoLog("JsonDataSourcesStorage::Load:"+errOut);
           goto RELEASE_DB_SOURCE;
       }

       pDBSrc->dbSrcName = dbsourceObj.value("dbsrcName").toString(); //数据源名称
       if(pDBSrc->dbSrcName.isEmpty())
       {
           errOut = "db source name is empty";
           WriteInfoLog("JsonDataSourcesStorage::Load:"+errOut);
            goto RELEASE_DB_SOURCE;
       }

       v = dbsourceObj.value("default");
       if(!v.isBool())
       {
           errOut = "dbsource.defualt is not bool";
           WriteInfoLog("JsonDataSourcesStorage::Load:"+errOut);
            goto RELEASE_DB_SOURCE;
       }
        pDBSrc->bDefault = v.toBool();

        //<表前缀 用于MS SQL Server>
        v = dbsourceObj.value("tableprefix");
        if(v.isString())
            pDBSrc->tablePrefix = v.toString();
        else
            pDBSrc->tablePrefix.clear();
        //</表前缀>

        pDBSrc->host = dbsourceObj.value("host").toString();
        if(pDBSrc->host.isEmpty())
        {
            errOut = "host is empty";
            WriteInfoLog("JsonDataSourcesStorage::Load:"+errOut);
            goto RELEASE_DB_SOURCE;
        }

        v = dbsourceObj.value("port");
        if(!v.isDouble())
        {
            errOut = "port error";
            WriteInfoLog("JsonDataSourcesStorage::Load:"+errOut);
            goto RELEASE_DB_SOURCE;
        }

        pDBSrc->port = v.toInt();

        pDBSrc->userName = dbsourceObj.value("username").toString();
        if(pDBSrc->userName.isEmpty())
        {
            errOut = "username is empty";
            WriteInfoLog("JsonDataSourcesStorage::Load:"+errOut);
            goto RELEASE_DB_SOURCE;
        }

        pDBSrc->password = dbsourceObj.value("password").toString();//密码可以为空

        dbType = dbsourceObj.value("dbType").toString();

        pDBSrc->dbType = DataSource_GetDBType(dbType);
        /*
        if(dbType.compare("MYSQL", Qt::CaseInsensitive) == 0)
            pDBSrc->dbType =  DBSOURCE_TYPE_MYSQL;
        else if(dbType.compare("SQLSERVER", Qt::CaseInsensitive) == 0)
            pDBSrc->dbType =  DBSOURCE_TYPE_SQL_SERVER;
        else if(dbType.compare("ORACLE", Qt::CaseInsensitive) == 0)
            pDBSrc->dbType =  DBSOURCE_TYPE_ORACLE;
        else if(dbType.compare("KINGBASE", Qt::CaseInsensitive) == 0)
            pDBSrc->dbType =  DBSOURCE_TYPE_KINGBASE;
        else
            pDBSrc->dbType = DBSOURCE_TYPE_UNKNOWN;
        */

        v = dbsourceObj.value("database");
        if(!v.isArray())
        {
            errOut = "database is not array";
            WriteInfoLog("JsonDataSourcesStorage::Load:"+errOut);
            goto RELEASE_DB_SOURCE;
        }
        dbArray = v.toArray();
        if(!ParseDB(dbArray, pDBSrc))
        {
            errOut = "ParseDB error";
            WriteInfoLog("JsonDataSourcesStorage::Load:"+errOut);
            goto RELEASE_DB_SOURCE;
        }
    }
   return true;
RELEASE_DB_SOURCE:
    //释放内存
    while(!dbSourcesOut.isEmpty())
        delete dbSourcesOut.takeFirst();
    return false;
}

bool JsonDataSourcesStorage::Save(const QString & fileName, QList<DBSource *> & dbSrcsIn, QString & errStrOut)
{
    Q_UNUSED(errStrOut);
    QJsonObject root;

    QJsonArray dbsourcesArray;
    QJsonObject ele; //data source
    int size = dbSrcsIn.size();
    for(int i=0;  i<size; i++)
    {
        DBSource * pDBSrc = dbSrcsIn[i];
        if(!pDBSrc)
        {
            WriteInfoLog("JsonDataSourcesStorage::Save:DBSource is empty");
            return false;
        }
        ele.insert("tableprefix", pDBSrc->tablePrefix);
        ele.insert("dbsrcName", pDBSrc->dbSrcName);
        ele.insert("default", pDBSrc->bDefault);
        ele.insert("host", pDBSrc->host);
        ele.insert("port", pDBSrc->port);
        ele.insert("username", pDBSrc->userName);
        ele.insert("password", pDBSrc->password);
        ele.insert("dbType", QString(GetTypeString(pDBSrc->dbType)));//兼容Qt 5.2
        WriteDatabasesNode(ele, pDBSrc->dbs);
        dbsourcesArray.insert(i, ele);
    }

    SaveJosnFileHead(root, 1, "DBQuery");
    root.insert("dbsources", dbsourcesArray);

    return WriteJsonObjectToFile(fileName, root);
}

bool JsonDataSourcesStorage::ParseDB(QJsonArray & dbArray, DBSource * pDBSrc)
{
    QJsonObject dbObj;
    QJsonValue v;
    DB db;
    int size = dbArray.size();

    for(int i=0; i<size;i++)
    {
        dbObj = dbArray[i].toObject();
        if(dbObj.isEmpty())
            return false;

        v = dbObj.value("default");
        if(!v.isBool())
            return false;
        db.bDefault = v.toBool();

        db.dbName = dbObj.value("name").toString();
        if(db.dbName.isEmpty())
            return false;

        pDBSrc->dbs.push_back(db);
    }
    return true;
}

void JsonDataSourcesStorage::WriteDatabasesNode(QJsonObject & ele, QList<struct DB> & dbs)
{
    QJsonArray dbArray;
    int size = dbs.size();
    for(int i=0; i<size; i++)
    {
        QJsonObject dbObj;
        dbObj.insert("default", dbs[i].bDefault);
        dbObj.insert("name", dbs[i].dbName);
        dbArray.append(dbObj);
    }

    ele.insert("database", dbArray);
}
}//end namespace

