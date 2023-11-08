/*!
\author emhhbmdsZQ==
\section  仪表组件选择对话框类
*/

#ifndef MODULESELECTDIALOG_H
#define MODULESELECTDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "DrawDefine.h"

namespace Ui {
class CModuleSelectDialog;
}

class CModuleSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CModuleSelectDialog(QWidget *parent = 0);
    ~CModuleSelectDialog();

private:
    Ui::CModuleSelectDialog *ui;

private slots:
    void moduleSelect(QListWidgetItem *item); //! 双击选择组件
    void on_CancelBtn_clicked();   //!  取消选择
    void on_OKBtn_clicked();    //!  确定选择的组件

public:
    qint32 m_index;
};

#endif // MODULESELECTDIALOG_H
