#include "MDQueryTaskMgr.h"
#include "MDQueryTaskMgrImp.h"

CMDQueryTaskMgr::CMDQueryTaskMgr()
{
    m_imp = new CMDQueryTaskMgrImp;
}

CMDQueryTaskMgr::~CMDQueryTaskMgr()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化与释放
*/
bool CMDQueryTaskMgr::init(CMDHistoryExpertImp* hisExpert)
{
    return m_imp->init(hisExpert);
}

void CMDQueryTaskMgr::release()
{
    if(m_imp)
        m_imp->release();
}

/*!
\section 设置当前查询任务索引
*/
void CMDQueryTaskMgr::setCurTaskIndex(int curTaskIndex)
{
    if(m_imp)
        m_imp->setCurTaskIndex(curTaskIndex);
}

int CMDQueryTaskMgr::curTaskIndex()
{
    return m_imp->curTaskIndex();
}

/*!
\section 设置 m_curTaskIndex 对应的查询任务参数；
                获取 m_curTaskIndex 对应的查询任务的返回数据。
*/
void CMDQueryTaskMgr::copyDataSourceParameter(const QString& dsName)
{
    m_imp->copyDataSourceParameter(dsName);
}

void CMDQueryTaskMgr::setDataSourceParameter(int paraType, const QString &para)
{
    m_imp->setDataSourceParameter(paraType, para);
}

void CMDQueryTaskMgr::setQueryTable(const QString &tableName)
{
    m_imp->setQueryTable(tableName);
}

void CMDQueryTaskMgr::setQueryFilter(const QString &filter)
{
    m_imp->setQueryFilter(filter);
}

void CMDQueryTaskMgr::setQueryFields(const QString &fields)
{
    m_imp->setQueryFields(fields);
}

bool CMDQueryTaskMgr::query()
{
    return m_imp->query();
}

QVariant CMDQueryTaskMgr::value(int row, int col)
{
    return m_imp->value(row, col);
}

int CMDQueryTaskMgr::rowCount()
{
    return m_imp->rowCount();
}

int CMDQueryTaskMgr::colCount()
{
    return m_imp->colCount();
}

/*!
\section 设置 taskIndex 对应的查询任务参数；
                获取 taskIndex 对应的查询任务的返回数据。
*/
void CMDQueryTaskMgr::copyDataSourceParameter(int taskIndex, const QString& dsName)
{
    m_imp->copyDataSourceParameter(taskIndex, dsName);
}

void CMDQueryTaskMgr::setDataSourceParameter(int taskIndex, int paraType, const QString &para)
{
    m_imp->setDataSourceParameter(taskIndex, paraType, para);
}

void CMDQueryTaskMgr::setQueryTable(int taskIndex, const QString &tableName)
{
    m_imp->setQueryTable(taskIndex, tableName);
}

void CMDQueryTaskMgr::setQueryFilter(int taskIndex, const QString &filter)
{
    m_imp->setQueryFilter(taskIndex, filter);
}

void CMDQueryTaskMgr::setQueryFields(int taskIndex, const QString &fields)
{
    m_imp->setQueryFields(taskIndex, fields);
}

bool CMDQueryTaskMgr::query(int taskIndex)
{
    return m_imp->query(taskIndex);
}

QVariant CMDQueryTaskMgr::value(int taskIndex, int row, int col)
{
    return m_imp->value(taskIndex, row, col);
}

int CMDQueryTaskMgr::rowCount(int taskIndex)
{
    return m_imp->rowCount(taskIndex);
}

int CMDQueryTaskMgr::colCount(int taskIndex)
{
    return m_imp->colCount(taskIndex);
}
