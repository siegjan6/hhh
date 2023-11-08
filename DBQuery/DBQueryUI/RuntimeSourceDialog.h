/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，数据源和数据库配置界面管理类，
    用来实现数据源列表的添加、删除、修改，数据库列表的添加、删除等操作】。

*/

#ifndef RUNTIMESOURCEDIALOG_H
#define RUNTIMESOURCEDIALOG_H

#include <QDialog>
#include <QMenu>
#include <QPoint>
#include <QListWidget>
#include <QListWidgetItem>
#include <QContextMenuEvent>
#include "MDDBQueryConfigure.h"

namespace Ui {
class RuntimeSourceDialog;
}

class RuntimeSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RuntimeSourceDialog(QWidget *parent = 0);
    ~RuntimeSourceDialog();

public:
    int getDBsourcesListCount();                                        //! 获取数据源个数
    QList<DBQ::DBSource *> DBsourcesList() const;
    void setDBsourcesList(const QList<DBQ::DBSource *> &DBsourcesList);
    //! 文件路径
    QString DBSourcePath() const;
    void setDBSourcePath(const QString &DBSourcePath);
    //! 初始化界面
    void initUI();

private:
    void sigAndSlots();                                                 //! 信号槽连接
    void showDataSourceMenu(const QPoint &pos);                         //! 显示数据源右键菜单子项
    void showDataBaseMenu(const QPoint &pos);                           //! 显示数据库右键菜单子项
    bool removeDataSource(DBQ::DBSource *dbsource);                     //! 删除指定数据源
    bool removeDB(DBQ::DBSource *dbsource, QListWidgetItem *item);      //! 删除指定数据库
    DBQ::DBSource *getDataSource(const QString &name);                  //! 根据数据源名称查找
    DBQ::DBSource *getDataSource(int index);                            //! 根据数据源索引查找
    DBQ::DB *getDataBase(DBQ::DBSource *dbsource,const QString &name);  //! 查找指定的数据库
    void setListWidgetItemStyle(QListWidgetItem *item);                 //! 设置列表子项字体和背景色等风格
    void setDSItemBackgroundColor();                                    //! 设置默认数据源背景色
    bool existDBSource(const QString &dbSrcName);                       //! 数据源链表中是否存在当前名称的数据源
    void setUNDefaultDBSouce();                                         //! 统一设置非当前数据源状态为false
    void modifyDBSource(QListWidgetItem *item,DBQ::DBSource *dbsource); //! 修改数据源
    void modifyDataBase(QListWidgetItem *item);                         //! 修改数据库

private slots:
    void slot_itemChanged(QListWidgetItem *item);
    void slot_EditDataSourceMenu(const QPoint &pos);
    void slot_EditDataBaseMenu(const QPoint &pos);
    //! 双击修改当前数据源参数
    void slot_DoubleClickedModifyDBSource(QModelIndex index);
    //! 双击修改当前数据库参数
    void slot_DoubleClickedModifyDataBase(QModelIndex index);
    //! 数据源菜单响应槽函数
    void slot_addDataArchivesDataSource();
    void slot_NewDBSource();
    void slot_DeleteDBSource();
    void slot_ModifyDBSource();
    void slot_setDefaultDataSource();
    //! 数据库菜单响应槽函数
    void slot_NewDataBase();
    void slot_DeleteDataBase();
    void slot_ModifyDataBase();
    void slot_setDefaultDataBase();
    //! 确认/取消操作
    void on_pbt_Confirm_clicked();
    void on_pbt_Cancel_clicked();
    //! 添加/删除数据源
    void on_btn_DSAdd_clicked();
    void on_btn_DSRemove_clicked();
    //! 添加/删除数据库
    void on_btn_DBAdd_clicked();
    void on_btn_DBRemove_clicked();

private:
    Ui::RuntimeSourceDialog *ui;

    QMenu *m_pDBSourcePopupMenu;                //! 数据源右键菜单
    QAction *m_pDBSource_DataArchives;          //! 从数据归档添加数据源
    QAction *m_pDBSource_NewAction;             //! 数据源新建选项
    QAction *m_pDBSource_DeleteAction;          //! 数据源删除选项
    QAction *m_pDBSource_EditAction;            //! 数据源编辑选项
    QAction *m_pDBSource_DefaultDataSource;     //! 设置默认数据源
    QMenu *m_pDataBasePopupMenu;                //! 数据库右键菜单
    QAction *m_pDataBase_NewAction;             //! 数据库新建选项
    QAction *m_pDataBase_DeleteAction;          //! 数据库删除选项
    QAction *m_pDataBase_ModifyAction;          //! 数据库修改选项
    QAction *m_pDataBase_DefaultDataBase;       //! 设置默认数据库
    QList<DBQ::DBSource *> m_DBsourcesList;     //! 数据源指针链表
    QString m_DBSourcePath;                     //! 数据源路径
};

#endif // RUNTIMESOURCEDIALOG_H

