/*!

\author dGFuZ3p3

\section 功能

事件：触发的动作配置对话类。

*/

#ifndef MDEVENTACTIONDIALOG_H
#define MDEVENTACTIONDIALOG_H

#include "MDObjectPropertyDialog.h"

namespace Ui {
class CMDEventActionDialog;
}

class CMDEventProperty_Config;
class CMDEventUIImp;
class CMDEventActionDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDEventActionDialog(QWidget *parent = 0);
    ~CMDEventActionDialog();

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);

private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void on_forwardButton_clicked();

    void on_backwardButton_clicked();

private:
    Ui::CMDEventActionDialog *ui;
};

#endif // MDEVENTACTIONDIALOG_H
