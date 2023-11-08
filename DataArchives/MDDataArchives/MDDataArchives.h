#ifndef MDDATAARCHIVES_H
#define MDDATAARCHIVES_H

/*!

\author bHVvY2hhbw==

\section 功能
1.IMDDataArchives的实现类。
2.加载、缷载、存储数据归档。
3.获取所有数据源、获取指定数据源中的所有表、获取指定数据源与数据表中指定的字段。
4.获取数据组管理器与数据源管理器。

*/


#include "IMDDataArchives.h"
class IMDDataGroupManager;
class IMDDataSourceManager;

class  CMDDataArchives : public IMDDataArchives
{
public:
    CMDDataArchives();
    virtual ~CMDDataArchives();

/*!
\section 加载、缷载数据归档
*/
public:
    void load(const QString &projectPath);
    void save(const QString &projectPath);
    void unLoad();
    QString projectPath();

/*!
\section 获取数据组管理器与数据源管理器
*/
public:
    inline IMDDataGroupManager *dataGroupManager(){return m_mdDataGroupManager;}
    inline IMDDataSourceManager *dataSourceManager(){return m_mdDataSourceManager;}

/*!
\section 查找所有表、字段或数据源名字
*/
public:
    /*!
        查找所有数据源名。
    \param 无。
    \returns QStringList。
    \note 无。
    */
    QStringList allSource();
    /*!
        查找指定数据源中的所有表名。
    \param sourceName 数据源名。
    \returns QStringList。
    \note 无。
    */
    QStringList tablesOfSource(const QString &sourceName);
    /*!
        查找指定数据源中某个表的所有字段
    \param sourceName 数据源名。
    \param tableName 表名。
    \returns QStringList。
    \note 无。
    */
    QStringList fieldsOfTable(const QString &sourceName, const QString &tableName);

/*!
\section 计算数据组与数据组的序列化文件路径
*/
private:
    /*!
        计算数据组与数据组的序列化文件路径
    \param projectPath 项目序列化路径。
    \param dataGrpPath 数据组序列化路径。
    \param dataSrcPath 数据源序列化路径。
    \returns 无。
    \note 无。
    */
    void calDataGrpAndDataSrcPath(const QString &projectPath,
                                  QString &dataGrpPath,
                                  QString &dataSrcPath);
private:
    IMDDataGroupManager *m_mdDataGroupManager     ; //! 数据组管理器
    IMDDataSourceManager *m_mdDataSourceManager   ; //! 数据源管理器
    QString m_projectPath;
};

#endif // MDDATAARCHIVES_H
