#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class CMDSwitchFactory;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    CMDSwitchFactory* m_fsFactory;

private slots:
    void on_pushButton_ConfigFuncFactory_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
