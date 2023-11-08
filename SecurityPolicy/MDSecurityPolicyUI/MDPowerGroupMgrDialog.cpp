#include "MDPowerGroupMgrDialog.h"
#include "ui_MDPowerGroupMgrDialog.h"
#include "MDSecurityPolicyConfiger.h"

CMDPowerGroupMgrDialog::CMDPowerGroupMgrDialog(CMDSecurityPolicyConfiger* configer, QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDPowerGroupMgrDialog)
{
    ui->setupUi(this);

    m_configer = configer;

//    m_pgUI.setConfiger(m_configer->powerGroupConfiger());// dGFuZ3p3
    QWidget* mgrUI = m_pgUI.createMgrWidget();
    mgrUI->setParent(this, Qt::Widget);

    ui->verticalLayout->addWidget(mgrUI);
}

CMDPowerGroupMgrDialog::~CMDPowerGroupMgrDialog()
{
    delete ui;
}

void CMDPowerGroupMgrDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);
}

bool CMDPowerGroupMgrDialog::UI2Property(void *destProperty)
{
    return true;
}

