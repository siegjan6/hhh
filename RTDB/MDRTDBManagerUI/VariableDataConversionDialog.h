/*!
\author enli 2015.01.08
\section 工程转换对话框。工程转换，转换类型，最大/最小，原始最大/最小
*/
#ifndef VARIABLEDATACONVERSIONDIALOG_H
#define VARIABLEDATACONVERSIONDIALOG_H


#include "IMDRTDBVariable.h"
#include <QDialog>
namespace Ui {
class CVariableDataConversionDialog;
}

class CVariableDataConversionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CVariableDataConversionDialog(QWidget *parent = 0);
    ~CVariableDataConversionDialog();
private slots:
    void onMaxValueEditTextChanged(const QString & arg1);
    void onMinValueEditTextChanged(const QString & arg1);
    void onOriginalMaxValueEditTextChanged(const QString & arg1);
    void onOriginalMinValueEditTextChanged(const QString & arg1);
    void onConversionTypeBoxCurrentIndexChanged(int index);
    void onEnableLinearConvertCheckBoxClicked();

public:
     void initial(IMDRTDBVariable *curRTDBVariable);
private:
     void updateContols();
private:
    Ui::CVariableDataConversionDialog *ui;
public:
    MDVariableDataConversionParameter  m_dataConversionParameter;
public:
    void setRunModel(bool isRunning);
private:
    bool m_isRunning;
    QStringList m_convertionTypeList;
};

#endif // VARIABLEDATACONVERSIONDIALOG_H
