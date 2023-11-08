#include "XMLDataSourcesStorage.h"

namespace DBQ
{
XMLDataSourcesStorage::XMLDataSourcesStorage()
{
}
//函数功能:从XML文件中加载数据源
//dbSourcesOut必须为空
bool XMLDataSourcesStorage::Load(const QString & fileName, QList<DBSource *> & dbSourcesOut, QString & errOut)
{
    //<加载文件>
    QString fileContent;
    QDomDocument xmlDoc;
    if(!GetFileContent(fileName, fileContent))
    {
        errOut = QString("open file:%1 fail").arg(fileName);
        return false;
    }

    if(! xmlDoc.setContent(fileContent))
    {
        errOut = QString("XML Error:cann't parse %1 file").arg(fileName);
         return false;
    }
    //</加载文件>

    QDomElement root = xmlDoc.documentElement();
    QDomElement dbSrcNode =  root.firstChildElement(); //data source

     QDomElement child;
    while(!dbSrcNode.isNull())
    {
        //如果出错异常 在RELASE_DB_SOURCE:中释放所有new出的DBSource
        DBSource * pDBSrc = new DBSource;
        dbSourcesOut.push_back(pDBSrc);

        if(dbSrcNode.attribute("dbsrcName").isEmpty()) //数据源名称不能为空
        {
            errOut = QString("XML Error:cann't parse %1 file:db source is null").arg(fileName);
            goto RELASE_DB_SOURCE;
        }
       pDBSrc->dbSrcName = dbSrcNode.attribute("dbsrcName");

       if(dbSrcNode.attribute("default").compare("true", Qt::CaseInsensitive) == 0)
           pDBSrc->bDefault = true;
       else
           pDBSrc->bDefault = false;

       //<data source 内容>
       child =  dbSrcNode.firstChildElement();
       int count = 0;
       //要进行异常测试
       while(!child.isNull())
       {
           if(child.tagName() == "host")
           {
               pDBSrc->host = child.text();
               count++;
           }else if(child.tagName() == "port")
           {
               pDBSrc->port = child.text().toInt();
               count++;
           }else if(child.tagName() == "username")
           {
               pDBSrc->userName = child.text();
               count++;
           }else if(child.tagName() == "password")
           {
               pDBSrc->password = child.text();
               count++;
           }else if(child.tagName() == "dbType")
           {
               count++;
                if(child.text().compare("MYSQL", Qt::CaseInsensitive) == 0)
                    pDBSrc->dbType =  DBSOURCE_TYPE_MYSQL;
                else if(child.text().compare("SQLSERVER", Qt::CaseInsensitive) == 0)
                    pDBSrc->dbType =  DBSOURCE_TYPE_SQL_SERVER;
                else if(child.text().compare("ORACLE", Qt::CaseInsensitive) == 0)
                     pDBSrc->dbType =  DBSOURCE_TYPE_ORACLE;
                else if(child.text().compare("KINGBASE", Qt::CaseInsensitive) == 0)
                    pDBSrc->dbType =  DBSOURCE_TYPE_KINGBASE;
                else
                    pDBSrc->dbType = DBSOURCE_TYPE_UNKNOWN;
           }else if(child.tagName() == "databases")
           {
               if(ParseDB(child, pDBSrc))
                   count++;
           }

           child = child.nextSiblingElement();
       } //end while:child
        //</data source 内容>

       //XML文件中共有六项:host,port,username,password,dbType,databases.
       //任意一项解析失败 则终止解析
       if(count != 6)
       {
            errOut = QString("XML Error:cann't parse %1 file:db format error").arg(fileName);
            goto RELASE_DB_SOURCE;
       }

       dbSrcNode = dbSrcNode.nextSiblingElement();
    }

    return true;
RELASE_DB_SOURCE: //需要测试
    //释放内存
    while(!dbSourcesOut.isEmpty())
        delete dbSourcesOut.takeFirst();

    return false;
}

bool XMLDataSourcesStorage::Save(const QString & fileName, QList<DBSource *> & dbSrcsIn, QString & errStrOut)
{
    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement dbSrcsNode = doc.createElement("DBSources");
    doc.appendChild(dbSrcsNode);

    int size = dbSrcsIn.size();
    DBSource * pDBSrc = 0;

    for(int i=0; i<size; i++)
    {
        QDomElement dbSrcNode = doc.createElement("DBSource");
        dbSrcsNode.appendChild(dbSrcNode);

        pDBSrc = dbSrcsIn[i];
        dbSrcNode.setAttribute("default", pDBSrc->bDefault?"true":"false");

        dbSrcNode.setAttribute("desrcName", pDBSrc->dbSrcName);

        AppendNode(doc, dbSrcNode, "host", pDBSrc->host);
        AppendNode(doc, dbSrcNode, "port", QString::number(pDBSrc->port));
        AppendNode(doc, dbSrcNode, "username", pDBSrc->userName);
        AppendNode(doc, dbSrcNode,"password", pDBSrc->password);
        AppendNode(doc, dbSrcNode, "dbType", QString(GetTypeString(pDBSrc->dbType))); //兼容Qt 5.2
        WriteDatabasesNode(doc, dbSrcNode, pDBSrc->dbs);
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
    {
        errStrOut  = QString("open file:%1 fail").arg(fileName);
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    doc.save(out,4,QDomNode::EncodingFromTextStream);
    file.close();

    return true;
}

bool XMLDataSourcesStorage::GetFileContent(const QString & fullFileName, QString & fileContentOut)
{
    QFile file(fullFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        return false;
    }

    QTextStream out(&file);
    fileContentOut = out.readAll();

    return true;
}

bool XMLDataSourcesStorage::ParseDB(QDomElement & child, DBSource * pDBSrc)
{
    QDomElement dbNode = child.firstChildElement();
    QDomElement nameNode;
    DB db;

    while(!dbNode.isNull())
    {
        if(dbNode.attribute("default").compare("true", Qt::CaseInsensitive) == 0)
            db.bDefault = true;
        else
            db.bDefault = false;

        nameNode = dbNode.firstChildElement("name");
        if(nameNode.isNull())
            return false;
        if(nameNode.text().isEmpty())
            return false;

        db.dbName = nameNode.text();
        pDBSrc->dbs.push_back(db);

        dbNode = dbNode.nextSiblingElement();
    }
    return true;
}

void XMLDataSourcesStorage::AppendNode(QDomDocument & doc, QDomElement & parentNode, const QString & nodeName, const QString & nodeText)
{
    QDomElement node = doc.createElement(nodeName);
    parentNode.appendChild(node);
    node.appendChild(doc.createTextNode(nodeText));
}

void XMLDataSourcesStorage::WriteDatabasesNode(QDomDocument & doc, QDomElement & dbSrcNode, QList<struct DB> & dbs)
{
    QDomElement dbsNode = doc.createElement("databases");
    dbSrcNode.appendChild(dbsNode);

    int size = dbs.size();

    for(int i=0; i<size; i++)
    {
        QDomElement dbNode = doc.createElement("db");
        dbsNode.appendChild(dbNode);

        dbNode.setAttribute("default",dbs[i].bDefault?"true":"false");

        QDomElement nameNode = doc.createElement("name");
        dbNode.appendChild(nameNode);
        nameNode.appendChild(doc.createTextNode(dbs[i].dbName));
    }
}
}
