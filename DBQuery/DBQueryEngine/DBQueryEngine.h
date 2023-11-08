#ifndef DB_QUERY_ENIGNE_2016_11_01_H
#define DB_QUERY_ENIGNE_2016_11_01_H

#include <QMutex>
#include <MDDBQueryEngine/IMDDBQueryEngine.h>
#include <MDDBQueryConfigure.h>
#include <QMap>

//加上命名空间 保证模块之间没有重名的类
//不然，在Linux下，加载so时，会找到多个重名的函数，导致运行时崩溃。
namespace DBQE
{
class CDBQueryCommand;
class CMDQueryDatabaseThread;
class DBSourcesMgr;
class ViewVarMgr;
class ViewMgr;

typedef QMap<QString, DBQUERY_STATUS> DBQueryStatusMap;

/*!
\section 查询引擎
*/
class CMDDBQueryEngine : public IMDDBQueryEngine
{
public:
    CMDDBQueryEngine();
    virtual ~CMDDBQueryEngine();

    /*!
    \section 功能--启动停止，中断与恢复，查询命令
    */
public:
    //函数功能:开启服务
    //参数说明:
    //configFileDir  数据源配置文件、全局视图变量配置文件、视图文件、运行时视图变量文件所在目录
    bool load(const QString & configFileDir); //! 启动服务
    bool unload(); //! 停止服务
    bool queryView(const QString & viewName, IMDDBRecordSetNotify * pResultNotify, QString & queryIDOut);
    QString getViewPath();
    //函数功能:使用带有缓存机制的查询接口来查询视图 queryID全局唯一
    bool queryViewWithCache(const QString & viewName, QString & queryIDOut);
    //函数功能:通过查询ID来查找查询结果
    CMDDatabaseRecordset *findWithCache(const QString & queryID);
    //函数功能:通过ID删除
    void deleteByIDWithCache(const QString & queryID);
    //函数功能:更新所有视图缓存
    void updateAllViewCache();
    //函数功能:查询m_QueryID标识的查询的状态
    DBQUERY_STATUS getQueryStatus(const QString & queryID);
    void setQueryStatus(const QString & queryID, DBQUERY_STATUS status); //基类中无此方法
private:
    bool queryWithOneDBSource(const QString & queryID, const QString & viewName, IMDDBRecordSetNotify *  pQueryResultNotify);
    bool queryWithDefaultDBSource(const QString & queryID, const QString & viewName, IMDDBRecordSetNotify *  pQueryResultNotify);
private:
    DBSourcesMgr * m_pDBSourcesMgr;
    ViewVarMgr * m_pViewVarMgr;
    ViewMgr * m_pViewMgr;
    CMDQueryDatabaseThread *m_pQueryThread     ;                  //! 查询线程
    QList<CDBQueryCommand*> m_QueryCommandList ;          //! 队列命令
    QMutex m_Mutex;                                                                 //! 线程锁
    QString m_ConfigFileDir;
    DBQueryStatusMap m_QueryStatusMap;
};
}
#endif // MDDBQUERYSERVICE_H
