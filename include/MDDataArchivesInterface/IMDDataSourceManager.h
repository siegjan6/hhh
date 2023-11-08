#ifndef IMDDATASOURCEMANAGER_H
#define IMDDATASOURCEMANAGER_H

/*!

\author luochao

\section 功能
1.数据源管理接口类--抽像类。
2.数据源序列化、增加、删除、获取数据源。

\section

\code

\endcode

*/

#include <QString>

class IMDDataSource;
class IMDDataGroupManager;
class IMDDataSourceManager
{
public:
    virtual ~IMDDataSourceManager(){}       
public:
    //! 加载数据源
    virtual bool load(const QString &file) = 0;
    virtual bool load_json(const QString &file) = 0;

    //! 存储数据源
    virtual bool save(const QString &file) = 0;
    virtual bool save_json(const QString &file) = 0;
    virtual bool save() = 0;

    //! 设置临时数据库工作目录
    virtual void setTempDatabasePath(const QString &path) = 0;

public:

    //! 增加数据源
    virtual bool addDataSource(IMDDataSource *pDBSource) = 0;

    //! 移除指定数据源
    virtual bool removeDataSource(IMDDataSource *pDBSource) = 0;

    //! 删除所有数据源
    virtual void removeAllDataSource() = 0;

    //! 获取数据源--通过名字
    virtual IMDDataSource *getDataSource(const QString &name) = 0;

    //! 获取数据源--通过索引
    virtual IMDDataSource *getDataSource(int index) = 0;

    //! 数据源个数
    virtual int dataSourceCount() = 0;
};

#endif // IMDDATASOURCEMANAGER_H
