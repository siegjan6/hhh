#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <MDRedisClient.h>
#include <QtCore>

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
    void on_Start_clicked();

    void on_RefreshInterval_textChanged();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

public slots:
    void redisConnected();
    void redisDisonnected();
    void UpdateTimer();

    void subscribeMessage();
    void responseOut(QStringList& res);
private:
    CMDRedisClient *cmdRedis;
    CMDRedisClient *subRedis;

    QByteArray resvalue;
    QTimer *timer;
};

#endif // MAINWINDOW_H
