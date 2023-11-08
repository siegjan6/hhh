#include "JsonRuntimeViewVarStorage.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "utils.h"

namespace DBQ
{
JsonRuntimeViewVarStorage::JsonRuntimeViewVarStorage()
{
}

bool JsonRuntimeViewVarStorage::Load(const QString & fileName, QList<RuntimeViewVariable *> & rtVarsOut, QString & errStrOut)
{
    //<加载文件>
    QByteArray fileContent;
    if(!GetFileContentToByteArray(fileName, fileContent))
    {
        errStrOut = QString("open file:%1 fail").arg(fileName);
        WriteInfoLog("JsonRuntimeViewVarStorage::Load:"+errStrOut);
        return false;
    }

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileContent, &error);
    if(error.error != QJsonParseError::NoError)
    {
        errStrOut = QString("Json Error:cann't parse %1 file").arg(fileName);
        WriteInfoLog("JsonRuntimeViewVarStorage::Load:"+errStrOut);
         return false;
    }
    //</加载文件>
    QJsonObject rootObj = jsonDoc.object();
    QJsonValue varsObj = rootObj.value("vars");
    if(!varsObj.isArray())
    {
        errStrOut = "vars is not array";
        WriteInfoLog("JsonRuntimeViewVarStorage::Load:"+errStrOut);
        return false;
    }

    QJsonArray varArray = varsObj.toArray();
    int size = varArray.size();
    for(int i=0; i<size; i++)
    {
        RuntimeViewVariable * pVar = new RuntimeViewVariable;
        rtVarsOut.push_back(pVar);

        QJsonObject varObj = varArray[i].toObject();
        if(varObj.isEmpty()) //数组元素不是对象 出错
        {
            errStrOut = "variable object is empty";
            WriteInfoLog("JsonRuntimeViewVarStorage::Load:"+errStrOut);
            goto PARSE_RUTIME_VAR_FAIL;
        }

        pVar->key = varObj.value("key").toString();
        if(pVar->key.isEmpty())
        {
            errStrOut = "variable:key error";
            WriteInfoLog("JsonRuntimeViewVarStorage::Load:"+errStrOut);
            goto PARSE_RUTIME_VAR_FAIL;
        }
        pVar->value = varObj.value("value").toString();//值可以为空
    }
    //文件中可以没有变量
    return true;
PARSE_RUTIME_VAR_FAIL:
    //释放内存 填写出错信息
    while(!rtVarsOut.isEmpty())
        delete rtVarsOut.takeFirst();

    return false;
}

bool JsonRuntimeViewVarStorage::Save(const QString & fileName, QList<RuntimeViewVariable *> & rtVarsIn, QString & errStrOut)
{
    QJsonObject root;
    QJsonArray varArray;

    int size = rtVarsIn.size();
    for(int i=0;  i<size; i++)
    {
        RuntimeViewVariable * pVar = rtVarsIn[i];
        if(!pVar)
        {
            errStrOut = "variable is empty";
            WriteInfoLog("JsonRuntimeViewVarStorage::Save:"+errStrOut);
            return false;
        }

        QJsonObject ele;
        ele.insert("key", pVar->key);
        ele.insert("value", pVar->value);
        varArray.insert(i, ele);
    }

    SaveJosnFileHead(root, 1, "DBQuery");
    root.insert("vars", varArray);

    return WriteJsonObjectToFile(fileName, root);
}
}
