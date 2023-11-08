#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MDPythonAMQClient/MDPythonAMQClient.h"
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

private:
    Ui::MainWindow *ui;
    CMDPythonAMQClient* m_AMQClient;
    //PythonQtScriptingConsole* m_pythonConsole;
};

#endif // MAINWINDOW_H
