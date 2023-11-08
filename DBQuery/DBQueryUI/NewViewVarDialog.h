/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，新建/修改视图变量值类，
    用来实现对视图变量的新建或修改等操作】。

*/

#ifndef NEWVIEWVARDIALOG_H
#define NEWVIEWVARDIALOG_H

#include <QDialog>
#include <MDDBQueryConfigure.h>

namespace Ui {
class NewViewVarDialog;
}

class NewViewVarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewViewVarDialog(int dialogType,QString title,QWidget *parent = 0);
    ~NewViewVarDialog();

public:
    //! 获取视图变量
    DBQ::ViewVariable *getViewVar() const;
    //! 设置视图变量原始参数信息
    void setOriginalViewVarParameter(DBQ::ViewVariable *viewVar);
    //! 设置修改需要保存的视图变量参数信息
    void setViewVarParameter();

    QString DataArchivesFilePath() const;
    void setDataArchivesFilePath(const QString &DataArchivesFilePath);

    QString DBQueryPath() const;
    void setDBQueryPath(const QString &DBQueryPath);

private:
    //! 初始化界面
    void initUI();
    //! 获取当前视图变量类型
    DBQ::VIEW_VARIABLE_TYPE getCurrentViewVarType();
    //! 获取当前视图变量值类型
    //DBQ::VIEW_VARIABLE_VALUE_TYPE getCurrentViewVarValueType();
    //! 设置变量类型为常量和函数时的tab键导航顺序
    void setConstAndFunctionTabOrder();
    //! 设置变量类型为枚举时的tab键导航顺序
    void setEnumTabOrder();
    //! 设置变量类型为字段时的tab键导航顺序
    void setFieldTabOrder();

private slots:
    //! 根据视图变量类型切换视图值stackWidget的显示
    void slot_changeStackWidget(int index);
    //! 管理枚举值
    void on_pbt_Add_EnumVar_clicked();
    //! 从数据归档添加字段
    void on_pbt_Add_DataArchiveField_clicked();
    //! 新建字段
    void on_pbt_Add_NewField_clicked();
    //! 确认/取消操作
    void on_pbt_Confirm_clicked();
    void on_pbt_Cancel_clicked();

private:
    Ui::NewViewVarDialog *ui;
    QString m_windowTitle;                                      //! 对话框标题
    DBQ::ViewVariable *m_ViewVar;                               //! 视图变量指针
    int m_dialogType;                                           //! 构造时新建或修改视图变量
    QList<DBQ::ViewVariableEnumValue> m_newEnumValueList;       //! 新建视图变量枚举值链表
    QList<DBQ::ViewVariableEnumValue> m_modifyEnumValueList;    //! 修改视图变量枚举值链表
    QString m_DataArchivesFilePath;                             //! 数据归档数据源二进制文件路径
    QString m_DBQueryPath;                                      //! 数据查询路径
};

#endif // NEWVIEWVARDIALOG_H
