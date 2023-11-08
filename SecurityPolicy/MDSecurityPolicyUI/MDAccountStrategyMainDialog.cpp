#include "MDAccountStrategyMainDialog.h"
#include "MDASAccountPasswordDialog.h"
#include "MDAccountStrategyConfiger.h"
#include "MDStringResource.h"

class CMDAccountStrategyPara
{
public:
     CMDAccountStrategyPara() {}
};

CMDAccountStrategyMainDialog::CMDAccountStrategyMainDialog(QWidget *parent) :
    CMDObjectPropertyMainDialog(parent)
{
    setWindowTitle(STRANS(STRING_AS_TEXT));
    m_configer = NULL;
}

CMDAccountStrategyMainDialog::~CMDAccountStrategyMainDialog()
{
}

void* CMDAccountStrategyMainDialog::newProperty(qint16 subType, void* sourceProperty)
{
    CMDAccountStrategyPara* para = new CMDAccountStrategyPara;
    return para;
}

void CMDAccountStrategyMainDialog::initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty)
{
    listWidget->insertItem(0,STRING_ACCOUNT_AND_PASSWORD);

    CMDASAccountPasswordDialog* dlg = new CMDASAccountPasswordDialog;
    dlg->m_configer = m_configer;
    stackWidget->addWidget(dlg);
}

bool CMDAccountStrategyMainDialog::modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage)
{
    return true;
}

bool CMDAccountStrategyMainDialog::addProperty(void* configer, void* destProperty, QString& resultMessage)
{
    m_configer->saveToFile();

    return true;
}
