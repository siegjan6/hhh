/*!

\author dGFuZ3p3

\section 功能

帐户和密码配置对话类，配置最小帐户长度、最小密码长度等信息。

*/

#ifndef MDASACCOUNTPASSWORDDIALOG_H
#define MDASACCOUNTPASSWORDDIALOG_H

#include "MDObjectPropertyDialog.h"

namespace Ui {
class CMDASAccountPasswordDialog;
}

class CMDAccountStrategyConfiger;
class CMDASAccountPasswordDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDASAccountPasswordDialog(QWidget *parent = 0);
    ~CMDASAccountPasswordDialog();
    CMDAccountStrategyConfiger* m_configer;

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
private:
    Ui::CMDASAccountPasswordDialog *ui;
};

#endif // MDASACCOUNTPASSWORDDIALOG_H
