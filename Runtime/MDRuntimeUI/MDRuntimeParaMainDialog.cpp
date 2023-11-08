#include "MDRuntimeParaMainDialog.h"

#include "MDObjectPropertyDialog.h"
#include "MDRuntimeParaDialog.h"
#include "MDRuntimeUIImp.h"
#include "MDProjectConfiger.h"
#include "MDStringResource.h"

CMDRuntimeParaMainDialog::CMDRuntimeParaMainDialog(QWidget *parent)
    : CMDObjectPropertyMainDialog(parent)
{
    setWindowTitle(STRANS(STRING_RUN_PARA));
    m_uiImp = NULL;
}

CMDRuntimeParaMainDialog::~CMDRuntimeParaMainDialog()
{
}

void CMDRuntimeParaMainDialog::setUIImp(CMDRuntimeUIImp* uiImp)
{
    m_uiImp = uiImp;
}

void* CMDRuntimeParaMainDialog::newProperty(qint16 subType, void* sourceProperty)
{
    CMDRuntimePara* para = new CMDRuntimePara;
    return para;
}

void CMDRuntimeParaMainDialog::initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty)
{
    listWidget->insertItem(0,STRANS(STRING_PROP_PROJECT));
    listWidget->insertItem(1,STRANS(STRING_PROP_HMI));
    listWidget->insertItem(2,STRANS(STRING_PROP_ALARM));
    listWidget->insertItem(3,STRANS(STRING_PROP_OPCUA));

    stackWidget->addWidget(new CMDProjectParaDialog(m_uiImp->m_studioInterface->projectConfiger()));
    stackWidget->addWidget(new CMDHMIParaDialog(m_uiImp->m_studioInterface->hmiFramework()));
    stackWidget->addWidget(new CMDAlarmParaDialog(m_uiImp->m_studioInterface->projectConfiger()->alarmConfiger()));
    stackWidget->addWidget(new CMDOPCUAServerParaDialog(m_uiImp->m_studioInterface->projectConfiger()));
}

bool CMDRuntimeParaMainDialog::modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage)
{
    return true;
}

bool CMDRuntimeParaMainDialog::addProperty(void* configer, void* destProperty, QString& resultMessage)
{
     return true;
}
