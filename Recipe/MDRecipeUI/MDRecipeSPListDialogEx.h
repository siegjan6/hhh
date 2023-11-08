#ifndef MDRECIPESPLISTDIALOGEX_H
#define MDRECIPESPLISTDIALOGEX_H

#include "MDActionSPListDialog.h"

class CMDRecipeSPListDialogEx : public CMDActionSPListDialog
{
    Q_OBJECT
public:
    explicit CMDRecipeSPListDialogEx(QWidget *parent = 0);
public:
    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
public:
    virtual void initDestList(void *sourceProperty, CMDActionSPList* destList);
};

#endif // MDRECIPESPLISTDIALOGEX_H
