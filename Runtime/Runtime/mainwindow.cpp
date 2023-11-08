#include <QtWidgets>
#include <QDir>
#include "../../version.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include "mainwindow.h"
#include "SingleRuntimeApp.h"
#include "../../include/MDRTDBManagerUI/MDRTDBManagerUIFactory.h"
#include "../../include/MDSQLDataBaseProxy/MDDatabaseRecordset.h"
#include "MDProjectConfiger.h"
#include "MDSecurityPolicyConfiger.h"
#include "MDSecurityPolicyUI.h"

#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"
#include "MDStringResource.h"
#include "../../include/MDSwitchFactoryUI/MDSwitchFactoryUI.h"
#include "../../include/MDSwitchFactory/MDLogSwitchFactory.h"

#ifdef Q_OS_LINUX
#ifdef QBREAKPAD
    #include "BreakpadHandler.h"
#endif
#endif

MainWindow::MainWindow(CMDLogSwitch* lsRuntime)
{
    m_lsRuntime = lsRuntime;

    m_HMIFramework = NULL;
    m_runtimeEnvSettings  = NULL;
    m_mdRTDBRuntimeUI = NULL;
    m_logDialog = NULL;
    m_pProcess = NULL;
    m_fileMenu = NULL;
    m_helpMenu = NULL;

    m_exitAct = NULL;
    m_aboutAct = NULL;
    m_configRuntimeEnvAct = NULL;
    m_loadProjectAct = NULL;
    m_unloadProjectAct = NULL;
    m_showSysLogUIAct = NULL;
    m_showEnginesUIAct = NULL;
    m_configFuncFactoryUIAct = NULL;
    m_configLogFactoryUIAct = NULL;
    m_toggleTerminalFactoryUIAct = NULL;
    m_toggleBackgroundRunAct = NULL;
    m_openRTDBWindowAct = NULL;
    m_enterFSAct = NULL;
    m_closeAllAct = NULL;
    m_loginAct = NULL;
    m_logoutAct = NULL;
    m_showSPMgrAct = NULL;
    m_breakpad_instance = NULL;

    //QSettings::setDefaultFormat(QSettings::IniFormat);
    //QSettings::setPath(QSettings::IniFormat, QSettings::UserScope,".");
    //m_runtimeEnvSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "MindSoft", "MindSCADA-Runtime");
    m_runtimeEnvSettings = new QSettings(paraFileName(), QSettings::IniFormat);
    m_runtimeEnvSettings->setIniCodec(QTextCodec::codecForName("UTF-8"));

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    setWindowTitle(buildCommonTitle());

    m_projectContacter.m_mainWnd = this;

    initHMIFramework();

    m_runtimeAppClient.init("Mind.MDRuntimeLoadMonitord");

    m_dictManager = NULL;
    m_translateWidgetTool = CMDMultiLanguage::createWidgetTool();
    m_translateWidgetTool->init(this);

//    trayIcon = new QSystemTrayIcon(this);
//    trayIcon->setIcon(QIcon(":/images/new.png"));
//    trayIcon->setToolTip("MindSCADASvc");
//    //QString titlec=tr("Hello");
//    //QString textc=QString::fromLocal8Bit("Hello QT?");
//    trayIcon->show();
//    //trayIcon->showMessage(titlec,textc,QSystemTrayIcon::Information,5000);
//    connect(trayIcon, &QSystemTrayIcon::activated, [this](auto reason)
//    {
//        if(reason == QSystemTrayIcon::Trigger)
//        {
//            if(isVisible())
//            {
//                hide();
//            }
//            else
//            {
//                show();
//                activateWindow();
//            }
//        }
//    });
}

MainWindow::~MainWindow()
{
    m_lsRuntime->LOG_ERROR(QString("MainWindow ~"));
    if(m_pProcess)
    {
        m_pProcess->close();
        delete m_pProcess;
        m_pProcess = NULL;
    }

    m_runtimeAppClient.release();
    m_lsRuntime->LOG_ERROR(QString("runtimeAppClient release"));

    if(m_translateWidgetTool)
        delete m_translateWidgetTool;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 卸载失败，且项目仍然处于“已加载”状态，不能退出
    if(false == unloadCurrentProject() &&
        m_projectExpert.loadStatus() == MDProjectLoadStatus::loaded)
    {
        event->ignore();
        return;
    }

    releaseHMIFramework();

    if(m_runtimeEnvSettings)
        delete m_runtimeEnvSettings;

    m_sysLogUI.closeAll();
    m_eginesUI.showMgrUI(false);
    m_eginesUI.release();

    event->accept();
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    if(event->key() == Qt::Key_Escape)
    {
        exitFSWithSecurityCheck();
    }
}

