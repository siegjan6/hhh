#include "XMLViewVariableStorage.h"
#include <QFile>
#include <QTextStream>

namespace DBQ
{
XMLViewVariableStorage::XMLViewVariableStorage()
{
}

bool XMLViewVariableStorage::Load(const QString & fileName, QList<ViewVariable *> & viewVarsOut, QString & errStrOut)
{
    //<加载文件>
    QString fileContent;
    QDomDocument xmlDoc;
    if(!GetFileContent(fileName, fileContent))
    {
        errStrOut = QString("open file:%1 fail").arg(fileName);
        return false;
    }

    if(! xmlDoc.setContent(fileContent))
    {
        errStrOut = QString("XML Error:cann't parse %1 file").arg(fileName);
         return false;
    }
    //</加载文件>

    QDomElement root = xmlDoc.documentElement();
    QDomElement varNode =  root.firstChildElement(); //a view variable

    QDomElement ele;
    QString strTemp;
    while(!varNode.isNull())
    {
        ViewVariable * pVar = new ViewVariable;
        viewVarsOut.push_back(pVar);

        ele = varNode.firstChildElement("name");
        if(ele.isNull()) goto PARSE_FAIL;
        if(ele.text().isEmpty())  goto PARSE_FAIL;
        pVar->name = ele.text();

        ele = varNode.firstChildElement("type");
        if(ele.isNull()) goto PARSE_FAIL;
        if(ele.text().isEmpty())  goto PARSE_FAIL;
        strTemp = ele.text();
        pVar->type = GetTypeFromString(strTemp);
        if(VIEW_VARIABLE_TYPE_UNKNOWN == pVar->type )
            goto PARSE_FAIL;
        /*
        if(strTemp.compare("const", Qt::CaseInsensitive) == 0)
            pVar->type = VIEW_VARIABLE_TYPE_CONST;
        else if(strTemp.compare("enum", Qt::CaseInsensitive) == 0)
            pVar->type = VIEW_VARIABLE_TYPE_ENUM;
        else if(strTemp.compare("field", Qt::CaseInsensitive) == 0)
            pVar->type = VIEW_VARIABLE_TYPE_FIELD;
        else if(strTemp.compare("function", Qt::CaseInsensitive) == 0)
            pVar->type = VIEW_VARIABLE_TYPE_FUNC;
        else
            goto PARSE_FAIL;
        */

        ele = varNode.firstChildElement("valuetype");
        if(ele.isNull()) goto PARSE_FAIL;
        if(ele.text().isEmpty())  goto PARSE_FAIL;
        strTemp = ele.text();
        pVar->valueType =View_GetFieldTypeFromString(strTemp); //视图变量值类型与视图字段类型相同
        if(VIEW_FIELD_TYPE_UNKNOWN ==  pVar->valueType)
            goto PARSE_FAIL;

        if(pVar->type == VIEW_VARIABLE_TYPE_CONST
            || pVar->type == VIEW_VARIABLE_TYPE_FIELD
            || pVar->type == VIEW_VARIABLE_TYPE_FUNC)
        {
            ele = varNode.firstChildElement("value");
            if(ele.isNull()) goto PARSE_FAIL;
            if(ele.text().isEmpty())
                pVar->value.clear(); //值可以为空
            else
                pVar->value = ele.text();
        }
        else if(pVar->type == VIEW_VARIABLE_TYPE_ENUM)
        {
            //if(ele.text().isEmpty())  goto PARSE_FAIL;
            if(!ParseEnumValue(varNode, pVar)) goto PARSE_FAIL;
        }
        else
            goto PARSE_FAIL;

        //描述可以为空
        ele = varNode.firstChildElement("description");
        if(!ele.isNull())
        {
            if(!ele.text().isEmpty())
                pVar->description = ele.text();
        }

        varNode = varNode.nextSiblingElement();
    }

    return true;
PARSE_FAIL:
    //释放内存
    while(!viewVarsOut.isEmpty())
        delete viewVarsOut.takeFirst();

    errStrOut = QString("XML Error:cann't parse %1 file:file format error").arg(fileName);
    return false;
}

bool XMLViewVariableStorage::Save(const QString & fileName, QList<ViewVariable *> & viewVarsIn, QString & errStrOut)
{
    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement varsNode = doc.createElement("variables");
    doc.appendChild(varsNode);

    int size = viewVarsIn.size();
    ViewVariable * pVar = 0;

    for(int i=0; i<size; i++)
    {
        QDomElement varNode = doc.createElement("variable");
        varsNode.appendChild(varNode);

        pVar = viewVarsIn[i];

        AppendNode(doc, varNode, "name", pVar->name);
        AppendNode(doc, varNode, "type", QString(GetTypeString(pVar->type))); //兼容Qt 5.2
        AppendNode(doc, varNode, "valuetype", QString(View_GetFieldTypeStringFromType(pVar->valueType))); //兼容Qt 5.2
        AppendNode(doc, varNode,"value", pVar->value);
        AppendNode(doc, varNode, "description", pVar->description);
        WriteEnumNode(doc, varNode, pVar->evs);
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

bool XMLViewVariableStorage::GetFileContent(const QString & fullFileName, QString & fileContentOut)
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

bool XMLViewVariableStorage::ParseEnumValue(QDomElement & varNode, ViewVariable * pVarOut)
{
    QDomElement tNode;
    ViewVariableEnumValue ev;

    QDomElement enumNode = varNode.firstChildElement("enumValue");
    if(enumNode.isNull())
        return false;

    QDomElement evNode = enumNode.firstChildElement("ev");
    if(evNode.isNull()) //没有ev节点 则出错
        return false;

    while(!evNode.isNull())
    {
        tNode = evNode.firstChildElement("v");
        if(tNode.isNull()) return false;
        if(tNode.text().isEmpty()) return false;
        ev.value = tNode.text();

        //可以没有描述信息
        tNode = evNode.firstChildElement("desc");
        if(!tNode.isNull())
        {
            if(!tNode.text().isEmpty())
                ev.desc = tNode.text();
        }

        pVarOut->evs.push_back(ev);
        evNode = evNode.nextSiblingElement();
    }

    return true;
}

void XMLViewVariableStorage::AppendNode(QDomDocument & doc, QDomElement & parentNode, const QString & nodeName, const QString & nodeText)
{
    QDomElement node = doc.createElement(nodeName);
    parentNode.appendChild(node);
    node.appendChild(doc.createTextNode(nodeText));
}

void XMLViewVariableStorage::WriteEnumNode(QDomDocument & doc, QDomElement & parentNode, QList<ViewVariableEnumValue> & evs)
{
    QDomElement enumNode = doc.createElement("enumValue");
    parentNode.appendChild(enumNode);

    int size = evs.size();
    for(int i=0; i<size; i++)
    {
        QDomElement evNode = doc.createElement("ev");
        enumNode.appendChild(evNode);

        QDomElement vNode = doc.createElement("v");
        evNode.appendChild(vNode);
        vNode.appendChild(doc.createTextNode(evs[i].value));

        vNode = doc.createElement("desc");
        evNode.appendChild(vNode);
        vNode.appendChild(doc.createTextNode(evs[i].desc));
    }
}
}
