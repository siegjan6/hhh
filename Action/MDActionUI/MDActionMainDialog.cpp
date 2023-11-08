#include "MDActionMainDialog.h"
#include "MDActionConfiger.h"

#include "MDObjectPropertyDialog.h"
#include "MDActionParaDialog_VariableWriteValue.h"
#include "MDActionGeneralDialog.h"
#include "MDActionSPListDialogEx.h"
#include "MDStringResource.h"

CMDActionMainDialog::CMDActionMainDialog(QWidget *parent)
    : CMDObjectPropertyMainDialog(parent)
{
    m_pageID = -1;
    setWindowTitle(STRANS(STRING_ACTION_TEXT));
    m_pgConfiger = NULL;
}

CMDActionMainDialog::~CMDActionMainDialog()
{
}

void CMDActionMainDialog::setPowerGroupConfiger(CMDPowerGroupConfiger* pgConfiger)
{
    m_pgConfiger = pgConfiger;
}

void CMDActionMainDialog::setTheOnlyPage(int pageID)
{
    m_pageID = pageID;
}

void* CMDActionMainDialog::newProperty(qint16 subType, void* sourceProperty)
{
    return CAPC_Factory::newConfig((MDActionType)subType, (CMDActionProperty_Config*)sourceProperty);
}

void CMDActionMainDialog::initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty)
{
    CMDActionProperty_Config* prop = (CMDActionProperty_Config*)(sourceProperty ? sourceProperty : destProperty);
    MDActionType type = (prop ? prop->actionID().type() : MDActionType::null_action);
    CMDObjectPropertyDialog* dialog = NULL;
    switch (type)
    {
    case MDActionType::variable_WriteValue:
        dialog = new CMDActionParaDialog_VariableWriteValue();
    break;
    case MDActionType::window_Open:
        dialog = NULL;
    break;
    case MDActionType::window_Close:
        dialog = NULL;
    break;
    case MDActionType::historyGroup_Save:
        dialog = NULL;
    break;
    default:
        break;
    }

    if(2 == m_pageID)
    {
        listWidget->insertItem(0,STRING_PROP_SP);
        CMDActionSPListDialogEx* spListDialog = new CMDActionSPListDialogEx;
        spListDialog->setPowerGroupConfiger(m_pgConfiger);
        stackWidget->addWidget(spListDialog);
    }
    else
    {
        listWidget->insertItem(0,STRING_PROP_GENERAL);
        if(dialog)
        {
            listWidget->insertItem(1,STRING_PROP_PARA);
            listWidget->insertItem(2,STRING_PROP_SP);
        }
        else
            listWidget->insertItem(1,STRING_PROP_SP);

        stackWidget->addWidget(new CMDActionGeneralDialog());
        if(dialog)
            stackWidget->addWidget(dialog);
        CMDActionSPListDialogEx* spListDialog = new CMDActionSPListDialogEx;
        spListDialog->setPowerGroupConfiger(m_pgConfiger);
        stackWidget->addWidget(spListDialog);
    }
}

bool CMDActionMainDialog::modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage)
{
    if(!configer)
        return false;

    if(!sourceProperty)
        return false;

    return ((CMDActionConfiger*)configer)->modifyAction((CMDActionProperty_Config*)sourceProperty, (CMDActionProperty_Config*)destProperty);
}

bool CMDActionMainDialog::addProperty(void* configer, void* destProperty, QString& resultMessage)
{
    if(!configer)
        return false;

    return ((CMDActionConfiger*)configer)->addAction((CMDActionProperty_Config*)destProperty);
}
