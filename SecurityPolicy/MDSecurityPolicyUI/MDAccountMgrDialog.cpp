#include "MDAccountMgrDialog.h"
#include "ui_MDAccountMgrDialog.h"

CMDAccountMgrDialog::CMDAccountMgrDialog(CMDSecurityPolicyConfiger* configer, QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDAccountMgrDialog)
{
    ui->setupUi(this);

    m_configer = configer;

//    m_accountUI.setConfiger(m_configer);// dGFuZ3p3
    QWidget* mgrUI = m_accountUI.createMgrWidget();
    mgrUI->setParent(this, Qt::Widget);

    ui->verticalLayout->addWidget(mgrUI);
}

CMDAccountMgrDialog::~CMDAccountMgrDialog()
{
    delete ui;
}

void CMDAccountMgrDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);
}

bool CMDAccountMgrDialog::UI2Property(void *destProperty)
{
    return true;
}
