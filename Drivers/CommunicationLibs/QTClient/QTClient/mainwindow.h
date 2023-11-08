#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "IEventCallback.h"
#include "ICommunicationComponent.h"
#include "NSCommunicationComponentFactory.h"
#include <QString>
#include <QByteArray>
#include <QWaitCondition>
#include <QMutex>
#include <QThread>
#include <QStringListModel>
//#include "CommunicationEvent.h"
//#include <QSerialPort>
namespace Ui {
class MainWindow;

}

class SendThead;
class CommunicationEvent;

class MainWindow : public QMainWindow,public IEventCallback
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
  SendThead *pthread;


private slots:

    void on_pbConnect_clicked();

    void on_BtnStart_clicked();

    void on_pbSerivalConnect_clicked();

    void on_btnStopTest_clicked();

    void OnEvent(const CommunicationEventType::_CommunicationEventType iEvent);

private:
    Ui::MainWindow *ui;




    ICommunicationComponent *mComm;
    IEventCallback *mCallBack;

    bool run;

    QStringListModel* slm;
   QStringList* sl;

   CommunicationEvent *_CommunicationEvent;


};


class SendThead:public QThread
{
    Q_OBJECT
public:
     SendThead(QByteArray qb,ICommunicationComponent *mCom,bool run);
     virtual void run();

     QByteArray qa;
     ICommunicationComponent *mComm;
   bool _run;

private slots:
   bool Connect();

signals:
   bool evt_connect();
};

#endif // MAINWINDOW_H

