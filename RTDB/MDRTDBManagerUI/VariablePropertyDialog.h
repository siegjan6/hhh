/*!
\author enli 2015.01.08
\section 变量属性总框架，内嵌多个子对话框：写保护、品质错误值设置、基本属性、工程转换、初始化、归档、安全
*/
#ifndef CVARIABLEPROPERTYDIALOG_H
#define CVARIABLEPROPERTYDIALOG_H

#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

#include "VariableWriteProtectDialog.h"
#include "VariableAlarmDialog.h"
#include "VariableOtherDialog.h"
#include "VariableInitializeDialog.h"
#include "VariableGeneralDialog.h"
#include "VariableDataConversionDialog.h"
#include "VariableArchivesDialog.h"
#include "VariableSecurityPolicyDialog.h"
#include "IMDRTDBManager.h"
#include "MDProjectConfiger.h"
class CVariablePropertyDialog: public QDialog
{
    Q_OBJECT
public:
    CVariablePropertyDialog(QWidget *parent = 0);
    ~CVariablePropertyDialog();
/*!
\section 变量父节点
*/
public:
    void setProjectConfiger(CMDProjectConfiger *projectConfiger);
    void setCurrentVariableGroup(IMDVariableGroup *varGroup);
    void setCurrentVariable(IMDRTDBVariable *variable);
    //! 双击变量配置或新增变量时显示该对话框，同时初始化相应变量属性
    void initial();
    void setRunModel(bool isRunning);
private:
    IMDVariableGroup *m_curVariableGroup;
    IMDRTDBManager *m_mdRTDBManager;
    IMDRTDBVariable *m_curRTDBVariable;
    CMDProjectConfiger *m_projectConfiger;
    bool m_isRunning;
/*!
\section 各变量属性对话框句柄
*/
private:
    QListWidget *m_listWidget;
    QStackedWidget *m_stackWidget;
    CVariableWriteProtectDialog *m_writeProtectDialog;
    CVariableAlarmDialog *m_variableAlarmDialog;
    CVariableOtherDialog *m_otherDialog;
    CVariableGeneralDialog *m_generalDialog;
    CVariableDataConversionDialog* m_dataConversionDialog;
    CVariableInitializeDialog *m_intializeDialog;
    CVariableArchivesDialog *m_variableArchivesDialog;
    CVariableSecurityPolicyDialog*m_variableSecurityPolicyDialog;

    QPushButton *m_okButton;
    QPushButton *m_cancleButton;
    QHBoxLayout *m_mainLayout ;
    QHBoxLayout *m_btnLayout;
    QVBoxLayout *m_rightLayout ;
private slots:
    void onOkButtonClicked();
    void onCancelButtonClicked();
};

#endif // CVARIABLEPROPERTYDIALOG_H
