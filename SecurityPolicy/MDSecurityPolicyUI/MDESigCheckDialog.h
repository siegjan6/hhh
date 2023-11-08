/*!

\author dGFuZ3p3

\section 功能

电子签名验证对话类。

*/

#ifndef MDESIGCHECKDIALOG_H
#define MDESIGCHECKDIALOG_H

#include "MDSecurityCheckDialog.h"

namespace Ui {
class CMDESigCheckDialog;
}

class CMDESigCheckDialog : public CMDSecurityCheckDialog
{
    Q_OBJECT

public:
    explicit CMDESigCheckDialog(QWidget *parent = 0);
    ~CMDESigCheckDialog();
    void initial(CMDSecurityPolicyExpert* spExpert, CMDActionSPSession* spSession = NULL);
private slots:
    void on_operatorOKButton_clicked();

    void on_verifierOKButton_clicked();

private:
    Ui::CMDESigCheckDialog *ui;
};

#endif // MDESIGCHECKDIALOG_H
