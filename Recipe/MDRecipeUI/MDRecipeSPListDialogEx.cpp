#include "MDRecipeSPListDialogEx.h"
#include "MDRecipeProperty_Config.h"
#include "MDActionSP.h"
#include "MDStringResource.h"

CMDRecipeSPListDialogEx::CMDRecipeSPListDialogEx(QWidget *parent) :
    CMDActionSPListDialog(parent)
{
}

void CMDRecipeSPListDialogEx::initial(void* objectUI, void *sourceProperty)
{
    CMDActionSPListDialog::initial(objectUI, sourceProperty);
}

bool CMDRecipeSPListDialogEx::UI2Property(void *destProperty)
{
    CMDRecipeProperty_Config* dest = (CMDRecipeProperty_Config*)destProperty;

    dest->head()->setSPList(destList());

    return true;
}

void CMDRecipeSPListDialogEx::initDestList(void *sourceProperty, CMDActionSPList* destList)
{
    CMDRecipeProperty_Config* src = (CMDRecipeProperty_Config*)sourceProperty;

    destList->append(MDActionType::recipe_Upload);
    destList->append(MDActionType::recipe_Download);
    destList->append(MDActionType::recipe_View);
    destList->append(MDActionType::recipe_Modify);
    destList->append(MDActionType::recipe_Delete);
    destList->update(*src->head()->spList(), src->head()->name());
}
