#include "MDSysLoggerDialog.h"
#include "ui_MDSysLoggerDialog.h"
#include "MDSysLogTableModel.h"
#include <QCloseEvent>

CMDSysLoggerDialog::CMDSysLoggerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDSysLoggerDialog)
{
    m_closeDialog = true;
    m_logModel = NULL;

    ui->setupUi(this);

    m_updateTimer.start(1000);
    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(updateLogTable()));
}

CMDSysLoggerDialog::~CMDSysLoggerDialog()
{
    m_updateTimer.stop();

    delete ui;
}

/*!
\section 初始化
*/
void CMDSysLoggerDialog::setlogger(CMDSysLogger* logger)
{
    if(!m_logModel)
        m_logModel = new CMDSysLogTableModel(this);
    m_logModel->setLogger(logger);
    ui->tableView->setModel(m_logModel);
    ui->tableView->setColumnWidth(0, 180);
    ui->tableView->setColumnWidth(1, 50);
    ui->tableView->setColumnWidth(2, 600);
    ui->tableView->setColumnWidth(3, 200);
}

/*!
\section 日志数据模型；刷新界面
*/
void CMDSysLoggerDialog::updateLogTable()
{
    if(m_logModel)
        m_logModel->update();
}

/*!
\section 关闭处理
*/
void CMDSysLoggerDialog::setCloseDialog(bool closeDialog)
{
    m_closeDialog = closeDialog;
}

void CMDSysLoggerDialog::keyPressEvent(QKeyEvent * event)
{
    if(m_closeDialog)
        QDialog::keyPressEvent(event);
    else
    {
        if(Qt::Key_Escape != event->key())
        {
            QDialog::keyPressEvent(event);
        }
    }
}

void CMDSysLoggerDialog::closeEvent(QCloseEvent *event)
{
    if(m_closeDialog)
        QDialog::closeEvent(event);
    else
        hide();
}
