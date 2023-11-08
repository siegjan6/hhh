#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ScriptThread.h"
#include "MDUser.h"

namespace Ui {
class MainWindow;
}

class CMDWindow;
class CMDUser;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    void initScript();
    QString m_PID;

    void initLoadWindows();
    QList<CMDWindow*> m_windowList;

    void initUsers();
    QList<CMDUser*> m_userList;
    CMDObjectWrapperMgr m_userWrappersMgr;
    CMDUserMgr m_userMgr;

    CMDWindow* m_w1;
    CMDWindow* m_w2;

    CScriptThread m_st;
private slots:
    void on_pushButton_clicked();

    void on_pushButtonRelease_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