#include <QApplication>
void MainWindow::about()
{
    QString str = tr("<b>MindSCADASvc</b>\
                     <p>上海鸣鼎自动化科技有限公司</p>\
                     <p>版本:") + QString(GIT_VERSION)+"</p>" +
                     "<p>标识:" + QString(GIT_HASH)+"</p>";
   QMessageBox::about(this, STRANS(STRING_ABOUT), str);
   //QMessageBox::about(this, STRANS(STRING_ABOUT), QString(STRANS(STRING_VERSION)).arg(QApplication::applicationVersion()));
}

/*!
\section 管理项目（加载；卸载；显示项目信息；...）
*/
void MainWindow::reloadProject(const QString& projectFileName, bool isVrtualRunMode/* = false*/)
{
    if(unloadCurrentProject())
    {
        loadProject(projectFileName, isVrtualRunMode);
    }
    else
    {
        if(m_projectExpert.canLoad())
            loadProject(projectFileName, isVrtualRunMode);
    }
}

void MainWindow::loadProject(const QString& projectFileName, bool isVrtualRunMode/* = false*/)
{
    if(!m_projectExpert.canLoad())
        return;

    enableMenuAction(false);

    LOG_RUNTIME("准备运行当前项目->" + projectFileName);
    if (!projectFileName.isEmpty())
        writeSettings_CurrentProject(projectFileName);

    initLanguage(projectFileName);
    changeLanguage();

    LOG_RUNTIME("开始加载当前项目 ...");
    m_projectContacter.setVirtualRunMode(isVrtualRunMode);
    if(!m_projectExpert.load(projectFileName, &m_projectContacter))
    {
        LOG_RUNTIME("加载当前项目失败！");

        unloadCurrentProject();

        return;
    }

    LOG_RUNTIME("加载当前项目完成！");

    LOG_RUNTIME("开始加载 HMI ...");
    loadHMI();
    LOG_RUNTIME("加载 HMI 完成！");

    initialRTDBRuntimeUI();

    enableMenuAction(true);
    m_loadProjectAct->setEnabled(false);
#ifdef Q_OS_LINUX
    m_toggleTerminalFactoryUIAct->setEnabled(false);
#endif
    m_recipeUI.setRecipeExpert(m_projectExpert.recipeExpert());
    m_recipeUI.load();

    LOG_RUNTIME("正在运行当前项目！->" + projectFileName);

    changeLanguage();

    // lyjdebug
//    CMDSysLogExpert::disableAllLoggerOutput();
}

void MainWindow::loadCurrentProject()
{
    QString fileName = readSettings_CurrentProject();
    if (fileName.isEmpty())
        return;

    loadProject(fileName);
}

bool MainWindow::unloadCurrentProject()
{
    if(!m_projectExpert.canUnload())
        return false;

    if(m_mdRTDBRuntimeUI != NULL)
        m_mdRTDBRuntimeUI->closeRTDBWindow();

    // 失败的原因可能是：
    // １、没有通过“加载安全策略”的检查；２、登录失败。
    // 如果失败，都不需要进行“卸载安全策略”的检查。
    // -------------------------------------------------------------------------------------
    if(m_projectExpert.resultOnLoaded())
    {
        if(!doSecurityCheck_Unload())
            return false;
    }

    if(!doSecurityCheck_ExitFS())
            return false;

    m_eginesUI.showMgrUI(false);

    enableMenuAction(false);

    QString curProjectFileName = readSettings_CurrentProject();
    LOG_RUNTIME("准备停止运行当前项目 ..." + curProjectFileName);

    LOG_RUNTIME("开始卸载 HMI ...");
    unloadHMI();
    LOG_RUNTIME("卸载 HMI 完成！");

    m_recipeUI.unload();

    LOG_RUNTIME("开始卸载当前项目 ...");
    m_projectExpert.unload();
    LOG_RUNTIME("卸载当前项目完成！");

    releaseRTDBRuntimeUI();    
    LOG_RUNTIME("当前项目已停止运行！->" + curProjectFileName);
    statusBar()->showMessage("当前项目已停止运行！->" + curProjectFileName);
    setWindowTitle(buildCommonTitle());

    releaseLanguage();

    m_loadProjectAct->setEnabled(true);

    return true;
}

