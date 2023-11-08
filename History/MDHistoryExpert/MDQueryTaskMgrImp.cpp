#include "MDQueryTaskMgrImp.h"

#include "MDDatabaseRecordset.h"
#include "MDDBQueryServiceFactory.h"
#include "MDHistoryExpertImp.h"

#include "IMDDatabaseParam.h"
#include "MDDataSourceFactory.h"
#include "IMDDataSourceManager.h"
#include "IMDDataSource.h"

const int MAX_QUERY_TASK_COUNT = 16;
CMDQueryTaskMgrImp::CMDQueryTaskMgrImp()
{
    m_hisExpert = NULL;
    m_curTaskIndex = 0;
}

CMDQueryTaskMgrImp::~CMDQueryTaskMgrImp()
{
    release();
}

/*!
\section 初始化与释放
*/
bool CMDQueryTaskMgrImp::init(CMDHistoryExpertImp* hisExpert)
{
    m_hisExpert = hisExpert;
    m_curTaskIndex = 0;
    m_taskVector.resize(MAX_QUERY_TASK_COUNT);
    for(int i = 0; i < MAX_QUERY_TASK_COUNT; i++)
        m_taskVector[i] = new CMDQueryTask(hisExpert);

    return true;
}

void CMDQueryTaskMgrImp::release()
{
    m_curTaskIndex = 0;
    if(!m_taskVector.isEmpty())
    {
        int count = m_taskVector.size();
        for(int i = 0; i < count; i++)
            delete m_taskVector[i];
        m_taskVector.clear();
    }
}

/*!
\section 设置当前查询任务索引
*/
void CMDQueryTaskMgrImp::setCurTaskIndex(int curTaskIndex)
{
    if(curTaskIndex >= 0)
        m_curTaskIndex = curTaskIndex;
}

int CMDQueryTaskMgrImp::curTaskIndex()
{
    return m_curTaskIndex;
}

/*!
\section 设置 m_curTaskIndex 对应的查询任务参数；
                获取 m_curTaskIndex 对应的查询任务的返回数据。
*/
void CMDQueryTaskMgrImp::copyDataSourceParameter(const QString& dsName)
{
    copyDataSourceParameter(m_curTaskIndex, dsName);
}

void CMDQueryTaskMgrImp::setDataSourceParameter(int paraType, const QString &para)
{
    setDataSourceParameter(m_curTaskIndex, paraType, para);
}

void CMDQueryTaskMgrImp::setQueryTable(const QString &tableName)
{
    setQueryTable(m_curTaskIndex, tableName);
}

void CMDQueryTaskMgrImp::setQueryFilter(const QString &filter)
{
    setQueryFilter(m_curTaskIndex, filter);
}

void CMDQueryTaskMgrImp::setQueryFields(const QString &fields)
{
    setQueryFields(m_curTaskIndex, fields);
}

bool CMDQueryTaskMgrImp::query()
{
    return query(m_curTaskIndex);
}

QVariant CMDQueryTaskMgrImp::value(int row, int col)
{
    return value(m_curTaskIndex, row, col);
}

int CMDQueryTaskMgrImp::rowCount()
{
    return rowCount(m_curTaskIndex);
}

int CMDQueryTaskMgrImp::colCount()
{
    return colCount(m_curTaskIndex);
}

CMDQueryTask* CMDQueryTaskMgrImp::task()
{
    return task(m_curTaskIndex);
}

/*!
\section 设置 taskIndex 对应的查询任务参数；
                获取 taskIndex 对应的查询任务的返回数据。
*/
void CMDQueryTaskMgrImp::copyDataSourceParameter(int taskIndex, const QString& dsName)
{
    CMDQueryTask* t = task(taskIndex);
    if(!t)
        return;

    t->m_para->setDataSourceName(dsName);
    IMDDataSource* ds = (m_hisExpert ? m_hisExpert->dataSource(dsName) : NULL);
    if(ds)
    {
        IMDDataBaseParam* source = ds->dbParam();
        IMDDataBaseParam* copy_of_source = source->clone();
        t->setDBPara(copy_of_source);
    }
}

