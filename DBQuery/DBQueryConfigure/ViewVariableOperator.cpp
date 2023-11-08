#include "MDDBQueryConfigure.h"
#include "utils.h"
#include <memory>

namespace DBQ
{
static bool UpdateViewVariableValueFromRunViewVars(const QString & viewPath, const QString & viewName, QList<DBQ::ViewVariable *> & viewVarsOut);

DBQUERYCONFIGURE_EXPORT DBQ::ViewVariable * ViewVariable_GetVarFromDir(const QString & viewPath, const QString & viewVarName)
{
    //获得视图变量
    QList<ViewVariable *> viewVars;
    QString errStr;

    if(!DBQ::ViewVariable_LoadFromDir(viewPath, viewVars, errStr))
    {
        WriteInfoLog(QString("ViewVariable_GetVarFromDir:fail when load view variable.file %1").arg(viewPath));
        return 0;
    }

    ViewVariable * pVarFound = 0;
    foreach(ViewVariable * pVar, viewVars)
    {
        if(!pVar)
        {
            WriteInfoLog(QString("ViewVariable_GetVarFromDir:variable is null.file %1").arg(viewPath));
            continue;
        }

        if(pVar->name.compare(viewVarName, Qt::CaseInsensitive) == 0)
        {
            pVarFound = new ViewVariable(*pVar);
            break;
        }
    }

    qDeleteAll(viewVars);

    return pVarFound;
}

DBQUERYCONFIGURE_EXPORT QString ViewVariable_GetTableName(ViewVariable * pVar)
{
    if(!pVar)
    {
        WriteInfoLog("ViewVariable_GetTableName:variable is null");
        return "";
    }

    QStringList tokens = pVar->value.split('.');
    if(tokens.size() != 2)
    {
        WriteInfoLog("ViewVariable_GetTableName:format error");
        return "";
    }

    return tokens[0];
}

DBQUERYCONFIGURE_EXPORT QString ViewVariable_GetFieldName(ViewVariable * pVar)
{
    if(!pVar)
    {
        WriteInfoLog("ViewVariable_GetFieldName:variable is null");
        return "";
    }

    QStringList tokens = pVar->value.split('.');
    if(tokens.size() != 2)
    {
        WriteInfoLog("ViewVariable_GetFieldName:format error");
        return "";
    }

    return tokens[1];
}

//调用者负责释放viewVarsOut
DBQUERYCONFIGURE_EXPORT bool ViewVariable_GetVarFromViewWhenRuntime(const QString & viewPath, const QString & viewName, QList<DBQ::ViewVariable *> & viewVarsOut)
{
    View * pView = 0;
    QString errStr;
    if(!DBQ::View_LoadFromDir(viewPath, viewName, &pView, errStr))
    {
        WriteInfoLog("GetViewVarsInViewFromDirWhenRuntime:load view error");
        return false;
    }

    std::unique_ptr<View> viewDeleter(pView);
    //获得where语句 其中包含了视图变量名称
    QString whereClause;
    if(!DBQ::FilterTree_CreateWhereClause(pView->pFilter,  whereClause, errStr))
    {
        WriteInfoLog("GetViewVarsInViewFromDirWhenRuntime:occur error when create where clause");
        return false;
    }

    //在从where语句中获得视图变量名称
    QStringList viewVarNames;
    if(!ExtractTokensWithBound2(whereClause, "[%", "%]", viewVarNames))
    {
        WriteInfoLog("GetViewVarsInViewFromDirWhenRuntime:extract token error");
        return  false;
    }

    if(viewVarNames.isEmpty())
        return false;

    //获得视图变量
    QList<DBQ::ViewVariable *> viewVars;

    if(!DBQ::ViewVariable_LoadFromDir(viewPath, viewVars, errStr))
    {
        WriteInfoLog("GetViewVarsInViewFromDirWhenRuntime:load view variable error");
        return false;
    }

    //查找视图名对应的视图变量
    foreach(DBQ::ViewVariable * pVar, viewVars)
    {
        for(int i=0; i<viewVarNames.size(); i++)
        {
            if(pVar->name.compare(viewVarNames[i], Qt::CaseInsensitive) == 0)
                viewVarsOut.append(new DBQ::ViewVariable(*pVar));
        }
    }

    //<add 2017.2.6>
    if(viewVarsOut.size() != viewVarNames.size())
    {
        WriteInfoLog("GetViewVarsInViewFromDirWhenRuntime:size unqual");
        qDeleteAll(viewVarsOut);
        viewVarsOut.clear();
        return false;
    }
    //</add>
    //以运行时视图变量 更新视图变量
    UpdateViewVariableValueFromRunViewVars(viewPath, viewName, viewVarsOut);
    return true;
}

//函数功能:加载视图对应的运行时视图变量文件，找到视图变量对应的运行时视图变量。如果找到，更新值到视图变量中。
static bool UpdateViewVariableValueFromRunViewVars(const QString & viewPath, const QString & viewName, QList<DBQ::ViewVariable *> & viewVarsOut)
{
    //<从文件中加载运行时视图变量>
    QList<DBQ::RuntimeViewVariable *> rtVars;
    QString errStr;
    if(!DBQ::RuntimeViewVariable_LoadFromDir(viewPath, viewName,  rtVars, errStr))
    {
        WriteInfoLog(QString("UpdateViewVariableValueFromRunViewVars:error when load runtime varialbe.file =%1 view name = %2").arg(viewPath).arg(viewName));
        return false;
    }
    //</从文件中加载运行时视图变量>

    //<更新到过滤条件的视图变量中>
    foreach (DBQ::RuntimeViewVariable * pRunVar, rtVars) {
        if(!pRunVar)
        {
            WriteInfoLog(QString("UpdateViewVariableValueFromRunViewVars:var is empty.file =%1 view name = %2").arg(viewPath).arg(viewName));
            continue;
        }

        if(pRunVar->value.isEmpty())
        {
            WriteInfoLog(QString("UpdateViewVariableValueFromRunViewVars:%1:value is empty.file =%2 view name = %3").arg(pRunVar->key).arg(viewPath).arg(viewName));
           qDeleteAll(rtVars);
            return false;
        }

        foreach(DBQ::ViewVariable * pVar, viewVarsOut)
        {
            if(pVar->name.compare(pRunVar->key, Qt::CaseInsensitive) == 0)
            {
                pVar->value = pRunVar->value;
            }
        }
    }
    //</更新到过滤条件的视图变量中>
    qDeleteAll(rtVars);
    return true;
}
}
