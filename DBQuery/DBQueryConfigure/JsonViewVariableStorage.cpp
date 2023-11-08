#include "JsonViewVariableStorage.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "utils.h"

namespace DBQ
{
JsonViewVariableStorage::JsonViewVariableStorage()
{
}

bool JsonViewVariableStorage::Load(const QString & fileName, QList<ViewVariable *> & viewVarsOut, QString & errStrOut)
{
    //<加载文件>
    QByteArray fileContent;
    if(!GetFileContentToByteArray(fileName, fileContent))
    {
        errStrOut = QString("open file:%1 fail").arg(fileName);
        WriteInfoLog("JsonViewVariableStorage::Load:"+errStrOut);
        return false;
    }

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileContent, &error);
    if(error.error != QJsonParseError::NoError)
    {
        errStrOut = QString("cann't parse %1 file").arg(fileName);
        WriteInfoLog("JsonViewVariableStorage::Load:"+errStrOut);
         return false;
    }
    //</加载文件>

    QJsonObject rootObj = jsonDoc.object();
    QJsonValue varsValue = rootObj.value("vars");
    QJsonArray varArray;
    QString typeStr;
    int size = 0;
    if(!varsValue.isArray())
    {
        errStrOut = QString("%1 file:vars is not array").arg(fileName);
        WriteInfoLog("JsonViewVariableStorage::Load:"+errStrOut);
        goto PARSE_FAIL;
    }
    varArray = varsValue.toArray();

    size =varArray.size();

    for(int i=0; i<size; ++i)
    {
        ViewVariable * pVar = new ViewVariable;
        viewVarsOut.push_back(pVar);

        QJsonObject varObj = varArray.at(i).toObject();
        if(varObj.isEmpty()) //数组元素不是对象 则出错
        {
            errStrOut = QString("%1 file:vars is not array").arg(fileName);
            WriteInfoLog("JsonViewVariableStorage::Load:"+errStrOut);
            goto PARSE_FAIL;
        }

        pVar->name = varObj.value("name").toString();
        if(pVar->name.isEmpty())
        {
            errStrOut = QString("%1 file:name is empty").arg(fileName);
            WriteInfoLog("JsonViewVariableStorage::Load:"+errStrOut);
            goto PARSE_FAIL;
        }

        pVar->description = varObj.value("description").toString(); //可以为空

        typeStr = varObj.value("type").toString();
        if(typeStr.isEmpty())
        {
            errStrOut = QString("%1 file:type is empty").arg(fileName);
            WriteInfoLog("JsonViewVariableStorage::Load:"+errStrOut);
            goto PARSE_FAIL;
        }
        pVar->type = GetTypeFromString(typeStr);
        if(VIEW_VARIABLE_TYPE_UNKNOWN == pVar->type )
        {
            errStrOut = QString("%1 file:type unknown").arg(fileName);
            WriteInfoLog("JsonViewVariableStorage::Load:"+errStrOut);
            goto PARSE_FAIL;
        }

        typeStr = varObj.value("valuetype").toString();
        if(typeStr.isEmpty())
        {
            errStrOut = QString("%1 file:valuetype is empty").arg(fileName);
            WriteInfoLog("JsonViewVariableStorage::Load:"+errStrOut);
            goto PARSE_FAIL;
        }
        pVar->valueType = View_GetFieldTypeFromString(typeStr);
        if(VIEW_FIELD_TYPE_UNKNOWN ==  pVar->valueType)
        {
            errStrOut = QString("%1 file:valuetype unknown").arg(fileName);
            WriteInfoLog("JsonViewVariableStorage::Load:"+errStrOut);
            goto PARSE_FAIL;
        }

        if(pVar->type == VIEW_VARIABLE_TYPE_CONST
            || pVar->type == VIEW_VARIABLE_TYPE_FIELD
            || pVar->type == VIEW_VARIABLE_TYPE_FUNC)
        {
            pVar->value = varObj.value("value").toString(); //值可以为空
        }
        else if(pVar->type == VIEW_VARIABLE_TYPE_ENUM)
        {
            if(!ParseEnumValue(varObj, pVar))
            {
                errStrOut = QString("%1 file:parse enum value error").arg(fileName);
                WriteInfoLog("JsonViewVariableStorage::Load:"+errStrOut);
                goto PARSE_FAIL;
            }
        }
        else
            goto PARSE_FAIL;
    }

    return true;
PARSE_FAIL:
    //释放内存
    while(!viewVarsOut.isEmpty())
        delete viewVarsOut.takeFirst();

    return false;
}

bool JsonViewVariableStorage::Save(const QString & fileName, QList<ViewVariable *> & viewVarsIn, QString & errStrOut)
{
    QJsonObject root;
    QJsonArray varArray;

    int size = viewVarsIn.size();
    for(int i=0;  i<size; i++)
    {
        ViewVariable * pVar = viewVarsIn[i];
        if(!pVar)
        {
            errStrOut = QString("%1 file:var is empty").arg(fileName);
            WriteInfoLog("JsonViewVariableStorage::Save:"+errStrOut);
            return false;
        }

        QJsonObject ele; //var 需要放在里面 因为有时会向ele添加枚举对象 之后又不会被释放 造成将之前的枚举对象写入当前对象中
        ele.insert("name", pVar->name);
        ele.insert("type", QString(GetTypeString(pVar->type))); //兼容Qt 5.2
        ele.insert("valuetype", QString(View_GetFieldTypeStringFromType(pVar->valueType))); //兼容Qt 5.2
        ele.insert("value", pVar->value);
        ele.insert("description", pVar->description);
        WriteEnumNode(ele, pVar->evs);
        varArray.insert(i, ele);
    }

    SaveJosnFileHead(root, 1, "DBQuery");
    root.insert("vars", varArray); //QJsonValue(const QJsonArray &a);

    return WriteJsonObjectToFile(fileName, root);
}

bool JsonViewVariableStorage::ParseEnumValue(QJsonObject & varObj, ViewVariable * pVarOut)
{
    ViewVariableEnumValue ev;
    QJsonObject ele; //数组元素
    QJsonValue enumValueValue= varObj.value("enumValue");
    if(!enumValueValue.isArray())
        return false;

    QJsonArray enumValueArray = enumValueValue.toArray();
    int size = enumValueArray.size();
    for(int i=0; i<size; i++)
    {
        ele = enumValueArray.at(i).toObject();
        if(ele.isEmpty())
            return false;

        ev.value = ele.value("v").toString();
        if(ev.value.isEmpty())
            return false;

        ev.desc = ele.value("desc").toString(); //允许为空
        pVarOut->evs.push_back(ev);
    }
    return true;
}

void JsonViewVariableStorage::WriteEnumNode(QJsonObject & ele, QList<ViewVariableEnumValue> & evs)
{
    int size = evs.size();
    if(0 == size)
        return ; //可以没有枚举值

    QJsonArray evArray;
    for(int i=0; i<size; i++)
    {
        QJsonObject evObj;
        if(evs[i].value.isEmpty())
            return ;
        evObj.insert("v", evs[i].value);
        evObj.insert("desc", evs[i].desc); //描述可以为空
        evArray.append(evObj);
    }

    ele.insert("enumValue", evArray);
}
}//end namespace
