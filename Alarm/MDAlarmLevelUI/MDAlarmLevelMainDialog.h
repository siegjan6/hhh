/*!

\author dGFuZ3p3

\section 功能

报警等级：配置对话主类，管理通用、颜色等对话类。

*/

#ifndef MDALARMLEVELMAINDIALOG_H
#define MDALARMLEVELMAINDIALOG_H

#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include "MDObjectPropertyMainDialog.h"

class CMDAlarmLevelMainDialog : public CMDObjectPropertyMainDialog
{
    Q_OBJECT
public:
    explicit CMDAlarmLevelMainDialog(QWidget *parent = 0);
    ~CMDAlarmLevelMainDialog();
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

#endif // MDALARMLEVELMAINDIALOG_H
