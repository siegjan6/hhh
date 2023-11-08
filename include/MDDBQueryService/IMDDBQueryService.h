#ifndef IDBQUERYSERVICE_H
#define IDBQUERYSERVICE_H

/*!

\author bHVvY2hhbw==

\section 功能
1.对外接口类--抽像类。
2.IMDDBResultNotify类负责数据库查询返回结果。
3.IMDConditionItem类为查询单个条件。
4.IMDCondition类为查询条件管理器。
5.IMDDBQueryParam类为查询服务参数。
6.IMDDBQueryService类来查询服务器。

\section 修改历史
2016.9 / c2pj / 实现同一个数据库实例中多库多表union操作；
2016.9 / c2pj / 添加IMDDBQueryParam获得字符值的接口
*/

#include <QtCore/qglobal.h>
//#include <QSqlRecord>
#include <qvariant.h>
#include <QList>
#include "../../include/MDDataArchivesInterface/IMDDataArchives.h"

class IMDVariable;
class IMDRTDBManager;
class IMDCondition;
class IMDDataBaseParam;
class CMDDatabaseRecordset;

/*!
\section 查询条件操作符
*/
namespace DB_QUERY {

//======================== DB_OPERATER begin ===============================
enum DB_OPERATER
{
    Oper_Undefine		= -1,  // 未定义
    Oper_Big			= 0,   // >
    Oper_Small			= 1,   // <
    Oper_Equal			= 2,   // =
    Oper_Big_Equal		= 3,   // >=
    Oper_Small_Equal	= 4,   // <=
    Oper_Not_Equal		= 5    // !=
};


/*!
\section 查询模式
*/
//======================== query mode begin ===============================
enum DB_QUERY_MODE
{
    Invalid_Mode        = -1,  //无用模式
    Where_Condi_Mode    = 0,   //where条件模式
    Combination_Mode    = 1,   //组合模式
    SQL_Mode            = 2    //SQL模式
};

}

//======================== IMDDBResultNotify begin ===============================
/*!
\section 查询回调类--需要得到查询结果，必须通过派生类实现方法来获取结果
*/
class IMDDBResultNotify
{
public:
    explicit IMDDBResultNotify(){}
    virtual ~IMDDBResultNotify(){}

    /*!
    \section 设置属性，回调
    */
public:
    virtual void NotityDBQueryResult(const QString &strQueryID, CMDDatabaseRecordset *) = 0;
};

//======================== IMDConditionItem begin ===============================
/*!
\section 条件项
*/
class IMDConditionItem
{
public:    
    virtual ~IMDConditionItem(){}
    virtual IMDConditionItem* clone() = 0;

    /*!
    \section 属性
    */
public:
    virtual void setObjName(const QString &strName) = 0; //! 设置对象名(字段名)

    //! 与或标识m_iOrAnd = 0为'与'操作，m_iOrAnd = 1时为'或'操作
    //! 标识与前一条件的关系，是与关系还是或关系
    virtual void setOrAnd(int iMode) = 0;
    virtual void setOperater(const DB_QUERY::DB_OPERATER &iOperater) = 0; //! 设置操作符
    virtual void setOperData(const QVariant	nData) = 0; //! 设置操作数

};

//======================== IMDCondition begin ===============================
/*!
\section 查询条件管理器
*/
class IMDCondition
{
public:
    virtual ~IMDCondition(){}
    virtual IMDCondition* clone() = 0;

    /*!
    \section 操作条件项
    */
public:   
    virtual bool addConditionItem(IMDConditionItem *pItem) = 0;
    virtual bool removeConditionItem(IMDConditionItem *pItem) = 0;
    virtual void removeAllConditionItem() = 0;

    /*!
    \section 属性
    */
public:
    //! 当m_bAdvanced = true 时，存放的条件为链表
    virtual void setAdvanced(bool bAdvance) = 0;
    virtual void setobjName(const QString &strName) = 0; //! 设置对象名(字段名)
    virtual void setOperater(const DB_QUERY::DB_OPERATER &iOperater) = 0; //! 设置操作符
    virtual void setOperData(QVariant	vData) = 0; //! 设置操作数
};

//======================== IMDDBQueryParam begin ===============================
/*!
\section 查询参数配置
*/
class IMDDBQueryParam
{
public:
    virtual ~IMDDBQueryParam(){}
    virtual IMDDBQueryParam *clone() = 0;

    /*!
    \section 属性
    */
public:
    virtual int pageRows() = 0;
    virtual void setPageRows(int Rows) = 0; //! 设置每页记录数
    virtual DB_QUERY::DB_QUERY_MODE queryMode() = 0;
    virtual void setQuerytable(const QString &strTable) = 0; //! 设置表名
    virtual QString queryTable() = 0;
    virtual void setQueryMode(DB_QUERY::DB_QUERY_MODE) = 0; //! 设置查询模式
    virtual QString queryID() = 0;
    virtual void setQueryID(const QString &strQueryID) = 0; //! 设置查询 ID
    virtual QList<QString> fieldList() = 0;
    virtual void setFieldList(QList<QString>) = 0; //! 设置字段名链表
    virtual IMDCondition *queryCondition() = 0;
    virtual void setQueryCondition(IMDCondition *pCondition) = 0; //! 设置查询条件
    virtual QString querySQL() = 0;
    virtual void setQuerySQL(const QString &strSQL) = 0; //! 设置查询SQL语句
    virtual QString queryWhereConditionstr() = 0;
    virtual void setQueryWhereConditionstr(const QString &) = 0; //! 设置查询where
    virtual QString dataSourceName() = 0;
    virtual void setDataSourceName(const QString &) = 0; //! 设置数据源名字
   virtual  IMDDBResultNotify *queryResultNotify() = 0;
    virtual void setQueryResultNotify(IMDDBResultNotify *) = 0; //! 设置回调指针
    virtual IMDDataBaseParam *DBParam() = 0;
    //! 设置数据库参数用于查询非本项目保存的数据库
    //! c2pj:内部会复制一份pDBParam.所以调用方无需克隆一份传给此函数.如果这样做，会导致内存泄漏。
    virtual void setDBParam(IMDDataBaseParam * pDBParam) = 0;
    virtual bool queryExternal() = 0;
    virtual void setQueryExternal(bool) = 0; //! 设置查询非项目配置的数据库标志
};

//======================== IMDDBQueryService begin ===============================
/*!
\section 查询服务器
*/
class IMDDBQueryService
{
public:
    virtual ~IMDDBQueryService(){}

    /*!
    \section 功能--启动停止，中断与恢复，查询命令
    */
public:
    virtual bool start() = 0; //! 启动服务
    virtual bool stop() = 0; //! 停止服务
    virtual bool breakoffQuery() = 0; //! 中断查询
    virtual bool recoverQuery() = 0; //! 恢复中断
    virtual bool query(IMDDBQueryParam *, IMDDataArchives *) = 0; //! 增加查询命令           
    virtual bool query(IMDDBQueryParam *pQueryParam) = 0;
    virtual bool queryUnion(const QList<IMDDBQueryParam *> & pQueryParams, IMDDataArchives *pDataArchives) = 0;//!实现多表union操作 c2pj
    virtual bool queryUnion(const QList<IMDDBQueryParam *> & pQueryParams) = 0;
};

#endif // IDBQUERYSERVICE_H
