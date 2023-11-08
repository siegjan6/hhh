#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QThread>


QWaitCondition mywait;
QMutex mt_wait;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings *settings=new QSettings("mainwindow","clients",this);
    ui->txtIpAddr->setText(settings->value("ip","127.0.0.1").toString());
    ui->txtPort->setText(settings->value("port","8888").toString());
    ui->txtClientCount->setText(settings->value("clientcount","1").toString());

     slm = new QStringListModel(this);
     sl = new QStringList();

   run=true;


}

MainWindow::~MainWindow()
{
    delete ui;
    delete slm;
    delete sl;


}

void MainWindow::on_pbConnect_clicked()
{

    mComm = NSCommunicationComponentFactory::CreateCommunicationComponent();
    QString comm_param;
    comm_param.append( "10.1.20.97,502,10.1.20.97,502,tcp,client;");
    comm_param.append( "10.1.20.97,502,10.1.20.97,502,tcp,client");
    mComm->SetCommParameter(comm_param);
    mComm->CommStatus = Communication::DisConnected;
    mComm->CommType = Communication::TCP;
    mCallBack = this;

    mComm->SetEventCallback(mCallBack);

    QByteArray  send_read_buf;
    send_read_buf[0] = 0x00;
    send_read_buf[1] = 0x00;
    send_read_buf[2] = 0x00;
    send_read_buf[3] = 0x00;
    send_read_buf[4] = 0x00;
    send_read_buf[5] = 0x06;
    send_read_buf[6] = 0x01;
    send_read_buf[7] = 0x03;
    send_read_buf[8] = 0x00;
    send_read_buf[9] = 0x09;
    send_read_buf[10] = 0x00;
    send_read_buf[11] = 0x5B;

    send_read_buf.resize(12);


    pthread=new SendThead(send_read_buf,mComm,run);

    pthread->start(QThread::HighPriority);


   QSettings *settings=new QSettings("mainwindow","clients",this);
   settings->setValue("ip",ui->txtIpAddr->toPlainText());
   settings->setValue("port",ui->txtPort->toPlainText());
   settings->setValue("clientcount",ui->txtClientCount->toPlainText());
}

void MainWindow::on_BtnStart_clicked()
{
    mywait.wakeAll();

}

void MainWindow::OnEvent(const CommunicationEventType::_CommunicationEventType iEvent)
{
    QByteArray buffer;

    switch (iEvent)
    {
        case CommunicationEventType::Connected:
            //buffer.append("connect ok");
            //ui->lvResult->("connect ok");
             sl->append("connect ok");
            break;
        case CommunicationEventType::Disconnected:
           // buffer.append("disconnected");
             sl->append("disconnected");
            break;
        case CommunicationEventType::ReceivedData:
           {
            mComm->GetData(buffer,1024);
            QString str;
            str.prepend(buffer.toHex());
            sl->append(str);

            mywait.wakeAll();
              }
            break;
        default:
            break;
    }
    slm->setStringList(*sl);
    ui->lvResult->setModel(slm);

}



void MainWindow::on_pbSerivalConnect_clicked()
{
    mComm = NSCommunicationComponentFactory::CreateCommunicationComponent();
    QString comm_param;
    comm_param.append( "COM3,9600,8,1,0,true,true;");
    comm_param.append( "COM3,9600,8,1,0,true,true");
    mComm->SetCommParameter(comm_param);
    mComm->CommStatus = Communication::DisConnected;
    mComm->CommType = Communication::Serial;
    //mComm->ResponesTimeout = 3000;
    mCallBack = this;
    mComm->SetEventCallback(mCallBack);


    QByteArray  send_read_buf;
    send_read_buf[0] = 0x01;
    send_read_buf[1] = 0x03;
    send_read_buf[2] = 0x00;
    send_read_buf[3] = 0x09;
    send_read_buf[4] = 0x00;
    send_read_buf[5] = 0x5B;
    send_read_buf[6] = 0xD4;
    send_read_buf[7] = 0x33;

    send_read_buf.resize(8);


    pthread=new SendThead(send_read_buf,mComm,run);
    pthread->start(QThread::HighPriority);


}

bool SendThead::Connect()
{
    mComm->Connect();
    return true;
}
SendThead::SendThead(QByteArray qb, ICommunicationComponent *mCom, bool run)
{
    this->qa=qb;
    this->mComm=mCom;
    this->_run=run;
    connect(this,SIGNAL(evt_connect()),this,SLOT(Connect()));

}

void SendThead::run()
{
    emit evt_connect();
    while(_run)
    {
        mt_wait.lock();
        mywait.wait(&mt_wait,0xFFFFFFF);
        QThread::msleep(1);
        mComm->SendData(qa);
        mt_wait.unlock();
    }
}

void MainWindow::on_btnStopTest_clicked()
{
    run=false;
    pthread->terminate();
    QThread::msleep(3);
}



