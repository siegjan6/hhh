#ifndef IMDDATAARCHIVES_H
#define IMDDATAARCHIVES_H

/*!

\author luochao

\section 功能
1.数据归档接口类，管理数据源管理器与数据组管理器--抽像类。
2.加载、缷载、存储数据归档。
3.获取所有数据源、获取指定数据源中的所有表、获取指定数据源与数据表中指定的字段。
4.获取数据组管理器与数据源管理器。

*/

#include<QString>

class IMDDataGroupManager;
class IMDDataSourceManager;
class CMDMsgManager;

class IMDDataArchives
{
public:
    virtual ~IMDDataArchives(){}

/*!
\section 加载、缷载数据归档
*/
public:
    virtual void load(const QString &projectPath) = 0;
    virtual void save(const QString &projectPath) = 0;
    virtual void unLoad() = 0;
    virtual QString projectPath() = 0;

/*!
\section 获取数据组管理器与数据源管理器
*/
public:
    virtual IMDDataGroupManager *dataGroupManager() = 0;
    virtual IMDDataSourceManager *dataSourceManager() = 0;

/*!
\section 查找所有表、字段或数据源名字
*/
public:
    virtual QStringList allSource() = 0;
    virtual QStringList tablesOfSource(const QString &sourceName) = 0;
    virtual QStringList fieldsOfTable(const QString &sourceName, const QString &tableName) = 0;
};

#endif // IMDDATAARCHIVES_H
