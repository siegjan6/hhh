/*!

\author dGFuZ3p3

\section 功能

权限组：关联帐户配置对话类。

*/

#ifndef MDPOWERGROUPACCOUNTDIALOG_H
#define MDPOWERGROUPACCOUNTDIALOG_H

#include "MDObjectPropertyDialog.h"

namespace Ui {
class CMDPowerGroupAccountDialog;
}

class CMDPowerGroupAccountDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDPowerGroupAccountDialog(QWidget *parent = 0);
    ~CMDPowerGroupAccountDialog();

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
private slots:
    void on_addButton_clicked();
    void on_removeButton_clicked();
private:
    Ui::CMDPowerGroupAccountDialog *ui;
};

#endif // MDPOWERGROUPACCOUNTDIALOG_H
