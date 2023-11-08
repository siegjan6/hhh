/*!
author enli 2015.01.08
\section 变量仿真管理类。每个CMDRTDBSimulatiorEngine对象（CMDRTDBManager创建）后台开启一个CMDRTDBSimulatiorThread线程。
*/
#ifndef CMDRTDBSIMULATIORENGINE_H
#define CMDRTDBSIMULATIORENGINE_H

#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <QTime>
class CMDRTDBManager;
/*!
\section 仿真线程，隐藏于CMDRTDBSimulatiorEngine对象
*/
class CMDRTDBSimulatiorThread : public QThread
{
    Q_OBJECT
public:
    CMDRTDBSimulatiorThread();
    ~CMDRTDBSimulatiorThread();
public:
    void setRTDBManager(CMDRTDBManager *nsRTDBManager);
    void stop();
protected:
    void run();
private:
    bool m_stopFlag;
    QMutex m_mutex;
    QTime m_time;
    CMDRTDBManager*m_mdRTDBManager;
};

/*!
\section 变量仿真管理类
*/
class CMDRTDBSimulatiorEngine:QObject
{
    Q_OBJECT
public:
    CMDRTDBSimulatiorEngine();
    ~CMDRTDBSimulatiorEngine();
public:
    void setRTDBManager(CMDRTDBManager *nsRTDBManager);
    void start();
    void stop();
private:
    CMDRTDBSimulatiorThread *m_simulatiorThread;
    CMDRTDBManager*m_mdRTDBManager;
};


#endif // CMDRTDBSIMULATIORENGINE_H
