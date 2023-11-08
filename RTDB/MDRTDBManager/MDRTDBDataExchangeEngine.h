/*!
author enli 2015.01.08
\section 目前没使用
*/
#ifndef CMDRTDBDATAEXCHANGEENGINE_H
#define CMDRTDBDATAEXCHANGEENGINE_H

#include "IMDRTDBVariable.h"
#include "IMDChangeVariable.h"
#include <QThread>
class  IMDRTDBDataExchangeEngine
{
public:
    virtual ~IMDRTDBDataExchangeEngine(){}
public:
    virtual void  start()=0;
    virtual void  stop()=0;
    virtual void  notifyNewChangeVariable(IMDRTDBVariable *changedVar,const VariableChangeType &varChangeType)=0;
};

#include "MDVariableQueueManager.h"

class CMDRTDBDataExchangeEngine;

class CMDDataExchangeThread : public QThread
{
    Q_OBJECT
public:
    CMDDataExchangeThread();
    ~CMDDataExchangeThread();
public:
    void setRTDBDataExchangeEngine(CMDRTDBDataExchangeEngine*engine);
    void stop();
protected:
    void run();
private:
    bool m_stopFlag;
    QMutex m_mutex;
    QTime m_time;
    CMDRTDBDataExchangeEngine*m_mdRTDBDataExchangeEngine;
};



class CMDRTDBDataExchangeEngine:public IMDRTDBDataExchangeEngine
{
public:
    CMDRTDBDataExchangeEngine();
    ~CMDRTDBDataExchangeEngine();
public:
    void notifyNewChangeVariable(IMDRTDBVariable *changedVar,const VariableChangeType &varChangeType);
    void start();
    void stop();
    void notifyDataExchangeProcess();
private:
    //待处理的变化变量
    CMDVariableQueueManager m_changedVariableQueueManager;
    //已处理的变化变量
    CMDVariableQueueManager m_processedChangedVariableQueueManager;
private:
    CMDDataExchangeThread *m_doDataExchangeThread;
    bool m_stopFlag;
    QMutex m_mutex;
};

#endif // CMDRTDBDATAEXCHANGEENGINE_H
