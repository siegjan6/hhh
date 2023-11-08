/*!

\author dGFuZ3p3

\section 功能

整体安全策略配置对话主类，管理【帐户和密码】等对话类。

*/

#ifndef MDACCOUNTSTRATEGYMAINDIALOG_H
#define MDACCOUNTSTRATEGYMAINDIALOG_H

#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include "MDObjectPropertyMainDialog.h"

class CMDAccountStrategyConfiger;
class CMDAccountStrategyMainDialog : public CMDObjectPropertyMainDialog
{
    Q_OBJECT
public:
    explicit CMDAccountStrategyMainDialog(QWidget *parent = 0);
    virtual ~CMDAccountStrategyMainDialog();
    CMDAccountStrategyConfiger* m_configer ;// NULL;
protected:
    virtual void* newProperty(qint16 subType, void* sourceProperty);
    virtual void initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty);
    virtual bool modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage);
    virtual bool addProperty(void* configer, void* destProperty, QString& resultMessage);
};

#endif // MDACCOUNTSTRATEGYMAINDIALOG_H
