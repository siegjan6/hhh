/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，新建字段对话框管理类，
    用来实现视图部分新建字段】。

*/

#ifndef NEWFIELDDIALOG_H
#define NEWFIELDDIALOG_H

#include <QDialog>
#include "MDDBQueryConfigure.h"

namespace Ui {
class NewFieldDialog;
}

class NewFieldDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFieldDialog(QString title,QWidget *parent = 0);
    ~NewFieldDialog();

public:
    DBQ::ViewField *getField();     //! 获取视图字段
    void setOriginalViewFieldParameter(DBQ::ViewField* viewField);

private slots:
    //! 确认/取消操作
    void on_pbt_Confirm_clicked();
    void on_pbt_Cancel_clicked();

private:
    Ui::NewFieldDialog *ui;
    DBQ::ViewField *m_Field;        //! 视图字段指针
    QString m_windowTitle;
};

#endif // NEWFIELDDIALOG_H
