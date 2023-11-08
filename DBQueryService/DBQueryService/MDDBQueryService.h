#ifndef MDDBQUERYSERVICE_H
#define MDDBQUERYSERVICE_H

/*!

\author bHVvY2hhbw==

\section 功能
1.内部实现类。
2.IMDDBResultNotify类具体实现。
3.IMDConditionItem类具体实现。
4.IMDCondition类具体实现。
5.IMDDBQueryParam类具体实现。
6.IMDDBQueryService类具体实现。

\section 修改历史
2016.9 / c2pj / 实现同一个数据库实例中多库多表union操作；
*/

#include <QMutex>
#include "MDDBQueryService/IMDDBQueryService.h"

class CDBQueryResult;
class CConditionItem;
class CConditionon;
class IMDDataBaseParam;
class CDBQueryService;
class CDBQueryParam;
class CThread;
class CDBQueryCommand;
class CMDQueryDatabaseThread;

#define RELATION_AND                                        0
#define RELATION_OR                                         1

//======================== CMDConditionItem begin ===============================
/*!
\section 条件项
*/
class CMDConditionItem : public IMDConditionItem
{
public:
    CMDConditionItem();
    virtual ~CMDConditionItem();    
    IMDConditionItem* clone();
    void operator=(const CMDConditionItem &otherObj);

    /*!
    \section 属性
    */
public:
    //! 设置/获取对象名(字段名)
    inline QString objName(){return m_strName;}
    inline void setObjName(const QString &strName){m_strName = strName;}

    //! 与或标识m_iOrAnd = 0为'与'操作，m_iOrAnd = 1时为'或'操作
    //! 标识与前一条件的关系，是与关系还是或关系
    //! 设置/获取条件之间的关系
    inline int getOrAnd(){return m_iRelation;}
    inline void setOrAnd(int iMode){m_iRelation = iMode;}

    //! 设置/获取操作符
    inline DB_QUERY::DB_OPERATER getOperater(){return m_iOperater;}
    inline void setOperater(const DB_QUERY::DB_OPERATER &iOperater){m_iOperater = iOperater;}

    //! 设置/获取操作数
    inline QVariant getOperData(){return m_varData;}
    inline void setOperData(const QVariant	vData){m_varData = vData;}

private:
    QString m_strName                   ;                       //! 字段名
    int m_iRelation                     ;                       //! 条件之间关系
    DB_QUERY::DB_OPERATER m_iOperater   ;  //! 操作符
    QVariant m_varData;                                             //! 值
};

//======================== CMDCondition begin ===============================
/*!
\section 查询条件管理器
*/
class CMDCondition : public IMDCondition
{
public:
    CMDCondition();
    virtual ~CMDCondition();
    void operator=(const CMDCondition &otherObj);
    IMDCondition *clone();

    /*!
    \section 操作条件项
    */
public:
    inline bool addConditionItem(IMDConditionItem *pItem)
    {
        bool success = false;
        if (pItem)
        {
            m_pListCondItem.append(pItem);
            success = true;
        }
        return success;
    }
    bool removeConditionItem(IMDConditionItem *pItem);
    void removeAllConditionItem();

    inline QList<IMDConditionItem*> getItemList(){return m_pListCondItem;} //! 获取所有条件项
    inline int getItemListCount(){return m_pListCondItem.count();} //! 获取条件个数

    //! 通过指定位置返回条件项
    inline IMDConditionItem* findConditionItem(int iPos)
    {
        IMDConditionItem *pCondItem = NULL;
        if (0 <= iPos && iPos < m_pListCondItem.count())
        {
            pCondItem = m_pListCondItem.at(iPos);
        }
        return pCondItem;
    }

    /*!
    \section 属性
    */
public:
    //! 当m_bAdvanced = true 时，存放的条件为链表
    inline bool isAdvanced(){return m_bAdvanced;}
    inline void setAdvanced(bool bAdvance){m_bAdvanced = bAdvance;}     

    //! 设置/获取对象名(字段名)
    inline QString objName(){return m_strName;}
    inline void setobjName(const QString &strName){m_strName = strName;}

    //! 设置/获取操作符
    inline DB_QUERY::DB_OPERATER getOperater(){return m_iOperater;}
    inline void setOperater(const DB_QUERY::DB_OPERATER &iOperater){m_iOperater = iOperater;}

    //! 设置/获取操作数
    inline QVariant operData(){return m_varData;}
    inline void setOperData(QVariant vData){m_varData = vData;}

private:
    bool m_bAdvanced                           ;                   //! 高级标识
    QString m_strName                          ;                      //! 字段名
    DB_QUERY::DB_OPERATER m_iOperater          ; //! 操作符
    QVariant m_varData;                                                   //! 值

    QList<IMDConditionItem*> m_pListCondItem;                            //! 条件项链表
};

//======================== CMDDBQueryParam begin ===============================
/*!
\section 查询参数配置
*/
class CMDDBQueryParam : public IMDDBQueryParam
{
public:
    CMDDBQueryParam();
    virtual ~CMDDBQueryParam();
    void operator= (const CMDDBQueryParam &otherObj);
    IMDDBQueryParam *clone();

    /*!
    \section 属性
    */
public:
    //! 设置/获取数据库参数
    void setDBParam(IMDDataBaseParam *pDBParam);
    inline IMDDataBaseParam *DBParam(){return m_pDBParam;}

    //! 设置/获取每页记录数
    inline void setPageRows(int Rows){m_iRows = Rows;}
    inline int pageRows(){return m_iRows;}

