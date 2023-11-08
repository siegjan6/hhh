#include "MDAccountSPListDialog.h"
#include "MDAccountProperty_Config.h"
#include "MDActionSP.h"
#include <QMessageBox>
#include "MDStringResource.h"

CMDAccountSPListDialog::CMDAccountSPListDialog(QWidget *parent) :
    CMDActionSPListDialog(parent)
{
}

void CMDAccountSPListDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDActionSPListDialog::initial(objectUI, sourceProperty);
}

bool CMDAccountSPListDialog::UI2Property(void *destProperty)
{
    CMDAccountProperty_Config* dest = (CMDAccountProperty_Config*)destProperty;

    dest->setSPList(destList());

    return true;
}

void CMDAccountSPListDialog::initDestList(void *sourceProperty, CMDActionSPList* destList)
{
    CMDAccountProperty_Config* src = (CMDAccountProperty_Config*)sourceProperty;

    destList->append(MDActionType::account_Modify);
    destList->append(MDActionType::account_ModifyPassword);
    destList->append(MDActionType::account_Delete);
    destList->update(*src->spList(), src->name());
}
