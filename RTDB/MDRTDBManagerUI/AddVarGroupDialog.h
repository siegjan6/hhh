/*!
\author enli 2015.01.08
\section 配置模式下添加变量组
*/
#ifndef ADDVARGROUPDIALOG_H
#define ADDVARGROUPDIALOG_H

#include <QDialog>
#include "IMDRTDBManager.h"
namespace Ui {
class CAddVarGroupDialog;
}

class CAddVarGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAddVarGroupDialog(QWidget *parent = 0);
    ~CAddVarGroupDialog();

private slots:
    //!修改组名
    void on_nameEdit_textChanged(const QString &arg1);
    //!修改描述
    void on_descriptionEdit_textChanged();
    //!确认添加变量组
    void on_okButton_clicked();
    //!取消添加变量组
    void on_cancelButton_clicked();

private:
    Ui::CAddVarGroupDialog *ui;
public:
    //!设置管理类
    void setRTDBManager(IMDRTDBManager *nsRTDBManager);
    //!设置组句柄
    void setCurrentVariableGroup(IMDVariableGroup *varGroup);
    //!设置模式
    void setConfigModel(bool isConfigModel);
    void setRunModel(bool isRunning);
    //!初始化变量组对话框（运行模式下：组名和属性不可更改）
    void initial();
    QString getGroupName();
private:
   IMDVariableGroup *m_curVariableGroup;
   IMDRTDBManager *m_mdRTDBManager;
   QString  m_name ;
   QString m_description;
   bool    m_configModel;
   bool   m_isRunning;
};

#endif // ADDVARGROUPDIALOG_H
