/*!

\author dGFuZ3p3

\section 功能

报警条件：配置对话主类，管理通用、条件、安全策略等对话类。

*/

#ifndef MDALARMCONDITIONMAINDIALOG_H
#define MDALARMCONDITIONMAINDIALOG_H

#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include "MDObjectPropertyMainDialog.h"

class CMDPowerGroupConfiger;
class CMDAlarmConditionUIImp;
class CMDAlarmConditionMainDialog : public CMDObjectPropertyMainDialog
{
    Q_OBJECT
public:
    explicit CMDAlarmConditionMainDialog(QWidget *parent = 0);
    virtual ~CMDAlarmConditionMainDialog();
public:
    void setPowerGroupConfiger(CMDPowerGroupConfiger* pgConfiger);
    void setAlarmConditionUIImp(CMDAlarmConditionUIImp* UIImp);
protected:
    CMDPowerGroupConfiger* m_pgConfiger ;// NULL;
    CMDAlarmConditionUIImp* m_UIImp;
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

#endif // MDALARMCONDITIONMAINDIALOG_H
