#include "MDRTDBDevicesWindow.h"
#include "MDRTDBDevicesWidget.h"
#include "MDMultiLanguage.h"
CMDRTDBDevicesWindow::CMDRTDBDevicesWindow(QWidget *parent)
{
    setWindowTitle(tr("设备"));
    m_devicesWidget = new CMDRTDBDevicesWidget();
    this->setCentralWidget(m_devicesWidget);
    connect(m_devicesWidget, SIGNAL(signal_Modified()),this, SLOT(on_Modified()));
    connect(m_devicesWidget, SIGNAL(signal_Run()),this, SLOT(on_Run()));

    //多语言
    m_translateTool = CMDMultiLanguage::createWidgetTool();
    m_translateTool->init(this);

    //toolbar
    m_toolBarList = new QList<QToolBar*>();

    //变化时不显示*标志
    setModifiedVisible(false);
}
CMDRTDBDevicesWindow::~CMDRTDBDevicesWindow()
{
    save();
    //!释放toolbar
    if(m_toolBarList != NULL)
    {
        QToolBar* toolbar = NULL;
        for(int i = 0; i< m_toolBarList->count(); i++)
        {
            toolbar = m_toolBarList->at(i);
            delete toolbar;
            toolbar = NULL;
        }
        m_toolBarList->clear();
        delete m_toolBarList;
        m_toolBarList = NULL;
    }
    //!销毁widget
    if(m_devicesWidget)
    {
        delete m_devicesWidget;
        m_devicesWidget =NULL;
    }
}
QList<QToolBar*>* CMDRTDBDevicesWindow::toolBars() const
{
    if(m_toolBarList->count() == 0)
        m_toolBarList->append(m_devicesWidget->toolBar());
    return m_toolBarList;
}

void CMDRTDBDevicesWindow::setRootTreeItem(CMDRTDBManagerUI* RTDBUI,const QString& devName)
{
    m_devicesWidget->setRootTreeItem(RTDBUI,devName);
}

CMDRTDBDevicesWidget * CMDRTDBDevicesWindow::getDeviceWidget()
{
    return m_devicesWidget;
}

bool CMDRTDBDevicesWindow::save()
{
    if(m_devicesWidget!= NULL)
    {
        return m_devicesWidget->save();
    }
    return false;
}
void CMDRTDBDevicesWindow::closeForm()
{
    save();
}

void CMDRTDBDevicesWindow::bindDeviceTreeModel()
{
    if(m_devicesWidget!= NULL)
    {
        m_devicesWidget->bindDeviceTreeModel();
    }
}
void CMDRTDBDevicesWindow::unBindDeviceTreeModel()
{
    if(m_devicesWidget!= NULL)
    {
        m_devicesWidget->unBindDeviceTreeModel();
    }
}

//!多语言切换
void CMDRTDBDevicesWindow::changeLanguage()
{
    if(m_translateTool != NULL)
    {
        m_translateTool->changeLanguage();
        m_devicesWidget->changeLanguage();
    }
}

void CMDRTDBDevicesWindow::setProjectConfiger(CMDProjectConfiger *projectConfiger)
{
    if(m_devicesWidget!= NULL)
    {
        m_devicesWidget->setProjectConfiger(projectConfiger);
    }
}
/*!
\section
*/
void CMDRTDBDevicesWindow::on_Modified()
{
     this->setModify(true);
}
void CMDRTDBDevicesWindow::on_Run()
{
    emit this->signal_Run();
}
