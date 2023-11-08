#include "MDActionSPListDialogEx.h"
#include "MDActionProperty_Config.h"
#include "MDActionSP.h"
#include "MDStringResource.h"

CMDActionSPListDialogEx::CMDActionSPListDialogEx(QWidget *parent) :
    CMDActionSPListDialog(parent)
{
}

void CMDActionSPListDialogEx::initial(void* objectUI, void *sourceProperty)
{
    CMDActionSPListDialog::initial(objectUI, sourceProperty);
}

bool CMDActionSPListDialogEx::UI2Property(void *destProperty)
{
    CMDActionProperty_Config* dest = (CMDActionProperty_Config*)destProperty;

    dest->setSPList(destList());

    return true;
}

void CMDActionSPListDialogEx::initDestList(void *sourceProperty, CMDActionSPList* destList)
{
    CMDActionProperty_Config* src = (CMDActionProperty_Config*)sourceProperty;

    destList->append(MDActionType::action_Execute);
    destList->update(*src->spList(), src->name());
}
