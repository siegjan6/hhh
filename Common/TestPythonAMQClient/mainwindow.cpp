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

   m_AMQClient = new CMDPythonAMQClient("127.0.0.1",61616,"","");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_AMQClient;
    m_AMQClient = NULL;
}

void MainWindow::on_connect_clicked()
{
    m_AMQClient->openConnection();
   //connect(m_ClusterRedisClient,SIGNAL(recvSubMessage(QVariant)),this,SLOT(getSubscribeMsg(QVariant)));
}
//测试传统命令
void MainWindow::on_setcommand_clicked()
{
    QString topic = "pinkie";
   m_AMQClient->setProduceName(topic,false);
   //void  produceMsg(QString& msg);
}

void MainWindow::on_Subscribe_clicked()
{
    QString name = "pinkie";
   m_AMQClient->subscribeMsg(name,true);
}
void MainWindow::getSubscribeMsg(QVariant message)
{

}

