/*!

\author dGFuZ3p3

\section 功能

CMDActionSP 对象配置对话类。

*/

#ifndef MDACTIONSPDIALOG_H
#define MDACTIONSPDIALOG_H

#include <QDialog>
#include "MDActionSP.h"

namespace Ui {
class CMDActionSPDialog;
}

class CMDActionSP;
class CMDPowerGroupConfiger;
class ITranslateWidgetTool;
class CMDActionSPDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMDActionSPDialog(QWidget *parent = 0);
    ~CMDActionSPDialog();

    void initial(CMDPowerGroupConfiger* configer, CMDActionSP *sourceProperty);
    bool UI2Property();
protected:
    void enableUI(MDActionProtectMode mode, MDSigType sType);
    void setUIVisible(bool visible);
protected:
    CMDPowerGroupConfiger* m_pgConfiger;
    CMDActionSP *m_sourceProp;
protected:
    ITranslateWidgetTool* m_twTool;

private slots:
    void on_OKButton_clicked();

    void on_removeOperatorGroupButton_clicked();

    void on_modifyOperatorGroupButton_clicked();

    void on_removeVerifierGroupButton_clicked();

    void on_modifyVerifierGroupButton_clicked();

    void on_cancelButton_clicked();

    void on_protectModeComboBox_currentIndexChanged(const QString &arg1);

    void on_sigTypeComboBox_currentIndexChanged(int index);

private:
    Ui::CMDActionSPDialog *ui;
};

#endif // MDACTIONSPDIALOG_H
