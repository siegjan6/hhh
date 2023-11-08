/*!

\author dGFuZ3p3

\section 功能

登录对话类。

*/

#ifndef MDLOGINDIALOG_H
#define MDLOGINDIALOG_H

#include "MDSecurityCheckDialog.h"

namespace Ui {
class CMDLoginDialog;
}

class CMDLoginDialog : public CMDSecurityCheckDialog
{
    Q_OBJECT

public:
    explicit CMDLoginDialog(QWidget *parent = 0);
    ~CMDLoginDialog();

private slots:
    void on_operatorOKButton_clicked();

private:
    Ui::CMDLoginDialog *ui;
};

#endif // MDLOGINDIALOG_H
