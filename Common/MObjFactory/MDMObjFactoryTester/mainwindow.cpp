#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MDSwitchFactory.h"
#include "MDFuncSwitchFactory.h"
#include "MDLogSwitchFactory.h"
#include <QDateTime>
#include "MDSwitchFactoryUI.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString pid;
    QString pid2;
    #ifdef Q_OS_LINUX
        pid = "/home/hite/NetSCADA6/project/testSwitch";
        pid2 = "/home/hite/NetSCADA6/project/testSwitch2";
    #else
        pid = "E:\\NetSCADA6\\project\\testSwitch";
        pid2 = "E:\\NetSCADA6\\project\\testSwitch2";
    #endif

    CMDSwitch* fs = CMDFuncSwitchFactory::createSwitch(pid, "script", "脚本");
    CMDFuncSwitchFactory::createSwitch(pid, "project.alarm.submit", "报警判断");
    CMDFuncSwitchFactory::createSwitch(pid, "drivers.rtu.device1", "RTU设备");
    m_fsFactory = (CMDSwitchFactory*)fs->factory();

    CMDFuncSwitchFactory::createSwitch(pid2, "drivers.rtu.device1", "RTU设备");

    CMDLogSwitch* ls = CMDLogSwitchFactory::createSwitchWithLogger(pid2, "drivers.rtu.device1" , "RTU设备");
    ls->turnOn();
    ls->LOG_INFO("收到变量写指令");


//    CMDSwitchFactory::createSwitch(CMDMObjPara(pid, "module", "alarm", "报警提交功能开关"));

//    for(int m = 0; m < 10000; m++)
//        CMDSwitchFactory::createSwitch(CMDMObjPara(pid, QString("module%1").arg(m), "alarm.submit.ss", "报警总开关"));

//    QDateTime dt1 = QDateTime::currentDateTime();
//    for(int i = 0; i < 10000; i++)
//        CMDSwitchFactory::createLogSwitch(pid, "script", "脚本日志");
//    QDateTime dt2 = QDateTime::currentDateTime();
//    qint64 dts = dt1.msecsTo(dt2);
//    QString ss = QString("%1").arg(dts);

//    CMDMObjPara p1(pid, "aa", "bb", "cc报警提交功能开关");
//    CMDMObjPara p2 = p1;
//    p2 = p1;

//    CMDMObjPara& p3 = p1;

    int i = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ConfigFuncFactory_clicked()
{
    CMDSwitchFactoryUI sui;
    sui.showFactory(m_fsFactory);
}
