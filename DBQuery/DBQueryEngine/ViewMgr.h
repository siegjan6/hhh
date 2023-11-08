#ifndef VIEW_MGR_2016_10_28_H
#define VIEW_MGR_2016_10_28_H
#include <QString>
#include <QMap>
#include <MDDBQueryConfigure.h>
#include <QMutex>
#include "DatabaseParam.h"
#include <MDDBConnect/MDDBConnect.h>

namespace DBQE
{
struct ViewAssist
{
    DBQ::View * pView;
    QString sqlTemplateStr; //sql字符串模板 包含视图变量.
    ///过滤参数中的视图变量容器.key为视图变量名.
    QMap<QString, DBQ::ViewVariable *> filterViewVarMap;
};

class ViewVarMgr;
class DBSourcesMgr;
typedef QMap<QString, ViewAssist *> ViewAssistMap;
class ViewMgr
{
public:
    ViewMgr();
    //函数说明:初始化
    //viewFileDir 视图文件所在目录 以绝对路径表示
    void init(DBSourcesMgr * pDBSourcesMgr, ViewVarMgr * pViewVarMgr, const QString & viewFileDir);
    //函数功能:获得View
    //返回值:成功时 非空 失败时 空
    ViewAssist * getView(const QString & viewName);
    //函数功能:检查过滤参数的值是否合法
    bool checkFilterParameter(const QString & viewName);
    //函数功能:获得某视图的数据源个数
    //返回值:数据源个数 -1为出错
    int getDBSourcesCount(const QString & viewName);
    //函数功能:获得某视图的SQL.替换掉视图变量.多数据库查询 则加入数据库名和union all
    //返回值:view对应的SQL  失败则返回空字符串
    QString getSQLFromView(const QString & viewName);
    //函数功能:获得某视图的SQL.替换掉视图变量.要查询的数据源由参数指定.
    //返回值:view对应的SQL  失败则返回空字符串
    QString getSQLFromViewWithDBSource(const QString & viewName, DBQ::DBSource * pDBSource);
    CDataBaseParam * getDBConnectionParam(const QString & dbSourceName);
    CDataBaseParam * getDBConnParamWithDBSourceName(const QString & dbSrcName);
    //函数功能:从此视图对应的运行时视图变量文件中获得值 更新过滤条件中的视图变量
    // 一个视图文件对应0或1个运行时视图变量文件
    bool updateFilterVariableValueFromRunViewVars(const QString & viewName);
    void updateAllViewCache(QStringList & viewNames);
    bool updateViewCache(const QString & viewName);
private:
    ViewAssist * findViewInCache(const QString & viewName);
    ViewAssist * loadViewToCache(const QString & viewName);
    //函数功能:从全局的视图变量列表中获得值 更新过滤条件中的视图变量
    void updateFilterVariableValueFromGlobalViewVars(ViewAssist * pVA);
    //函数功能:从此视图对应的运行时视图变量文件中获得值 更新过滤条件中的视图变量
    // 一个视图文件对应0或1个运行时视图变量文件
    bool updateFilterVariableValueFromRunViewVars(ViewAssist * pVA);
    void deleteViewAssist(const QString & viewName);
private:
    ViewAssistMap m_ViewAssitCacheMap; //视图缓存 保存所有视图
    QString m_viewFileDir; //视图文件所在目录 以绝对路径表示
    ViewVarMgr * m_pViewVarMgr;
    DBSourcesMgr * m_pDBSourcesMgr;
    //QMutex m_Mutex;
};
}

#endif //VIEW_MGR_2016_10_28_H
