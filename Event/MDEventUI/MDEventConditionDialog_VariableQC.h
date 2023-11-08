/*!

\author dGFuZ3p3

\section 功能

事件：变量品质事件配置对话类。

*/

#ifndef MDEVENTCONDITIONDIALOG_VARIABLEQC_H
#define MDEVENTCONDITIONDIALOG_VARIABLEQC_H

#include "MDObjectPropertyDialog.h"

namespace Ui {
class CMDEventConditionDialog_VariableQC;
}

class CMDEventConditionDialog_VariableQC : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDEventConditionDialog_VariableQC(QWidget *parent = 0);
    ~CMDEventConditionDialog_VariableQC();

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
private:
    Ui::CMDEventConditionDialog_VariableQC *ui;
};

#endif // MDEVENTCONDITIONDIALOG_VARIABLEQC_H
