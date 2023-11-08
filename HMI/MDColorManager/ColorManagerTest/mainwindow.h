#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MDBrushManager.h"
#include "MDPenManager.h"
#include "MDBrushSelectDialog.h"
#include "MDPenSelectDialog.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;

public:
    CMDBrushManager m_myBrushManager;
    CMDPenManager m_myPenMgr;

protected:
    virtual void paintEvent(QPaintEvent *);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H