bool MainWindow::doSecurityCheck_Unload()
{
    CMDProjectConfiger* configer = m_projectExpert.projectConfiger();
    CMDActionSP* sp = configer->projectPara()->spList()->find(MDActionType::project_Unload);
    if(sp && sp->isEnable())
    {
        LOG_RUNTIME("检查【卸载】安全策略 ...");

        CMDActionSPSession spSession(*sp);
        if(!m_projectContacter.checkSP_WithUI(&spSession))
        {
            LOG_RUNTIME("没有通过【卸载】安全策略！");
            return false;
        }
        LOG_RUNTIME("【卸载】安全策略通过！");
    }
    return true;
}

void MainWindow::configRuntimeEnv()
{
    QString curProjectFileName = readSettings_CurrentProject();
    QString curPath, nameOnly;
    CMDProjectConfiger::parseProjectName(curProjectFileName, curPath, nameOnly);

    QString fileName = QFileDialog::getOpenFileName(this, STRANS(STRING_LOAD_PROJECT),
                                                                                            curPath, STRANS(STRING_PROJECT) + "(*.devx)");

    if (!fileName.isEmpty())
        writeSettings_CurrentProject(fileName);
}

void MainWindow::viewProjectInfo()
{

}

void MainWindow::login()
{
    m_projectContacter.loginByUI();
}

void MainWindow::logout()
{
    m_projectExpert.securityExpert()->logout();
}

void MainWindow::showSPMgr()
{
    if(!doSecurityCheck_ManageAccountPowerGroup())
        return;

    CMDSecurityPolicyConfiger* sourceConfiger = m_projectExpert.securityExpert()->configer();

    // 临时配置器，直接修改文件但不影响内存信息
    CMDSecurityPolicyConfiger tempConfiger;
    tempConfiger.loadConfig(m_projectExpert.projectConfiger()->path());

    CMDSecurityPolicyUI spUI;
    spUI.setConfiger(&tempConfiger);
    if(spUI.showMgrDialog())
    {
        // 从修改后的文件重新加载
        sourceConfiger->unloadConfig();
        sourceConfiger->loadConfig(m_projectExpert.projectConfiger()->path());
    }
    else
    {
        // 恢复源文件
        sourceConfiger->accountConfiger()->saveToFile();
        sourceConfiger->powerGroupConfiger()->saveToFile();
    }
}

void MainWindow::showRecipeRuntime()
{
    m_recipeUI.showRuntime();
}

void MainWindow::hideRecipeRuntime()
{
    m_recipeUI.hideRuntime();
}

void MainWindow::showRuntime()
{
    show();
}

void MainWindow::hideRuntime()
{
    hide();
}

bool MainWindow::doSecurityCheck_ManageAccountPowerGroup()
{
    CMDProjectConfiger* configer = m_projectExpert.projectConfiger();
    CMDActionSP* sp = configer->projectPara()->spList()->find(MDActionType::account_powergroup_Manage);
    if(sp && sp->isEnable())
    {
        LOG_RUNTIME("检查【管理帐户和组】安全策略 ...");

        CMDActionSPSession spSession(*sp);
        if(!m_projectContacter.checkSP_WithUI(&spSession))
        {
            LOG_RUNTIME("没有通过【管理帐户和组】安全策略！");
            return false;
        }
        LOG_RUNTIME("【管理帐户和组】安全策略通过！");
    }
    return true;
}

QString MainWindow::readSettings_CurrentProject()
{
    return (m_runtimeEnvSettings ? m_runtimeEnvSettings->value("CurrentProject", "").toString() : "");
}

