#include "XMLRuntimeViewVarStorage.h"
#include <QFile>
#include <QTextStream>

namespace DBQ
{
XMLRuntimeViewVarStorage::XMLRuntimeViewVarStorage()
{
}

bool XMLRuntimeViewVarStorage::Load(const QString & fileName, QList<RuntimeViewVariable *> & rtVarsOut, QString & errStrOut)
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
    QDomElement varNode =  root.firstChildElement(); //a runtime view variable
    QDomElement tNode;

    while(!varNode.isNull())
    {
        RuntimeViewVariable * pVar = new RuntimeViewVariable;
        rtVarsOut.push_back(pVar);

        tNode = varNode.firstChildElement("key");
        if(tNode.isNull()) goto PARSE_RUTIME_VAR_FAIL;
        if(tNode.text().isEmpty()) goto PARSE_RUTIME_VAR_FAIL;
        pVar->key = tNode.text();

        tNode = varNode.firstChildElement("value");
        if(tNode.isNull()) goto PARSE_RUTIME_VAR_FAIL;
        pVar->value = tNode.text(); //值 可以为空

        varNode = varNode.nextSiblingElement();
    }

    //文件中可以没有变量
    return true;
PARSE_RUTIME_VAR_FAIL:
    //释放内存 填写出错信息
    while(!rtVarsOut.isEmpty())
        delete rtVarsOut.takeFirst();

    errStrOut = QString("XML Error:cann't parse %1 file:content invaild").arg(fileName);
    return false;
}

bool XMLRuntimeViewVarStorage::Save(const QString & fileName, QList<RuntimeViewVariable *> & rtVarsIn, QString & errStrOut)
{
    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement varsNode = doc.createElement("variables");
    doc.appendChild(varsNode);

    int size = rtVarsIn.size();
    for(int i=0; i<size; i++)
    {
        QDomElement varNode = doc.createElement("variable");
        varsNode.appendChild(varNode);

        AppendNode(doc, varNode, "key", rtVarsIn[i]->key);
        AppendNode(doc, varNode, "value", rtVarsIn[i]->value);
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

bool XMLRuntimeViewVarStorage::GetFileContent(const QString & fullFileName, QString & fileContentOut)
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

void XMLRuntimeViewVarStorage::AppendNode(QDomDocument & doc, QDomElement & parentNode, const QString & nodeName, const QString & nodeText)
{
    QDomElement node = doc.createElement(nodeName);
    parentNode.appendChild(node);
    node.appendChild(doc.createTextNode(nodeText));
}
}
