#include "MDEngineGeneralDialog.h"
#include "ui_MDEngineGeneralDialog.h"
#include "MDCommandQueueEngine.h"

CMDEngineGeneralDialog::CMDEngineGeneralDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDEngineGeneralDialog)
{
    ui->setupUi(this);

    m_totalProcessed = 0;
    m_totalSubmit = 0;
    m_engine = NULL;
    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(onUpdateTimedOut()));
}

CMDEngineGeneralDialog::~CMDEngineGeneralDialog()
{
    delete ui;
}

void CMDEngineGeneralDialog::init(CMDCommandQueueEngine* engine)
{
    m_engine = engine;

    QString comment = engine->comment();
    if(comment.isEmpty())
        comment = QString("%1").arg((qint64)engine);

    setWindowTitle(comment);
    ui->engineCommentLabel->setText(comment);
}

void CMDEngineGeneralDialog::startUpdate()
{
    if(m_engine)
    {
        m_totalProcessed = m_engine->totalProcessed();
        m_totalSubmit = m_engine->totalSubmit();

        ui->processSpeedLineEdit->setText("0");
        ui->submitSpeedLineEdit->setText("0");
        onUpdateTimedOut();

        m_updateTimer.start(1000);
    }
}

void CMDEngineGeneralDialog::stopUpdate()
{
    m_updateTimer.stop();
}

void CMDEngineGeneralDialog::onUpdateTimedOut()
{
    if(!m_engine)
        return;

    //  队列
    ui->watingLineEdit->setText(QString("%1").arg(m_engine->sizeOfWaitingQueue()));
    ui->processingLineEdit->setText(QString("%1").arg(m_engine->sizeOfProcessingQueue()));

    ui->processedLineEdit->setText(QString("%1").arg(m_engine->sizeOfProcessedQueue()));
    ui->recycleLineEdit->setText(QString("%1").arg(m_engine->sizeOfRecycleQueue()));

    // 处理数量
    ui->totalProcessedLineEdit->setText(QString("%1").arg(m_engine->totalProcessed()));
    ui->totalSubmitLineEdit->setText(QString("%1").arg(m_engine->totalSubmit()));

    ui->totalProcessedHistoryLineEdit->setText(QString("%1").arg(m_engine->totalProcessedHistory()));
    ui->totalSubmitHistoryLineEdit->setText(QString("%1").arg(m_engine->totalSubmitHistory()));

    // 运行时间
    qint64 timeSpan = m_engine->startTime().secsTo(QDateTime::currentDateTime());
    ui->totalTimeLineEdit->setText(QString("%1").arg(timeSpan));

    // 速度
    qint64 totalProcessed  = m_engine->totalProcessed();
    qint64 totalSubmit  = m_engine->totalSubmit();
    ui->processSpeedLineEdit->setText(QString("%1").arg(totalProcessed - m_totalProcessed));
    m_totalProcessed = totalProcessed;
    ui->submitSpeedLineEdit->setText(QString("%1").arg(totalSubmit - m_totalSubmit));
    m_totalSubmit = totalSubmit;
}