void CMDQueryTaskMgrImp::setDataSourceParameter(int taskIndex, int paraType, const QString &para)
{
    CMDQueryTask* t = task(taskIndex);
    if(!t)
        return;

    switch ((DataSourceParameters)paraType) {
    case DataSourceParameters::DBType:
        t->m_dbPara->setdbServeType((DATABASEPARAM::DATABASE_TYPE)DBTye_FromString(para));
        break;
    case DataSourceParameters::DBServerName:
        t->m_dbPara->setdbServerName(para);
        break;
    case DataSourceParameters::DBIP:
        t->m_dbPara->setdbServerIP(para);
        break;
    case DataSourceParameters::DBName:
        t->m_dbPara->setdbName(para);
        break;
    case DataSourceParameters::DBUser:
        t->m_dbPara->setdbUser(para);
        break;
    case DataSourceParameters::DBPassword:
        t->m_dbPara->setdbPassword(para);
        break;
    default:
        break;
    }
}

int CMDQueryTaskMgrImp::DBTye_FromString(const QString & DBTypeString)
{
    int nType = -1;
    if (0 == DBTypeString.compare(QString("MYSQL"), Qt::CaseInsensitive)){
        nType = DATABASEPARAM::DB_TYPE_MYSQL;
    } else if (0 == DBTypeString.compare(QString("ORACLE"), Qt::CaseInsensitive)){
        nType = DATABASEPARAM::DB_TYPE_ORACLE;
    } else if (0 == DBTypeString.compare(QString("KINGBASE"), Qt::CaseInsensitive)){
        nType = DATABASEPARAM::DB_TYPE_KINGBASE;
    } else if (0 == DBTypeString.compare(QString("DB2"), Qt::CaseInsensitive)){
        nType = DATABASEPARAM::DB_TYPE_DB2;
    } else if (0 == DBTypeString.compare(QString("SQLITE"), Qt::CaseInsensitive)){
        nType = DATABASEPARAM::DB_TYPE_SQLLITE;
    } else if (0 == DBTypeString.compare(QString("SQLSERVER"), Qt::CaseInsensitive)){
        nType = DATABASEPARAM::DB_TYPE_SQL;
    }
    return nType;
}

void CMDQueryTaskMgrImp::setQueryTable(int taskIndex, const QString &tableName)
{
    CMDQueryTask* t = task(taskIndex);
    if(!t)
        return;
    t->m_para->setQuerytable(tableName);
}

void CMDQueryTaskMgrImp::setQueryFilter(int taskIndex, const QString &filter)
{
    CMDQueryTask* t = task(taskIndex);
    if(!t)
        return;
    t->m_para->setQueryWhereConditionstr(filter);
}

void CMDQueryTaskMgrImp::setQueryFields(int taskIndex, const QString &fields)
{
    CMDQueryTask* t = task(taskIndex);
    if(!t)
        return;
    t->m_fields = fields.split(",", QString::SkipEmptyParts);
    t->m_para->setFieldList(t->m_fields);
}

bool CMDQueryTaskMgrImp::query(int taskIndex)
{
    CMDQueryTask* t = task(taskIndex);
    if(!t)
        return false;

    t->resetResultRecordset();
    t->m_para->setQueryMode(DB_QUERY::Where_Condi_Mode);
    t->m_para->setQueryResultNotify(&t->m_result);
//    if(queryPara){
//        queryPara->setDBParam(m_databaseParam); // connect the new db on the run time.
//        queryPara->setDataSourceName(dataSource.source());
//        queryPara->setQuerytable(dataSource.table());
//        queryPara->setFieldList(fields);
//        queryPara->setQueryMode(DB_QUERY::Where_Condi_Mode);
//        queryPara->setQueryWhereConditionstr(filter); // change to filter
//        queryPara->setQueryResultNotify(notify);

    return (m_hisExpert ? m_hisExpert->queryArchiveData(t->m_para) : false);
}

QVariant CMDQueryTaskMgrImp::value(int taskIndex, int row, int col)
{
    CMDQueryTask* t = task(taskIndex);
    if(!t)
        return QVariant();

    return t->m_result.m_recordSet.value(row, col);
}

int CMDQueryTaskMgrImp::rowCount(int taskIndex)
{
    CMDQueryTask* t = task(taskIndex);
    if(!t)
        return 0;
    return t->m_result.m_recordSet.rowCount();
}

int CMDQueryTaskMgrImp::colCount(int taskIndex)
{
    CMDQueryTask* t = task(taskIndex);
    if(!t)
        return 0;
    return t->m_result.m_recordSet.columnCount();
}

CMDQueryTask* CMDQueryTaskMgrImp::task(int taskIndex)
{
    if(taskIndex >= MAX_QUERY_TASK_COUNT)
        return NULL;

    return m_taskVector[taskIndex];
}