void MainWindow::writeSettings_CurrentProject(const QString& projectFileName)
{
//    QSettings settings("MindSoft", "MindSCADA-Runtime");
//    settings.setValue("CurrentProject", projectFileName);
    if(m_runtimeEnvSettings) {
        QFileInfo info(projectFileName);
        if(info.exists()) {
            m_runtimeEnvSettings->setValue("CurrentProject", projectFileName);
            m_runtimeEnvSettings->setValue("CurrentProjectPath", info.canonicalPath());
        }
    }
}

/*!
\section 管理 HMI 框架
*/
void MainWindow::initHMIFramework()
{
    m_HMIFramework = CMDHMICreator::createHMIFramework();
    m_HMIFramework->setParent(this);
}

void MainWindow::releaseHMIFramework()
{
    if(m_HMIFramework)
    {
        m_HMIFramework->forms()->closeAll();

        delete m_HMIFramework;
        m_HMIFramework = NULL;
    }
}

void MainWindow::loadHMI()
{
    if(m_HMIFramework)
    {
        m_HMIFramework->setProjectExpert(m_projectExpert.IProjectExpert());
        m_HMIFramework->setProjectConfiger(m_projectExpert.projectConfiger());
        m_HMIFramework->load();

        if(m_HMIFramework->isLaunchFullScreen())
        {
            enterFS();
        }
    }
}

void MainWindow::unloadHMI()
{
    if(m_HMIFramework)
    {
        m_HMIFramework->forms()->closeAll();
    }
    exitFS();
}

void MainWindow::enterFS()
{
    if(m_HMIFramework)
    {
        m_HMIFramework->enterFullScreen();
    }
    menuBar()->setVisible(false);
    showFullScreen();
}

void MainWindow::exitFS()
{
    if(m_HMIFramework)
    {
        m_HMIFramework->exitFullScreen();
    }
    menuBar()->setVisible(true);
    //showMaximized();
    showNormal();
}

void MainWindow::exitFSWithSecurityCheck()
{
    if(doSecurityCheck_ExitFS())
    {
        exitFS();
    }
    else
    {
        if(m_HMIFramework->isFullScreen())
            enterFS();
    }
}

bool MainWindow::doSecurityCheck_ExitFS()
{
    if(m_HMIFramework && m_HMIFramework->isFullScreen())
    {
        CMDProjectConfiger* configer = m_projectExpert.projectConfiger();
        CMDActionSP* sp = configer->projectPara()->spList()->find(MDActionType::project_ExitFS);
        if(sp && sp->isEnable())
        {
            LOG_RUNTIME("检查【退出全屏】安全策略 ...");

            CMDActionSPSession spSession(*sp);
            if(!m_projectContacter.checkSP_WithUI(&spSession))
            {
                LOG_RUNTIME("没有通过【退出全屏】安全策略！");
                return false;
            }
            LOG_RUNTIME("【退出全屏】安全策略通过！");
        }
    }

    return true;
}

void MainWindow::closeAll()
{
    if(m_HMIFramework)
    {
        m_HMIFramework->forms()->closeAll();
    }
}

/*!
\section 管理运行时的 RTDB 界面
*/
void MainWindow::initialRTDBRuntimeUI()
{
    m_mdRTDBRuntimeUI =    CMDRTDBManagerUIFactory::createMDRTDBRuntimeUI();

    if(m_mdRTDBRuntimeUI != NULL)
    {
        m_mdRTDBRuntimeUI->setProjectConfiger(m_projectExpert.projectConfiger());
    }
}
void MainWindow::releaseRTDBRuntimeUI()
{
    if(m_mdRTDBRuntimeUI != NULL)
    {
        m_mdRTDBRuntimeUI->closeRTDBWindow();
        delete m_mdRTDBRuntimeUI;
        m_mdRTDBRuntimeUI= NULL;
    }
}
void MainWindow::openRTDBWindow()
{
    if(m_mdRTDBRuntimeUI != NULL)
    {
        m_mdRTDBRuntimeUI->openRTDBWindow(this);
    }
}

void MainWindow::closeRTDBWindow()
{
    if(m_mdRTDBRuntimeUI != NULL)
    {
        m_mdRTDBRuntimeUI->closeRTDBWindow();
    }
}

