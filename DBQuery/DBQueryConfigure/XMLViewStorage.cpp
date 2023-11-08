#include "XMLViewStorage.h"
#include <QFile>
#include <QTextStream>
#include <QtXml/QDomDocument>

namespace DBQ
{
const char * Filter_GetCompareString(VIEW_FILTER_COMPARE_TYPE type);

XMLViewStorage::XMLViewStorage()
{
}

bool XMLViewStorage::Load(const QString & fileName, View ** ppViewOut, QString & errOut)
{
    struct View * pView = 0;
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
    pView = new View;
    pView->pFilter = 0; //初始化View

    QDomElement root = xmlDoc.documentElement();
    if(root.isNull())
    {
        errOut = QString("XML Error:cann't parse %1 file").arg(fileName);
        goto PARSE_FAIL;
    }

    pView->viewName = root.attribute("name");
    if(pView->viewName.isEmpty())
    {
        errOut = QString("XML Error:cann't parse %1 file:view name is empty").arg(fileName);
        goto PARSE_FAIL;
    }

    if(!ParseDBSources(root, pView))
    {
        errOut = QString("XML Error:cann't parse %1 file:db sources error").arg(fileName);
        goto PARSE_FAIL;
    }

    if(!ParseFields(root, pView))
    {
        errOut = QString("XML Error:cann't parse %1 file:fields error").arg(fileName);
        goto PARSE_FAIL;
    }

    if(!ParseFilter(root, pView))
    {
        errOut = QString("XML Error:cann't parse %1 file:filter error").arg(fileName);
        goto PARSE_FAIL;
    }

    *ppViewOut = pView;
    return true;
PARSE_FAIL:
    delete pView;
    return false;
}

bool XMLViewStorage::Save(const QString & fileName, View * pViewIn, QString & errStrOut)
{
    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement viewNode = doc.createElement("view");
    doc.appendChild(viewNode);
    viewNode.setAttribute("name", pViewIn->viewName);

    WriteDBSources(doc, viewNode, pViewIn->dbSources);
    WriteFields(doc, viewNode, pViewIn->fields);
    if(!WriteFilter(doc, viewNode, pViewIn->pFilter, errStrOut))
        return false;

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

bool XMLViewStorage::GetFileContent(const QString & fullFileName, QString & fileContentOut)
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

bool XMLViewStorage::ParseDBSources(QDomElement & root, struct View * pViewOut)
{
    QDomElement dbSourcesNode = root.firstChildElement("dbsources");
    if(dbSourcesNode.isNull())
        return false;

    QDomElement dbSrcNode = dbSourcesNode.firstChildElement("dbsource");
    ViewDBSource dbSrc;
    QDomElement dbsNode;

    while(!dbSrcNode.isNull())
    {
        if(dbSrcNode.attribute("dbsrcName").isEmpty())
            return false;
        dbSrc.dbSrcName = dbSrcNode.attribute("dbsrcName");

        dbsNode = dbSrcNode.firstChildElement("dbs");
        if(dbsNode.isNull())
            return false;

        dbSrc.dbNames = dbsNode.text().split(',', QString::SkipEmptyParts, Qt::CaseInsensitive);

        pViewOut->dbSources.push_back(dbSrc);
        dbSrcNode = dbSrcNode.nextSiblingElement();
    }
    return true;
}

bool XMLViewStorage::ParseFields(QDomElement & root,  struct View * pViewOut)
{
    QDomElement fieldsNode = root.firstChildElement("fields");
    if(fieldsNode.isNull())
        return false;

    QDomElement fieldNode = fieldsNode.firstChildElement("field");
    QDomElement tNode;
    QString tStr;

    //没有字段也认识是正确的
    while(!fieldNode.isNull())
    {
        ViewField * pField = new ViewField;
         pViewOut->fields.push_back(pField);

        tNode = fieldNode.firstChildElement("fieldName");
        if(tNode.isNull()) goto PARSE_FIELDS_FAIL;
        if(tNode.text().isEmpty()) goto PARSE_FIELDS_FAIL;
        pField->fieldName = tNode.text();

        tNode = fieldNode.firstChildElement("tbName");
        if(tNode.isNull()) goto PARSE_FIELDS_FAIL;
        if(tNode.text().isEmpty()) goto PARSE_FIELDS_FAIL;
        pField->tableName = tNode.text();

        tNode = fieldNode.firstChildElement("type");
        if(tNode.isNull()) goto PARSE_FIELDS_FAIL;
        tStr = tNode.text();
        pField->type = GetFieldTypeFromString(tStr);

        tNode = fieldNode.firstChildElement("alias");
        if(tNode.isNull()) goto PARSE_FIELDS_FAIL;
        pField->alias = tNode.text(); //别名可以是空

        fieldNode = fieldNode.nextSiblingElement();
    }
    return true;
PARSE_FIELDS_FAIL:
    //释放内存
    while(!pViewOut->fields.isEmpty())
        delete pViewOut->fields.takeFirst();

    return  false;
}

//static bool ParserFilterTree(QDomElement & itemNode, ViewFilterNode * pFilter, QList<ViewFilterNode * > & pNodeHelper);
bool XMLViewStorage::ParseFilter(QDomElement & root,  struct View * pViewOut)
{
    QDomElement filterNode = root.firstChildElement("filter");
    if(filterNode.isNull())
        return true;//没有过滤条件

    QDomElement itemNode = filterNode.firstChildElement();
    if(itemNode.isNull())
        return true; //没有过滤条件

    pViewOut->pFilter = new ViewFilterNode;
    pViewOut->pFilter->type = VIEW_FILTER_NODE_TYPE_ROOT;
    //pViewOut->pFilterNodeList.push_back(pViewOut->pFilter);
    if(ParserFilterTree(itemNode, pViewOut->pFilter))
        return true;

    FilterTree_Delete(0, pViewOut->pFilter);
    pViewOut->pFilter = 0;
    //while(!pViewOut->pFilterNodeList.empty())
      //  delete pViewOut->pFilterNodeList.takeFirst();

    return false;
}

void XMLViewStorage::WriteDBSources(QDomDocument & doc, QDomElement & viewNode, QList<ViewDBSource> & dbSources)
{
    QDomElement dbSourcesNode = doc.createElement("dbsources");
    viewNode.appendChild(dbSourcesNode);

    int size = dbSources.size();
    ViewDBSource src;

    for(int i=0; i<size; i++)
    {
        src = dbSources[i];
        QDomElement dbSrcNode = doc.createElement("dbsource");
        dbSourcesNode.appendChild(dbSrcNode);

        dbSrcNode.setAttribute("dbsrcName", src.dbSrcName);

         //<db1,db2,...>
        QDomElement dbsNode = doc.createElement("dbs");
        dbSrcNode.appendChild(dbsNode);

        int len = src.dbNames.size();
        QString dbNamesText;
        for(int j=0; j<len; j++) //按"db1,db2,..."格式输出
        {
            dbNamesText.append(src.dbNames[j]).append(",");
        }

        //去掉最后一个逗号
        if(dbNamesText.size()>0)
        {
            dbNamesText = dbNamesText.left(dbNamesText.size()-1);
        }

        dbsNode.appendChild(doc.createTextNode(dbNamesText));
        //</db1,db2,...>
    }
}

void XMLViewStorage::WriteFields(QDomDocument & doc, QDomElement & viewNode, QList<ViewField *> & fields)
{
    QDomElement fieldsNode = doc.createElement("fields");
    viewNode.appendChild(fieldsNode);

    int size = fields.size();
    for(int i=0; i<size; i++)
    {
        ViewField * pField = fields[i];
        if(!pField)
            continue;

        QDomElement fieldNode = doc.createElement("field");
        fieldsNode.appendChild(fieldNode);

        AppendNode(doc, fieldNode, "fieldName", pField->fieldName);
        AppendNode(doc, fieldNode, "tbName", pField->tableName);
        AppendNode(doc, fieldNode, "type", GetFiledTypeStr(pField->type));
        AppendNode(doc, fieldNode, "alias", pField->alias);
    }
}
void XMLViewStorage::AppendNode(QDomDocument & doc, QDomElement & parentNode, const QString & nodeName, const QString & nodeText)
{
    QDomElement node = doc.createElement(nodeName);
    parentNode.appendChild(node);
    node.appendChild(doc.createTextNode(nodeText));
}

bool XMLViewStorage::WriteFilter(QDomDocument & doc, QDomElement & viewNode, ViewFilterNode * pFilter, QString & errStrOut)
{
    QDomElement filterNode = doc.createElement("filter");
    viewNode.appendChild(filterNode);

    if(!pFilter) //可以没有过滤条件
        return true;

    if(pFilter->type != VIEW_FILTER_NODE_TYPE_ROOT) //第一个节点必须是根节点
    {
        errStrOut = "WriteViewToFile Error:hasn't root node";
        return false;
    }

    for(int i=0; i<pFilter->child.size(); i++)
        WriteFilterTree(doc, filterNode, pFilter->child[i]);

    return true;
}

void XMLViewStorage::WriteFilterTree(QDomDocument & doc, QDomElement & parentNode, ViewFilterNode * pFilter)
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
        QString itemText = QString("{%1}{%2}{%3}").arg(pFilter->leftValue, FilterTree_GetCompareString(pFilter->compare), pFilter->rightValue);
        AppendNode(doc, parentNode, "item", itemText);
        return ;
    }
}
}
