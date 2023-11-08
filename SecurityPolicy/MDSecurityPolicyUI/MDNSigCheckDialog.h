/*!

\author dGFuZ3p3

\section 功能

MindSCADA 签名验证对话类。

*/

#ifndef MDNSIGCHECKDIALOG_H
#define MDNSIGCHECKDIALOG_H

#include "MDSecurityCheckDialog.h"

namespace Ui {
class CMDNSigCheckDialog;
}

class CMDNSigCheckDialog : public CMDSecurityCheckDialog
{
    Q_OBJECT

public:
    explicit CMDNSigCheckDialog(QWidget *parent = 0);
    ~CMDNSigCheckDialog();
    void initial(CMDSecurityPolicyExpert* spExpert, CMDActionSPSession* spSession);
private slots:
    void on_operatorOKButton_clicked();

private:
    Ui::CMDNSigCheckDialog *ui;
};

#endif // MDNSIGCHECKDIALOG_H
