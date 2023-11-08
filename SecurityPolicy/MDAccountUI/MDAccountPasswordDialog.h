/*!

\author dGFuZ3p3

\section 功能

帐户：密码配置对话类。

*/

#ifndef MDACCOUNTPASSWORDDIALOG_H
#define MDACCOUNTPASSWORDDIALOG_H

#include "MDObjectPropertyDialog.h"

namespace Ui {
class CMDAccountPasswordDialog;
}

class CMDSecurityPolicyConfiger;
class CMDAccountPasswordDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDAccountPasswordDialog(QWidget *parent = 0);
    ~CMDAccountPasswordDialog();

    CMDSecurityPolicyConfiger* m_spConfiger ;// NULL;

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
private:
    Ui::CMDAccountPasswordDialog *ui;
};

#endif // MDACCOUNTPASSWORDDIALOG_H
