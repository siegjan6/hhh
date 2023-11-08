/*!

\author dGFuZ3p3

\section 功能

报警条件：通用属性（如名称、注释等）配置对话类。

*/

#ifndef MDALARMCONDITIONGENERALDIALOG_H
#define MDALARMCONDITIONGENERALDIALOG_H

#include "MDObjectPropertyDialog.h"

namespace Ui {
class CMDAlarmConditionGeneralDialog;
}

class CMDAlarmConditionGeneralDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDAlarmConditionGeneralDialog(QWidget *parent = 0);
    ~CMDAlarmConditionGeneralDialog();

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
private slots:
    void on_checkBox_acknowledgeRequired_toggled(bool checked);

    void on_selectLevelButton_clicked();
private:
    Ui::CMDAlarmConditionGeneralDialog *ui;
};

#endif // MDALARMCONDITIONGENERALDIALOG_H