/*!
\section 系统日志
*/
void MainWindow::showSysLogUI()
{
    if(!m_logDialog)
        m_logDialog = m_sysLogUI.createModaless(m_lsRuntime->logger(), "", this);

    if(m_logDialog)
        m_logDialog->show();
}

void MainWindow::showEnginesUI()
{
    m_eginesUI.showMgrUI(true);
}

void MainWindow::configFuncFactoryUI()
{
    CMDSwitchFactoryUI factoryUI;
    factoryUI.configFactory(m_projectExpert.funcSwitchFactory());
}

void MainWindow::toggleTerminalFactoryUI()
{
#ifdef Q_OS_WIN
    bool bShowConsole = m_toggleTerminalFactoryUIAct->isChecked();
    HWND cwnd = GetConsoleWindow();
    if(cwnd == 0) {
        AllocConsole();//打开控制台
        freopen("CON","w",stdout);//将输出定向到控制台
    }
    ShowWindow(GetConsoleWindow(), bShowConsole ? SW_SHOW : SW_HIDE);
#endif

}

void MainWindow::toggleBackgroundRunAct()
{
    bool bBackgroundRun = m_toggleBackgroundRunAct->isChecked();
    if(bBackgroundRun)
        showRuntime();
    else
        hideRuntime();
}

void MainWindow::configLogFactoryUI()
{
    CMDSwitchFactoryUI factoryUI;
    factoryUI.configFactory(m_projectExpert.logSwitchFactory());
}

/*!
\section 管理框架界面布局：菜单；工具栏；状态栏
               关闭框架
*/
void MainWindow::createActions()
{
    m_exitAct = newQAction("", STRING_EXIT, STRING_EXIT_MINDSCADA, true, SLOT(close()));
    m_exitAct->setShortcuts(QKeySequence::Quit);
    m_aboutAct = newQAction("", STRING_ABOUT, STRING_ABOUT_MINDSCADA, true, SLOT(about()));

    m_configRuntimeEnvAct = newQAction(":/images/open.png", STRING_CONFIG, STRING_CONFIG_RUNTIME, true, SLOT(configRuntimeEnv()));
    m_loadProjectAct = newQAction(":/images/open.png", STRING_LOAD_PROJECT, true, SLOT(loadCurrentProject()));
    m_unloadProjectAct = newQAction(":/images/close.png", STRING_UNLOAD_PROJECT, false, SLOT(unloadCurrentProject()));
    m_showSysLogUIAct = newQAction(":/images/close.png", STRING_SHOW_RUNTIME_LOG, true, SLOT(showSysLogUI()));
    m_showEnginesUIAct = newQAction(":/images/close.png", STRING_SHOW_ENGINES_UI, true, SLOT(showEnginesUI()));

    m_configFuncFactoryUIAct = newQAction(":/images/close.png", STRING_CONFIG_FUNC_FACTORY, false, SLOT(configFuncFactoryUI()));
    m_configLogFactoryUIAct = newQAction(":/images/close.png", STRING_CONFIG_LOG_FACTORY, false, SLOT(configLogFactoryUI()));
    m_toggleTerminalFactoryUIAct = newQAction(":/images/close.png", STRING_TOGGLE_TERMINAL, false, SLOT(toggleTerminalFactoryUI()));
    m_toggleTerminalFactoryUIAct->setEnabled(true);
    m_toggleTerminalFactoryUIAct->setCheckable(true);
    m_toggleBackgroundRunAct = newQAction(":/images/close.png", STRING_TOGGLE_BACKGROUNDRUN, false, SLOT(toggleBackgroundRunAct()));
    m_toggleBackgroundRunAct->setEnabled(true);
    m_toggleBackgroundRunAct->setCheckable(false);

    m_openRTDBWindowAct = newQAction(":/images/close.png", STRING_RTDB, false, SLOT(openRTDBWindow()));

    // 窗体
    m_enterFSAct = newQAction(":/images/close.png", STRING_ENTER_FS, false, SLOT(enterFS()));
    m_closeAllAct = newQAction(":/images/close.png", STRING_CLOSE_ALL, false, SLOT(closeAll()));

    // 安全
    m_loginAct = newQAction(":/images/close.png", STRING_LOGIN, false, SLOT(login()));
    m_logoutAct = newQAction(":/images/close.png", STRING_LOGOUT, false, SLOT(logout()));
    m_showSPMgrAct = newQAction(":/images/close.png", STRING_ACCOUNT_AND_POWER_GGROUP, false, SLOT(showSPMgr()));

    m_switchLanguageAct = newQAction(":/images/close.png", STRING_SWITCH_LANGUAGE, false, SLOT(switchLanguage()));
}

