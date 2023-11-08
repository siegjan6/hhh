#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MDPythonRedisClient.h"
//#include "gui/PythonQtScriptingConsole.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_connect_clicked();

    void on_setcommand_clicked();

    void getSubscribeMsg(QVariant message);
    void on_Subscribe_clicked();

    void on_ZADDBtn_clicked();

    void on_ZScoreBtn_clicked();

    void on_ClustersubscribeBtn_clicked();

private:
    Ui::MainWindow *ui;
    CMDPythonRedisClient* m_RedisClient;
    CMDPythonRedisClient* m_ClusterRedisClient;
    //PythonQtScriptingConsole* m_pythonConsole;
};

#endif // MAINWINDOW_H
