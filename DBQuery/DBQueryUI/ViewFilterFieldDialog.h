/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，视图过滤条件过滤项；配置对话框，
    通过  从数据归档添加字段/从视图获得字段/新建字段  三种方式添加过滤条件过滤字段】。

*/

#ifndef VIEWFILTERFIELDDIALOG_H
#define VIEWFILTERFIELDDIALOG_H

#include <QDialog>
#include <QMenu>
#include <MDDBQueryConfigure.h>

namespace Ui {
class ViewFilterFieldDialog;
}

class ViewFilterFieldDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewFilterFieldDialog(QWidget *parent = 0);
    ~ViewFilterFieldDialog();

public:
    //! 设置数据查询路径
    void setDBQueryPath(const QString &DBQueryPath);
    //! 设置视图
    void setView(DBQ::View * pView);
    //! 设置FilterNode
    void setFilterItem(DBQ::ViewFilterNode * pFilterNode);
    //!调用者负责删除
    DBQ::ViewFilterNode * getResult();
private:
    DBQ::VIEW_FILTER_COMPARE_TYPE getCompare();
    //! 获取字段类型的字符串
    QString getViewFieldTypeString(DBQ::VIEW_FIELD_TYPE fieldType);
    //! 设置字段参数
    void setFieldParamenter();
    //! 设置比较条件界面
    void setCompareComboBox(DBQ::VIEW_FILTER_COMPARE_TYPE type);
    //! 设置比较值界面
    void setRightValueUI(DBQ::ViewFilterNode * pNode);
    //! 设置参数类型为常量时的tab键导航顺序
    void setConstParamenterTabOrder();
    //! 设置参数类型为变量时的tab键导航顺序
    void setVarParamenterTabOrder();
    //! 初始化向视图变量combobox控件中添加变量子项
    void loadViewVarList();
private slots:
    //! 视图变量类型切换时，stackWidget显示相应控件
    void showCurrentViewVar(int index);
    //! 右键菜单槽函数
    void slot_showMenu(const QPoint &pos);
    void slot_DataArchivesField_Action();
    void slot_ViewField_Action();
    void slot_NewField_Action();
    //! 添加字段
    void on_pbt_viewField_clicked();
    //void on_pbt_newField_clicked();
    //! 确认/取消操作
    void on_pbt_Confirm_clicked();
    void on_pbt_Cancel_clicked();

private:
    Ui::ViewFilterFieldDialog *ui;
    DBQ::ViewField *m_Field;            //! 过滤条件左值过滤项字段指针
    QMenu* m_Field_Menu;                //! 右键菜单
    QAction* m_DataArchivesField_Action;//! 从数据归档添加字段
    QAction* m_ViewField_Action;        //! 从视图获得字段
    QAction* m_NewField_Action;         //! 新建字段
    QString m_DBQueryPath;              //! 数据查询模块项目路径
    DBQ::View * m_pView;                 //! 视图
};

#endif // VIEWFILTERFIELDDIALOG_H
