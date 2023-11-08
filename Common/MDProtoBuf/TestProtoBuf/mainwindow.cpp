#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include "cao.pb.h"

#include "MDProtoBuf/MDProtoBuf.h"
using namespace lm;
using namespace google::protobuf;
using namespace google::protobuf::io;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MDProtoBuf protobuf;
    int mm = 9;


}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{
    QString  configFile = "./12345.cfg";

    helloworld::testHw  hw1;
    QByteArray str2data;
    QByteArray aaa;
    //5个大message
    for(int i = 0; i<5; i++)
    {
        helloworld* phworld1 = hw1.add_hw();
        //int
        phworld1->set_id(200 + i);
        //string
        phworld1->set_str("abc");
        //bytearray
        str2data.append(i+1);
        phworld1->set_str2(str2data.data(),str2data.size());
        //每个message中RTDB有3个元素
        for(int j = 0; j<3; j++)
        {
            helloworld_RTDB* rb1 = phworld1->add_rtdb();
            rb1->set_a("a");
            rb1->set_b(12.34);
            rb1->set_c(111.222);
            rb1->set_d(1234+ j + i);
        }
    }

    QByteArray buf;
    buf.resize(hw1.ByteSize()+10);
    hw1.SerializeToArray(buf.data(), buf.size());

    //write
    QFile file(configFile);
     if(file.open(QIODevice::WriteOnly))
     {
         QDataStream out(&file);
         //版本
         out<<buf.data();
         file.close();
     }

    //read
     str2data.clear();
     QFile file1(configFile);
      if(file1.open(QIODevice::ReadOnly))
      {
          QDataStream in(&file1);
          helloworld::testHw  hwRead;
          QString strData;
          int nsize = file1.size();
          char* cdata = new char[nsize];
          in >> cdata;
          QByteArray bufdata(cdata);
          delete cdata;
          hwRead.ParseFromArray(bufdata.data(),bufdata.size());

          //parse the content

          int nhwsize = hwRead.hw_size();
          int nrbsize = 0;
          for(int i = 0; i < nhwsize; i++)
          {
            helloworld hwsub1 = hwRead.hw(i);
            int nid = hwsub1.id();
            QString str = QString(hwsub1.str().c_str());
            str2data.clear();
            str2data.append(hwsub1.str2().c_str());
            nrbsize = hwsub1.rtdb_size();
            for(int j = 0; j<nrbsize; j++)
            {
                helloworld_RTDB rb1 = hwsub1.rtdb(j);
                QString stra = QString(rb1.a().c_str());
                float ft = rb1.b();
                double db = rb1.c();
                int ii = rb1.d();
                int mm = 9;
            }
          }
          file1.close();
      }
      google::protobuf::ShutdownProtobufLibrary();
}
