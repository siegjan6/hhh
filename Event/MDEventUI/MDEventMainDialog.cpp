#include "MDEventMainDialog.h"
#include "MDEventConfiger.h"
#include "EPC.h"
#include "MDObjectPropertyDialog.h"
#include "MDEventGeneralDialog.h"
#include "MDEventActionDialog.h"
#include "MDEventConditionDialog_VariableVCIR.h"
#include "MDEventConditionDialog_Combined.h"
#include "MDEventConditionDialog_VariableQC.h"
#include "MDEventConditionDialog_CycleTimer.h"
#include "MDStringResource.h"

CMDEventMainDialog::CMDEventMainDialog(QWidget *parent)
    : CMDObjectPropertyMainDialog(parent)
{
    m_pageID = -1;
    setWindowTitle(STRANS(STRING_EVENT_TEXT));
}

CMDEventMainDialog::~CMDEventMainDialog()
{
}

void CMDEventMainDialog::setTheOnlyPage(int pageID)
{
    m_pageID = pageID;
}

void* CMDEventMainDialog::newProperty(qint16 subType, void* sourceProperty)
{
    return CEPC_Factory::newConfig((MDEventType)subType, (CMDEventProperty_Config*)sourceProperty);
}

void CMDEventMainDialog::initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty)
{
    CMDEventProperty_Config* prop = (CMDEventProperty_Config*)(sourceProperty ? sourceProperty : destProperty);
    MDEventType type = (prop ? prop->type() : MDEventType::null_event);
    CMDObjectPropertyDialog* dialog = NULL;
    switch (type)
    {
    case MDEventType::variable_ValueChangedInRange:
        dialog = new CMDEventConditionDialog_VariableVCIR();
    break;

    case MDEventType::variable_ValueChanged:
        dialog = NULL;
    break;

    case MDEventType::variable_QualityChanged:
        dialog = new CMDEventConditionDialog_VariableQC();
    break;

    case MDEventType::timer_Cycle:
        dialog = new CMDEventConditionDialog_CycleTimer();
    break;

    case MDEventType::variable_ValueWrite:
        dialog = NULL;
    break;

    case MDEventType::variable_ValueRead:
        dialog = NULL;
    break;

    case MDEventType::combined:
        dialog = new CMDEventConditionDialog_Combined();
    break;

    default:
        break;
    }

    if(1 == m_pageID)
    {
        listWidget->insertItem(0,STRING_PROP_ACTION);
        stackWidget->addWidget(new CMDEventActionDialog());
    }
    else if(2 == m_pageID)
    {
        listWidget->insertItem(0,STRING_PROP_COND);
        if(dialog)
            stackWidget->addWidget(dialog);
    }
    else
    {
        listWidget->insertItem(0,STRING_PROP_GENERAL);
        if(dialog)
        {
            listWidget->insertItem(1,STRING_PROP_ACTION);
            listWidget->insertItem(2,STRING_PROP_COND);
        }
        else
            listWidget->insertItem(1,STRING_PROP_ACTION);

        stackWidget->addWidget(new CMDEventGeneralDialog());
        stackWidget->addWidget(new CMDEventActionDialog());
        if(dialog)
            stackWidget->addWidget(dialog);
    }
}

bool CMDEventMainDialog::modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage)
{
    if(!configer)
        return false;

    if(!sourceProperty)
        return false;

    return ((CMDEventConfiger*)configer)->modifyEvent((CMDEventProperty_Config*)sourceProperty, (CMDEventProperty_Config*)destProperty);
}

bool CMDEventMainDialog::addProperty(void* configer, void* destProperty, QString& resultMessage)
{
    if(!configer)
        return false;

    return ((CMDEventConfiger*)configer)->addEvent((CMDEventProperty_Config*)destProperty);
}
