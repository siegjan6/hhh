/*!
\author enli 2015.01.08
\section 数据归档对话框
*/
#ifndef VARIABLEARCHIVESDIALOG_H
#define VARIABLEARCHIVESDIALOG_H

#include "IMDRTDBVariable.h"
#include <QDialog>
#include <QValidator>

namespace Ui {
class CVariableArchivesDialog;
}

class CVariableArchivesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CVariableArchivesDialog(QWidget *parent = 0);
    ~CVariableArchivesDialog();
private:
    Ui::CVariableArchivesDialog *ui;
    QValidator *m_timeRangeValidator;
    QValidator *m_cycleValidator;
private slots:
    //!内存时间跨度
    void onTimeRangeEditTextChanged(const QString & arg1);
    //!历史归档时间
    void onCycleEditTextChanged(const QString & arg1);
    //!归档规则：不归档/变化归档/周期归档
    void onRulerRadioButtonClicked();
    //!归档模式：内存/磁盘/内存+磁盘
    void onModeRadioButtonClicked();
    void updateContols();
public:
     void initial(IMDRTDBVariable *curRTDBVariable);
     void setRunModel(bool isRunning);
private:
     bool m_isRunning;
public:
      MDRTDataSaveRulerParameter  m_dataSaveRulerParameter;
};

#endif // VARIABLEARCHIVESDIALOG_H
