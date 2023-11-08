/*!

\author dGFuZ3p3

\section 功能

帐户：通用属性（如名称、注释等）配置对话类。

*/

#ifndef MDACCOUNTGENERALDIALOG_H
#define MDACCOUNTGENERALDIALOG_H

#include "MDObjectPropertyDialog.h"

namespace Ui {
class CMDAccountGeneralDialog;
}

class CMDSecurityPolicyConfiger;
class CMDAccountGeneralDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDAccountGeneralDialog(QWidget *parent = 0);
    ~CMDAccountGeneralDialog();

    CMDSecurityPolicyConfiger* m_spConfiger ;// NULL;

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);

private:
    Ui::CMDAccountGeneralDialog *ui;
};

#endif // MDACCOUNTGENERALDIALOG_H
