/*!
\author bWVpeGw=
\section 属性配置对话框
*/
#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "DataGroupData.h"
#include "DataGroupWidget.h"

namespace Ui {
class CPropertyDialog;
}
class CMDFilterTreePlugin;
class CMDFilterTreeData;
class CAppearanceForm;
class CCommonForm;
class CDataGroupWidget;
/*!
\section 属性管理框架类
*/
class CPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CPropertyDialog(QWidget *parent = 0);
    ~CPropertyDialog();

private:
    Ui::CPropertyDialog *ui;

/*!
\section 初始化
*/
public:
    void initialize();                      //! 初始化属性配置对话框
    void setTitleName(const QString &str);  //! 设置表头名称

/*!
\section 页面添加
*/
private:
    void addWidgets();  //! 添加窗口
    void addItems();    //! 添加导航项
    void insertItem(const QString &name,int userRole, bool visible = true); //! 插入子项

    //! 导航项枚举
    enum class WidgetName{
        Common = 0,
        Appearance
    };

/*!
\section 信号槽连接
*/
private:
    void creatConnections();

private slots:
    void slotShowWidget(QListWidgetItem *item); //! 显示指定的子配置界面
    void slotConfirmButtonClicked();            //! 确认操作
    void slotCancelButtonClicked();             //! 取消操作

/*!
\section 字段数据
*/
public:
    void setPropertyData(CDataGroupBasicData* value);
    CDataGroupBasicData* propertyData() ;

    void setFields(QList<CDataGroupField> *value);
    QList<CDataGroupField>* fields();

    QString getViewName();

/*!
\section 属性配置类指针
*/
public:
    CCommonForm *m_commonForm;
    CAppearanceForm * m_apperanceForm;
    CDataGroupWidget* m_dataGroupWidget;
};

#endif // PROPERTYDIALOG_H
