#ifndef IMD_DB_QUERY_ENGINE_2016_10_27_H
#define IMD_DB_QUERY_ENGINE_2016_10_27_H

/*!
\author sjc

\section 功能
1.对外接口类--抽像类。
2.IMDDBRecordSetNotify类负责数据库查询返回结果。
3.IMDDBQueryEngine类负责数据库查询。
*/
#include <QString>

class CMDDatabaseRecordset;

enum DBQUERY_STATUS
{
    DBQUERY_STATUS_NOEXSIST = 0,      //没有此查询
    DBQUERY_STATUS_QUERYING,           //正在查询
    DBQUERY_STATUS_OK,                        //查询成功
    DBQUERY_STATUS_FAIL                       //查询失败
};

/*!
\section 查询回调类--需要得到查询结果，必须通过派生类实现方法来获取结果
*/
class IMDDBRecordSetNotify
{
public:
    explicit IMDDBRecordSetNotify(){}
    virtual ~IMDDBRecordSetNotify(){}

    /*!
    \section 设置属性，回调
    */
public:
    virtual void NotityDBQueryResult(const QString &strQueryID, CMDDatabaseRecordset *) = 0;
};

/*!
\section 数据库查询引擎接口
*/
class IMDDBQueryEngine
{
public:
    virtual ~IMDDBQueryEngine(){}

    /*!
    \section 功能--启动停止，中断与恢复，查询命令
    */
public:
    //函数功能:开启服务
    //参数说明:
    //configFileDir  数据源配置文件、全局视图变量配置文件、视图文件、运行时视图变量文件所在目录
    virtual bool load(const QString & configFileDir) = 0; //! 启动服务
    virtual bool unload() = 0; //! 停止服务
    virtual bool queryView(const QString & viewName, IMDDBRecordSetNotify * pNotify, QString & queryIDOut) = 0; //查询视图
    virtual QString getViewPath() = 0;
    //函数功能:使用带有缓存机制的查询接口来查询视图 queryID全局唯一
    virtual bool queryViewWithCache(const QString & viewName, QString & queryIDOut)=0;
    //函数功能:通过查询ID来查找查询结果
    virtual CMDDatabaseRecordset *findWithCache(const QString & queryID) = 0;
    //函数功能:通过ID删除
    virtual void deleteByIDWithCache(const QString & queryID)=0;
    //函数功能:更新所有视图缓存
    virtual void updateAllViewCache() = 0;
    //函数功能:查询m_QueryID标识的查询的状态
    virtual DBQUERY_STATUS getQueryStatus(const QString & queryID) = 0;
};
#endif // IMD_DB_QUERY_ENGINE_2016_10_27_H
