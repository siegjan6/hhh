#include "MDRecipeMainDialog.h"

#include "MDObjectPropertyDialog.h"
#include "MDRecipeItemsDialog.h"
#include "MDRecipeDownloadParaDialog.h"
#include "MDRecipeSPListDialogEx.h"
#include "MDStringResource.h"
#include "MDRecipeProperty_Config.h"
#include "MDProjectConfiger.h"

CMDRecipeMainDialog::CMDRecipeMainDialog(QWidget *parent)
    : CMDObjectPropertyMainDialog(parent)
{
    m_pageID = -1;
    setWindowTitle(STRANS(STRING_RECIPE_TEXT));
    m_pConfiger = NULL;
}

CMDRecipeMainDialog::~CMDRecipeMainDialog()
{
}

void CMDRecipeMainDialog::setProjectConfiger(CMDProjectConfiger* pConfiger)
{
    m_pConfiger = pConfiger;
}

void CMDRecipeMainDialog::setTheOnlyPage(int pageID)
{
    m_pageID = pageID;
}

void* CMDRecipeMainDialog::newProperty(qint16 subType, void* sourceProperty)
{
    CMDRecipeProperty_Config* src = (CMDRecipeProperty_Config*)sourceProperty;
    CMDRecipeProperty_Config* newProperty = new CMDRecipeProperty_Config(*src);
    return newProperty;
}

void CMDRecipeMainDialog::initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty)
{
    if(0 == m_pageID)
    {
        listWidget->insertItem(0,STRING_PROP_RECIPE_ITEMS);
        stackWidget->addWidget(new CMDRecipeItemsDialog(m_pConfiger));
    }
    else if(2 == m_pageID)
    {
        listWidget->insertItem(0,STRING_PROP_SP);

        CMDRecipeSPListDialogEx* spListDialog = new CMDRecipeSPListDialogEx;
        spListDialog->setPowerGroupConfiger(m_pConfiger->powerGroupConfiger());
        stackWidget->addWidget(spListDialog);
    }
    else
    {
        listWidget->insertItem(0,STRING_PROP_RECIPE_ITEMS);
        listWidget->insertItem(1,STRING_PROP_DOWNLOAD_PARA);
        listWidget->insertItem(2,STRING_PROP_SP);

        stackWidget->addWidget(new CMDRecipeItemsDialog(m_pConfiger));
        stackWidget->addWidget(new CMDRecipeDownloadParaDialog());
        CMDRecipeSPListDialogEx* spListDialog = new CMDRecipeSPListDialogEx;
        spListDialog->setPowerGroupConfiger(m_pConfiger->powerGroupConfiger());
        stackWidget->addWidget(spListDialog);
    }
}

bool CMDRecipeMainDialog::modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage)
{
        return true;
//    if(!configer)
//        return false;

//    if(!sourceProperty)
//        return false;

//    return ((CMDActionConfiger*)configer)->modifyAction((CMDActionProperty_Config*)sourceProperty, (CMDActionProperty_Config*)destProperty);
}

bool CMDRecipeMainDialog::addProperty(void* configer, void* destProperty, QString& resultMessage)
{
    if(!configer)
        return false;
        return true;

//    return ((CMDActionConfiger*)configer)->addAction((CMDActionProperty_Config*)destProperty);
}
