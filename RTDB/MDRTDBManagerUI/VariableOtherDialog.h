/*!
\author enli 2015.01.08
\section 其它设置。变量品质错误时值的设置
*/
#ifndef VARIABLEOTHERDIALOG_H
#define VARIABLEOTHERDIALOG_H

#include <QDialog>

namespace Ui {
class CVariableOtherDialog;
}

class CVariableOtherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CVariableOtherDialog(QWidget *parent = 0);
    ~CVariableOtherDialog();

private:
    Ui::CVariableOtherDialog *ui;
};

#endif // VARIABLEOTHERDIALOG_H
