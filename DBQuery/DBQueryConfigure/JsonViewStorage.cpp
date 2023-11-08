#include "JsonViewStorage.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "utils.h"

namespace DBQ
{
JsonViewStorage::JsonViewStorage()
{
}
bool JsonViewStorage::Load(const QString & fileName, View ** ppViewOut, QString & errOut)
{
    struct View * pView = 0;
    //<加载文件>
    QByteArray fileContent;
    if(!GetFileContentToByteArray(fileName, fileContent))
    {
        errOut = QString("open file:%1 fail").arg(fileName);
        WriteInfoLog("JsonViewStorage::Load"+errOut);
        return false;
    }

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileContent, &error);
    if(error.error != QJsonParseError::NoError)
    {
        errOut = QString("cann't parse %1 file").arg(fileName);
        WriteInfoLog("JsonViewStorage::Load"+errOut);
         return false;
    }
    //</加载文件>

    QString filterXMLStr ;
    QJsonArray fieldsArray;
    QJsonValue fieldsObj;
    QJsonArray dbsourcesArray;
    QJsonValue dbsourcesObj;

    pView = new View;
    //pView->pFilter = 0; //初始化View

    QJsonObject rootObj = jsonDoc.object();
    pView->viewName = rootObj.value("name").toString();
    if(pView->viewName.isEmpty())
    {
        errOut = QString("%1 file:view name is empty").arg(fileName);
        WriteInfoLog("JsonViewStorage::Load"+errOut);
        goto PARSE_FAIL;
    }

    dbsourcesObj = rootObj.value("dbsources");
    if(!dbsourcesObj.isArray())
    {
        errOut = QString("%1 file:db source is not array").arg(fileName);
        WriteInfoLog("JsonViewStorage::Load:"+errOut);
        goto PARSE_FAIL;
    }

    dbsourcesArray = dbsourcesObj.toArray();
    if(!ParseDBSources(dbsourcesArray, pView))
    {
        errOut = QString("%1 file:db sources error").arg(fileName);
        WriteInfoLog("JsonViewStorage::Load:"+errOut);
        goto PARSE_FAIL;
    }

    fieldsObj = rootObj.value("fields");
    if(!fieldsObj.isArray())
    {
        errOut = QString("%1 file:fields is not array").arg(fileName);
        WriteInfoLog("JsonViewStorage::Load:"+errOut);
        goto PARSE_FAIL;
    }

    fieldsArray = fieldsObj.toArray();
    if(!ParseFields(fieldsArray, pView))
    {
        errOut = QString("%1 file:cann't parse fields").arg(fileName);
        WriteInfoLog("JsonViewStorage::Load:"+errOut);
        goto PARSE_FAIL;
    }

    filterXMLStr = rootObj.value("filter").toString();
    if(!filterXMLStr.isEmpty()) //filter可以为空
    {
        if(!ParseFilter(filterXMLStr, pView))
        {
            errOut = QString("%1 file:cann't parse filter").arg(fileName);
            WriteInfoLog("JsonViewStorage::Load:"+errOut);
            goto PARSE_FAIL;
        }
    }

    *ppViewOut = pView;
    return true;
PARSE_FAIL:
    delete pView;
    return false;
}

bool JsonViewStorage::Save(const QString & fileName, View * pViewIn, QString & errStrOut)
{
    if(!pViewIn)
    {
        errStrOut = QString("%1 file:view is null").arg(fileName);
        WriteInfoLog("JsonViewStorage::Save"+errStrOut);
        return false;
    }

    QJsonObject root;
    QJsonArray dbSourceArray;
    QJsonArray fieldArray;
    QString xmlFilterStr;
    QJsonValue nameV(pViewIn->viewName);
    root.insert("name", nameV);

    WriteDBSources(dbSourceArray, pViewIn->dbSources);
    WriteFields(fieldArray, pViewIn->fields);
    if(!WriteFilter(xmlFilterStr, pViewIn->pFilter, errStrOut))
    {
        errStrOut = QString("%1 file:save filter error").arg(fileName);
        WriteInfoLog("JsonViewStorage::Save"+errStrOut);
        return false;
    }

    SaveJosnFileHead(root, 1, "DBQuery");
    root.insert("name", nameV);
    root.insert("dbsources", dbSourceArray);
    root.insert("fields", fieldArray);
    root.insert("filter", xmlFilterStr);

    return WriteJsonObjectToFile(fileName, root);
}

