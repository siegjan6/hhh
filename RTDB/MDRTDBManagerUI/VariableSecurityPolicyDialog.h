/*!
\author enli 2015.01.08
\section 变量动作、安全策略
*/
#ifndef VARIABLESECURITYPOLICYDIALOG_H
#define VARIABLESECURITYPOLICYDIALOG_H

#include <QDialog>
#include "MDActionSPUI.h"
#include "IMDRTDBVariable.h"

namespace Ui {
class CVariableSecurityPolicyDialog;
}
class QTableWidgetItem;
class CVariableSecurityPolicyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CVariableSecurityPolicyDialog(QWidget *parent = 0);
    ~CVariableSecurityPolicyDialog();
private slots:
    void onSecurityPolicyTableWidgetItemDoubleClicked(QTableWidgetItem *item);
public:
     void initial(CMDPowerGroupConfiger* powerGroupConfiger,IMDRTDBVariable *curRTDBVariable);
public:
     CMDActionSPList m_actionSPList;
     CMDPowerGroupConfiger* m_powerGroupConfiger;
private:
     void updateContols();
   // MDVariableWriteProtectParameter  m_writeProtectParameter;
private:
    Ui::CVariableSecurityPolicyDialog *ui;
public:
    void setRunModel(bool isRunning);
private:
    bool m_isRunning;
};

#endif // VARIABLESECURITYPOLICYDIALOG_H
