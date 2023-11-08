#include "MDSecurityPolicyMainDialog.h"
#include "MDAccountMgrDialog.h"
#include "MDPowerGroupMgrDialog.h"
#include "MDStringResource.h"

class CMDSPPara
{
public:
     CMDSPPara() {}
};

CMDSecurityPolicyMainDialog::CMDSecurityPolicyMainDialog(QWidget *parent) :
    CMDObjectPropertyMainDialog(parent)
{
    setWindowTitle(STRANS(STRING_ACCOUNT_AND_POWER_GROUP));
    m_configer = NULL;
}

CMDSecurityPolicyMainDialog::~CMDSecurityPolicyMainDialog()
{
}

void* CMDSecurityPolicyMainDialog::newProperty(qint16 subType, void* sourceProperty)
{
    CMDSPPara* para = new CMDSPPara;
    return para;
}

void CMDSecurityPolicyMainDialog::initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty)
{
    listWidget->insertItem(0,STRING_ACCOUNT);
    listWidget->insertItem(1,STRING_POWER_GROUP);

    stackWidget->addWidget(new CMDAccountMgrDialog(m_configer));
    stackWidget->addWidget(new CMDPowerGroupMgrDialog(m_configer));
}

bool CMDSecurityPolicyMainDialog::modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage)
{
    return true;
}

bool CMDSecurityPolicyMainDialog::addProperty(void* configer, void* destProperty, QString& resultMessage)
{
    //m_configer->saveToFile();

    return true;
}
