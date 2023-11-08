/*!
\author aGRw 2015.08.13
\section 输入变量对话框
*/
#ifndef INPUTVARIABLEDIALOG_H
#define INPUTVARIABLEDIALOG_H

#include <QDialog>
#include "IHMIFramework.h"

namespace Ui {
class CInputVariableDialog;
}

class CInputVariableDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CInputVariableDialog(QWidget *parent = 0);
    ~CInputVariableDialog();
private slots:
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();
private:
    Ui::CInputVariableDialog *ui;
//////////////////////////////////////////////////////////////////////////
private:
    QString m_name;
    IHMIFramework* m_framework;
//////////////////////////////////////////////////////////////////////////
public:
    void init(const QString& name, IHMIFramework* framework);
//////////////////////////////////////////////////////////////////////////
};

#endif // INPUTVARIABLEDIALOG_H