bool JsonViewStorage::ParseDBSources(QJsonArray & dbsourcesArray, View * pView)
{
    int size =dbsourcesArray.size();
    ViewDBSource dbSrc;
    QString str;

    for(int i=0; i<size; ++i)
    {
        QJsonObject dbObj = dbsourcesArray.at(i).toObject();
        if(dbObj.isEmpty()) //数组元素不是对象 则出错
            return false;

        dbSrc.dbSrcName = dbObj.value("dbsrcName").toString();
        if(dbSrc.dbSrcName.isEmpty())
            return false;

         str = dbObj.value("dbs").toString();
         if(!str.isEmpty())
            dbSrc.dbNames = str.split(',', QString::SkipEmptyParts, Qt::CaseInsensitive);

         pView->dbSources.push_back(dbSrc);
    }
    return true;
}

bool JsonViewStorage::ParseFields(QJsonArray & fieldsArray, View * pView)
{
    QString str;
    int size = fieldsArray.size();
    for(int i=0; i<size; i++)
    {
        ViewField * pField = new ViewField;
         pView->fields.push_back(pField);

         QJsonObject fieldObj = fieldsArray.at(i).toObject();
         if(fieldObj.isEmpty()) //数组元素不是对象 则出错
             goto PARSE_FIELDS_FAIL;

         pField->fieldName = fieldObj.value("fieldName").toString();
        if(pField->fieldName.isEmpty())
            goto PARSE_FIELDS_FAIL;

        pField->tableName = fieldObj.value("tbName").toString();
        if(pField->tableName.isEmpty())
            goto PARSE_FIELDS_FAIL;

        str = fieldObj.value("type").toString();
        if(str.isEmpty())
            goto PARSE_FIELDS_FAIL;
        pField->type = GetFieldTypeFromString(str);

        pField->alias = fieldObj.value("alias").toString(); //别名可以是空
        pField->description = fieldObj.value("desc").toString();//描述可以是空
    }
    return true;
PARSE_FIELDS_FAIL:
    //释放内存
    while(!pView->fields.isEmpty())
        delete pView->fields.takeFirst();

    return  false;
}

bool JsonViewStorage::ParseFilter(QString & filterXMLStr, View * pView)
{
    QDomDocument xmlDoc;
    QString errMsg;
    if(!xmlDoc.setContent(filterXMLStr, &errMsg))
        return false;

    QDomElement itemNode = xmlDoc.firstChildElement();
    if(itemNode.isNull())
        return true; //没有过滤条件

    pView->pFilter = new ViewFilterNode;
    pView->pFilter->type = VIEW_FILTER_NODE_TYPE_ROOT;
    if(ParserFilterTree(itemNode, pView->pFilter))
        return true;

    FilterTree_Delete(0, pView->pFilter);
    pView->pFilter = 0;

    return false;
}

void JsonViewStorage::WriteDBSources(QJsonArray & dbSourceArray, QList<ViewDBSource> & dbSources)
{
    int size = dbSources.size();
    ViewDBSource src;
    QJsonObject ele; //数组元素 一个数据源
    QString dbNamesText; //多个数据库名称 名称之间通过逗号隔开

    for(int i=0; i<size; i++)
    {
        src = dbSources[i];
        ele.insert("dbsrcName", src.dbSrcName);

        //<db1,db2,...>
       int len = src.dbNames.size();
       dbNamesText.clear();
       for(int j=0; j<len; j++) //按"db1,db2,..."格式输出
       {
           dbNamesText.append(src.dbNames[j]).append(",");
       }

       //去掉最后一个逗号
       if(dbNamesText.size()>0)
           dbNamesText = dbNamesText.left(dbNamesText.size()-1);

       ele.insert("dbs", dbNamesText);
       //</db1,db2,...>
       dbSourceArray.append(ele);
    }
}

