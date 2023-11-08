#include "MDAlarmConditionMainDialog.h"

#include "MDAlarmCondition.h"
#include "MDAlarmConditionConfiger.h"

#include "MDAlarmConditionGeneralDialog.h"
#include "MDAlarmConditionParaDialog.h"
#include "MDAlarmConditionSPListDialog.h"
#include "MDStringResource.h"

#include "MDAlarmConditionUIImp.h"
#include "PropertyBrowser.h"

CMDAlarmConditionMainDialog::CMDAlarmConditionMainDialog(QWidget *parent) :
    CMDObjectPropertyMainDialog(parent)
{
    m_pageID = -1;
    setWindowTitle(STRANS(STRING_COND_TEXT));
    m_pgConfiger = NULL;
    m_UIImp = NULL;
}

CMDAlarmConditionMainDialog::~CMDAlarmConditionMainDialog()
{
}

void CMDAlarmConditionMainDialog::setPowerGroupConfiger(CMDPowerGroupConfiger* pgConfiger)
{
    m_pgConfiger = pgConfiger;
}

void CMDAlarmConditionMainDialog::setAlarmConditionUIImp(CMDAlarmConditionUIImp* UIImp)
{
    m_UIImp = UIImp;
}

void CMDAlarmConditionMainDialog::setTheOnlyPage(int pageID)
{
    m_pageID = pageID;
}

void* CMDAlarmConditionMainDialog::newProperty(qint16 subType, void* sourceProperty)
{
    CMDAlarmCondition* newProp = NULL;
    if(!sourceProperty)
        newProp = new CMDAlarmCondition();
    else
        newProp = new CMDAlarmCondition(*((CMDAlarmCondition*)sourceProperty));
    return newProp;
}

void CMDAlarmConditionMainDialog::initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty)
{
    if(1 == m_pageID)
    {
        listWidget->insertItem(0,STRING_PROP_COND);
        stackWidget->addWidget(new CMDAlarmConditionParaDialog());
    }
    else if(2 == m_pageID)
    {
        listWidget->insertItem(0,STRING_PROP_SP);
        CMDAlarmConditionSPListDialog* spListDialog = new CMDAlarmConditionSPListDialog;
        spListDialog->setPowerGroupConfiger(m_pgConfiger);
        stackWidget->addWidget(spListDialog);
    }
    else
    {
        listWidget->insertItem(0,STRING_PROP_COMMON);
        listWidget->insertItem(1,STRING_PROP_COND);
        listWidget->insertItem(2,STRING_PROP_SP);

        stackWidget->addWidget(new CMDAlarmConditionGeneralDialog());
        stackWidget->addWidget(new CMDAlarmConditionParaDialog());
        CMDAlarmConditionSPListDialog* spListDialog = new CMDAlarmConditionSPListDialog;
        spListDialog->setPowerGroupConfiger(m_pgConfiger);
        stackWidget->addWidget(spListDialog);
    }
}

bool CMDAlarmConditionMainDialog::modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage)
{
    if(!configer)
        return false;

    if(!sourceProperty)
        return false;

    return ((CMDAlarmConditionConfiger*)configer)->modify((CMDAlarmCondition*)sourceProperty, (CMDAlarmCondition*)destProperty);
}

bool CMDAlarmConditionMainDialog::addProperty(void* configer, void* destProperty, QString& resultMessage)
{
    if(!configer)
        return false;

    return ((CMDAlarmConditionConfiger*)configer)->add((CMDAlarmCondition*)destProperty);
}
