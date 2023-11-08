#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MDSysLogUI.h"
#include <QTimer>
#include "../../include/MDCommandQueueEngine/MDCommandQueueEngine.h"
#include "../../include/MDCommandQueueEngineUI/MDCommandQueueEngineUI.h"

namespace Ui {
class MainWindow;
}

class QTableView;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTimer m_timer;
    CMDCommandQueueEngineUI m_eUI;
private slots:
    void on_pushButton_AddLog_clicked();
    void on_pushButton_ShowUI_clicked();
    void on_pushButton_CloseAllUI_clicked();

    void on_pressureTestStartButton_clicked();
    void on_pressureTestStopButton_clicked();

    void onTimedOut();
private:
    void createLog();
    CMDSysLogUI m_logUI;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
