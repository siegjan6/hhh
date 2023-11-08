#include "MDAccountMainDialog.h"
#include "MDAccountProperty_Config.h"
#include "MDAccountConfiger.h"
#include "MDSecurityPolicyConfiger.h"

#include "MDAccountGeneralDialog.h"
#include "MDAccountPasswordDialog.h"
#include "MDAccountSPListDialog.h"
#include "MDStringResource.h"

CMDAccountMainDialog::CMDAccountMainDialog(QWidget *parent) :
    CMDObjectPropertyMainDialog(parent)
{
    m_pageID = -1;
    setWindowTitle(STRANS(STRING_ACCOUNT_TEXT));
}

CMDAccountMainDialog::~CMDAccountMainDialog()
{
}

void CMDAccountMainDialog::setTheOnlyPage(int pageID)
{
    m_pageID = pageID;
}

void* CMDAccountMainDialog::newProperty(qint16 subType, void* sourceProperty)
{
    CMDAccountProperty_Config* newProp = NULL;
    if(!sourceProperty)
        newProp = new CMDAccountProperty_Config();
    else
        newProp = new CMDAccountProperty_Config(*((CMDAccountProperty_Config*)sourceProperty));
    return newProp;
}

void CMDAccountMainDialog::initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty)
{
    if(1 == m_pageID)
    {
        listWidget->insertItem(0,STRING_PROP_PASSWORD);

        CMDSecurityPolicyConfiger* spConfiger = (CMDSecurityPolicyConfiger*)m_para.configer();
        CMDAccountPasswordDialog* pDlg = new CMDAccountPasswordDialog;
        pDlg->m_spConfiger = spConfiger;
        stackWidget->addWidget(pDlg);
    }
    else if(2 == m_pageID)
    {
        listWidget->insertItem(0,STRING_PROP_SP);

        CMDSecurityPolicyConfiger* spConfiger = (CMDSecurityPolicyConfiger*)m_para.configer();
        CMDAccountSPListDialog* spListDialog = new CMDAccountSPListDialog;
        spListDialog->setPowerGroupConfiger(spConfiger->powerGroupConfiger());
        stackWidget->addWidget(spListDialog);
    }
    else
    {
        listWidget->insertItem(0,STRING_PROP_COMMON);
        listWidget->insertItem(1,STRING_PROP_PASSWORD);
        listWidget->insertItem(2,STRING_PROP_SP);

        CMDSecurityPolicyConfiger* spConfiger = (CMDSecurityPolicyConfiger*)m_para.configer();
        CMDAccountGeneralDialog* gDlg = new CMDAccountGeneralDialog;
        gDlg->m_spConfiger = spConfiger;
        CMDAccountPasswordDialog* pDlg = new CMDAccountPasswordDialog;
        pDlg->m_spConfiger = spConfiger;
        stackWidget->addWidget(gDlg);
        stackWidget->addWidget(pDlg);

        CMDAccountSPListDialog* spListDialog = new CMDAccountSPListDialog;
        spListDialog->setPowerGroupConfiger(spConfiger->powerGroupConfiger());
        stackWidget->addWidget(spListDialog);
    }
}

bool CMDAccountMainDialog::modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage)
{
    if(!configer)
        return false;

    if(!sourceProperty)
        return false;

    CMDSecurityPolicyConfiger* spConfiger = (CMDSecurityPolicyConfiger*)configer;
    return spConfiger->accountConfiger()->modify((CMDAccountProperty_Config*)sourceProperty, (CMDAccountProperty_Config*)destProperty);
}

bool CMDAccountMainDialog::addProperty(void* configer, void* destProperty, QString& resultMessage)
{
    if(!configer)
        return false;

    CMDSecurityPolicyConfiger* spConfiger = (CMDSecurityPolicyConfiger*)configer;
    return spConfiger->accountConfiger()->add((CMDAccountProperty_Config*)destProperty);
}
