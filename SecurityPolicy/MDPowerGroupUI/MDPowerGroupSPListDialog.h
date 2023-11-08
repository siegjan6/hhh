/*!

\author dGFuZ3p3

\section 功能

权限组：安全策略配置对话类。

*/

#ifndef MDPOWERGROUPSPLISTDIALOG_H
#define MDPOWERGROUPSPLISTDIALOG_H

#include "MDActionSPListDialog.h"

class CMDPowerGroupSPListDialog : public CMDActionSPListDialog
{
    Q_OBJECT
public:
    explicit CMDPowerGroupSPListDialog(QWidget *parent = 0);
public:
    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
public:
    virtual void initDestList(void *sourceProperty, CMDActionSPList* destList);
};

#endif // MDPOWERGROUPSPLISTDIALOG_H
