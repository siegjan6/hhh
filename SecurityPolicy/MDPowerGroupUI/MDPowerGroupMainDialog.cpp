#include "MDPowerGroupMainDialog.h"
#include "MDPowerGroup.h"
#include "MDPowerGroupConfiger.h"

#include "MDPowerGroupGeneralDialog.h"
#include "MDPowerGroupAccountDialog.h"
#include "MDPowerGroupSPListDialog.h"
#include "MDStringResource.h"

CMDPowerGroupMainDialog::CMDPowerGroupMainDialog(QWidget *parent) :
    CMDObjectPropertyMainDialog(parent)
{
    m_pageID = -1;
    setWindowTitle(STRANS(STRING_POWER_GROUP_TEXT));
}

CMDPowerGroupMainDialog::~CMDPowerGroupMainDialog()
{
}

void CMDPowerGroupMainDialog::setTheOnlyPage(int pageID)
{
    m_pageID = pageID;
}

void* CMDPowerGroupMainDialog::newProperty(qint16 subType, void* sourceProperty)
{
    CMDPowerGroup* newProp = NULL;
    if(!sourceProperty)
        newProp = new CMDPowerGroup();
    else
        newProp = new CMDPowerGroup(*((CMDPowerGroup*)sourceProperty));
    return newProp;
}

void CMDPowerGroupMainDialog::initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty)
{
    if(1 == m_pageID)
    {
        listWidget->insertItem(0,STRING_PROP_ACCOUNT);
        stackWidget->addWidget(new CMDPowerGroupAccountDialog());
    }
    else if(2 == m_pageID)
    {
        listWidget->insertItem(0,STRING_PROP_SP);
        CMDPowerGroupSPListDialog* spListDialog = new CMDPowerGroupSPListDialog;
        spListDialog->setPowerGroupConfiger((CMDPowerGroupConfiger*)m_para.configer());
        stackWidget->addWidget(spListDialog);
    }
    else
    {
        listWidget->insertItem(0,STRING_PROP_COMMON);
        listWidget->insertItem(1,STRING_PROP_ACCOUNT);
        listWidget->insertItem(2,STRING_PROP_SP);

        stackWidget->addWidget(new CMDPowerGroupGeneralDialog());
        stackWidget->addWidget(new CMDPowerGroupAccountDialog());
        CMDPowerGroupSPListDialog* spListDialog = new CMDPowerGroupSPListDialog;
        spListDialog->setPowerGroupConfiger((CMDPowerGroupConfiger*)m_para.configer());
        stackWidget->addWidget(spListDialog);
    }
}

bool CMDPowerGroupMainDialog::modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage)
{
    if(!configer)
        return false;

    if(!sourceProperty)
        return false;

    return ((CMDPowerGroupConfiger*)configer)->modify((CMDPowerGroup*)sourceProperty, (CMDPowerGroup*)destProperty);
}

bool CMDPowerGroupMainDialog::addProperty(void* configer, void* destProperty, QString& resultMessage)
{
    if(!configer)
        return false;

    return ((CMDPowerGroupConfiger*)configer)->add((CMDPowerGroup*)destProperty);
}