QAction* MainWindow::newQAction(const QString &iconFileName, const QString &text, bool enable, const char *member)
{
    return newQAction(iconFileName, text, text, enable, member);
}

QAction* MainWindow::newQAction(const QString &iconFileName, const QString &text, const QString &statusTip, bool enable, const char *member)
{
    QAction* na = NULL;
    if(iconFileName.isEmpty())
        na = new QAction(text, this);
    else
        na = new QAction(QIcon(iconFileName), text, this);

    na->setStatusTip(statusTip);
    na->setEnabled(enable);
    connect(na, SIGNAL(triggered()), this, member);
    return na;
}

void MainWindow::LOG_RUNTIME(const QString &msg)
{
    m_lsRuntime->LOG_INFO(msg);

    statusBar()->showMessage(msg);
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(STRING_RUN);
    m_fileMenu->addAction(m_configRuntimeEnvAct);
    m_fileMenu->addSeparator();

    m_fileMenu->addAction(m_loadProjectAct);
    m_fileMenu->addAction(m_unloadProjectAct);
    m_fileMenu->addSeparator();

    m_fileMenu->addAction(m_showSysLogUIAct);
    m_fileMenu->addAction(m_showEnginesUIAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_configFuncFactoryUIAct);
    m_fileMenu->addAction(m_configLogFactoryUIAct);
    m_fileMenu->addAction(m_toggleTerminalFactoryUIAct);
    m_fileMenu->addAction(m_toggleBackgroundRunAct);
    m_fileMenu->addSeparator();

    m_fileMenu->addAction(m_openRTDBWindowAct);
    m_fileMenu->addSeparator();

    m_fileMenu->addAction(m_exitAct);
    menuBar()->addSeparator();

    m_fileMenu = menuBar()->addMenu(STRING_HMI);
    m_fileMenu->addAction(m_enterFSAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_closeAllAct);

    m_fileMenu = menuBar()->addMenu(STRING_SECURITY);
    m_fileMenu->addAction(m_loginAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_logoutAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_showSPMgrAct);

    m_fileMenu = menuBar()->addMenu(STRING_LANGUAGE);
    m_fileMenu->addAction(m_switchLanguageAct);

    m_helpMenu = menuBar()->addMenu(STRING_HELP);
    m_helpMenu->addAction(m_aboutAct);
}

