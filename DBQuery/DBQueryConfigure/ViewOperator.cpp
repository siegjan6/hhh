#include "MDDBQueryConfigure.h"
#include "utils.h"
#include <QSet>
#include <QMap>
#include <memory>
#include <QStringBuilder>
#include <QDir>

namespace DBQ
{
class FieldTypeMapHelper
{
public:
    FieldTypeMapHelper();
    QMap<QString, VIEW_FIELD_TYPE> siMap;
    QMap<VIEW_FIELD_TYPE, QString> isMap;
};

FieldTypeMapHelper::FieldTypeMapHelper()
{
    siMap["int"] = VIEW_FIELD_TYPE_INT32;
    siMap["string"] = VIEW_FIELD_TYPE_STRING;
    siMap["real"] = VIEW_FIELD_TYPE_DOUBLE;
    siMap["datetime"] = VIEW_FIELD_TYPE_DATETIME;
    siMap["bool"] = VIEW_FIELD_TYPE_BOOL;
    siMap["sbyte"] = VIEW_FIELD_TYPE_SBYTE;
    siMap["byte"] = VIEW_FIELD_TYPE_BYTE;
    siMap["int16"] = VIEW_FIELD_TYPE_INT16;
    siMap["uint16"] = VIEW_FIELD_TYPE_UINT16;
    siMap["int32"] = VIEW_FIELD_TYPE_INT32;
    siMap["uint32"] = VIEW_FIELD_TYPE_UINT32;
    siMap["int64"] = VIEW_FIELD_TYPE_INT64;
    siMap["uint64"] = VIEW_FIELD_TYPE_UINT64;
    siMap["float"] = VIEW_FIELD_TYPE_FLOAT;
    siMap["double"] = VIEW_FIELD_TYPE_DOUBLE;

    isMap[VIEW_FIELD_TYPE_INT32] = "int32";
    isMap[VIEW_FIELD_TYPE_STRING] = "string";
    isMap[VIEW_FIELD_TYPE_DOUBLE] = "double";
    isMap[VIEW_FIELD_TYPE_DATETIME] = "datetime";
    isMap[VIEW_FIELD_TYPE_BOOL] = "bool";
    isMap[VIEW_FIELD_TYPE_SBYTE] = "sbyte";
    isMap[VIEW_FIELD_TYPE_BYTE] = "byte";
    isMap[VIEW_FIELD_TYPE_INT16] = "int16";
    isMap[VIEW_FIELD_TYPE_UINT16] = "uint16";
    isMap[VIEW_FIELD_TYPE_INT32] = "int32";
    isMap[VIEW_FIELD_TYPE_UINT32] = "uint32";
    isMap[VIEW_FIELD_TYPE_INT64] = "int64";
    isMap[VIEW_FIELD_TYPE_UINT64] = "uint64";
    isMap[VIEW_FIELD_TYPE_FLOAT] = "float";
}

static FieldTypeMapHelper ftmHellper;

View::~View()
{
   qDeleteAll(fields);
   FilterTree_Delete(0, pFilter);
}

//函数功能:由视图获得SQL模板.模板中包含视图变量.
DBQUERYCONFIGURE_EXPORT bool View_CreateSQLTemplate(View * pView, const QString & dbName, QString & sqlTemplateOut, QString & errStrOut)
{
    if(!pView)
    {
        WriteInfoLog("View_CreateSQLTemplate:view is null");
        return false;
    }

    int size = pView->fields.size();
    if(0 == size)
    {
        WriteInfoLog("View_CreateSQLTemplate:the count fo fields is 0");
        return false;
    }

    //<生成select分句>
    QString selectStr("select ");
    //pView->fields必须是List或Vector 不能是Map等排序容器
    //要保证生成select field1, field2,...的字段顺序与文件中定义的顺序相同
    //不然的话 用户无法使用选择集.
    for(int i=0; i<size; i++)
    {
        ViewField * pField = pView->fields[i];
        if(!pField)
        {
            WriteInfoLog("View_CreateSQLTemplate:field is null");
            return false;
        }

        if(pField->fieldName.isEmpty())
        {
            WriteInfoLog("View_CreateSQLTemplate:field name is empty");
            return false;
        }
        if(pField->tableName.isEmpty())
        {
            WriteInfoLog("View_CreateSQLTemplate:table name is empty");
            return false;
        }

        selectStr = selectStr%pField->tableName%"."%pField->fieldName;
        //selectStr.append(pField->tableName).append(".").append(pField->name); //生成字段

        if(pField->alias.isEmpty()) //字段别名
            selectStr.append(",");
        else
            selectStr = selectStr%" as "%pField->alias%",";
            //selectStr.append(" as ").append(pField->alias).append(",");
    }
    //程序运行到这里 selectStr一定不为空 且最后一个字符为","  去掉","
    selectStr = selectStr.left(selectStr.size()-1);
    //<生成select分句>

     //<生成from分句>
    //前提:一定有至少一个table 且table不为空
    //上面的代码 保证了这一前提假设
    QSet<QString> tableSet; //利用QSet自动去掉重复值的特性 从字段中找出所有表
    foreach (ViewField * pField, pView->fields) {
        if(!pField->tableName.isEmpty())
            tableSet.insert(pField->tableName);
    }
    if(tableSet.isEmpty())
    {
        WriteInfoLog("View_CreateSQLTemplate:table set is empty");
        return false;
    }

    QString fromStr("from ");

    if(dbName.isEmpty())
    {
        foreach(const QString & tbName, tableSet)
            fromStr.append(tbName).append(",");
    }
    else
    {
        foreach(const QString & tbName, tableSet)
            fromStr.append(dbName).append(".").append(tbName).append(",");
    }
    //去掉最后一个","
    fromStr = fromStr.left(fromStr.size() - 1);
    //</生成from分句>

    QString whereStr;
    if(!FilterTree_CreateWhereClause(pView->pFilter, whereStr, errStrOut))
    {
        WriteInfoLog("View_CreateSQLTemplate:cause error when create where clause");
        return false;
    }

    if(whereStr.isEmpty())
        sqlTemplateOut = selectStr%" "%fromStr;
    else
        sqlTemplateOut = selectStr%" "%fromStr%" "%whereStr;
        //sqlTemplateOut = selectStr+" "+fromStr+" "+whereStr;
    return true;
}

static bool GetViewNameFromFileName(const QString & strIn, QString & viewNameOut)
{
    //前提假设:文件名strIn以.json结尾.
    QString str = strIn;
    int index = str.indexOf("view_", 0, Qt::CaseInsensitive); //文件名必须以view_t开始
    if(index != 0)
        return false;

    if(str.indexOf("run_view_vars")>=0)
        return false;

    //Test:str.clear(); 不会崩溃
    str = str.right(str.size()-5);//6==view_ 去掉view_
    str = str.left(str.size()-5); //4=.json 去掉.json
    if(str.isEmpty())
        return false;

    viewNameOut = str;
    return true;
}

DBQUERYCONFIGURE_EXPORT bool View_GetAllViewNameFromDir(const QString & dirStr, QStringList & viewNamesOut)
{
    QDir dir(dirStr);
    if (!dir.exists())
    {
        WriteInfoLog("GetAllViewNameFromDir:"+dirStr+" hasn't exist");
        return false;
    }

    //遍历文件名
    QStringList filters;
    filters << QString("*.%1").arg("json") ;
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files); // | QDir::NoDotAndDotDot

