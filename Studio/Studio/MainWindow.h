#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QProcess>
#include <QPointer>
#include <QTreeWidget>
#include <QAction>
#include <QMenu>
#include <QMdiArea>
#include <QMdiSubWindow>

#include "PropertyBrowser.h"
#include "DevProjectManager.h"
#include "MDSingleApplication.h"
#include "MDSysLogExpert.h"
#include "MDSysLogUI.h"
#include "DockConfig.h"
#include "BaseForm.h"

class CMDLogSwitch;

class MainWindow : public QMainWindow, public ILanguageObject
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();
/*!
\section event
*/
protected:
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
/*!
\section property
*/
public:
    CPropertyBrowser *propertyBrowser();
    CDevProjectManager *projectManager(){return m_projectManager;}
    QMdiArea *MdiArea(){return m_mdiArea;}
private:
    CPropertyBrowser *m_property;
    CDevProjectManager *m_projectManager;
    QMdiArea *m_mdiArea;
/*!
\section field
*/
private:
    QTreeWidget *m_treeProject;
    QPointer<CBaseForm> m_activeForm;
/*!
\section public function
*/
public:
    //关闭项目前的提示保存函数
    bool closeHint(bool isAll = true, const QString &projectId = "");
/*!
\section private function
*/
private:
    void init();
    void release();
    void initTree();
    void initMdiArea();
    CBaseForm *activeForm();
    void activeChanged(QMdiSubWindow *subWindow);
    void newProject();
    void openProject();
    void loadProject(const QString &name);
    void removeCurrentProject(const QString &fileName);
    void setCurrentProject(const QString &fileName);
    void about();
    void setActiveSubWindow(QWidget *window);
/*!
\section run
*/
private:
    void run();
    void getRuntimePara(QString& runtimeName, QString& projectName);
private:
    QProcess m_runTimeProcess;
    CMDSingleAppClient m_runtimeAppClient;
/*!
\section DockWidget
*/
private:
    CDockConfig* m_dockConfig;
private:
    void initDockWidget();
/*!
\section pop menu
*/
private:
    void showPopMenu();
    //刷新工具栏显示状态
    void refreshToolBarVisible(bool checked);
/*!
\section config
*/
private:
    void saveConfig();
    void loadConfig();
    QString initConfigPath();
/*!
\section recent Projects
*/
private:
    //需要放在多语言初始化之前
    void initRecentProjectActions();
    void updateRecentProjectActions();
    void openRecentProject();
private:
    //最近工程列表
    QStringList m_recentProjects;
    QList<QAction*> m_recentProjectActions;
/*!
\section save & close
*/
private:
    void saveAll();
    void saveCurrent();
private:
    void closeCurrentProject();
    void closeAllProject();
public:
    void runProject(CDevProjectItem *project);
    void virtualRunProject(CDevProjectItem *project);
    void closeProject(CDevProjectItem *project);
    void reloadProject(CDevProjectItem *project);
/*!
\section language
*/
public:
    void changeLanguage();
    void collectWords();
private:
    void initLanguage();
private:
    ITranslateWidgetTool* m_translateWidgetTool;
/*!
\section log
*/
public:
    void showLog(const QString& message);
private:
    void initLog();
private:
    QWidget *m_logWidget;
    CMDSysLogUI m_logUI;
    CMDLogSwitch* m_logSwitch;
/*!
\section menu,toolbar,action
*/
private:
    void initActions();
    void initMenus();
    void initToolBars();
private:
    QAction* m_newAction;
    QAction* m_openAction;
    QAction* m_saveAction;
    QAction* m_closeCurrentAction;
    QAction* m_closeAllAction;
    QAction* m_saveAllAction;

    QList<QToolBar*> m_toolBars;
    QMap<QString, bool> m_toolBarVisibles;
    QList<QMenu*> m_studioMenus;
    //studio模块的工具栏数目
    int m_studioToolCount;

    void* m_breakpad_instance;
/*
 * breakpad
*/
public:
    void setBreakpadInstance(void* inst);
public slots:
    void onCrashDump(bool is_success, QString dump_dir, QString dump_file_name);
};

#endif
