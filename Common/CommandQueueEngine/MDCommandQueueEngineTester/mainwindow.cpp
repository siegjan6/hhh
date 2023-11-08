#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_engine.setComment("测试命令引擎");

    //m_engine.setDelayHandleProcessed(false);
//    m_engine.setModeToHandleProcessed(MDModeToHandleProcessedCommand::mode_delete);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pressureTestStartButton_clicked()
{
    m_timer.start(100);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimedOut()));

    m_engine.start();

    m_eUI.showMgrUI(true);
}

const qint32 SUBMIT_COUNT = 200000;
void MainWindow::onTimedOut()
{
    for(int i = 0; i < SUBMIT_COUNT; i++)
    {
//        m_engine.submitCommand(new CMDCommand());
        CMDCommand* newCmd = m_engine.fetchRecycleCommand();
        m_engine.submitCommand(newCmd ? newCmd : new CMDCommand());
    }
}

void MainWindow::on_pressureTestStopButton_clicked()
{
    m_eUI.showMgrUI(false);

    m_timer.stop();
    m_engine.stop();
}
