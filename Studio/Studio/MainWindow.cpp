#include <QtWidgets>
#include <QApplication>
#include <QtGlobal>
#include <QSettings>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDir>
#include <QToolBar>
#include <QDockWidget>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#ifdef Q_OS_LINUX
#ifdef QBREAKPAD
    #include "BreakpadHandler.h"
#endif
#endif

#include "MainWindow.h"
#include "NewProjectDialog.h"
#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"
#include "MDSwitchFactory/MDSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"
#include "../../version.h"

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
{
    m_logWidget = NULL;
    m_logSwitch = NULL;
    m_activeForm = NULL;
    m_translateWidgetTool = NULL;
    m_treeProject = NULL;
    m_mdiArea = NULL;

    m_newAction = NULL;
    m_openAction = NULL;
    m_saveAction = NULL;
    m_saveAllAction = NULL;
    m_closeCurrentAction = NULL;
    m_closeAllAction = NULL;
    m_dockConfig = NULL;
    m_studioToolCount = 0;

    m_breakpad_instance = NULL;

    init();
}
MainWindow::~MainWindow()
{
    saveConfig();
    release();
}
//////////////////////////////////////////////////////////////////
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!closeHint())
    {
        event->ignore();
        return;
    }

    return QMainWindow::closeEvent(event);
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    return QMainWindow::eventFilter(obj,event);
}
//////////////////////////////////////////////////////////////////
CPropertyBrowser *MainWindow::propertyBrowser()
{
    return m_property;
}
//////////////////////////////////////////////////////////////////
bool MainWindow::closeHint(bool isAll, const QString& projectId)
{
    QMdiArea* area = m_mdiArea;
    bool isModified = false;

    foreach(QMdiSubWindow* sub, area->subWindowList())
    {
        //单个项目，去除项目外的窗体
        if(!isAll)
        {
            QString id = sub->property("projectID").toString();
            if(id != projectId)
                continue;
        }

        CBaseForm* base = CDevProjectItem::getBaseForm(sub);
        if(base != NULL && base->isModify())
        {
            //不显示修改标记的窗体自动保存，不会有提示保存信息
            if(!base->modifiedVisible())
                base->saveEvent();
            else
                isModified = true;
        }
    }

    if(isModified)
    {
        QMessageBox::StandardButton result = QMessageBox::question(area, tr("确认"), "项目关闭，是否保存？",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);

        if(result == QMessageBox::Cancel)
            return false;

        foreach(QMdiSubWindow* sub, area->subWindowList())
        {
            //单个项目，去除项目外的窗体
            if(!isAll)
            {
                QString id = sub->property("projectID").toString();
                if(id != projectId)
                    continue;
            }

            CBaseForm* base = CDevProjectItem::getBaseForm(sub);
            if(base != NULL && base->isModify())
            {
                if(result == QMessageBox::Yes)
                    base->saveEvent();
                else if(result == QMessageBox::No)
                    base->setModify(false);
            }
        }

    }

    //关闭窗体
    foreach(QMdiSubWindow* sub, area->subWindowList())
    {
        if(!isAll)
        {
            QString id = sub->property("projectID").toString();
            if(id != projectId)
                continue;
        }

        sub->close();
    }

    return true;
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::init()
{
    m_projectManager = new CDevProjectManager;
    m_property = m_projectManager->hmiStudio()->property();

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &MainWindow::customContextMenuRequested, this, &MainWindow::showPopMenu);
    loadConfig();

    initLog();
    initMdiArea();
    initActions();
    initRecentProjectActions();
    initToolBars();
    initTree();
    initDockWidget();
    initMenus();
    updateRecentProjectActions();
    initLanguage();

    setWindowTitle("MindSCADA 1.0");
    setUnifiedTitleAndToolBarOnMac(true);

    m_runtimeAppClient.init("Mind.Runtime");
}
void MainWindow::release()
{
    disconnect(m_mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::activeChanged);

    m_runTimeProcess.close();
    m_logUI.closeAll();

    m_runtimeAppClient.release();

    delete m_projectManager;
    delete m_translateWidgetTool;
    delete m_dockConfig;

    CMDSysLogExpert::release();

    CMDMObjFactory::deleteAllFactories();
}
void MainWindow::initTree()
{
    if(m_treeProject != NULL)
        return;

    m_treeProject = new QTreeWidget(this);

    m_treeProject->setAutoScroll(true);
    m_treeProject->setSortingEnabled(false);
    m_treeProject->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_treeProject->setContextMenuPolicy(Qt::CustomContextMenu);

    QStringList headList;
    headList << tr("Project");
    m_treeProject->setHeaderLabels(headList);
    m_treeProject->header()->setSectionResizeMode(QHeaderView::Interactive);
    m_treeProject->header()->setVisible(false);
}
void MainWindow::initMdiArea()
{
    if(m_mdiArea != NULL)
        return;

    m_mdiArea = new QMdiArea;
    m_mdiArea->installEventFilter(this);
    m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(m_mdiArea);
    m_mdiArea->setViewMode(QMdiArea::TabbedView);
    m_mdiArea->setTabsClosable(true);
    connect(m_mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::activeChanged);

    //tab minimize
    QTabBar *tabBar = m_mdiArea->findChildren<QTabBar*>().at(0);
    if(tabBar)
        tabBar->setExpanding(false);
}
CBaseForm *MainWindow::activeForm()
{
    if (QMdiSubWindow *activeSubWindow = m_mdiArea->activeSubWindow())
        return CDevProjectItem::getBaseForm(activeSubWindow);
    return NULL;
}
void MainWindow::activeChanged(QMdiSubWindow *subWindow)
{
    QMenuBar* mBar = menuBar();
    bool isNoChange = (m_activeForm == CDevProjectItem::getBaseForm(subWindow));

    if(subWindow == NULL)
    {
        m_property->setObject(NULL);
        if(m_activeForm)
        {
            m_activeForm->setActive(false);
            m_activeForm = NULL;
        }
    }
    else
    {
        CBaseForm* form = CDevProjectItem::getBaseForm(subWindow);
        if(!isNoChange)
        {
            if(m_activeForm != NULL)
                m_activeForm->setActive(false);
        }

        form->setActive(true);
        m_activeForm = form;
    }

    if(!isNoChange)
    {
        //menu
        mBar->clear();
        int count = m_studioMenus.count();
        for(int i = 0; i < count - 1; i++)
            mBar->addMenu(m_studioMenus.at(i));

        if(m_activeForm != NULL)
        {
            QList<QMenu*>* menus = m_activeForm->menus();
            if(menus != NULL)
            {
                foreach(QMenu* m, *menus)
                    QAction* a = mBar->addMenu(m);
            }
        }

        //help菜单放最后
        mBar->addMenu(m_studioMenus.at(count - 1));

        //toolbar
        while(m_toolBars.count() > m_studioToolCount)
        {
            removeToolBar(m_toolBars.last());
            m_toolBars.removeLast();
        }

        if(m_activeForm != NULL)
        {
            QList<QToolBar*>* toolBars = m_activeForm->toolBars();
            if(toolBars != NULL)
            {
                foreach(QToolBar* t, *toolBars)
                {
                    addToolBar(t);
                    m_toolBars.append(t);

                    if(!m_toolBarVisibles.contains(t->windowTitle()))
                        m_toolBarVisibles.insert(t->windowTitle(), true);
                    bool visible = m_toolBarVisibles.value(t->windowTitle());
                    t->setVisible(visible);
                }
            }
        }
    }

}
void MainWindow::newProject()
{
    CNewProjectDialog dlg;
    if(dlg.exec() == QDialog::Accepted)
    {
        CMDProjectConfiger configer;
        configer.setName(dlg.m_projectName);
        configer.setPath(dlg.m_projectPath+"/");
        configer.setDescription(dlg.m_description);
        configer.save();
        QString  projectName =dlg.m_projectPath+"/"+dlg.m_projectName+CMDProjectConfiger::projectFileExt();
        loadProject(projectName);
    }
}
void MainWindow::openProject()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("打开项目"),"c:\\","*"  + CMDProjectConfiger::projectFileExt());
    if(!fileName.isEmpty())
        loadProject(fileName);
}
void MainWindow::loadProject(const QString &name)
{
    CDevProjectManager *m = m_projectManager;
    if(m->treeWidget() == NULL)
    {
        m->setTreeWidget(m_treeProject);
        m->setMDIArea(MdiArea());
        m->setMainWindow(this);
    }

    QString strFile = name;

    strFile.replace("\\","/");
    strFile.replace("//","/");

    QFile file(strFile);
    if(file.exists())
    {
        QString strMsg = "加载项目["+strFile+"]";

        if(m->addProject(strFile))
        {
            setCurrentProject(strFile);
            strMsg += "成功";
            changeLanguage();
        }
        else
            strMsg += "失败";
        showLog(strMsg);
    }
    else
    {
        QMessageBox message;
        message.warning(this,tr("警告"),QString("%1 无法访问").arg(strFile));
        message.addButton(tr("确认"),QMessageBox::YesRole);
        removeCurrentProject(strFile);
    }
}
void MainWindow::removeCurrentProject(const QString &fileName)
{
    m_recentProjects.removeAll(fileName);
    updateRecentProjectActions();
}
void MainWindow::setCurrentProject(const QString &fileName)
{
    setWindowFilePath(fileName);

    m_recentProjects.removeAll(fileName);
    m_recentProjects.prepend(fileName);
    while(m_recentProjects.count() > m_recentProjectActions.count())
        m_recentProjects.removeLast();

    updateRecentProjectActions();
}

