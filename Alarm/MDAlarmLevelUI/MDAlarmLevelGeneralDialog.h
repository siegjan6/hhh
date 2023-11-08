/*!

\author dGFuZ3p3

\section 功能

报警等级：通用属性（如名称、注释等）配置对话类。

*/

#ifndef MDALARMLEVELGENERALDIALOG_H
#define MDALARMLEVELGENERALDIALOG_H

#include "MDObjectPropertyDialog.h"

namespace Ui {
class CMDAlarmLevelGeneralDialog;
}

class CMDAlarmLevelGeneralDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDAlarmLevelGeneralDialog(QWidget *parent = 0);
    ~CMDAlarmLevelGeneralDialog();

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
private:
    Ui::CMDAlarmLevelGeneralDialog *ui;
};

#endif // MDALARMLEVELGENERALDIALOG_H
