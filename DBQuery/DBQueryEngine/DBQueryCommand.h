#ifndef DBQUERYCOMMAND_H
#define DBQUERYCOMMAND_H

/*!

\author bHVvY2hhbw==

\section 功能
1.数据库查询命令实现类。
2.处理数据库连接、查询、并回调结果。

\section 修改历史
2016.9 / c2pj / 使用连接缓冲池代替CMDDBConnect 以提供效率 精简代码；
2016.11.1 c2pj 修改此类 使之适应DBQueryEngine的需求
2107.1.5  c2pj 添加CMDDBQueryEngine指针成员 以现实查询状态更新的功能
*/

#include <QtCore/qglobal.h>
#include <QString>
#include <MDDBQueryEngine/IMDDBQueryEngine.h>
#include "MDSysLogExpert.h"
#include "DatabaseParam.h"
#include <MDDBConnect/MDDBConnect.h>
//#define LOG_SOURCE "DBQueryEngine"



//加上命名空间 保证模块之间没有重名的类
//不然，在Linux下，加载so时，会找到多个重名的函数，导致运行时崩溃。
namespace DBQE
{
class CMDDBQueryEngine;
class CDBQueryCommand
{
public:
    CDBQueryCommand(const QString &strQueryID,const QString &strCmd);
    virtual ~CDBQueryCommand();

    /*!
    \section 属性配置，获取属性
    */
public:
    QString getQueryID(); //! 获取查询ID
    QString getQueryCommand(); //! 获取查询语句

    inline void setDBParam(CDataBaseParam *pDBParam){m_pDBParam = pDBParam;} //! 设置数据库参数
    inline void setQueryRslNotify(IMDDBRecordSetNotify *pDBQueryRst){m_pDBQueryRst = pDBQueryRst;} //! 设置回调实现类指针
    inline void setQueryEngine(CMDDBQueryEngine * pQueryEngine){m_pQueryEngine = pQueryEngine;}
    /*!
    \section 对外调用接口--开始查询，运行状态，执行sql语句
    */
public:
    void start(const QString & viewName);
    bool isStarted();
    void setStarted(bool bStarted);
    void execQuery(); //! 执行查询语句
    /*!
    \section 数据库连接断开以及回调
    */
private:
    void callbackQueryResult(); //! 回调查询结果
private:
    QString m_strQueryID;                                             //! 查询ID
    QString m_strQueryCommand;                                 //! 查询sql语句
    bool m_isStarted;                                                     //! 运行标识
    CDataBaseParam *m_pDBParam;                           //! 数据库参数
    IMDDBRecordSetNotify *m_pDBQueryRst;             //! 查询回调指针
    QString m_viewName;                                             //! 视图名称
    CMDDBQueryEngine * m_pQueryEngine;
};
}
#endif // DBQUERYCOMMAND_H
