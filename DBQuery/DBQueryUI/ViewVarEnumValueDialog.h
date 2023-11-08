/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，视图变量枚举值管理类对话框，
    用来实现对视图变量枚举值的增加删除等操作】。

*/

#ifndef VIEWVARENUMVALUEDIALOG_H
#define VIEWVARENUMVALUEDIALOG_H

#include <QDialog>
#include <MDDBQueryConfigure.h>

namespace Ui {
class ViewVarEnumValueDialog;
}

class ViewVarEnumValueDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewVarEnumValueDialog(QWidget *parent = 0);
    ~ViewVarEnumValueDialog();

public:
    //! 获取枚举值链表
    QList<DBQ::ViewVariableEnumValue> getEnumValueList() const;
    //! 设置枚举值链表
    void setEnumValueList(const QList<DBQ::ViewVariableEnumValue> &enumValueList);
    //! 初始化表格控件
    void initTableWidget();
    //! 根据枚举值名称从枚举链表中查找指定枚举值
    bool getEnumValue(const QString &name);
    //! 设置枚举值类型
    void setEnumValueType(int type);

private:
    void saveEnumValueList();

private slots:
    //! 添加/删除枚举值
    void on_pbt_Add_clicked();
    void on_pbt_Delete_clicked();
    //! 确认/取消操作
    void on_pbt_Confirm_clicked();
    void on_pbt_Cancel_clicked();

private:
    Ui::ViewVarEnumValueDialog *ui;
    QList<DBQ::ViewVariableEnumValue> m_enumValueList;     //! 枚举值链表
    QList<DBQ::ViewVariableEnumValue> m_enumValueListCopy; //! 枚举值链表备份
};

#endif // VIEWVARENUMVALUEDIALOG_H
