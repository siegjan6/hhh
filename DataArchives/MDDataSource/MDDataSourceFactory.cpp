#include "MDDataSourceFactory.h"
#include "MDDataSourceManager.h"
#include "MDDataSource.h"
#include "MDDataField.h"
#include "MDDataTable.h"
#include "MDDatabaseParam.h"

CMDDataSourceFactory::CMDDataSourceFactory()
{

}

/*!
\section 静态创建数据源管理器
*/
IMDDataSourceManager *CMDDataSourceFactory::createDataSourceManager()
{
    IMDDataSourceManager *pDataSrcMana = new CMDDataSourceManager();
    return pDataSrcMana;
}

/*!
\section 静态创建数据源
*/
IMDDataSource *CMDDataSourceFactory::createDataSource()
{
    IMDDataSource *pDataSrc = new CMDDataSource();
    return pDataSrc;
}

/*!
\section 静态创建数据表
*/
IMDDataTable *CMDDataSourceFactory::createDataTable()
{
    IMDDataTable *pDataTable = new CMDDataTable();
    return pDataTable;
}

/*!
\section 静态创建数据字段
*/
IMDDataField *CMDDataSourceFactory::createDataField()
{
    IMDDataField *pDataField = new CMDDataField();
    return pDataField;
}

/*!
\section 静态创建数据库参数
*/
IMDDataBaseParam *CMDDataSourceFactory::createDatabasePaream()
{
    IMDDataBaseParam *pDBParam =  new CMDDataBaseParam();
    return pDBParam;
}