void JsonViewStorage::WriteFields(QJsonArray & fieldArray, QList<ViewField *> & fields)
{
    QJsonObject ele; //数组元素 一个字段
    int size = fields.size();
    for(int i=0; i<size; i++)
    {
        ViewField * pField = fields[i];
        if(!pField)
            continue;

        ele.insert("fieldName", pField->fieldName);
        ele.insert("tbName", pField->tableName);
        ele.insert("type", GetFiledTypeStr(pField->type));
        ele.insert("alias", pField->alias);
        ele.insert("desc", pField->description);

        fieldArray.insert(i, ele);
    }
}

bool JsonViewStorage::WriteFilter(QString & xmlFilterStrOut, ViewFilterNode * pFilter, QString & errStrOut)
{
    QDomDocument doc;
    QDomElement filterNode = doc.createElement("filter");
    doc.appendChild(filterNode);

    if(!pFilter) //可以没有过滤条件
        return true;
    if(pFilter->type != VIEW_FILTER_NODE_TYPE_ROOT) //第一个节点必须是根节点
    {
        errStrOut = "hasn't root node";
        WriteInfoLog("JsonViewStorage::WriteFilter"+errStrOut);
        return false;
    }

    for(int i=0; i<pFilter->child.size(); i++)
        WriteFilterTree(doc, filterNode, pFilter->child[i]);

    xmlFilterStrOut = doc.toString(-1);
    //去掉头部<filter>和尾部的</filter>  xmlFilter有可能为"<filter/>"
    xmlFilterStrOut.replace("<filter>", "",Qt::CaseInsensitive);
    xmlFilterStrOut.replace("</filter>", "",Qt::CaseInsensitive);
    xmlFilterStrOut.replace("<filter/>", "",Qt::CaseInsensitive);

    return true;
}

void JsonViewStorage::WriteFilterTree(QDomDocument & doc, QDomElement & parentNode, ViewFilterNode * pFilter)
{
    if(!pFilter)
        return ;

    //递归终止条件:遍历完某子树的所有节点
    if(VIEW_FILTER_NODE_TYPE_AND == pFilter->type) //and节点 有子节点
    {
        QDomElement andNode = doc.createElement("and");
        parentNode.appendChild(andNode);

        for(int i=0; i<pFilter->child.size(); i++)
            WriteFilterTree(doc, andNode, pFilter->child[i]);
        return ;
    }

    if(VIEW_FILTER_NODE_TYPE_OR == pFilter->type) //or节点 有子节点
    {
        QDomElement orNode = doc.createElement("or");
        parentNode.appendChild(orNode);

        for(int i=0; i<pFilter->child.size(); i++)
            WriteFilterTree(doc, orNode, pFilter->child[i]);
    }

    if(VIEW_FILTER_NODE_TYPE_LEAF == pFilter->type) //叶子节点 有子节点
    {
        //添加左值类型 //TODO:更新文档
        QString itemText;
        //{左值}{操作符}{右值}{左值类型}
        if(VIEW_FILTER_RIGHT_VALUE_TYPE_VAR == pFilter->rightValueType)    //变量类型 右值加上[%右值%]
            itemText = QString("{%1}{%2}{[%%3%]}{%4}").arg(pFilter->leftValue, FilterTree_GetCompareString(pFilter->compare), pFilter->rightValue, GetFiledTypeStr(pFilter->leftType));
        else
            itemText = QString("{%1}{%2}{%3}{%4}").arg(pFilter->leftValue, FilterTree_GetCompareString(pFilter->compare), pFilter->rightValue, GetFiledTypeStr(pFilter->leftType));

        AppendNode(doc, parentNode, "item", itemText);
        return ;
    }
}

void JsonViewStorage::AppendNode(QDomDocument & doc, QDomElement & parentNode, const QString & nodeName, const QString & nodeText)
{
    QDomElement node = doc.createElement(nodeName);
    parentNode.appendChild(node);
    node.appendChild(doc.createTextNode(nodeText));
}
}
