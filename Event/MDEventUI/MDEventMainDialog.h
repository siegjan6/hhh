/*!

\author dGFuZ3p3

\section 功能

事件：配置对话主类，管理通用、条件、触发的动作等对话类。

*/

#ifndef MDEVENTMAINDIALOG_H
#define MDEVENTMAINDIALOG_H

#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include "MDObjectPropertyMainDialog.h"

class CMDEventUIImp;
class CMDEventMainDialog : public CMDObjectPropertyMainDialog
{
    Q_OBJECT
public:
    CMDEventMainDialog(QWidget *parent = 0);
    ~CMDEventMainDialog();
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

#endif // MDEVENTMAINDIALOG_H
