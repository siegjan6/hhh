/*!

\author dGFuZ3p3

\section 功能

权限组：配置对话主类，管理通用、关联帐户、安全策略等对话类。

*/

#ifndef MDPOWERGROUPMAINDIALOG_H
#define MDPOWERGROUPMAINDIALOG_H

#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include "MDObjectPropertyMainDialog.h"

class CMDPowerGroupMainDialog : public CMDObjectPropertyMainDialog
{
    Q_OBJECT
public:
    explicit CMDPowerGroupMainDialog(QWidget *parent = 0);
    ~CMDPowerGroupMainDialog();
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

#endif // MDPOWERGROUPMAINDIALOG_H
