#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MDWindow.h"
#include "MDUiWrappers.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_PID = "\home\projects\test.dev";

    initLoadWindows();
    initUsers();

#ifdef Q_OS_LINUX
      initScript();

      m_w1->runScript();
      m_w2->runScript();
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initLoadWindows()
{
    // w1
    CMDWindow* wnd1 = new CMDWindow("w1");
    QString source1
     (
        "w1.showMsg('w1_onSizeChanged');"
        "w1.showMsg(String('中文'));"
        "function w1_onSizeChanged(x,y)"
       "{"
           "w1.showMsg('function w1_onSizeChanged');"
            "w1.showMsg(String(u1.uid));"
       "}"

        "function g_UserMgr_OnLogin(UserName)"
       "{"
            "w1.showMsg('Login in w1: ' + UserName);"
       "}"
     );
    wnd1->setCode(source1);
    m_w1 = wnd1;
    m_windowList.append(wnd1);

    // w2
    CMDWindow* wnd2 = new CMDWindow("w2");
    QString source2
     (
        "w2.showMsg('w2_onSizeChanged');"
        "function w2_onSizeChanged(x,y)"
        "{"
           "w2.showMsg('function w2_onSizeChanged');"
            "w2.showMsg(String(u1.uid));"
        "}"

        "function g_UserMgr_OnLogin(UserName)"
       "{"
            "w2.showMsg('Login in w2: ' + UserName);"
       "}"
     );
    wnd2->setCode(source2);
    m_w2 = wnd2;
    m_windowList.append(wnd2);
}

void MainWindow::initScript()
{
    int count = m_windowList.count();
    for(int i = 0; i < count; i++)
    {
        CMDWindow* wnd = m_windowList.at(i);
        wnd->initScript(m_userWrappersMgr, m_PID);
    }

    m_userMgr.setPID(m_PID);
}

void MainWindow::initUsers()
{
    CMDUser* user1 = new CMDUser;
    user1->m_name = "u1";
    user1->m_uid = 1111;
    m_userList.append(user1);
    CMDUserWrapper* uw1 = new CMDUserWrapper(user1, user1->m_name);
    m_userWrappersMgr.addWrapper(uw1);

    CMDUser* user2 = new CMDUser;
    user2->m_name = "u2";
    user2->m_uid = 2222;
    m_userList.append(user2);
    CMDUserWrapper* uw2 = new CMDUserWrapper(user2, user2->m_name);
    m_userWrappersMgr.addWrapper(uw2);
}

void MainWindow::on_pushButton_clicked()
{
#ifdef Q_OS_LINUX
    m_w1->fireEvent_SizeChanged(11, 22);
    m_w2->fireEvent_SizeChanged(33, 44);

    m_userMgr.login("tangzw");

    m_w1->releaseScript();
    //m_w1->initScript(m_userWrappersMgr, m_PID);
    //m_w1->runScript();

    //m_st.start();
#endif
}

void MainWindow::on_pushButtonRelease_clicked()
{
#ifdef Q_OS_LINUX
    m_w1->releaseScript();
    m_w2->releaseScript();
#endif
}
