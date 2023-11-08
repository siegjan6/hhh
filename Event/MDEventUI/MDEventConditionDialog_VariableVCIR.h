/*!

\author dGFuZ3p3

\section 功能

事件：变量数值变化范围事件配置对话类。

*/

#ifndef MDEVENTCONDITIONDIALOG_VARIABLEVCIR_H
#define MDEVENTCONDITIONDIALOG_VARIABLEVCIR_H

#include "MDObjectPropertyDialog.h"

namespace Ui {
class CMDEventConditionDialog_VariableVCIR;
}

class CMDEventProperty_Config;
class CMDEventConditionDialog_VariableVCIR : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDEventConditionDialog_VariableVCIR(QWidget *parent = 0);
    ~CMDEventConditionDialog_VariableVCIR();

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
private:
    Ui::CMDEventConditionDialog_VariableVCIR *ui;
};

#endif // MDEVENTCONDITIONDIALOG_VARIABLEVCIR_H
