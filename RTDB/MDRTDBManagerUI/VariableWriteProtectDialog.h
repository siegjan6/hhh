/*!
\author enli 2015.01.08
\section 变量写保护：可写+写限制+超限处理
*/
#ifndef VARIABLEWRITEPROTECTDIALOG_H
#define VARIABLEWRITEPROTECTDIALOG_H

#include "IMDRTDBVariable.h"
#include <QDialog>

namespace Ui {
class CVariableWriteProtectDialog;
}

class CVariableWriteProtectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CVariableWriteProtectDialog(QWidget *parent = 0);
    ~CVariableWriteProtectDialog();
private slots:
    //!最大最小值处理
    void onMaxValueEditTextChanged(const QString & arg1);
    void onMinValueEditTextChanged(const QString & arg1);
    //!超限选项处理
    void onProectTypeRadioButtonClicked();
    //!可写选项
    void onEnableWriteCheckBoxClicked();
    void onEnableWriteProtectCheckBoxClicked();
public:
     void initial(IMDRTDBVariable *curRTDBVariable);
private:
     void updateContols();
private:
    Ui::CVariableWriteProtectDialog *ui;
public:
    MDVariableWriteProtectParameter  m_writeProtectParameter;
public:
    void setRunModel(bool isRunning);
private:
    bool m_isRunning;
};

#endif // VARIABLEWRITEPROTECTDIALOG_H
