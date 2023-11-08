#include "MDRTDBDataExchangeEngine.h"
#include "MDChangeVariable.h"


//--------------------------CMDDataExchangeThread---------------------------------
CMDDataExchangeThread::CMDDataExchangeThread()
{
    m_stopFlag = false;
    m_mdRTDBDataExchangeEngine = NULL;
    m_time.start();
}
CMDDataExchangeThread::~CMDDataExchangeThread()
{
}
void CMDDataExchangeThread::stop()
{
    QMutexLocker locker(&m_mutex);
    m_stopFlag = true;
}
void CMDDataExchangeThread::setRTDBDataExchangeEngine(CMDRTDBDataExchangeEngine*engine)
{
    m_mdRTDBDataExchangeEngine = engine;
}

void CMDDataExchangeThread::run()
{
    m_time.restart();
    bool isFirst = true;
    while(1)
    {

        m_mutex.lock();
        if(m_stopFlag)
        {
            m_mutex.unlock();
            break;
        }
        else
        {
            m_mutex.unlock();
        }

        uint timeDiff=m_time.elapsed();
        if(timeDiff>=1000||isFirst)
        {
            isFirst = false;
            if(m_mdRTDBDataExchangeEngine== NULL)
            {
                continue;
            }
            m_mdRTDBDataExchangeEngine->notifyDataExchangeProcess();
            m_time.restart();
        }
        else
        {
             sleep(10);
        }
    }
    m_stopFlag = false;
}



//---------------------------------CMDRTDBDataExchangeEngine--------------------

CMDRTDBDataExchangeEngine::CMDRTDBDataExchangeEngine()
{
    m_doDataExchangeThread = NULL;
}

CMDRTDBDataExchangeEngine::~CMDRTDBDataExchangeEngine()
{

}
//收到变量变化回调后，添加到变量链表并触发线程事件
void CMDRTDBDataExchangeEngine::notifyNewChangeVariable(IMDRTDBVariable *changedVar,const VariableChangeType &varChangeType)
{
    CMDChangeVariable *changeVariable =  (CMDChangeVariable *)m_processedChangedVariableQueueManager.removeAt(0);
    if(changeVariable== NULL)
    {
        changeVariable =new CMDChangeVariable();
    }
    changeVariable->copyEx((IMDVariable*)changedVar);
    changeVariable->setVariableChangeType(varChangeType);
    /*
    changeVariable->setVariableChangeType(varChangeType);
    changeVariable->setCurQuality(changedVar->getCurQuality());
    changeVariable->setLastQuality(changedVar->getLastQuality());
    changeVariable->setTimeStamp(changedVar->getTimeStamp());
    changeVariable->setName(changedVar->getName());
    changeVariable->setDataType(changedVar->getDataType());
    changeVariable->setCurValue(changedVar->getCurValue());
    changeVariable->setLastValue(changedVar->getLastValue());
    */

    m_changedVariableQueueManager.add(changeVariable);
    if(m_doDataExchangeThread==NULL)
    {
       m_doDataExchangeThread = new CMDDataExchangeThread();
       m_doDataExchangeThread->setRTDBDataExchangeEngine(this);
       m_doDataExchangeThread->start();
    }
}

void CMDRTDBDataExchangeEngine::start()
{
    m_mutex.lock();
    m_stopFlag = false;
    m_mutex.unlock();

}
void CMDRTDBDataExchangeEngine::stop()
{
    m_mutex.lock();
    m_stopFlag = true;
    m_mutex.unlock();
    if(m_doDataExchangeThread!=NULL)
    {
        m_doDataExchangeThread->stop();
        m_doDataExchangeThread->wait();
        m_doDataExchangeThread->deleteLater();
        m_doDataExchangeThread = NULL;
    }
}
void CMDRTDBDataExchangeEngine::notifyDataExchangeProcess()
{
    while(m_changedVariableQueueManager.getCount()>0 && m_stopFlag==false)
    {
        IMDVariable* var = m_changedVariableQueueManager.removeAt(0);

        m_processedChangedVariableQueueManager.add(var);
    }
}
