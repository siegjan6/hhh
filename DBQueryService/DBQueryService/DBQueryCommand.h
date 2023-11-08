#ifndef DBQUERYCOMMAND_H
#define DBQUERYCOMMAND_H

/*!

\author bHVvY2hhbw==

\section 功能
1.数据库查询命令实现类。
2.处理数据库连接、查询、并回调结果。

\section 修改历史
2016.9 / c2pj / 使用连接缓冲池代替CMDDBConnect 以提供效率 精简代码；
*/

#include <QtCore/qglobal.h>
#include <qstring.h>

#include "MDDBQueryService.h"
#include "MDSysLogExpert.h"
#define LOG_SOURCE "DataArchives"

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

    inline void setDBParam(IMDDataBaseParam *pDBParam){m_pDBParam = pDBParam;} //! 设置数据库参数
    inline void setQueryRslNotify(IMDDBResultNotify *pDBQueryRst){m_pDBQueryRst = pDBQueryRst;} //! 设置回调实现类指针
    //inline CMDDBConnect * getDBConnect(){return m_pDBConnect;} //! 获取数据库连接
    /*!
    \section 对外调用接口--开始查询，运行状态，执行sql语句
    */
public:
    void start();
    bool isStarted();
    void setStarted(bool bStarted);
    void execQuery(); //! 执行查询语句
    /*!
    \section 数据库连接断开以及回调
    */
private:
    void callbackQueryResult(); //! 回调查询结果
private:
    QString m_strQueryID                 ;    //! 查询ID
    QString m_strQueryCommand            ;    //! 查询sql语句
    bool m_isStarted                     ; //! 运行标识
    IMDDataBaseParam *m_pDBParam         ;  //! 数据库参数
    IMDDBResultNotify *m_pDBQueryRst     ;  //! 查询回调指针
};

#endif // DBQUERYCOMMAND_H
