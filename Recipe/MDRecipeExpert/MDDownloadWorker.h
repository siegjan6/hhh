#ifndef MDRECIPEDOWNLOADTHREAD_H
#define MDRECIPEDOWNLOADTHREAD_H

#include <QThread>
#include <QMutex>
#include <QTimer>
class CMDDownloadExpert;
class CMDDownloadWorker : public QObject
{
    Q_OBJECT
public:
    explicit CMDDownloadWorker(CMDDownloadExpert* downloadExpert, QObject *parent = 0);
    ~CMDDownloadWorker();
protected:
    CMDDownloadExpert* m_downloadExpert;

/*!
\section 下载线程
*/
public:
    bool startThread();
    bool stopThread(unsigned long timeout);
    bool isThreadRunning();
    bool isThreadRunning_NoLocker();
protected:
    QThread* m_thread;
    QMutex m_threadMutex;
    QTimer* m_timer;
signals:
    void stopTimer();
public slots:
    void handleTask();
    void handleTaskOnTimer();
    void destroyTimer();
};

#endif // MDRECIPEDOWNLOADTHREAD_H
