#include <QtWidgets>
#include <QApplication>
#include <QMessageBox>
#include <QAction>
#include <QDesktopServices>

#include "MainWindow.h"
#include "DevProjectManager.h"
//////////////////////////////////////////////////////////////////////////
CDevProjectManager::CDevProjectManager()
{
    m_popupMenu = NULL;
    m_treeWidget = NULL;
    m_mdiArea = NULL;
    m_mainWindow = NULL;
    m_activeProjectItem = NULL;
    m_hmiStudio = CMDHMICreator::createHMIStudio();
}
CDevProjectManager::~CDevProjectManager()
{
    removeAllItem();
    delete m_hmiStudio;
}
//////////////////////////////////////////////////////////////////////////
void CDevProjectManager::setTreeWidget(QTreeWidget *value)
{
    m_treeWidget = value;
    init();
}
void CDevProjectManager::setMainWindow(MainWindow *value)
{
    m_mainWindow = value;
}
MainWindow *CDevProjectManager::mainWindow()
{
    return m_mainWindow;
}
void CDevProjectManager::setActiveProjectItem(CDevProjectItem* value)
{
    m_activeProjectItem = value;
    updateActiveUI();
}
//////////////////////////////////////////////////////////////////////////
void CDevProjectManager::init()
{
    connect(m_treeWidget, &QTreeWidget::customContextMenuRequested, this, &CDevProjectManager::callTreeMenu);

    m_popupMenu = new QMenu(m_treeWidget);
    QAction* active = new QAction(tr("设置活动项目"),m_popupMenu);
    connect(active, &QAction::triggered, this, &CDevProjectManager::activeProject);
    QAction* run = new QAction(tr("运行"),m_popupMenu);
    connect(run, &QAction::triggered, this, &CDevProjectManager::runProject);
    QAction* virtualRun = new QAction(tr("虚拟化"),m_popupMenu);
    connect(virtualRun, &QAction::triggered, this, &CDevProjectManager::virtualRunProject);
    QAction* locate = new QAction(tr("定位到目录"),m_popupMenu);
    connect(locate, &QAction::triggered, this, &CDevProjectManager::locateProject);
    QAction* close = new QAction(tr("关闭项目"),m_popupMenu);
    connect(close, &QAction::triggered, this, &CDevProjectManager::closeProjectFun);
    QAction* reload = new QAction(tr("重新加载"),m_popupMenu);
    connect(reload, &QAction::triggered, this, &CDevProjectManager::reloadProject);

    m_popupMenu->addAction(active);
    m_popupMenu->addAction(run);
    m_popupMenu->addAction(virtualRun);
    m_popupMenu->addAction(locate);
    m_popupMenu->addAction(close);
    m_popupMenu->addAction(reload);
}
void CDevProjectManager::activeProject()
{
    QTreeWidgetItem* current = m_treeWidget->currentItem();
    if(current == NULL)
        return;

    CDevProjectItem* item = findItem(current);
    if(item)
        setActiveProjectItem(item);
}
void CDevProjectManager::runProject()
{
    QTreeWidgetItem* current = m_treeWidget->currentItem();
    if(current == NULL)
        return;
    CDevProjectItem* item = findItem(current);
    m_mainWindow->runProject(item);
}
void CDevProjectManager::virtualRunProject()
{
    QTreeWidgetItem* current = m_treeWidget->currentItem();
    if(current == NULL)
        return;
    CDevProjectItem* item = findItem(current);
    m_mainWindow->virtualRunProject(item);
}
void CDevProjectManager::locateProject()
{
    QTreeWidgetItem* current = m_treeWidget->currentItem();
    if(current == NULL)
        return;

    CDevProjectItem* item = findItem(current);
    if(item)
    {
        QFileInfo info = QFileInfo(item->fullName());
        QDesktopServices::openUrl(QUrl::fromLocalFile(info.absolutePath()));
    }
}
void CDevProjectManager::closeProjectFun()
{
    QTreeWidgetItem* current = m_treeWidget->currentItem();
    if(current == NULL)
        return;
    CDevProjectItem* item = findItem(current);
    m_mainWindow->closeProject(item);
}
void CDevProjectManager::reloadProject()
{
    QTreeWidgetItem* current = m_treeWidget->currentItem();
    if(current == NULL)
        return;
    CDevProjectItem* item = findItem(current);
    m_mainWindow->reloadProject(item);
}
void CDevProjectManager::callTreeMenu(const QPoint &pos)
{
    int index = m_treeWidget->indexOfTopLevelItem(m_treeWidget->currentItem());
    if(index == -1)
        return;

    QPoint point = m_treeWidget->viewport()->mapToGlobal(pos);
    m_popupMenu->exec(point);
}
//////////////////////////////////////////////////////////////////////////
bool CDevProjectManager::removeItem(CDevProjectItem *item)
{
    bool success = m_projectItems.removeOne(item);
    if(success)
    {
        item->unload();
        delete item;
    }

    return success;
}
void CDevProjectManager::removeAllItem()
{
    foreach(CDevProjectItem* item, m_projectItems)
        item->unload();
    qDeleteAll(m_projectItems);
    m_projectItems.clear();
}
CDevProjectItem *CDevProjectManager::findItem(QTreeWidgetItem *item)
{
    foreach(CDevProjectItem* project, m_projectItems)
    {
        if(project->projectRoot() == item)
            return project;
    }
    return NULL;
}
CDevProjectItem *CDevProjectManager::findItem(const QString &fullName)
{
    foreach(CDevProjectItem* project, m_projectItems)
    {
        if(project->fullName().compare(fullName, Qt::CaseInsensitive) == 0)
            return project;
    }
    return NULL;
}
void CDevProjectManager::resetCurrentProject()
{
    int count = m_projectItems.count();
    CDevProjectItem* item = (count>0) ? m_projectItems.last() : NULL;
    setActiveProjectItem(item);
}
void CDevProjectManager::updateActiveUI()
{
    foreach(CDevProjectItem* project, m_projectItems)
    {
        bool isActive = (project == m_activeProjectItem);
        changeItemFont(project, isActive);
        if(isActive)
        {
            QTreeWidgetItem* item = project->projectRoot();
            m_treeWidget->setCurrentItem(item);
            item->setExpanded(true);
        }
    }
}
void CDevProjectManager::changeItemFont(CDevProjectItem *project, bool isSelect)
{
    QTreeWidgetItem *item = project->projectRoot();

    QFont font = QGuiApplication::font();
    font.setBold(isSelect);
    item->setFont(0,font);
    if(isSelect)
        item->setTextColor(0,QColor(0, 0, 255, 255));
    else
        item->setTextColor(0,QColor(0, 0, 0, 255));
}
//////////////////////////////////////////////////////////////////////////
void CDevProjectManager::closeProject(CDevProjectItem *item)
{
    removeItem(item);
    resetCurrentProject();
}
void CDevProjectManager::closeAllProject()
{
    removeAllItem();
    resetCurrentProject();
}
bool CDevProjectManager::addProject(const QString &fullName)
{
    if(findItem(fullName))
        return false;

    CDevProjectItem *project = new CDevProjectItem;
    project->setManager(this);
    project->load(fullName);
    m_projectItems.append(project);
    setActiveProjectItem(project);
    return true;
}
//////////////////////////////////////////////////////////////////////////
void CDevProjectManager::changeLanguage()
{
    foreach (CDevProjectItem* item, m_projectItems)
        item->changeLanguage();
}
void CDevProjectManager::collectWords()
{
    foreach (CDevProjectItem* item, m_projectItems)
        item->collectWords();
}