    QStringList m_CaseNameList = dir.entryList();

    //从文件名中读取视图名称 添加到表格中
    int size = m_CaseNameList.size();
    QString viewName;
    for(int i=0; i<size;i++)
    {
        if(GetViewNameFromFileName(m_CaseNameList[i], viewName))
            viewNamesOut.append(viewName);
    }

    return true;
}

//返回值:成功 返回类型对应的字符串 不成功 返回unknown
DBQUERYCONFIGURE_EXPORT QString View_GetFieldTypeStringFromType(VIEW_FIELD_TYPE type)
{
    QMap<VIEW_FIELD_TYPE, QString>::iterator ite = ftmHellper.isMap.find(type);
    if(ite == ftmHellper.isMap.end())
        return "unknown";

    return ite.value();
}

//函数功能:返回字符串对应的字段类型
//返回值:成功 返回字符串对应的类型 不成功 返回unknown类型
DBQUERYCONFIGURE_EXPORT VIEW_FIELD_TYPE View_GetFieldTypeFromString(QString & typeStr)
{    
    if(typeStr.isEmpty())
        return VIEW_FIELD_TYPE_UNKNOWN;

    QMap<QString, VIEW_FIELD_TYPE>::iterator ite = ftmHellper.siMap.find(typeStr.toLower());
    if(ite == ftmHellper.siMap.end())
        return VIEW_FIELD_TYPE_UNKNOWN;

    return ite.value();
}

/*
DBQUERYCONFIGURE_EXPORT View * View_Create(const QString & viewName)
{
    DBQ::View * pView = new DBQ::View;
    pView->viewName = viewName;
    return pView;
}
*/

DBQUERYCONFIGURE_EXPORT bool View_AddField(View * pView, ViewField * pField)
{
    if(0==pView || 0==pField)
        return false;
    if(pField->fieldName.isEmpty() || pField->tableName.isEmpty())
        return false;

    pView->fields.append(new ViewField(*pField));
    return true;
}

