#include "ViewMgr.h"
#include <stdlib.h>
#include <QStringList>
#include <QStringBuilder>
#include "DBSourcesMgr.h"
#include "ViewVarMgr.h"
#include "utils.h"

namespace DBQE
{
//ViewMgr变成串行执行  以此来实现线程安全
//ViewMgr没有阻塞操作 调用频率不高 对性能影响小

ViewMgr::ViewMgr()
    :m_pViewVarMgr(0),
     m_pDBSourcesMgr(0)
{
}

void ViewMgr::init(DBSourcesMgr * pDBSourcesMgr, ViewVarMgr * pViewVarMgr, const QString & viewFileDir)
{
    m_pDBSourcesMgr = pDBSourcesMgr;
    m_pViewVarMgr = pViewVarMgr;
    m_viewFileDir = viewFileDir;
}

//函数功能:获得View
//返回值:成功时 非空 失败时 空
ViewAssist * ViewMgr::getView(const QString & viewName)
{
    ViewAssist * pVA=findViewInCache(viewName);

    if(pVA)
        return pVA;

     if(!(pVA=loadViewToCache(viewName)))
     {
         DBQE::WriteInfoLog(QString("ViewMgr::getView:cann't load %1 view.").arg(viewName));
         return 0;
     }

     //from [#db#].table 之后用实际的数据库名替换掉[#db#]
     QString errStr;
     if(!DBQ::View_CreateSQLTemplate(pVA->pView, "[#db#]", pVA->sqlTemplateStr, errStr))
     {
         DBQE::WriteInfoLog(QString("ViewMgr::getView:cann't create SQL Template in %1 view.").arg(viewName));
         deleteViewAssist(viewName);
         return 0;
      }

      //生成过滤条件中的视图变量
      QStringList tokens;
      if(!DBQ::ExtractTokensWithBound2(pVA->sqlTemplateStr, "[%", "%]", tokens))
      {
            DBQE::WriteInfoLog(QString("ViewMgr::getView:it's fail when extract view variable in %1 view.").arg(viewName));
            deleteViewAssist(viewName);
            return 0;
       }

      foreach(const QString & var, tokens)
          pVA->filterViewVarMap.insert(var, new DBQ::ViewVariable); //初始值为空

     //从全局的视图变量列表中获得值 更新过滤条件中的视图变量
    updateFilterVariableValueFromGlobalViewVars(pVA);
    //从此视图对应的运行时视图变量文件中获得值 更新过滤条件中的视图变量
    updateFilterVariableValueFromRunViewVars(pVA);
    return pVA;
}

bool ViewMgr::updateViewCache(const QString & viewName)
{
    deleteViewAssist(viewName);
    ViewAssist * pVA=loadViewToCache(viewName);
    if(!pVA)
    {
        DBQE::WriteInfoLog(QString("ViewMgr::updateViewCache:cann't find view:%1").arg(viewName));
        return false;
    }

     //from [#db#].table 之后用实际的数据库名替换掉[#db#]
     QString errStr;
     if(!DBQ::View_CreateSQLTemplate(pVA->pView, "[#db#]", pVA->sqlTemplateStr, errStr))
     {
         DBQE::WriteInfoLog(QString("ViewMgr::updateViewCache:cann't create SQL Template in %1 view.").arg(viewName));
         deleteViewAssist(viewName);
         return false;
      }

      //生成过滤条件中的视图变量
     QStringList tokens;
     if(!DBQ::ExtractTokensWithBound2(pVA->sqlTemplateStr, "[%", "%]", tokens))
     {
         DBQE::WriteInfoLog(QString("ViewMgr::updateViewCache:it's fail when extract view variable in %1 view.").arg(viewName));
         deleteViewAssist(viewName);
         return false;
     }

     foreach(const QString & var, tokens)
         pVA->filterViewVarMap.insert(var, new DBQ::ViewVariable); //初始值为空

     //从全局的视图变量列表中获得值 更新过滤条件中的视图变量
     updateFilterVariableValueFromGlobalViewVars(pVA);
     //从此视图对应的运行时视图变量文件中获得值 更新过滤条件中的视图变量
     updateFilterVariableValueFromRunViewVars(pVA);
     return true;
}

void ViewMgr::updateAllViewCache(QStringList & viewNames)
{
    for(int i=0; i<viewNames.size(); i++)
        updateViewCache(viewNames[i]);
}

ViewAssist * ViewMgr::findViewInCache(const QString & viewName)
{
    //缓存中是否有此view
    if(m_ViewAssitCacheMap.contains(viewName))
        return m_ViewAssitCacheMap[viewName];

    return 0;
}

bool ViewMgr::checkFilterParameter(const QString & viewName)
{
    ViewAssist * pVA = 0;
    if(!(pVA=findViewInCache(viewName)))
    {
        DBQE::WriteInfoLog(QString("ViewMgr::checkFilterParameter:cann't find view:%1").arg(viewName));
        return false;
    }

    QMapIterator<QString, DBQ::ViewVariable *> ite(pVA->filterViewVarMap);
    while(ite.hasNext())
    {
        ite.next();
        if(ite.value()->name.isEmpty())
        {
            DBQE::WriteInfoLog(QString("ViewMgr::checkFilterParameter:parameter is empty in view %1").arg(viewName));
            return false;
        }
    }
    return true;
}

//函数功能:获得某视图的数据源个数
int ViewMgr::getDBSourcesCount(const QString & viewName)
{
    ViewAssist * pVA=findViewInCache(viewName);

    if(!pVA)
    {
        DBQE::WriteInfoLog(QString("ViewMgr::getDBSourcesCount:cann't find view:%1").arg(viewName));
        return -1;
    }
    if(!pVA->pView)
    {
        DBQE::WriteInfoLog(QString("ViewMgr::getDBSourcesCount:view %1 is null").arg(viewName));
        return -1;
    }

    return pVA->pView->dbSources.size();
}

static const char * UNION_ALL_STR = " union all ";
QString ViewMgr::getSQLFromView(const QString & viewName)
{
    ViewAssist * pVA=findViewInCache(viewName);

    if(!pVA)
    {
        DBQE::WriteInfoLog(QString("ViewMgr::getSQLFromView:cann't find view:%1").arg(viewName));
        return "";
    }
    if(!pVA->pView)
    {
        DBQE::WriteInfoLog(QString("ViewMgr::getSQLFromView:view %1 is null").arg(viewName));
        return "";
    }

    if(pVA->pView->dbSources.isEmpty())
    {
        DBQE::WriteInfoLog(QString("ViewMgr::getSQLFromView:view %1:db source is empty").arg(viewName));
        return "";
    }

    if(pVA->pView->dbSources.size() != 1) //只支持单数据源
    {
        DBQE::WriteInfoLog(QString("ViewMgr::getSQLFromView:view %1:support one db source").arg(viewName));
        return "";
    }

    DBQ::ViewDBSource  dbSrc = pVA->pView->dbSources.at(0);
    if(dbSrc.dbSrcName.isEmpty())
    {
        DBQE::WriteInfoLog(QString("ViewMgr::getSQLFromView:view %1:db source name is empty").arg(viewName));
        return "";
    }

    //<替换视图变量>
    QMapIterator<QString, DBQ::ViewVariable *> ite(pVA->filterViewVarMap);
    QString sqlNoViewVar = pVA->sqlTemplateStr; //sqlTemplateStr在初始化时已验证了正确性.在初始化之后也不会修改.
    QString viewVar;
    while(ite.hasNext())
    {
        ite.next();
        viewVar = "[%" + ite.key()+"%]";
        sqlNoViewVar = sqlNoViewVar.replace(viewVar, ite.value()->value);
    }

    //</替换视图变量>
    //<替换数据库>
    //将[#db#]替换成数据库名
    //查找对应的数据源 判断是否存在表前缀 如果存在 要以"数据库名.表前缀"来替换[#db#]
    //添加表前缀的原因:为了支持MS SQL Server
    QString tablePrefix = m_pDBSourcesMgr->GetTablePrefix(dbSrc.dbSrcName);
    if(dbSrc.dbNames.size() == 1)
    {
        //dbSrc.dbSrcName数据源名称 TODO:重命名
        if(tablePrefix.isEmpty())
            return sqlNoViewVar.replace("[#db#]", dbSrc.dbNames[0], Qt::CaseInsensitive);
        else
        {
            QString dbNameWithTablePrefix = dbSrc.dbNames[0]%"."%tablePrefix;
            return sqlNoViewVar.replace("[#db#]", dbNameWithTablePrefix, Qt::CaseInsensitive);
        }
    }

    //db数据库数量>1 union all操作
    QString strSQL;
    int size = dbSrc.dbNames.size();
    if(tablePrefix.isEmpty())
    {
        for(int i=0;i<size;i++)
        {
            strSQL += sqlNoViewVar.replace("[#db#]", dbSrc.dbNames[i], Qt::CaseInsensitive);
            strSQL.append(UNION_ALL_STR);
        }
    }
    else
    {
        QString dbNameWithTablePrefix;
        for(int i=0;i<size;i++)
        {
            dbNameWithTablePrefix = dbSrc.dbNames[i]%"."%tablePrefix;
            strSQL += sqlNoViewVar.replace("[#db#]", dbNameWithTablePrefix, Qt::CaseInsensitive);
            strSQL.append(UNION_ALL_STR);
        }
    }

    //去掉最后一个UNION_ALL_STR
    strSQL = strSQL.left(strSQL.size()- strlen(UNION_ALL_STR));
    //<替换数据库>
    return strSQL;
}

QString ViewMgr::getSQLFromViewWithDBSource(const QString & viewName, DBQ::DBSource * pDBSource)
{
    if(!pDBSource)
    {
        DBQE::WriteInfoLog(QString("ViewMgr::getSQLFromViewWithDBSource:pDBSource is null in %1 view").arg(viewName));
        return "";
    }

    ViewAssist * pVA=findViewInCache(viewName);

    if(!pVA)
    {
        DBQE::WriteInfoLog(QString("ViewMgr::getSQLFromViewWithDBSource:cann't find' %1 view").arg(viewName));
        return "";
    }

    if(!pVA->pView)
    {
        DBQE::WriteInfoLog(QString("ViewMgr::getSQLFromViewWithDBSource:%1 view is null").arg(viewName));
        return "";
    }

    //获取数据源中的当前数据库 如果没有当前数据库 则出错
    QString defaultDBName;
    for(int i=0; i<pDBSource->dbs.size(); i++)
    {
        if(pDBSource->dbs[i].bDefault)
        {
            defaultDBName = pDBSource->dbs[i].dbName;
            break;
        }
    }//end for

    if(defaultDBName.isEmpty())
    {
        DBQE::WriteInfoLog(QString("ViewMgr::getSQLFromViewWithDBSource:hasn't exist current database in %1 view").arg(viewName));
        return "";
    }

    //<替换视图变量>
    QMapIterator<QString, DBQ::ViewVariable *> ite(pVA->filterViewVarMap);
    QString sqlNoViewVar = pVA->sqlTemplateStr; //sqlTemplateStr在初始化时已验证了正确性.在初始化之后也不会修改.
    QString viewVar;

    while(ite.hasNext())
    {
        ite.next();
        viewVar = "[%" + ite.key()+"%]";
        //如果是字符串类型或时间类型，则在生成SQL模板时就已经加上了单引号。所以不用再加了。
        sqlNoViewVar = sqlNoViewVar.replace(viewVar, ite.value()->value);
    }
    //</替换视图变量>

    //替换数据库
    //将[#db#]替换成数据库名 最终生成了SQL
    QString tablePrefix = pDBSource->tablePrefix;
    if(tablePrefix.isEmpty())
        return sqlNoViewVar.replace("[#db#]", defaultDBName, Qt::CaseInsensitive);

    QString dbNameWithTablePrefix = defaultDBName%"."%tablePrefix;
    return sqlNoViewVar.replace("[#db#]", dbNameWithTablePrefix, Qt::CaseInsensitive);
}

bool ViewMgr::updateFilterVariableValueFromRunViewVars(const QString & viewName)
{
    ViewAssist * pVA = 0;
    if(!(pVA=findViewInCache(viewName)))
    {
        DBQE::WriteInfoLog(QString("ViewMgr::updateFilterVariableValueFromRunViewVars:cann't find view %1").arg(viewName));
        return false;
    }
    return updateFilterVariableValueFromRunViewVars(pVA);
}

ViewAssist * ViewMgr::loadViewToCache(const QString & viewName)
{
    ViewAssist * pVA = 0;

    //TODO:使用封装好的函数
    QString viewXMLFileFullPath = m_viewFileDir;
    viewXMLFileFullPath.append("/view_").append(viewName).append(".json");
    DBQ::View * pView = 0;
    QString errStr;
    if(!DBQ::View_Load(viewXMLFileFullPath, DBQ::FILE_TYPE_JSON, &pView, errStr))
    {
        DBQE::WriteInfoLog(QString("ViewMgr::loadViewToCache:cann't find view %1").arg(viewName));
        return 0;
    }

    pVA = new ViewAssist;
    pVA->pView = pView;
    m_ViewAssitCacheMap[viewName] = pVA;

    return pVA;
}

//函数功能:从全局的视图变量列表中获得值 更新过滤条件中的视图变量
void ViewMgr::updateFilterVariableValueFromGlobalViewVars(ViewAssist * pVA)
{
    //从文件中加载运行时视图变量
    QMapIterator<QString, DBQ::ViewVariable *> ite(pVA->filterViewVarMap);
    DBQ::ViewVariable * pViewVar = 0;
    while(ite.hasNext())
    {
        ite.next();
        if((pViewVar=m_pViewVarMgr->Find(ite.key()))) //找到了
        {
            *pVA->filterViewVarMap[ite.key()] = *pViewVar;
            //如果视图变量的类型为字段类型 则要清空value.
            //因为在文件中的字段类型的视图变量的值为table.field
            //实际的值要动态查询数据库 才能获得.其值只能从运行时视图变量中获得
            //如果没有查数据库 其值自然为空
            if(DBQ::VIEW_VARIABLE_TYPE_FIELD == pViewVar->type )
                pVA->filterViewVarMap[ite.key()]->value.clear();
        }
    }
}

bool ViewMgr::updateFilterVariableValueFromRunViewVars(ViewAssist * pVA)
{
    //<从文件中加载运行时视图变量>
    QList<DBQ::RuntimeViewVariable *> rtVars;
    QString errStr;
    if(!DBQ::RuntimeViewVariable_LoadFromDir(m_viewFileDir,  pVA->pView->viewName, rtVars, errStr))
    {
        return false; //找到不运行时视图变量是正常的.因为用户可能没有配置动态查询参数
    }
    //</从文件中加载运行时视图变量>

    //<更新到过滤条件的视图变量中>
    foreach (DBQ::RuntimeViewVariable * pRunVar, rtVars) {
        if(pRunVar->value.isEmpty())
        {
            DBQE::WriteInfoLog("ViewMgr::updateFilterVariableValueFromRunViewVars:the vaule of runtime view vairable is null");
            qDeleteAll(rtVars);
            return false;
        }

        QMap<QString, DBQ::ViewVariable*>::iterator ite = pVA->filterViewVarMap.find(pRunVar->key);
        if(ite != pVA->filterViewVarMap.end())
           (*ite)->value =  pRunVar->value;
    }
    //</更新到过滤条件的视图变量中>
    qDeleteAll(rtVars);
    return true;
}

void ViewMgr::deleteViewAssist(const QString & viewName)
{

    ViewAssist * pVA = 0;
    if(!(pVA=findViewInCache(viewName)))
        return ;

    m_ViewAssitCacheMap.remove(viewName);
    delete pVA->pView;

    QMapIterator<QString, DBQ::ViewVariable *> ite(pVA->filterViewVarMap);
    while(ite.hasNext())
    {
        ite.next();
        delete ite.value();
    }

    delete pVA;
}

CDataBaseParam * ViewMgr::getDBConnectionParam(const QString & viewName)
{
    ViewAssist * pVA = 0;
    if(!(pVA=findViewInCache(viewName)))
    {
        DBQE::WriteInfoLog(QString("ViewMgr::getDBConnectionParam:cann't find view %1").arg(viewName));
        return 0;
    }
    if(!pVA->pView)
    {
        DBQE::WriteInfoLog(QString("ViewMgr::getDBConnectionParam:view %1 is null").arg(viewName));
        return 0;
    }
    if(pVA->pView->dbSources.isEmpty())
    {
        DBQE::WriteInfoLog(QString("ViewMgr::getDBConnectionParam:db source is empty in view %1").arg(viewName));
        return 0;
    }

    if(pVA->pView->dbSources.size()==0)
    {
        DBQE::WriteInfoLog(QString("ViewMgr::getDBConnectionParam:the count of db source is 0 in view %1").arg(viewName));
        return 0;
    }

    return m_pDBSourcesMgr->GetDBConnectionParam(pVA->pView->dbSources[0].dbSrcName);
}

CDataBaseParam * ViewMgr::getDBConnParamWithDBSourceName(const QString & dbSrcName)
{
    return m_pDBSourcesMgr->GetDBConnectionParam(dbSrcName);
}
}
