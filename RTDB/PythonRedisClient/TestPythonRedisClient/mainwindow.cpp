#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QThread>
#include <QTextCodec>
//#include "PythonQt.h"
//*py类对象
//PythonQtObjectPtr m_pythonObFactory;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    ui->setupUi(this);

    //PythonQt::init();

    //m_pythonObFactory = PythonQt::self()->getMainModule();
    //m_pythonConsole = new PythonQtScriptingConsole(NULL,m_pythonObFactory);
   // m_pythonObFactory.evalScript(QString("import sys\n"));

   m_RedisClient = new CMDPythonRedisClient();
   m_ClusterRedisClient = new CMDPythonRedisClient();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connect_clicked()
{
    /*
    //控制台
    m_pythonConsole = new PythonQtScriptingConsole(NULL,m_pythonObFactory);
    //显示python 控制台
    m_pythonConsole->appendCommandPrompt();
    m_pythonConsole->show();
    */
   //传统
    /*
   connect(m_RedisClient,SIGNAL(recvSubMessage(QVariant)),this,SLOT(getSubscribeMsg(QVariant)));
   QVector<QString> hostArr;
   QVector<int> portArr;

   hostArr.append("192.168.10.99");
   portArr.append(7001);
   m_RedisClient->openConnection(hostArr,portArr,false);
    */
    //集群

   connect(m_ClusterRedisClient,SIGNAL(recvSubMessage(QVariant)),this,SLOT(getSubscribeMsg(QVariant)));
   QVector<QString> hostArr;
   QVector<int> portArr;

   hostArr.append("192.168.10.99");
   hostArr.append("192.168.10.99");
   portArr.append(7008);
   portArr.append(7003);
   m_ClusterRedisClient->openConnection(hostArr,portArr,true);

}
//测试传统命令
void MainWindow::on_setcommand_clicked()
{
    QStringList cmd;
    cmd.append("hset");
    cmd.append("TestClassic");
    cmd.append("100");
    cmd.append("a b 曹");
   QVariant ret = m_RedisClient->sendCommand(cmd);
   ui->submsgtext->setText(ret.toString());
}

void MainWindow::on_Subscribe_clicked()
{
    QStringList cmd;
    cmd.append("subscribe");
    cmd.append("ccc");
   QVariant ret = m_RedisClient->sendCommand(cmd);
}
void MainWindow::getSubscribeMsg(QVariant message)
{
    ui->submsgtext->setText(message.toString());
}

//测试集群
void MainWindow::on_ZADDBtn_clicked()
{
   QVariant ret;
    /*
   QDateTime timebegin = QDateTime::currentDateTime();
   ui->BeginTimeEdit->setText(timebegin.toString());
   for(int i = 0; i< 1000; i++)
   {
    m_ClusterRedisClient->sendCommand("ZADD Test 100000","a b c" );
    QThread::msleep(1);
   }

   QDateTime timeend = QDateTime::currentDateTime();
   ui->EndTimeEdit->setText(timeend.toString());
   int n = 9;
    */
   //ret = m_RedisClient->sendCommand("hset chp pinkie", "daydayup");
   //ret = m_ClusterRedisClient->sendCommand("cluster","nodes");
   //ret = m_ClusterRedisClient->sendCommand("hset chp pinkie", "daydayup");
   QStringList cmd;
   cmd.append("Zadd");
   cmd.append("Test");
   cmd.append("100");
   cmd.append("a b 曹");
   ret = m_ClusterRedisClient->sendCommand(cmd);
   ui->CmdRet->setText(ret.toString());
}

void MainWindow::on_ZScoreBtn_clicked()
{
    QStringList cmd;
    cmd.append("zrange");
    cmd.append("Test");
    cmd.append("0");
    cmd.append("-1");
    cmd.append("withscores");
    QVariant ret = m_ClusterRedisClient->sendCommand(cmd);
    ui->CmdRet->setText(ret.toString());
    int n = 9;
}

void MainWindow::on_ClustersubscribeBtn_clicked()
{
    QStringList cmd;
    cmd.append("subscribe");
    cmd.append("ccc");
    QVariant ret = m_ClusterRedisClient->sendCommand(cmd);
    int n = 9;
}
