/*!

\author dGFuZ3p3

\section 功能

动作 ：“写变量”特定参数配置对话类。

*/

#ifndef MDACTIONPARADIALOG_VARIABLEWRITEVALUE_H
#define MDACTIONPARADIALOG_VARIABLEWRITEVALUE_H

#include <QDialog>
#include "MDObjectPropertyDialog.h"
#include "APC.h"
#include "MDActionUIImp.h"

namespace Ui {
class CMDActionParaDialog_VariableWriteValue;
}

class CMDActionParaDialog_VariableWriteValue : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDActionParaDialog_VariableWriteValue(QWidget *parent = 0);
    ~CMDActionParaDialog_VariableWriteValue();
public:
    virtual void initial(void* objectUI, void *sourceProperty);
    virtual bool UI2Property(void *destProperty);
private:
    Ui::CMDActionParaDialog_VariableWriteValue *ui;
};

#endif // MDACTIONPARADIALOG_VARIABLEWRITEVALUE_H
