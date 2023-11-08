#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cmdRedis = NULL;
    subRedis = NULL;
    timer = NULL;
}

MainWindow::~MainWindow()
{
    if(cmdRedis != NULL)
     delete cmdRedis;

    if(subRedis != NULL)
        delete subRedis;

    delete ui;    
}

void MainWindow::on_Start_clicked()
{
    //subscribe connection
    subRedis = new CMDRedisClient("192.167.8.13",6379);
    connect(subRedis, SIGNAL(subMessage()), this, SLOT(subscribeMessage()));
    subRedis->openConnection();
    QThread::msleep(1000);
    if(subRedis->isconnected())
    {
        QString cmd = "Subscribe GetDeviceDriverInfoPush";
        subRedis->sendCommand(cmd);
    }

    //General command connection
    if(cmdRedis == NULL)
    {
        cmdRedis = new CMDRedisClient("192.167.8.13",6379);
        connect(cmdRedis, SIGNAL(generalCmd(QStringList&)), this, SLOT(responseOut(QStringList&)));
        connect(cmdRedis, SIGNAL(connected()), this, SLOT(redisConnected()));
        connect(cmdRedis, SIGNAL(disconnected()), this, SLOT(redisDisonnected()));

    }
    if(cmdRedis->isconnected() == false)
        cmdRedis->openConnection();

    QThread::msleep(1000);
    if(cmdRedis->isconnected())
    {
        QString cmd1 = "publish GetDeviceDriverInfoReq HUABEIRTDB";
        cmdRedis->sendCommand(cmd1);
    }
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(UpdateTimer()));
    timer->start(200);
    ui->RefreshInterval->setText("200");
}

void MainWindow::subscribeMessage()
{
    static int i = 0;
    static int j = 0;
    CMDRedisClient::Reply res = subRedis->removeHead();
    while(res.type == "list")
    {
        switch(i)
        {
        case 0:
            ui->Msg1->setText(res.value.toString());
            break;
        case 1:
            ui->Msg2->setText(res.value.toString());
            break;
        case 2:
            ui->Msg3->setText(res.value.toString());
            break;
        }
        i++;
        if(i >= 3)
            i = 0;
        res = subRedis->removeHead();
    }
    j++;
    ui->GetSubCnt->setText(QString("%1").arg(j));
    int n = 9;
}

void MainWindow::responseOut(QStringList& res)
{
    CMDRedisClient::Reply reply;
    if(res[0] == "integer")
    {
        reply.type = res[0];
        reply.value = res[1].toInt();
    }
    else if(res[0] == "list")
    {
        reply.type = res[0];

        QStringList list;
        for(int i=1; i < res.length(); i++)
        {
            list << res[i];
        }

        reply.value = list;
    }
    else
    {
        reply.type = res[0];
        reply.value = res[1];
    }

}
void MainWindow::UpdateTimer()
{

    if(cmdRedis != NULL)
    {
        static int m = 0;
        ui->Msg1->setText("");
        ui->Msg2->setText("");
        ui->Msg3->setText("");

        if(cmdRedis->isconnected())
        {
            ui->connected->setText("true");
             QString cmd1,cmd;
             cmd = "hget RTData.Devices.BeiJingHDGCZXMM1203 varHD1 ";

            m++;
            if(m > 1000)
            {
                m = 0;
            }
            cmdRedis->sendCommand(cmd);
            cmd = "publish SubscribeDeviceRTDBVarValueChangePush BeiJingHDGCZXMM1203";
            cmdRedis->sendCommand(cmd);
        }
        else
        {
            ui->connected->setText("false");
        }
    }
}

void MainWindow::on_RefreshInterval_textChanged()
{
    if(timer)
        timer->stop();
    QString str = ui->RefreshInterval->toPlainText();
    timer->start(str.toInt());
}
void MainWindow::redisConnected()
{
   int nn = 9;
}

void MainWindow::redisDisonnected()
{
    int nn = 9;
}

void MainWindow::on_pushButton_clicked()
{
    if(cmdRedis != NULL)
    {
        QString cmd = "@123 dd ss";
        cmdRedis->sendCommand(cmd);
    }
}
