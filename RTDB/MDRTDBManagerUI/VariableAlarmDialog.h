/*!
\author enli 2015.01.08
\section 变量写保护：限值报警
*/
#ifndef VARIABLEALARMDIALOG_H
#define VARIABLEALARMDIALOG_H

#include "IMDRTDBVariable.h"
#include "IMDRTDBManager.h"
#include <QDialog>

namespace Ui {
class CVariableAlarmDialog;
}

class QLineEdit;
class CVariableAlarmDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CVariableAlarmDialog(QWidget *parent = 0);
    ~CVariableAlarmDialog();
private slots:
    //!限值处理
    void onHHLEditTextChanged(const QString & arg1);
    void onHLEditTextChanged(const QString & arg1);
    void onLLLEditTextChanged(const QString & arg1);
    void onLLEditTextChanged(const QString & arg1);

    //!使能选项
    void onEnableAlarmCheckBoxClicked();
    void on_HHLExpressButton_clicked();

    void on_HLExpressButton_clicked();

    void on_LLExpressButton_clicked();

    void on_LLLExpressButton_clicked();

public:
     void initial(IMDRTDBVariable *curRTDBVariable);
private:
     void updateContols();
private:
    Ui::CVariableAlarmDialog *ui;
public:
    MDVariableAlarmParameter  m_variableAlarmParameter;
public:
    void setRunModel(bool isRunning);
    void setRTDBManager(IMDRTDBManager  *rtdbManager);
private:
    bool selectVariables(QLineEdit* le);
    bool selectVariables(QList<IMDVariable*>&varList);
    bool selectVariables(QStringList &varNameList);
private:
    bool m_isRunning;
    IMDRTDBManager  *m_rtdbManager;
};

#endif // VARIABLEALARMDIALOG_H
