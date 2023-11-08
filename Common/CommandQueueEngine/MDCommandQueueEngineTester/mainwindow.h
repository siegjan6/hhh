#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
namespace Ui {
class MainWindow;
}

#include "MDCommandQueueEngine.h"
#include "MDCommandQueueEngineUI.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    CMDCommandQueueEngine m_engine;
    CMDCommandQueueEngineUI m_eUI;
public:
    QTimer m_timer;
private slots:
    void onTimedOut();
private slots:
    void on_pressureTestStartButton_clicked();
    void on_pressureTestStopButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