void MainWindow::createToolBars()
{
//    fileToolBar = addToolBar(tr("File"));
//    fileToolBar->addAction(newAct);

//    fileToolBar->addAction(openAct);

//    fileToolBar->addAction(saveAct);

//    editToolBar = addToolBar(tr("Edit"));
//    editToolBar->addAction(cutAct);
//    editToolBar->addAction(copyAct);
//    editToolBar->addAction(pasteAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(STRING_READY);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

QString MainWindow::buildCommonTitle()
{
    QString title = "MindSCADA - " + STRANS(STRING_RUNTIME);
    if(m_projectContacter.isVirtualRunMode())
    {
        title = "MindSCADA("+ STRANS(STRING_VIRTUALRUNMODE) +") - " + STRANS(STRING_RUNTIME);
    }
    return title;
}

void MainWindow::enableMenuAction(bool enable)
{
    m_loadProjectAct->setEnabled(enable);
    m_unloadProjectAct->setEnabled(enable);
    m_openRTDBWindowAct->setEnabled(enable);
    m_loginAct->setEnabled(enable);
    m_logoutAct->setEnabled(enable);
    m_showSPMgrAct->setEnabled(enable);
    m_enterFSAct->setEnabled(enable);
    m_closeAllAct->setEnabled(enable);
    m_switchLanguageAct->setEnabled(enable);
    m_showEnginesUIAct->setEnabled(enable);

    m_configFuncFactoryUIAct->setEnabled(enable);
    m_configLogFactoryUIAct->setEnabled(enable);
}

/*!
\section 处理项目通知（变量变化；...）
*/
bool MainWindow::onVariableChanged(IMDChangeVariable* changedVariable)
{
    if(!m_HMIFramework)
        return false;

    m_HMIFramework->onDataChanged(changedVariable);

    return true;
}

qint8 MainWindow::onExecuteHMIAction(CMDAction* action)
{
    LOG_RUNTIME("执行 HMI 动作");

    qint8 result = 1;

    if(!m_HMIFramework)
        return -1;

    QString windowName = action->configProperty()->actionID().objName();
    QString windowFullName;
    if(windowName.endsWith(".gdi", Qt::CaseInsensitive))
        windowFullName = QDir::cleanPath(m_HMIFramework->hmiPath() + QDir::separator() + windowName);
    else
        windowFullName = QDir::cleanPath(m_HMIFramework->hmiPath() + QDir::separator() + windowName + ".gdi");

    switch (action->type())
    {
    case MDActionType::window_Open:
    {
        CAPC_WindowOperation* wo = (CAPC_WindowOperation*)action->configProperty();
        bool opened = m_HMIFramework->forms()->isOpened(windowFullName);
        m_HMIFramework->forms()->open(windowFullName, wo->pwid(), wo->geometry().x(), wo->geometry().y(), wo->geometry().width(), wo->geometry().height());
    }
        break;

    case MDActionType::window_Close:
    {
        m_HMIFramework->forms()->close(windowFullName);
    }
        break;

    case MDActionType::window_Move:
    {
        CAPC_WindowOperation* wo = (CAPC_WindowOperation*)action->configProperty();
        m_HMIFramework->moveForm(windowFullName, QPoint(wo->left(), wo->top()));
    }
        break;
    case MDActionType::window_Hide:
    {
        CAPC_WindowOperation* wo = (CAPC_WindowOperation*)action->configProperty();
        m_HMIFramework->forms()->hide(windowFullName);
    }
        break;
    case MDActionType::window_RunScript:
    {
        CAPC_WindowOperation* wo = (CAPC_WindowOperation*)action->configProperty();
        const QString& meta = wo->meta();
        m_HMIFramework->forms()->runFormScriptDynamically(windowFullName, meta);
    }
        break;
    case MDActionType::window_ShowByPid:
    {
        CAPC_WindowOperation* wo = (CAPC_WindowOperation*)action->configProperty();
        const QString& meta = wo->meta();
        QStringList list = meta.split(',');
        if(list.count() == 2) {
            int pid = list[0].toInt();
            int phwnd = list[1].toInt();
            m_HMIFramework->forms()->showByPid(pid, phwnd);
        }
    }
        break;
    case MDActionType::window_HideByPid:
    {
        CAPC_WindowOperation* wo = (CAPC_WindowOperation*)action->configProperty();
        const QString& meta = wo->meta();
        QStringList list = meta.split(',');
        if(list.count() == 2) {
            int pid = list[0].toInt();
            int phwnd = list[1].toInt();
            m_HMIFramework->forms()->hideByPid(pid, phwnd);
        }
    }
        break;
    case MDActionType::window_CloseByPid:
    {
        CAPC_WindowOperation* wo = (CAPC_WindowOperation*)action->configProperty();
        const QString& meta = wo->meta();
        QStringList list = meta.split(',');
        if(list.count() == 2) {
            int pid = list[0].toInt();
            int phwnd = list[1].toInt();
            m_HMIFramework->forms()->closeByPid(pid, phwnd);
        }
    }
        break;
    default: result = 0;
        break;
    }

    // 名称为空，表明不是项目定义的动作，而是临时 new 的动作，必须删除;
    // CMDActionConfiger 确保不会序列化空名字的动作到内存，参考
    // CMDActionConfigerImp::loadFromFile()
    // --------------------------------------------------------------
    if(action->configProperty()->name().isEmpty())
    {
        delete action;
    }

    return result;
}

/*!
\section 控制单实例运行
*/
void MainWindow::showLoadProgress(const QString& msg, bool finished, int finishCode)
{
    //showLoadMonitor(msg);
}

void MainWindow::showLoadMonitor(const QString& msg)
{
    QString strRuntime;
    strRuntime = QCoreApplication::applicationDirPath();
    QString strApp = "";

#       ifdef Q_OS_WIN
#           ifdef QT_DEBUG
    strApp =  "Mind.MDRuntimeLoadMonitord.exe";
#           else
    strApp =  "Mind.MDRuntimeLoadMonitor.exe";
#           endif
#       endif
#       ifdef Q_OS_LINUX
#           ifdef QT_DEBUG
    strApp =  "Mind.MDRuntimeLoadMonitord";
#           else
    strApp =  "Mind.MDRuntimeLoadMonitor";
#           endif
#       endif

    strRuntime += "/";
    strRuntime += strApp;

    if(!m_pProcess)
    {
        m_pProcess = new QProcess;
        QObject::connect(m_pProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError (QProcess::ProcessError)));
    }

    QString cmd = "Load:" + msg;
    if(!m_runtimeAppClient.sendToServer(cmd))
    {
        // app.LOG_RUNTIME("准备启动MindSCADA运行时 ... " + strRuntime + "[" + strProject + "]");
        QStringList arguments;
        arguments.append(msg);
        m_pProcess->start(strRuntime, arguments);
    }
}
void MainWindow::processError(QProcess::ProcessError error)
{
    switch(error)
    {
    case QProcess::FailedToStart:
        QMessageBox::information(0,"FailedToStart","FailedToStart");
        break;
    case QProcess::Crashed:
        QMessageBox::information(0,"Crashed","Crashed");
        break;
    case QProcess::Timedout:
        QMessageBox::information(0,"Timedout","Timedout");
        break;
    case QProcess::WriteError:
        QMessageBox::information(0,"WriteError","WriteError");
        break;
    case QProcess::ReadError:
        QMessageBox::information(0,"ReadError","ReadError");
        break;
    case QProcess::UnknownError:
        QMessageBox::information(0,"UnknownError","UnknownError");
        break;
    default:
        QMessageBox::information(0,"default","default");
        break;
    }
}

