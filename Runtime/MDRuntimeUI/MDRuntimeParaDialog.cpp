#include "MDRuntimeParaDialog.h"
#include "ui_MDRuntimeParaDialog.h"
#include "MDProjectParaForm.h"
#include "MDOPCUAServerParaForm.h"
#include "MDAlarmParaForm.h"
#include "MDHMICreator.h"

/*!
 * \brief 运行参数界面基类
 */
CMDRuntimeParaDialog::CMDRuntimeParaDialog(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDRuntimeParaDialog)
{
    ui->setupUi(this);
}

CMDRuntimeParaDialog::~CMDRuntimeParaDialog()
{
    delete ui;
}

void CMDRuntimeParaDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    //CMDActionProperty_Config* src = (CMDActionProperty_Config*)sourceProperty;
}

bool CMDRuntimeParaDialog::UI2Property(void *destProperty)
{
    return true;
}

/*!
 * \brief 项目运行参数类
 */
CMDProjectParaDialog::CMDProjectParaDialog(CMDProjectConfiger* configer, QWidget *parent) :
    CMDRuntimeParaDialog(parent)
{
    m_form = new CMDProjectParaForm;
    m_form->setParent(this, Qt::Widget);
    m_form->initial(configer);
}

CMDProjectParaDialog::~CMDProjectParaDialog()
{

}

void CMDProjectParaDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    //CMDActionProperty_Config* src = (CMDActionProperty_Config*)sourceProperty;
}

bool CMDProjectParaDialog::UI2Property(void *destProperty)
{
    m_form->accept();

    return true;
}

/*!
 * \brief OPC UA Server参数类
 */
CMDOPCUAServerParaDialog::CMDOPCUAServerParaDialog(CMDProjectConfiger* configer, QWidget *parent) :
    CMDRuntimeParaDialog(parent)
{
    m_form = new CMDOPCUAServerParaForm;
    m_form->setParent(this, Qt::Widget);
    m_form->initial(configer);
}

CMDOPCUAServerParaDialog::~CMDOPCUAServerParaDialog()
{

}

void CMDOPCUAServerParaDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);
}

bool CMDOPCUAServerParaDialog::UI2Property(void *destProperty)
{
    m_form->accept();
    return true;
}

/*!
 * \brief HMI 运行参数类
 */
CMDHMIParaDialog::CMDHMIParaDialog(IHMIFramework* HMIFramework, QWidget *parent) :
    CMDRuntimeParaDialog(parent)
{
    m_form = CMDHMICreator::createLaunchConfigWidget();
    (dynamic_cast<QWidget*>(m_form))->setParent(this, Qt::Widget);
    m_form->init(HMIFramework);

    ui->verticalLayout->addWidget((dynamic_cast<QWidget*>(m_form)));
}

CMDHMIParaDialog::~CMDHMIParaDialog()
{

}

void CMDHMIParaDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    //CMDActionProperty_Config* src = (CMDActionProperty_Config*)sourceProperty;
}

bool CMDHMIParaDialog::UI2Property(void *destProperty)
{
    m_form->onOk();

    return true;
}

/*!
 * \brief 报警运行参数类
 */
CMDAlarmParaDialog::CMDAlarmParaDialog(CMDAlarmConfiger* configer, QWidget *parent) :
    CMDRuntimeParaDialog(parent)
{
    m_form = new CMDAlarmParaForm;
    m_form->setParent(this, Qt::Widget);
    m_form->initial(configer);
}

CMDAlarmParaDialog::~CMDAlarmParaDialog()
{

}

void CMDAlarmParaDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    //CMDActionProperty_Config* src = (CMDActionProperty_Config*)sourceProperty;
}

bool CMDAlarmParaDialog::UI2Property(void *destProperty)
{
    m_form->accept();

    return true;
}
