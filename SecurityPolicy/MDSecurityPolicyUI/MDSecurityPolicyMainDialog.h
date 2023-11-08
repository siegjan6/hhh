/*!

\author dGFuZ3p3

\section 功能

安全策略配置对话主类，管理帐户、权限组等对话类。

*/

#ifndef MDSECURITYPOLICYMAINDIALOG_H
#define MDSECURITYPOLICYMAINDIALOG_H

#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include "MDObjectPropertyMainDialog.h"

class CMDSecurityPolicyConfiger;
class CMDSecurityPolicyMainDialog : public CMDObjectPropertyMainDialog
{
    Q_OBJECT
public:
    explicit CMDSecurityPolicyMainDialog(QWidget *parent = 0);
    virtual ~CMDSecurityPolicyMainDialog();
    CMDSecurityPolicyConfiger* m_configer ;// NULL;
protected:
    virtual void* newProperty(qint16 subType, void* sourceProperty);
    virtual void initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty);
    virtual bool modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage);
    virtual bool addProperty(void* configer, void* destProperty, QString& resultMessage);
};

#endif // MDSECURITYPOLICYMAINDIALOG_H