void MainWindow::onDeviceCloudStatusChanged(const QString& statusMsg)
{
    closeRTDBWindow(); // 关闭界面，确保在主线程调用

    setWindowTitle(buildCommonTitle() + " - " + statusMsg);
}

// ----------------------------------------------------------------------------
// 多语言
// ----------------------------------------------------------------------------
bool MainWindow::initLanguage(const QString& projectName)
{
    QString path, name;
    CMDProjectConfiger::parseProjectName(projectName, path, name);

    m_languageObj.setMainWindow(this);
    m_dictManager = CMDMultiLanguage::createDictionaryManager(&m_languageObj, path);
    CMDMultiLanguage::instance()->setCurrentProject(m_dictManager);

    return true;
}

void MainWindow::releaseLanguage()
{
    CMDMultiLanguage::instance()->setCurrentProject(NULL);
    if(m_dictManager)
    {
        delete m_dictManager;
        m_dictManager = NULL;
    }
}

void MainWindow::changeLanguage()
{
    m_translateWidgetTool->changeLanguage();

    if(m_HMIFramework)
        m_HMIFramework->changeLanguage();

    if(m_mdRTDBRuntimeUI)
        m_mdRTDBRuntimeUI->changeLanguage();

    QString title = buildCommonTitle();
    if(m_projectExpert.loadStatus() == MDProjectLoadStatus::loaded)
    {
        title += " - [" +  m_projectExpert.projectConfiger()->fullFileName() + "]";
    }
    setWindowTitle(title);

//    m_sysLogUI.changeLanguage();
}

void MainWindow::switchLanguage()
{
    m_languageDlg.setWindowTitle(STRANS(STRING_LANGUAGE));
    m_languageDlg.exec();
}

QString MainWindow::paraFileName()
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString file;
    #ifdef Q_OS_LINUX
        file = appPath + "/runtime.ini";
    #else // windows
        file = appPath + "\\runtime.ini";
    #endif
    return file;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::setBreakpadInstance
/// \param inst
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
    m_lsRuntime->LOG_ERROR(QString("crash dump file: result:%1 path:%2 file:%3").arg(is_success).arg(dump_dir).arg(dump_file_name));
}

