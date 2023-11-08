/*!

\author dGFuZ3p3

\section 功能

权限组：通用属性（如名称、注释等）配置对话类。

*/

#ifndef MDPOWERGROUPGENERALDIALOG_H
#define MDPOWERGROUPGENERALDIALOG_H

#include "MDObjectPropertyDialog.h"

namespace Ui {
class CMDPowerGroupGeneralDialog;
}

class CMDPowerGroupGeneralDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDPowerGroupGeneralDialog(QWidget *parent = 0);
    ~CMDPowerGroupGeneralDialog();

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
private:
    Ui::CMDPowerGroupGeneralDialog *ui;
};

#endif // MDPOWERGROUPGENERALDIALOG_H
