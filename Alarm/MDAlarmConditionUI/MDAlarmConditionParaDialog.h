/*!

\author dGFuZ3p3

\section 功能

报警条件：条件配置对话类。

*/

#ifndef MDALARMCONDITIONPARADIALOG_H
#define MDALARMCONDITIONPARADIALOG_H

#include "MDObjectPropertyDialog.h"

namespace Ui {
class CMDAlarmConditionParaDialog;
}

class CMDAlarmConditionParaDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDAlarmConditionParaDialog(QWidget *parent = 0);
    ~CMDAlarmConditionParaDialog();

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
private slots:
    void on_selectObjectButton_clicked();

private:
    Ui::CMDAlarmConditionParaDialog *ui;
};

#endif // MDALARMCONDITIONPARADIALOG_H
