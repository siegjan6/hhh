/*!
\author enli 2015.01.08
\section 变量初始值设置  是否初始化，初始触发事件，值（固定 or 上次保存值）
*/
#ifndef VARIABLEINITIALIZEDIALOG_H
#define VARIABLEINITIALIZEDIALOG_H
#include "IMDRTDBVariable.h"
#include <QDialog>

namespace Ui {
class CVariableInitializeDialog;
}

class CVariableInitializeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CVariableInitializeDialog(QWidget *parent = 0);
    ~CVariableInitializeDialog();

private:
    Ui::CVariableInitializeDialog *ui;

public:
    void setRunModel(bool isRunning);
    void initial(IMDRTDBVariable *curRTDBVariable);
    MDVariableInitializeParameter  m_initialParameter;

private slots:
    //!无初始化
    void onClickNoneRadio();
    //!事件使能
    void onClickEventEnable();
    //!上传值Radio
    void onClickLastValueRadio();
    //!固定值Radio
    void onClickFixedValueRadio();
    //!改变固定设置值
    void onFixedValueChanged();
//!切换对话框界面控件使能状态
private:
     void updateContols();
private:
    bool m_isRunning;
    //!记录项目上次的保存值，不可更改
    QString m_LastVal;
    QString m_FixedValue;
};

#endif // VARIABLEINITIALIZEDIALOG_H
