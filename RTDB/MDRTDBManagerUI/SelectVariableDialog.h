/*!
\author enli 2015.01.08
\section 选择变量对话框，用于中间变量+tang：selectVariables
*/
#ifndef SELECTVARIABLEDIALOG_H
#define SELECTVARIABLEDIALOG_H

#include <QDialog>

#include "IMDVariable.h"
#include "IMDTreeModel.h"
#include "IMDRTDBManager.h"
namespace Ui {
class CSelectVariableDialog;
}

class CSelectVariableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSelectVariableDialog(QWidget *parent = 0);
    ~CSelectVariableDialog();

private:
    Ui::CSelectVariableDialog *ui;
public:
    void setRTDBManager(IMDRTDBManager*nsRTDBManager);
    //!获取选中变量链表
    bool getSelectedVariables(QList<IMDVariable*>&varList);

private slots:
    void on_OkButton_clicked();
    void on_CancelButton_clicked();

private:
    IMDRTDBManager *m_rtdbManager;
    IMDTreeModel *m_variableTreeModel;    
};

#endif // SELECTVARIABLEDIALOG_H
