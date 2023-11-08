/*!
\author enli 2015.01.08
\section 执行周期或变化归档线程，调用 by CMDVariableArchivesManager
*/
#ifndef MDVARIABLEARCHIVESENGINE_H
#define MDVARIABLEARCHIVESENGINE_H

#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <QTime>
#include "MDArchivesVariableManager.h"
class CMDRTDBManager;
class CMDVariableArchivesThread : public QThread
{
    Q_OBJECT
public:
    CMDVariableArchivesThread();
    ~CMDVariableArchivesThread();
public:
    void setArchivesVariableManager(CMDArchivesVariableManager *archivesVariableManager );
    void stop();
protected:
    void run();
private:
    bool m_stopFlag;
    QMutex m_mutex;
    QTime m_time;
    CMDArchivesVariableManager *m_archivesVariableManager;
};


class CMDVariableArchivesEngine:QObject
{
    Q_OBJECT
public:
    CMDVariableArchivesEngine();
    ~CMDVariableArchivesEngine();
public:
    void start(CMDArchivesVariableManager *archivesVariableManager);
    void stop();
    bool isRunning();
private:
    CMDVariableArchivesThread *m_archivesThread ;
};
#endif // MDVARIABLEARCHIVESENGINE_H
