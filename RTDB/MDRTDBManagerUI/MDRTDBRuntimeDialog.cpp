#include "MDRTDBRuntimeDialog.h"
#include "MDMultiLanguage.h"
CMDRTDBRuntimeDialog::CMDRTDBRuntimeDialog(QWidget *parent):QDialog(parent)
{
    setWindowTitle(tr("数据采集"));
    m_listWidget=new QListWidget(this);
    m_listWidget->insertItem(0,tr("变量"));
    m_listWidget->insertItem(1,tr("设备"));
    m_listWidget->setMaximumWidth(120);

    m_variablesWidget = new CMDRTDBVariablesWidget();
    m_devicesWidget = new CMDRTDBDevicesWidget();
    m_stackWidget=new QStackedWidget(this);
    m_stackWidget->addWidget(m_variablesWidget);
    m_stackWidget->addWidget(m_devicesWidget);

    m_mainLayout = new QHBoxLayout;
    m_mainLayout->addWidget(m_listWidget);
    m_mainLayout->addWidget(m_stackWidget);
    m_mainLayout->setMargin(1);
    m_mainLayout->setStretchFactor(m_listWidget,0);
    m_mainLayout->setStretchFactor(m_stackWidget,1);
    //多语言
    CMDMultiLanguage::instance()->translateWidget(this);

    setLayout(m_mainLayout);
    connect(m_listWidget,SIGNAL(currentRowChanged(int)),m_stackWidget,SLOT(setCurrentIndex(int)));
}

CMDRTDBRuntimeDialog::~CMDRTDBRuntimeDialog()
{
    if(m_listWidget)
    {
        delete m_listWidget;
        m_listWidget = NULL;
    }

    if(m_variablesWidget)
    {
        delete m_variablesWidget;
        m_variablesWidget = NULL;
    }

    if(m_devicesWidget)
    {
        delete m_devicesWidget;
        m_devicesWidget = NULL;
    }

    if(m_stackWidget)
    {
        delete m_stackWidget;
        m_stackWidget = NULL;
    }

    if(m_mainLayout)
    {
        delete m_mainLayout;
        m_mainLayout = NULL;
    }
}

void CMDRTDBRuntimeDialog::setProjectConfiger(CMDProjectConfiger *projectConfiger)
{
    if(m_variablesWidget)
    {
        m_variablesWidget->setProjectConfiger(projectConfiger);
    }
    if(m_devicesWidget)
    {
        m_devicesWidget->setProjectConfiger(projectConfiger);
    }
}

void CMDRTDBRuntimeDialog::showEvent(QShowEvent *event)
{
    m_variablesWidget->toolBar()->setVisible(false);
}
