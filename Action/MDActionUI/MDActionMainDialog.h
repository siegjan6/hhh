/*!

\author dGFuZ3p3

\section 功能

动作：配置对话主类，管理通用、条件、安全策略等对话类。

*/

#ifndef MDACTIONMAINDIALOG_H
#define MDACTIONMAINDIALOG_H

#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include "MDObjectPropertyMainDialog.h"

class CMDPowerGroupConfiger;
class CMDActionMainDialog: public CMDObjectPropertyMainDialog
{
    Q_OBJECT
public:
    CMDActionMainDialog(QWidget *parent = 0);
    ~CMDActionMainDialog();
public:
    void setPowerGroupConfiger(CMDPowerGroupConfiger* pgConfiger);
protected:
    CMDPowerGroupConfiger* m_pgConfiger;
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

#endif // MDACTIONMAINDIALOG_H