void MainWindow::about()
{
    QString str = tr("<b>MindStudio</b>\
                     <p>上海鸣鼎自动化科技有限公司</p>\
                     <p>版本:") + QString(GIT_VERSION)+"</p>" +
                     "<p>标识:" + QString(GIT_HASH)+"</p>";
    QMessageBox::about(this, tr("关于"), str);
}
void MainWindow::setActiveSubWindow(QWidget *window)
{
    if(window == NULL || m_mdiArea == NULL)
        return;

    m_mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::run()
{
    CDevProjectItem* project = projectManager()->activeProjectItem();
    if(project == NULL)
        return;

    saveAll();

    QString runtimeName;
    QString projectName;
    getRuntimePara(runtimeName, projectName);

    QString cmd = "RUN:" + projectName;
    if(!m_runtimeAppClient.sendToServer(cmd))
    {
        m_runTimeProcess.start(runtimeName);
        if(m_runTimeProcess.waitForStarted(10000))
        {
            QThread::msleep(3000); // 等待 runtime 启动服务端口
            m_runtimeAppClient.sendToServer(cmd);
        }
    }
}
void MainWindow::getRuntimePara(QString& runtimeName, QString& projectName)
{
    QString name = "MindSCADASvc.x11.32";
#ifdef Q_OS_WIN

#ifdef QT_DEBUG
    name = "MindSCADASvcd.windows.64.exe";
#else
    name = "MindSCADASvc.windows.opt.64.exe";

#endif
#elif defined (Q_OS_LINUX)

#ifdef QT_DEBUG
    name = "MindSCADASvcd.x11.32";
#else
    name = "MindSCADASvc.x11.opt.32";
#endif

#endif

    runtimeName = QCoreApplication::applicationDirPath()+"/"+name;
    CDevProjectManager *m  = projectManager();
    if(m)
        projectName = m->activeProjectItem()->fullName();
}
void MainWindow::runProject(CDevProjectItem *project)
{
    if(project == NULL)
        return;
//    CDevProjectManager *manager = projectManager();
//    if(!closeHint(false, project->fullName()))
//        return;

    saveAll();

    QString runtimeName;
    QString activeProjectName;
    getRuntimePara(runtimeName, activeProjectName);

    QString projectName = project->fullName();

    QString cmd = "RUN:" + projectName;
    if(!m_runtimeAppClient.sendToServer(cmd))
    {
        m_runTimeProcess.start(runtimeName + " -t");
        if(m_runTimeProcess.waitForStarted(10000))
        {
            int trycount = 3;
            int tryIndex = 0;
            while(tryIndex++ < trycount)
            {
                QThread::msleep(1000); // 等待 runtime 启动服务端口
                if(m_runtimeAppClient.sendToServer(cmd))
                    break;
            }
        }
    }
}
void MainWindow::virtualRunProject(CDevProjectItem *project)
{
    if(project == NULL)
        return;
//    CDevProjectManager *manager = projectManager();
//    if(!closeHint(false, project->fullName()))
//        return;

    saveAll();

    QString runtimeName;
    QString activeProjectName;
    getRuntimePara(runtimeName, activeProjectName);

    QString projectName = project->fullName();

    QString cmd = "VirtualRUN:" + projectName;
    if(!m_runtimeAppClient.sendToServer(cmd))
    {
        m_runTimeProcess.start(runtimeName + " -t");
        if(m_runTimeProcess.waitForStarted(10000))
        {
            QThread::msleep(3000); // 等待 runtime 启动服务端口
            m_runtimeAppClient.sendToServer(cmd);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////
void MainWindow::initDockWidget()
{
    QList<QWidget*> widgets;
    widgets << m_treeProject << m_projectManager->hmiStudio()->toolKitDialog();
    widgets << m_logWidget << m_property;
    widgets << m_projectManager->hmiStudio()->objectTreeDialog();

    m_dockConfig = new CDockConfig(this);
    m_dockConfig->loadWidget(&widgets);
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::showPopMenu()
{
    if(m_toolBars.count() == 0)
        return;

    QMenu m;
    foreach(QToolBar* t, m_toolBars)
    {
        QString title = t->windowTitle();
        QAction* a = m.addAction(title);
        a->setData(title);
        a->setCheckable(true);
        a->setChecked(m_toolBarVisibles.value(title));
        connect(a, &QAction::triggered, this, &MainWindow::refreshToolBarVisible);
    }

    CMDMultiLanguage::instance()->translateWidget(&m);
    m.exec(QCursor::pos());
}
void MainWindow::refreshToolBarVisible(bool checked)
{
    QAction* a = qobject_cast<QAction*>(sender());
    if(a == NULL)
        return;

    QString title = a->data().toString();
    m_toolBarVisibles[title] = checked;
    foreach(QToolBar* t, m_toolBars)
    {
        if(t->windowTitle() == title)
        {
            t->setVisible(checked);
            return;
        }
    }
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::saveConfig()
{
    QString name = initConfigPath() + "studio.config";
    QFile file(name);
    if (!file.open(QIODevice::WriteOnly))
        return;

    QJsonObject json;
    QJsonArray array;
    foreach(QString p, m_recentProjects)
        array.append(p);
    json.insert("RecentProjects", array);

    QJsonDocument doc;
    doc.setObject(json);
    QByteArray ba = doc.toJson();

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << ba;

    file.close();
}
void MainWindow::loadConfig()
{
    QString name = initConfigPath() + "studio.config";
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    QByteArray ba = stream.readAll().toUtf8();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(ba, &error);
    if(error.error != QJsonParseError::NoError)
        return;

    QJsonArray array = doc.object().value("RecentProjects").toArray();
    m_recentProjects.clear();
    foreach(QJsonValue v, array)
        m_recentProjects.append(v.toString());
}
QString MainWindow::initConfigPath()
{
    QString path = QCoreApplication::applicationDirPath() + "/config/";
    QDir dir(path);
    if (!dir.exists())
        dir.mkdir(path);

    return path;
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::initRecentProjectActions()
{
    if(m_recentProjectActions.count() > 0)
        return;

    const int Max = 5;
    for(int i = 0; i < Max; i++)
    {
        QAction* a = new QAction(this);
        a->setVisible(false);
        connect(a, &QAction::triggered, this, &MainWindow::openRecentProject);
        QString name = QString("recentProject %1").arg(i);
        a->setObjectName(name);

        m_recentProjectActions.append(a);
    }
}
void MainWindow::updateRecentProjectActions()
{
    int max = m_recentProjectActions.count();
    int count = qMin(m_recentProjects.length(), max);
    for(int i = 0; i < count; i++)
    {
        QString name = m_recentProjects.at(i);
        QString text = QString("&%1 %2").arg(i+1).arg(name);
        QAction* a = m_recentProjectActions.at(i);
        a->setText(text);
        a->setData(name);
        a->setVisible(true);
    }

    for(int i = count; i < max; i++)
        m_recentProjectActions.at(i)->setVisible(false);
}
void MainWindow::openRecentProject()
{
    QAction *a = qobject_cast<QAction *>(sender());
    if(!a)
        return;

    QString name = a->data().toString();
    loadProject(name);
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::saveAll()
{
    QMdiArea* area = m_mdiArea;
    foreach(QMdiSubWindow* sub, area->subWindowList())
    {
        CBaseForm* f = CDevProjectItem::getBaseForm(sub);
        if(f != NULL)
            f->saveEvent();
    }
}
void MainWindow::saveCurrent()
{
    CBaseForm* f = activeForm();
    if (f == NULL)
        return;
    f->saveEvent();
}
void MainWindow::closeCurrentProject()
{
    closeProject(projectManager()->activeProjectItem());
}
void MainWindow::closeAllProject()
{
    CDevProjectManager *m = projectManager();
    if(m)
    {
        if(!closeHint())
            return;

        m->closeAllProject();
    }
}
void MainWindow::closeProject(CDevProjectItem* project)
{
    if(project == NULL)
        return;
    CDevProjectManager *manager = projectManager();
    if(!closeHint(false, project->fullName()))
        return;

    manager->closeProject(project);
}
void MainWindow::reloadProject(CDevProjectItem *project)
{
    QString projectPath = project->fullName();
    closeProject(project);
    loadProject(projectPath);
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::changeLanguage()
{
    m_translateWidgetTool->changeLanguage();
    m_projectManager->changeLanguage();
}
void MainWindow::collectWords()
{
    m_projectManager->collectWords();
}
void MainWindow::initLanguage()
{
    if(m_translateWidgetTool != NULL)
        return;

    m_translateWidgetTool = CMDMultiLanguage::createWidgetTool();
    QStringList names;
    for(int i = 0; i < m_recentProjectActions.count(); i++)
        names.append(m_recentProjectActions.at(i)->objectName());
    m_translateWidgetTool->setSeveredNames(names);
    m_translateWidgetTool->init(this);
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::showLog(const QString &msg)
{
    m_logSwitch->LOG_INFO(msg);
}
void MainWindow::initLog()
{
    if(m_logSwitch != NULL)
        return;

    QString appPath = QCoreApplication::applicationDirPath();
    m_logSwitch = CMDLogSwitchFactory::createSwitchWithLogger(appPath, "Studio", "MindSCADA 开发器");
    m_logSwitch->turnOn();
    m_logWidget = m_logUI.createModaless(m_logSwitch->logger());
}
//////////////////////////////////////////////////////////////////////////////
void MainWindow::initActions()
{
    m_newAction = new QAction(QIcon(":/images/new.png"), tr("新建项目"), this);
    m_newAction->setShortcuts(QKeySequence::New);
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newProject);

    m_openAction = new QAction(QIcon(":/images/open.png"), tr("打开项目..."), this);
    m_openAction->setShortcuts(QKeySequence::Open);
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openProject);

    m_saveAction = new QAction(QIcon(":/images/save.png"), tr("保存"), this);
    m_saveAction->setShortcuts(QKeySequence::Save);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveCurrent);

    m_saveAllAction = new QAction(QIcon(":/images/saveAll.png"), tr("全部保存"), this);
    connect(m_saveAllAction, &QAction::triggered, this, &MainWindow::saveAll);

    m_closeCurrentAction = new QAction(QIcon(),tr("关闭当前项目"), this);
    m_closeCurrentAction->setShortcuts(QKeySequence::Quit);
    connect(m_closeCurrentAction, &QAction::triggered,
            this, &MainWindow::closeCurrentProject);

    m_closeAllAction = new QAction(QIcon(),tr("关闭所有项目"), this);
    m_closeAllAction->setShortcuts(QKeySequence::Quit);
    connect(m_closeAllAction, &QAction::triggered, this, &MainWindow::closeAllProject);
}
void MainWindow::initMenus()
{
    QAction* a = NULL;

    QMenu* m = menuBar()->addMenu(tr("文件"));
    m->addAction(m_newAction);
    m->addAction(m_openAction);
    m->addSeparator();
    m->addAction(m_closeCurrentAction);
    m->addAction(m_closeAllAction);
    m->addSeparator();
    m->addAction(m_saveAction);
    m->addAction(m_saveAllAction);
    m->addSeparator();

    foreach(QAction* act, m_recentProjectActions)
        m->addAction(act);
    m->addSeparator();

    a = new QAction(tr("退出"), this);
    a->setShortcuts(QKeySequence::Quit);
    connect(a, &QAction::triggered, qApp, &QApplication::closeAllWindows);
    m->addAction(a);
    m_studioMenus.append(m);

    QMenu* dockMenu = menuBar()->addMenu(tr("视图"));
    m_dockConfig->fillDockMenu(dockMenu);
    m_studioMenus.append(dockMenu);

    m = menuBar()->addMenu(tr("帮助"));
    a = new QAction(tr("关于"), this);
    connect(a, &QAction::triggered, this, &MainWindow::about);
    m->addAction(a);
    m_studioMenus.append(m);
}
void MainWindow::initToolBars()
{
    QToolBar* file = addToolBar(tr("文件"));
    file->addAction(m_newAction);
    file->addAction(m_openAction);
    file->addAction(m_saveAction);
    file->addAction(m_saveAllAction);

    QAction* runAction = new QAction(QIcon(":/images/run.png"), tr("运行"), this);
    connect(runAction, &QAction::triggered, this, &MainWindow::run);
    file->addAction(runAction);

    QToolBar* language = addToolBar(tr("翻译"));
    language->addWidget(CMDMultiLanguage::instance()->toolWidget());

    m_toolBars << file << language;

    foreach(QToolBar* t, m_toolBars)
        m_toolBarVisibles.insert(t->windowTitle(), true);

    m_studioToolCount = m_toolBars.count();
}
//////////////////////////////////////////////////////////////////////////////

void MainWindow::setBreakpadInstance(void* inst)
{
#ifdef Q_OS_LINUX
#ifdef QBREAKPAD
    if(inst){
        BreakpadHandler* handle = (BreakpadHandler*)m_breakpad_instance;
        if(handle){
            m_breakpad_instance = inst;
            connect(handle, &BreakpadHandler::onCrash, this, &MainWindow::onCrashDump);
        }
    }
#endif
#endif
}

void MainWindow::onCrashDump(bool is_success, QString dump_dir, QString dump_file_name)
{
    m_logSwitch->LOG_ERROR(QString("crash dump file: result:%1 path:%2 file:%3").arg(is_success).arg(dump_dir).arg(dump_file_name));
    //showLog();
}