    //! 设置/获取查询模式
    inline DB_QUERY::DB_QUERY_MODE queryMode(){return m_iMode;}
    inline void setQueryMode(DB_QUERY::DB_QUERY_MODE iMode){m_iMode = iMode;}

    //! 设置/获取表名
    inline QString queryTable(){return m_strTabName;}
    inline void setQuerytable(const QString &strTable){m_strTabName = strTable;}

    //! 设置/获取查询 ID
    inline void setQueryID(const QString &strQueryID){m_strQueryID = strQueryID;}
    inline QString queryID(){return m_strQueryID;}

    //! 设置/获取字段名链表
    inline void setFieldList(QList<QString> pListField)
    {
        m_pListField.clear();
        m_pListField = pListField;
    }
    inline QList<QString> fieldList(){return m_pListField;}

    //! 设置/获取查询条件
    inline IMDCondition *queryCondition(){return m_pCond;}
    inline void setQueryCondition(IMDCondition *pCondition){m_pCond = pCondition;}

    //! 设置/获取查询SQL语句
    inline QString querySQL(){return m_strSQLStr;}
    inline void setQuerySQL(const QString &strSQL){m_strSQLStr = strSQL;}

    //! 设置/获取查询where
    inline QString queryWhereConditionstr(){return m_strWhereCond;}
    inline void setQueryWhereConditionstr(const QString &strWhere){m_strWhereCond = strWhere;}

    //! 设置/获取数据源名字
    inline void setDataSourceName(const QString &strDataSrcNmae){m_strDataSrcNmae = strDataSrcNmae;}
    inline QString dataSourceName(){return m_strDataSrcNmae;}

    //! 设置/获取回调指针
    inline void setQueryResultNotify(IMDDBResultNotify *pRestNotify){m_pRestNotify = pRestNotify;}
    inline IMDDBResultNotify *queryResultNotify(){return m_pRestNotify;}

    //! 设置/获取查询非项目配置的数据库标志
    inline void setQueryExternal(bool iExternalFlag){m_iExternalFlag = iExternalFlag;}
    inline bool queryExternal(){return m_iExternalFlag;}

private:
    IMDDataBaseParam *m_pDBParam           ;                      //! 数据库参数
    IMDCondition *m_pCond                  ;                      //! 条件
    int m_iRows                            ;                         //! 行数
    QString m_strTabName                   ;                        //! 表名
    QString m_strQueryID                   ;                        //! 查询ID
    QString m_strWhereCond                 ;                        //! where条件
    QString m_strSQLStr                    ;                        //! sql语句
    QString m_strDataSrcNmae               ;                        //! 数据源名
    DB_QUERY::DB_QUERY_MODE m_iMode        ;                       //! 查询模式
    IMDDBResultNotify *m_pRestNotify       ;                      //! 回调指针
    bool m_iExternalFlag                   ;
    QList<QString> m_pListField;                                        //! 字段链表    
};

//======================== CMDDBQueryService begin ===============================
/*!
\section 查询服务器
*/
class CMDDBQueryService : public IMDDBQueryService
{

public:
    CMDDBQueryService();
    virtual ~CMDDBQueryService();

    /*!
    \section 功能--启动停止，中断与恢复，查询命令
    */
public:
    bool start(); //! 启动服务
    bool stop(); //! 停止服务
    bool breakoffQuery(); //! 中断查询
    bool recoverQuery(); //! 恢复中断
    bool query(IMDDBQueryParam *pQueryParam, IMDDataArchives *pDataArchives); //! 增加查询命令
    bool query(IMDDBQueryParam *pQueryParam);
    bool queryUnion(const QList<IMDDBQueryParam *> & pQueryParams, IMDDataArchives *pDataArchives);//!实现多表union操作 c2pj
    bool queryUnion(const QList<IMDDBQueryParam *> & pQueryParams);//!实现多表union操作 c2pj
    /*!
    \section 命令处理
    */
private:
    bool addQueryCmd(CDBQueryCommand *pCmd);
    void removeAllQueryCmd();

    //! 根据查询ID查找CDBQueryCommand对象
    CDBQueryCommand *findQueryCmd(const QString &strQueryID);
    CDBQueryCommand *removeQueryCmdHead();

    /*!
    \section 功能--sql语句拼接，中断处理
    */
private:
    QString calculateSQLString(CMDDBQueryParam *pQueryParam);//! 将传入的数据表、字段、条件组织成SQL语句
    bool calculateSQLStringWithUnion(const QList<IMDDBQueryParam *> & pQueryParams, QString & sqlOut);//!拼接带union的sql语句 c2pj
    QString calculateBeforeWhereSQL(CMDDBQueryParam *pQueryParam, bool bDBNamePrefix);//! 拼接where条件之前的字串
    bool calculateVarianType(const QVariant &vData);//! 判断字段类型用以是否加单引号
    QString getOperator(DB_QUERY::DB_OPERATER oper);//! 获取条件操作符
    QString getRelation(int orAnd);//! 获取条件之间的关系类型
    bool breakoffAndRecoverQuery(bool iFlag);//! 中断 恢复中断接口
    void isEnabledMarks(IMDDataBaseParam *pDBParam, QString &strSql); //! 判定是否加双引号

private:
    int m_iPageRows                            ;     //! 每页行数
    CMDQueryDatabaseThread *m_pQueryThread     ;  //! 查询线程
    QList<CDBQueryCommand*> m_QueryCommandList ;          //! 队列命令
    QMutex m_Mutex_cmd;                                  //! 线程锁
};

#endif // MDDBQUERYSERVICE_H
