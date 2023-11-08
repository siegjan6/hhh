/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，视图模块数据源选择对话框，
    用来实现数据源和数据库的选择】。

*/

#ifndef VIEWDBSOURCEDIALOG_H
#define VIEWDBSOURCEDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include "MDDBQueryConfigure.h"

Q_DECLARE_METATYPE(DBQ::DBSource*)

namespace Ui {
class ViewDBSourceDialog;
}

class ViewDBSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewDBSourceDialog(QWidget *parent = 0);
    ~ViewDBSourceDialog();

public:
    //! 设置数据查询模块项目路径
    void setDBQueryPath(const QString &DBQueryPath);
    //! 获取视图数据源链表
    QList<DBQ::ViewDBSource> ViewDBsourceList() const;
    //! 设置视图数据源链表
    void setViewDBsourceList(const QList<DBQ::ViewDBSource> &ViewDBsourceList);
    //! 初始化数据源选中状态
    void initViewDBSource();   

private:
    //! 设置列表子项字体和背景色等风格
    void setListWidgetItemStyle(QListWidgetItem *item);
    //! 根据数据源名称查找
    DBQ::DBSource *getDataSource(const QString &name);
    //! 从指定的视图数据源的数据库链表中查找该数据库名称，并返回
    QString getDataBase(DBQ::ViewDBSource viewdbsource, const QString &name);
    //! 是否为数据源选中项
    bool isDSChecked(DBQ::DBSource * dbsource);
    //! 是否为数据库选中项
    bool isDBChecked(DBQ::ViewDBSource viewdbsource, QString name);
    //! 视图数据源链表中是否存在该数据源名称
    bool isExistDBSrcName(QString dbsrcname);
    //! 根据数据源名称获得视图数据源对象
    DBQ::ViewDBSource getViewDBSource(QString dbsrcname);
    //! 根据视图数据源名称删除指定视图数据源
    bool removeViewDBSource(QString dbsrcname);
    //! 添加视图数据源项到视图数据源链表中
    void AddDBToViewDBs(QString dbSrcName, QString currentDBName);
    //! 移除指定视图数据源项从视图数据源链表中
    void RemoveDBFromViewDBs(QString dbSrcName, QString currentDBName);

private slots:
    //! 数据源点击选中状态处理
    void slot_itemDSChanged(QListWidgetItem *item);
    //! 数据库点击选中状态处理
    void slot_itemDBChanged(QListWidgetItem *item);
    //! 确认/取消操作
    void on_pbt_Confirm_clicked();
    void on_pbt_Cancel_clicked();

private:
    Ui::ViewDBSourceDialog *ui;
    QList<DBQ::DBSource*> m_DBsourcesList;          //! 数据源指针链表
    QList<DBQ::ViewDBSource> m_ViewDBsourceList;    //! 视图文件的数据源链表
    QString m_DBQueryPath;                          //! 项目中数据查询模块子目录路径
};

#endif // VIEWDBSOURCEDIALOG_H
