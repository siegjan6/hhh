/*!

\author dGFuZ3p3

\section 功能

帐户：安全策略配置对话类。

*/

#ifndef MDACCOUNTSPLISTDIALOG_H
#define MDACCOUNTSPLISTDIALOG_H

#include "MDActionSPListDialog.h"

class CMDAccountSPListDialog : public CMDActionSPListDialog
{
    Q_OBJECT
public:
    explicit CMDAccountSPListDialog(QWidget *parent = 0);
public:
    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
public:
    virtual void initDestList(void *sourceProperty, CMDActionSPList* destList);
};

#endif // MDACCOUNTSPLISTDIALOG_H
