/*!

\author dGFuZ3p3

\section 功能

帐户：配置对话主类，管理通用、密码、安全策略等对话类。

*/

#ifndef MDACCOUNTMAINDIALOG_H
#define MDACCOUNTMAINDIALOG_H

#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include "MDObjectPropertyMainDialog.h"

class CMDPowerGroupConfiger;
class CMDAccountMainDialog : public CMDObjectPropertyMainDialog
{
    Q_OBJECT
public:
    explicit CMDAccountMainDialog(QWidget *parent = 0);
    ~CMDAccountMainDialog();
public:
    void setTheOnlyPage(int pageID);
protected:
    int m_pageID;
protected:
    virtual void* newProperty(qint16 subType, void* sourceProperty);
    virtual void initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty);
    virtual bool modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage);
    virtual bool addProperty(void* configer, void* destProperty, QString& resultMessage);
};

#endif // MDACCOUNTMAINDIALOG_H
