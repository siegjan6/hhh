/*!

\author dGFuZ3p3

\section 功能

报警条件：安全策略配置对话类。

*/

#ifndef MDALARMCONDITIONSPLISTDIALOG_H
#define MDALARMCONDITIONSPLISTDIALOG_H

#include "MDActionSPListDialog.h"

class CMDAlarmConditionSPListDialog : public CMDActionSPListDialog
{
    Q_OBJECT
public:
    explicit CMDAlarmConditionSPListDialog(QWidget *parent = 0);
public:
    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
public:
    virtual void initDestList(void *sourceProperty, CMDActionSPList* destList);
};

#endif // MDALARMCONDITIONSPLISTDIALOG_H
