/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，视图字段管理类对话框，
    用来实现对视图字段的增加删除等操作】。

*/

#ifndef VIEWFIELDTABLEDIALOG_H
#define VIEWFIELDTABLEDIALOG_H

#include <QDialog>
#include <QMenu>
#include <MDDBQueryConfigure.h>
#include <QStandardItemModel>
#include <QContextMenuEvent>

namespace Ui {
class ViewFieldTableDialog;
}

class ViewFieldTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewFieldTableDialog(QWidget *parent = 0);
    ~ViewFieldTableDialog();

public:
    QList<DBQ::ViewField *> getViewFieldList() const;
    void setViewFieldList(const QList<DBQ::ViewField *> &viewFieldList);
    //! 初始化表格控件
    void initTableWidget();

    QString DataArchivesFilePath() const;
    void setDataArchivesFilePath(const QString &DataArchivesFilePath);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    //! 根据字段名称查找链表中同名字段
    DBQ::ViewField *getViewField(const QString &name, const QString &tableName);
    //! 视图变量值类型枚举值转换成字符
    QString getViewFieldValueTypeString(DBQ::VIEW_FIELD_TYPE valueType);
    //! 获取删除索引链表
    QModelIndexList deleteRepeatList(QModelIndexList indexList);
    //! 字段添加重复字段判断代码
    bool checkViewFieldStringFromFieldList(DBQ::ViewField *viewField);
    //! 字段中有相同别名的判断
    bool checkViewFieldAliasFromFieldList(const QString &alias);

private slots:
    //! 根据表格控件当前列表头排序
    void slot_sortByColumn(int column);
    //! 双击当前行弹出修改字段对话框
    void slot_doubleClickedModifyViewField(QModelIndex index);
    //! 右键菜单槽函数
    void slot_AddViewFileld();
    void slot_NewViewFileld();
    void slot_DeleteViewFileld();
    void slot_ModifyViewFileld();
    //! 确认/取消操作
    void on_pbt_Cancel_clicked();
    void on_pbt_Confirm_clicked();

private:
    Ui::ViewFieldTableDialog *ui;
    QList<DBQ::ViewField *> m_viewFieldList;            //! 字段链表
    QMenu *m_pViewField_PopupMenu;                      //! 右键菜单
    QAction *m_pViewField_AddDataArchivesFieldAction;   //! 从数据归档添加字段
    QAction *m_pViewField_NewAction;                    //! 新建字段
    QAction *m_pViewField_DeleteAction;                 //! 删除字段
    QAction *m_pViewField_ModifyAction;                 //! 修改字段
    QString m_DataArchivesFilePath;
};

#endif // VIEWFIELDTABLEDIALOG_H
