/*!
\author enli 2015.01.08
\section 写值对话框，非模态形式。对话框显示时可切换变量
*/
#ifndef VARIABLEWRITEVALUEDIALOG_H
#define VARIABLEWRITEVALUEDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class CVariableWriteValueDialog;
}
#include "IMDRTDBVariable.h"
#include<QMutex>
class CVariableWriteValueDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CVariableWriteValueDialog(QWidget *parent = 0);
    ~CVariableWriteValueDialog();

private:
    Ui::CVariableWriteValueDialog *ui;
public:
    void setVariable(IMDRTDBVariable *variable);
private:
    IMDRTDBVariable *m_variable;
    //!定时根据浮点的精度刷新当前值
    QTimer m_timer;
    QMutex m_objectMutex;
private slots:
    void onWriteVarButtonClicked();
    void onCloseButtonClicked();
    void onTimerOut();
    void on_bt_copy_clicked();
    void on_bt_copy_value_clicked();
    void on_writeValueEdit_textChanged(const QString &arg1);
};

#endif // VARIABLEWRITEVALUEDIALOG_H
