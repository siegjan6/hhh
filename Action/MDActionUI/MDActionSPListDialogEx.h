/*!

\author dGFuZ3p3

\section 功能

动作 ：安全策略配置对话类。

*/

#ifndef MDACTIONSPLISTDIALOGEX_H
#define MDACTIONSPLISTDIALOGEX_H

#include "MDActionSPListDialog.h"

class CMDActionSPListDialogEx : public CMDActionSPListDialog
{
    Q_OBJECT
public:
    explicit CMDActionSPListDialogEx(QWidget *parent = 0);
public:
    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
public:
    virtual void initDestList(void *sourceProperty, CMDActionSPList* destList);
};

#endif // MDACTIONSPLISTDIALOGEX_H
