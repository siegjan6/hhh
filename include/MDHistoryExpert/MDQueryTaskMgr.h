/*!

\author dGFuZ3p3

\section 功能

查询任务管理类，实现运行时对查询任务的管理。

*/

#ifndef MDQUERYTASKMGR_H
#define MDQUERYTASKMGR_H

#include "MDHistoryExpert_Global.h"
#include <QVariant>

class CMDQueryTaskMgrImp;
class CMDHistoryExpertImp;
class MDHISTORYEXPERTSHARED_EXPORT CMDQueryTaskMgr
{
public:
    CMDQueryTaskMgr();
    ~CMDQueryTaskMgr();

/*!
\section 初始化与释放
*/
public:
    bool init(CMDHistoryExpertImp* hisExpert);
    void release();

/*!
\section 设置当前查询任务索引
*/
public:
    void setCurTaskIndex(int curTaskIndex);
    int curTaskIndex();

/*!
\section 设置 m_curTaskIndex 对应的查询任务参数；
                获取 m_curTaskIndex 对应的查询任务的返回数据。
*/
public:
    void copyDataSourceParameter(const QString& dsName);
    void setDataSourceParameter(int paraType, const QString &para); //! 设置数据库参数
    void setQueryTable(const QString &tableName);
    void setQueryFilter(const QString &filter);
    void setQueryFields(const QString &fields);
    bool query();
public:
    QVariant value(int row, int col);
    int rowCount();
    int colCount();

/*!
\section 设置 taskIndex 对应的查询任务参数；
                获取 taskIndex 对应的查询任务的返回数据。
*/
public:
    void copyDataSourceParameter(int taskIndex, const QString& dsName);
    void setDataSourceParameter(int taskIndex, int paraType, const QString &para); //! 设置数据库参数
    void setQueryTable(int taskIndex, const QString &tableName);
    void setQueryFilter(int taskIndex, const QString &filter);
    void setQueryFields(int taskIndex, const QString &fields);
    bool query(int taskIndex);
public:
    QVariant value(int taskIndex, int row, int col);
    int rowCount(int taskIndex);
    int colCount(int taskIndex);

protected:
    CMDQueryTaskMgrImp* m_imp;
};

#endif // MDQUERYTASKMGR_H
