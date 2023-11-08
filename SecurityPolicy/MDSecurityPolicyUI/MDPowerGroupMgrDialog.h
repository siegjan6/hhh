/*!

\author dGFuZ3p3

\section 功能

权限组管理对话类。

*/

#ifndef MDPOWERGROUPMGRDIALOG_H
#define MDPOWERGROUPMGRDIALOG_H

#include "MDObjectPropertyDialog.h"
#include "MDPowerGroupUI.h"

namespace Ui {
class CMDPowerGroupMgrDialog;
}

class CMDSecurityPolicyConfiger;
class CMDPowerGroupMgrDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDPowerGroupMgrDialog(CMDSecurityPolicyConfiger* configer, QWidget *parent = 0);
    ~CMDPowerGroupMgrDialog();

    CMDSecurityPolicyConfiger* m_configer ;// NULL;
    CMDPowerGroupUI m_pgUI;

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
private:
    Ui::CMDPowerGroupMgrDialog *ui;
};

#endif // MDPOWERGROUPMGRDIALOG_H
