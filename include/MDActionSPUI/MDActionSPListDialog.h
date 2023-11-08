/*!

\author dGFuZ3p3

\section 功能

CMDActionSP 链表配置对话类，管理一个 CMDActionSP 链表。

*/

#ifndef MDACTIONSPLISTDIALOG_H
#define MDACTIONSPLISTDIALOG_H

#include "MDObjectPropertyDialog.h"
#include "../../include/MDActionHelper/MDActionConstant.h"
#include "MDActionSPUI_Global.h"

namespace Ui {
class CMDActionSPListDialog;
}

class CMDActionSPList;
class QTableWidgetItem;
class CMDPowerGroupConfiger;
class CMDActionSPListDialogParaImp;
class MDACTIONSPUISHARED_EXPORT CMDActionSPListDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDActionSPListDialog(QWidget *parent = 0);
    ~CMDActionSPListDialog();
public:
    void setPowerGroupConfiger(CMDPowerGroupConfiger* configer);
    CMDPowerGroupConfiger* powerGroupConfiger();
public:
    virtual void initial(void* objectUI, void *sourceProperty);
    virtual bool UI2Property(void *destProperty);
public:
    virtual void initDestList(void *sourceProperty, CMDActionSPList* destList);
public:
    void setDestList(const CMDActionSPList* srcList);
    CMDActionSPList* destList();
protected:
    void tableWidgetDoubleClicked(QTableWidgetItem* item);
    void updateTableWidget();
private:
    Ui::CMDActionSPListDialog *ui;
protected:
    CMDActionSPListDialogParaImp* m_para;
};

#endif // MDACTIONSPLISTDIALOG_H
