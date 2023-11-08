/*!

\author dGFuZ3p3

\section 功能

安全检查对话类，作为其他类的基类，
参考 CMDESigCheckDialog 、 CMDNSigCheckDialog 和 CMDLoginDialog。

*/

#ifndef MDSECURITYCHECKDIALOG_H
#define MDSECURITYCHECKDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "MDSecurityPolicyExpert.h"
#include "MDActionSPSession.h"
#include "MDSecurityPolicyConstant.h"

namespace Ui {
class CMDSecurityCheckDialog;
}

class CMDSecurityPolicyExpert;
class CMDActionSPSession;
class ITranslateWidgetTool;
class CMDSecurityCheckDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMDSecurityCheckDialog(QWidget *parent = 0);
    ~CMDSecurityCheckDialog();
public:
    virtual void initial(CMDSecurityPolicyExpert* spExpert, CMDActionSPSession* spSession = NULL);
    void showErrorMessageBox(const QString& account, MDSecurityResultCode code);
protected:
    CMDSecurityPolicyExpert* m_spExpert;
    CMDActionSPSession* m_spSession;
protected:
    ITranslateWidgetTool* m_twTool;
private slots:
    void on_cancelButton_clicked();

private:
    Ui::CMDSecurityCheckDialog *ui;
};

#endif // MDSECURITYCHECKDIALOG_H
