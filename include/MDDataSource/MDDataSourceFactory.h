#ifndef MDDATASOURCEFACTORY_H
#define MDDATASOURCEFACTORY_H

/*!

\author bHVvY2hhbw==

\section 功能
数据源工厂类，对外接口，用于创建以下相关子模块。
1.静态创建数据源管理器。
2.静态创建数据源。
3.静态创建数据表。
4.静态创建数据字段。
5.静态创建数据库参数。

*/

#include "mddatasource_global.h"

class IMDDataSourceManager;
class IMDDataSource;
class IMDDataTable;
class IMDDataField;
//tmep LD
class IMDDataBaseParam;

class MDDATASOURCESHARED_EXPORT CMDDataSourceFactory
{
public:
    CMDDataSourceFactory();

/*!
\section 静态创建相关模块
*/
public:
    //! 静态创建数据源管理器
    static IMDDataSourceManager *createDataSourceManager();

    //! 静态创建数据源
    static IMDDataSource *createDataSource();

    //! 静态创建数据表
    static IMDDataTable *createDataTable();

    //! 静态创建数据字段
    static IMDDataField *createDataField();

    //! 静态创建数据库参数
    static IMDDataBaseParam *createDatabasePaream();
};


#endif // MDDATASOURCEFACTORY_H
