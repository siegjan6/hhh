#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../include/MDSysLogExpert/MDSysLogExpert.h"
#define LOG_SOURCE "sys_log_tester"

#include <QtWidgets>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CMDSysLoggerPara loggerPara;
    loggerPara.setLogEngineType(CMDSysLoggerPara::LogEngineType::Default);
//    loggerPara.enableListAppender(false);
//    loggerPara.enableFileAppender(false);
    loggerPara.setMaxFileSize(8192);
    QString appPath = QCoreApplication::applicationDirPath();
    QString fileDir;
    #ifdef Q_OS_LINUX
        fileDir = appPath + "/netscada_sys_log_tester/";
    #else // windows
        fileDir = appPath + "/netscada_sys_log_tester/";
    #endif
    loggerPara.setFileDir(fileDir);
    loggerPara.setFileMainName("nslog_tester");
    loggerPara.setComment("我的日志记录器");
    CMDSysLogExpert::init(loggerPara);
    MDLOG_INFO("【我的日志记录器】已经启动");

    setWindowTitle(tr("NetSCADA 系统日志测试器"));
    //resize(640, 480);
}

MainWindow::~MainWindow()
{
    delete ui;

    m_logUI.closeAll();
    CMDSysLogExpert::release();
}

void MainWindow::createLog()
{
     MDLOG_INFO("测试一下::信息");
     MDLOG_INFO_SOURCE("测试一下：：警告", "Log::tester");
    CMDSysLogExpert::defaultLogger()->logError("测试一下::错误");
}

void MainWindow::on_pushButton_AddLog_clicked()
{
    createLog();
}

void MainWindow::on_pushButton_ShowUI_clicked()
{
    m_logUI.showModaless(CMDSysLogExpert::defaultLogger(), "", this);
}

void MainWindow::on_pushButton_CloseAllUI_clicked()
{
    m_logUI.closeAll();
}

void MainWindow::on_pressureTestStartButton_clicked()
{
    m_timer.start(100);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimedOut()));

    CMDCommandQueueEngineUI::showMgrUI(true);
}

void MainWindow::onTimedOut()
{
    QDateTime dt1 = QDateTime::currentDateTime();
    for(int i = 0; i < 10000; i++)
        MDLOG_INFO("测试一下::信息测试一下::信息测试一下::信息一下::信息");
    QDateTime dt2 = QDateTime::currentDateTime();
    QString ss = QString("%1").arg(dt1.msecsTo(dt2));
    qDebug() << ss;
}

void MainWindow::on_pressureTestStopButton_clicked()
{
    m_timer.stop();    
}
