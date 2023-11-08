#ifndef MDDATASELECTUI_H
#define MDDATASELECTUI_H

/*!

\author bHVvY2hhbw==

\section 功能
1.外部接口类。
2.用于UI界面选择，选择当前项目中配置的所有数据源以及表。

*/


#include "mddataselectui_global.h"

#include <QWidget>

class IMDDataSource;
class IMDDataTable;
class IMDDataArchives;
class CDataSelectUI;

class MDDATASELECTUISHARED_EXPORT CMDDataSelectUI : public QObject
{
    Q_OBJECT
public:
    CMDDataSelectUI();
    ~CMDDataSelectUI();

    /*!
    \section 设置/获取属性
    */
public:
    QWidget *dataSelectUI();

    void setDataArchives(IMDDataArchives *pDataArchives);
    void setComboxText(const QString &strSrc, const QString &strTab); //! 设置/获取数据归档

    QString dataSourceName(); //! 获取当前选择数据源名
    QString dataTableName(); //! 获取当前选择的数据表名

    IMDDataSource *dataSource(); //! 获取当前选择数据源指针
    IMDDataTable *dataTable(); //! 获取当前选择的数据指针

    /*!
    \section 通知选择的数据源或者表名改变的信号
    */
signals:
    void currentTableNameChange(const QString &);
    void currentSourceNameChange(const QString &);

private:
    CDataSelectUI *m_pDataSelUI; //! 内部实现类
};

#endif // MDDATASELECTUI_H

