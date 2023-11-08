/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，视图字段管理对话框，
    用来实现获得已选中的字段】。

*/

#ifndef VIEWFIELDSDIALOG_H
#define VIEWFIELDSDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "MDDBQueryConfigure.h"
#include "../include/MDDBQueryUI/MDDBQueryUI.h"

namespace Ui {
class ViewFieldsDialog;
}

class ViewFieldsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewFieldsDialog(QWidget *parent = 0);
    ~ViewFieldsDialog();

public:
    bool LoadView(const QString & configFileDir, const QString & viewName);
    void setReturnFieldType(DBQUI::VIEW_FIELD_RETURN_TYPE type);
    //! 函数功能:获得已选中的字段 调用者负责释放ViewField元素内存
    void GetFields(QList<DBQ::ViewField *> & fieldOut);

private slots:
    //! 确认/取消操作
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::ViewFieldsDialog *ui;
    QStandardItemModel * m_pModel;              //! 标准模型
    QList<DBQ::ViewField *> m_FieldsSelected;   //! 选中的视图变量
    DBQUI::VIEW_FIELD_RETURN_TYPE m_returnType;   //! 字段返回类型
};

#endif // VIEWFIELDSDIALOG_H
