/*!

\author dGFuZ3p3

\section 功能 - MindSCADA 运行时框架类，完成运行时的总体控制。

*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>

#include "../../include/MDCommonHelper/MDObject.h"
#include "../../include/MDCommonHelper/MDCommonConstant.h"

#include "MDProjectContacterImp.h"

#include "../../include/MDEventHelper/MDEventConstant.h"
#include "../../include/MDEventHelper/MDRawEvent.h"
#include "../../include/MDEventExpert/MDEventExpert.h"

#include "IHMIFramework.h"
#include "MDHMICreator.h"

#include "MDLoadProgressDialog.h"

#include <QProcess>
#include <QPointer>
#include <QSystemTrayIcon>
#include "MDSingleApplication.h"

#include "MDSysLogUI.h"
#include "MDSysLogExpert.h"
#ifndef LOG_SOURCE
#define LOG_SOURCE "Runtime"
#endif

#include "MDRuntimeLanguageObject.h"
#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"
#include "MDLanguageDialog.h"
#include "../../include/MDCommandQueueEngineUI/MDCommandQueueEngineUI.h"

#include "MDRecipeUI/MDRecipeUI.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE
class QSettings;
class QPlainTextEdit;
class IMDRTDBRuntimeUI;

class IHMIFramework;
class ITranslateWidgetTool;
class CMDLogSwitch;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(CMDLogSwitch* lsRuntime);
    ~MainWindow();

/*!
\section 管理 HMI 框架
*/
private:
    void initHMIFramework();
    void releaseHMIFramework();
    IHMIFramework* m_HMIFramework ;// NULL;
private:
    void loadHMI();
    void unloadHMI();
public slots:
    void closeAll();
    void enterFS();
    void exitFS();
    void exitFSWithSecurityCheck();
public:
    bool doSecurityCheck_ExitFS();

/*!
\section 管理项目（加载；卸载；显示项目信息；...）
*/
public:
    CMDProjectExpert m_projectExpert;
    void loadProject(const QString& projectFileName, bool isVrtualRunMode = false);
    void reloadProject(const QString& projectFileName, bool isVrtualRunMode = false);
public slots:
    void loadCurrentProject();
    bool unloadCurrentProject();
public:
    bool doSecurityCheck_Unload();
    bool doSecurityCheck_ManageAccountPowerGroup();
private:
    CMDProjectContacterImp m_projectContacter;
public slots:
    void configRuntimeEnv();
    void viewProjectInfo();
    void login();
    void logout();
    void showSPMgr();

    void switchLanguage();
private:
    QString readSettings_CurrentProject();
    void writeSettings_CurrentProject(const QString& projectFileName);
    QSettings* m_runtimeEnvSettings  ;// NULL;
    QString paraFileName();

/*!
\section 处理项目通知（变量变化；...）
*/
public:
    bool onVariableChanged(IMDChangeVariable* changedVariable);
    qint8 onExecuteHMIAction(CMDAction* action);
public:
    void showLoadProgress(const QString& msg, bool finished, int finishCode);
    CMDLoadProgressDialog m_loadProgressDialog;
    void onDeviceCloudStatusChanged(const QString& statusMsg);

/*!
\section 管理运行时的配方界面
*/
public:
    CMDRecipeUI m_recipeUI;
    void showRecipeRuntime();
    void hideRecipeRuntime();
    void showRuntime();
    void hideRuntime();

/*!
\section 管理运行时的 RTDB 界面
*/
private:
    void initialRTDBRuntimeUI();
    void releaseRTDBRuntimeUI();
private:
      IMDRTDBRuntimeUI *m_mdRTDBRuntimeUI ;// NULL;
public slots:
    void openRTDBWindow();
public:
    void closeRTDBWindow();

/*!
\section 系统日志
*/
public:
    CMDLogSwitch* m_lsRuntime;
protected:
    CMDSysLogUI m_sysLogUI;
    QDialog* m_logDialog ;// NULL;
    CMDCommandQueueEngineUI m_eginesUI;
public slots:
    void showSysLogUI();
    void showEnginesUI();
    void configFuncFactoryUI();
    void configLogFactoryUI();
    void toggleTerminalFactoryUI();
    void toggleBackgroundRunAct();

/*!
\section 控制单实例运行
*/
private:
    QProcess *m_pProcess ;// NULL;
    CMDSingleAppClient m_runtimeAppClient;
    void showLoadMonitor(const QString& msg);
private slots:
    void processError(QProcess::ProcessError error);

/*!
\section 管理框架界面布局：菜单；工具栏；状态栏
               关闭框架
*/
private slots:
    void about();
private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    QString strippedName(const QString &fullFileName);
    QString buildCommonTitle();
    void enableMenuAction(bool enable);
    QAction* newQAction(const QString &iconFileName, const QString &text, bool enable, const char *member);
    QAction* newQAction(const QString &iconFileName, const QString &text, const QString &statusTip, bool enable, const char *member);
private:
    void LOG_RUNTIME(const QString &msg);
private:
    QMenu *m_fileMenu;
    QMenu *m_helpMenu;
    QAction *m_exitAct;
    QAction *m_aboutAct;
    QAction *m_configRuntimeEnvAct;
     QAction *m_showSysLogUIAct;
     QAction *m_showEnginesUIAct;
private:
     QAction *m_configFuncFactoryUIAct;
     QAction *m_configLogFactoryUIAct;
     QAction *m_toggleTerminalFactoryUIAct;
     QAction *m_toggleBackgroundRunAct;
private:
    QAction *m_loadProjectAct;
    QAction *m_unloadProjectAct;
    QAction *m_openRTDBWindowAct;
    QAction *m_enterFSAct;
    QAction *m_closeAllAct;

    QAction *m_loginAct;
    QAction *m_logoutAct;
    QAction *m_showSPMgrAct;

    QAction *m_switchLanguageAct;
protected:
    void closeEvent(QCloseEvent *event); //! 关闭框架
    void keyPressEvent(QKeyEvent * event); //! 退出全屏

// ----------------------------------------------------------------------------
// 多语言
// ----------------------------------------------------------------------------
public:
    bool initLanguage(const QString& projectName);
    void releaseLanguage();
public:
    void changeLanguage();
private:
    CMDRuntimeLanguageObject m_languageObj;
    IDictionaryManager* m_dictManager;
    ITranslateWidgetTool* m_translateWidgetTool;
private:
    CMDLanguageDialog m_languageDlg;
    //QSystemTrayIcon *trayIcon;

private:
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
