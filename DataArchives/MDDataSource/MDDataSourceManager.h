#ifndef MDDATASOURCEMANAGER_H
#define MDDATASOURCEMANAGER_H

/*!

\author bHVvY2hhbw==

\section 功能
1.数据源管理器。
2.用于加载、存储数据源。
3.用于增加、删除、获取数据源。

*/

#include "IMDDataSourceManager.h"
#include <QList>

class IMDDataSource;

class CMDDataSourceManager : public IMDDataSourceManager
{
public:
    CMDDataSourceManager();
    ~CMDDataSourceManager();

/*!
\section 加载、存储数据源
*/
public:
    bool load(const QString &file);
    bool load_json(const QString &file);
    bool save(const QString &file);
    bool save_json(const QString &file);
    bool save();

/*!
\section 设置临时数据库工作目录
*/
public:
    inline void setTempDatabasePath(const QString &path){m_fileTmpDBPath = path;}

/*!
\section 增加、删除、获取数据源
*/
public:
    //! 增加数据源
    bool addDataSource(IMDDataSource *pDBSource);

    //! 移除指定数据源
    bool removeDataSource(IMDDataSource *pDBSource);

    //! 删除所有数据源
    void removeAllDataSource();

    //! 获取数据源--通过名字
    IMDDataSource *getDataSource(const QString &name);

    //! 获取数据源--通过索引
    IMDDataSource *getDataSource(int index);

    //! 数据源个数
    inline int dataSourceCount(){return m_dataSourceList.size();}

private:
    QString m_file                          ; //! 数据源序列化文件
    QString m_fileTmpDBPath                 ; //! 临时数据库文件存放路径
    QList<IMDDataSource*> m_dataSourceList;       //! 所有数据源链表
};

#endif // MDDATASOURCEMANAGER_H
