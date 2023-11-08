#include "MDAlarmConditionSPListDialog.h"
#include "MDAlarmCondition.h"
#include "MDActionSP.h"
#include <QMessageBox>
#include "MDStringResource.h"

CMDAlarmConditionSPListDialog::CMDAlarmConditionSPListDialog(QWidget *parent) :
    CMDActionSPListDialog(parent)
{
}

void CMDAlarmConditionSPListDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDActionSPListDialog::initial(objectUI, sourceProperty);
}

bool CMDAlarmConditionSPListDialog::UI2Property(void *destProperty)
{
    CMDAlarmCondition* dest = (CMDAlarmCondition*)destProperty;

    dest->setSPList(destList());

    return true;
}

void CMDAlarmConditionSPListDialog::initDestList(void *sourceProperty, CMDActionSPList* destList)
{
    CMDAlarmCondition* src = (CMDAlarmCondition*)sourceProperty;

    destList->append(MDActionType::alarm_Acknowledge);
    destList->append(MDActionType::alarm_Confirm);
    destList->update(*src->spList(), src->name());
}
