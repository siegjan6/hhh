#include "MDPowerGroupSPListDialog.h"
#include "MDPowerGroup.h"
#include "MDActionSP.h"
#include "MDStringResource.h"

CMDPowerGroupSPListDialog::CMDPowerGroupSPListDialog(QWidget *parent) :
    CMDActionSPListDialog(parent)
{
}

void CMDPowerGroupSPListDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDActionSPListDialog::initial(objectUI, sourceProperty);
}

bool CMDPowerGroupSPListDialog::UI2Property(void *destProperty)
{
    CMDPowerGroup* dest = (CMDPowerGroup*)destProperty;

    dest->setSPList(destList());

    return true;
}

void CMDPowerGroupSPListDialog::initDestList(void *sourceProperty, CMDActionSPList* destList)
{
    CMDPowerGroup* src = (CMDPowerGroup*)sourceProperty;

    destList->append(MDActionType::powerGroup_Modify);
    destList->append(MDActionType::powerGroup_Delete);
    destList->update(*src->spList(), src->name());
}