DBQUERYCONFIGURE_EXPORT bool View_ReplaceDBSources(View * pDestView, View * pSourceView)
{
    if(0==pDestView || 0==pSourceView)
        return false;

    pDestView->dbSources = pSourceView->dbSources;
    return true;
}

DBQUERYCONFIGURE_EXPORT ViewField * View_GetFieldByTableFieldName(View * pViewIn, const QString & tbFieldName)
{
    if(!pViewIn)
        return 0;

    QStringList tbAndField = tbFieldName.split('.');
    if(tbAndField.size() != 2)
        return 0;

    foreach(DBQ::ViewField * pField, pViewIn->fields)
    {
        if(!pField)
        {
            WriteInfoLog("View_GetFieldByTableFieldName:field is null");
            continue;
        }

        if(pField->tableName.compare(tbAndField[0], Qt::CaseInsensitive) == 0)
        {
            if(pField->fieldName.compare(tbAndField[1], Qt::CaseInsensitive) == 0)
            {
                return pField;
            }
        }
    }

    return 0;
}

//函数功能:删除View中配置数据库 如果删除完数据库之后 对应的数据源没有数据库了 则自动删除数据源
DBQUERYCONFIGURE_EXPORT bool View_DeleteDB(const QString & viewPath, const QString & viewName, const QString & dsName, const QString & dbName)
{
    //加载view 获得view中配置的数据源
    View * pView = 0;
    QString errStr;
    if(!View_LoadFromDir(viewPath, viewName, &pView, errStr))
    {
        WriteInfoLog("View_DeleteDBInDir:"+errStr);
        return false;
    }

    std::unique_ptr<View> viewDeleter(pView);

    bool bFound = false;
    for(int i=0; i<pView->dbSources.size(); i++)
    {
        if(pView->dbSources[i].dbSrcName.compare(dsName, Qt::CaseInsensitive) != 0)
            continue;

        for(int j=0; j<pView->dbSources[i].dbNames.size(); j++)
        {
            if(pView->dbSources[i].dbNames[j].compare(dbName, Qt::CaseInsensitive) == 0)
            {
                pView->dbSources[i].dbNames.removeAt(j);
                bFound = true;
                break;
            }
        }
    }//end for

    if(!bFound)
        return false;

    //如果dbNames为空 则移除对应的数据源
    //TODO:待测试
    int dbSrcCount = pView->dbSources.size();
    for(int i=0; i<dbSrcCount; i++)
    {
        if(pView->dbSources[i].dbNames.size() == 0)
        {
            pView->dbSources.removeAt(i);
            dbSrcCount--;
            i--;
        }
    }

    if(!View_WriteToDir(viewPath, viewName, pView, errStr))
    {
        WriteInfoLog("View_DeleteDBInDir:"+errStr);
        return false;
    }

    return true;
}

DBQUERYCONFIGURE_EXPORT bool View_AddDB(const QString & viewPath, const QString & viewName, const QString & dsName, const QString & dbName)
{
    //加载view 获得view中配置的数据源
    View * pView = 0;
    QString errStr;
    if(!View_LoadFromDir(viewPath, viewName, &pView, errStr))
    {
        WriteInfoLog("View_AddDBInDir:"+errStr);
        return false;
    }

    std::unique_ptr<View> viewDeleter(pView);

    bool bFound = false;
    for(int i=0; i<pView->dbSources.size(); i++)
    {
        if(pView->dbSources[i].dbSrcName.compare(dsName, Qt::CaseInsensitive) != 0)
            continue;

        for(int j=0; j<pView->dbSources[i].dbNames.size(); j++)
        {
            if(pView->dbSources[i].dbNames[j].compare(dbName, Qt::CaseInsensitive) == 0)
            {
                pView->dbSources[i].dbNames.removeAt(j);
                bFound = true;
                break;
            }
        }
    }//end for

    if(bFound) //要添加的数据源.数据源存在于view中 不能添加
    {
        return false;
    }

    bFound = false;
    for(int i=0; i<pView->dbSources.size(); i++)
    {
        if(pView->dbSources[i].dbSrcName.compare(dsName, Qt::CaseInsensitive) == 0)
        {
            //原有的数据源里 有dsName 则将数据源添加到对应的数据源中
            pView->dbSources[i].dbNames.append(dbName);
            bFound = true;
            break;
        }
    }//end for

    if(!bFound) //说明view的数据源里 没有此数据源 那么就要新建数据源
    {
        ViewDBSource viewDBS;
        viewDBS.dbSrcName = dsName;
        viewDBS.dbNames.append(dbName);
        pView->dbSources.append(viewDBS);
    }

    if(!View_WriteToDir(viewPath, viewName, pView, errStr))
    {
        WriteInfoLog("View_AddDBInDir:"+errStr);
        return false;
    }

    return true;
}
}//end namespace
