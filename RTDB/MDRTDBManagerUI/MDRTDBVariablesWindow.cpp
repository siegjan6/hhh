#include "MDRTDBVariablesWindow.h"
#include "MDRTDBVariablesWidget.h"
#include "MDMultiLanguage.h"
CMDRTDBVariablesWindow::CMDRTDBVariablesWindow(QWidget *parent)
{
    setWindowTitle(tr("变量"));
    m_variablesWidget = new CMDRTDBVariablesWidget();
    this->setCentralWidget(m_variablesWidget);
    //多语言
    m_translateTool = CMDMultiLanguage::createWidgetTool();
    m_translateTool->init(this);
    m_translateTool->changeLanguage();

    connect(m_variablesWidget, SIGNAL(signal_Modified()),this, SLOT(on_Modified()));
    connect(m_variablesWidget, SIGNAL(signal_Run()),this, SLOT(on_Run()));

    m_toolBarList = new QList<QToolBar*>();

    //变化时不显示*标志
    setModifiedVisible(false);
}
CMDRTDBVariablesWindow::~CMDRTDBVariablesWindow()
{
    save();
    //!释放toolbar
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

    delete m_variablesWidget;
    m_variablesWidget =NULL;
}

QList<QToolBar*>* CMDRTDBVariablesWindow::toolBars() const
{
    if(m_toolBarList->count() == 0)
    {
        m_toolBarList->append(m_variablesWidget->toolBar());
    }
    return m_toolBarList;
}
void CMDRTDBVariablesWindow::setRootTreeItem(CMDRTDBManagerUI* RTDBUI, const QString& groupName)
{
    m_variablesWidget->setRootTreeItem(RTDBUI,groupName);
}

CMDRTDBVariablesWidget * CMDRTDBVariablesWindow::getVariableWidget()
{
    return m_variablesWidget;
}

bool CMDRTDBVariablesWindow::save()
{
    if(m_variablesWidget!= NULL && this->isModify())
    {
        return m_variablesWidget->save();
    }
    return false;
}

void CMDRTDBVariablesWindow::unBindVariableTreeModel()
{
    if(m_variablesWidget!= NULL)
    {
        m_variablesWidget->unBindVariableTreeModel();
    }
}

void CMDRTDBVariablesWindow::bindVariableTreeModel()
{
    if(m_variablesWidget!= NULL)
    {
        m_variablesWidget->bindVariableTreeModel();
    }
}

//!多语言切换
void CMDRTDBVariablesWindow::changeLanguage()
{
    if(m_translateTool != NULL)
    {
        m_translateTool->changeLanguage();
        m_variablesWidget->changeLanguage();
    }
}

void CMDRTDBVariablesWindow::setProjectConfiger(CMDProjectConfiger *projectConfiger)
{
    if(m_variablesWidget!= NULL)
    {
        m_variablesWidget->setProjectConfiger(projectConfiger);
    }
}
void CMDRTDBVariablesWindow::on_Modified()
{
     this->setModify(true);
}
void CMDRTDBVariablesWindow::on_Run()
{
    emit this->signal_Run();
}
