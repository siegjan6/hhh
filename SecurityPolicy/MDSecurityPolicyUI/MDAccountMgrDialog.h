/*!

\author dGFuZ3p3

\section 功能

帐户管理对话类。

*/

#ifndef MDACCOUNTMGRDIALOG_H
#define MDACCOUNTMGRDIALOG_H

#include "MDObjectPropertyDialog.h"
#include "MDAccountUI.h"

namespace Ui {
class CMDAccountMgrDialog;
}

class CMDSecurityPolicyConfiger;
class CMDAccountMgrDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDAccountMgrDialog(CMDSecurityPolicyConfiger* configer, QWidget *parent = 0);
    ~CMDAccountMgrDialog();
    CMDSecurityPolicyConfiger* m_configer ;// NULL;
    CMDAccountUI m_accountUI;
    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
private:
    Ui::CMDAccountMgrDialog *ui;
};

#endif // MDACCOUNTMGRDIALOG_H
