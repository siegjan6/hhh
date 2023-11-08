/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，视图变量表格管理类，
    用来实现对视图变量的增加/删除/选择等操作】。

*/

#ifndef VIEWVARTABLEDIALOG_H
#define VIEWVARTABLEDIALOG_H

#include <QDialog>
#include <MDDBQueryConfigure.h>
#include <QStandardItemModel>
#include <QItemDelegate>
#include <QComboBox>
#include <QItemEditorCreator>
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QMenu>
#include <QPoint>

Q_DECLARE_METATYPE(DBQ::ViewVariable *)

namespace Ui {
class ViewVarTableDialog;
}

class ViewVarTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewVarTableDialog(QWidget *parent = 0);
    ~ViewVarTableDialog();

public:
    //! 初始化tableview
    void initTableView();
    //! 从文件读取视图变量链表
    bool LoadViewVarTable(const QString & configFileDir);
    //! 添加tableview数据项
    void creatTableView();
    //! 获取所有的视图变量链表
    QList<DBQ::ViewVariable *> getViewVarList() const;
    //! 获取选中的视图变量链表
    QList<DBQ::ViewVariable *> getViewVarListSelects() const;
    //! 获取删除索引链表
    QModelIndexList deleteRepeatList(QModelIndexList indexList);
    //! 根据视图变量名称获取视图变量
    DBQ::ViewVariable *getViewVariable(const QString &name);
    //! 项目路径
    QString DBQueryPath() const;
    void setDBQueryPath(const QString &DBQueryPath);

private:
    //! 视图变量类型枚举值转换成字符
    QString getViewVariableTypeString(DBQ::VIEW_VARIABLE_TYPE type);
    //! 视图变量值类型枚举值转换成字符
    //QString getViewVariableValueTypeString(DBQ::VIEW_VARIABLE_VALUE_TYPE valueType);
    //! 根据视图变量链表的索引移除链表中的子项
    bool deleteViewVariable(int index);

private slots:
    //! 双击修改当前行
    void slot_doubleClickedModifyViewVar(QModelIndex index);
    //! 点击表头，根据当前列排序
    void slot_sortByColumn(int column);
    //! 右键菜单槽函数
    void slot_showMenu(const QPoint &pos);
    void slot_NewViewVariable();
    void slot_DeleteViewVariable();
    void slot_ModifyViewVariable();
    //! 确认/取消操作
    void on_pbt_Confirm_clicked();
    void on_pbt_Cancel_clicked();

private:
    Ui::ViewVarTableDialog *ui;
    QStandardItemModel * m_pModel;                  //! 标准模型指针
    QList<DBQ::ViewVariable*> m_viewVarList;        //! 视图变量链表
    QList<DBQ::ViewVariable*> m_viewVarListSelects; //! 选中的视图变量链表
    QMenu *m_pViewVarPopupMenu;                     //! 视图变量右键菜单
    QAction *m_pViewVar_NewAction;                  //! 视图变量新建选项
    QAction *m_pViewVar_DeleteAction;               //! 视图变量删除选项
    QAction *m_pViewVar_ModifyAction;               //! 视图变量修改选项
    QString m_DBQueryPath;                          //! 数据查询子模块工程目录
};




///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，视图变量表格枚举值子项combobox控件代理类，
    用来实现在视图变量tableview中变量类型为枚举时“值”Item项内嵌QCombobox控件】。

*/

class EnumValueDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    EnumValueDelegate(int row,QStringList menuList,QObject *parent);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    QStringList m_itemList; //! 构造combobox控件时传参item子项
    int m_row;              //! 需要内嵌combobox控件的行
};
#endif // VIEWVARTABLEDIALOG_H
