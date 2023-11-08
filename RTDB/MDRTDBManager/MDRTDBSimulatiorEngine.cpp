#include "MDRTDBSimulatiorEngine.h"
#include "MDRTDBManager.h"
#include "IMDVariableManager.h"
//--------------------------------CMDRTDBSimulatiorThread-------------------------
CMDRTDBSimulatiorThread::CMDRTDBSimulatiorThread()
{
    m_stopFlag = false;
    m_mdRTDBManager = NULL;
    m_time.start();
}
CMDRTDBSimulatiorThread::~CMDRTDBSimulatiorThread()
{
}
void CMDRTDBSimulatiorThread::stop()
{
    QMutexLocker locker(&m_mutex);
    m_stopFlag = true;
}
void CMDRTDBSimulatiorThread::setRTDBManager(CMDRTDBManager *nsRTDBManager)
{
    m_mdRTDBManager = nsRTDBManager;
}

void CMDRTDBSimulatiorThread::run()
{
    if(m_mdRTDBManager != NULL && m_mdRTDBManager->getSwitchState(MDLogicSwitchType::Log))
        m_mdRTDBManager->m_logSwitch->LOG_INFO("Enter Simulatior Thread");

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
        if(timeDiff>=100||isFirst)
        {
            isFirst = false;
            if(m_mdRTDBManager== NULL)
            {
                continue;
            }
            IMDVariableManager *variableManager = m_mdRTDBManager->variableManager();
            for(int i=0;i < variableManager->getVariableCount();i++)
            {
                 IMDRTDBVariable* var = variableManager->getVariableAt(i);
                 if(var == NULL)
                 {
                     continue;
                 }
                 var->simulate();
                 if(i%500==0&& i>0)
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
                 }
            }
            m_time.restart();
        }
        else
        {
             msleep(100);
        }
    }
    m_stopFlag = false;
    if(m_mdRTDBManager != NULL && m_mdRTDBManager->getSwitchState(MDLogicSwitchType::Log))
        m_mdRTDBManager->m_logSwitch->LOG_INFO("Leave Simulatior Thread");
}

//--------------------------------CMDRTDBSimulatiorEngine-------------------------

CMDRTDBSimulatiorEngine::CMDRTDBSimulatiorEngine()
{
    m_mdRTDBManager =NULL;
    m_simulatiorThread =NULL;
}

CMDRTDBSimulatiorEngine::~CMDRTDBSimulatiorEngine()
{

}

void CMDRTDBSimulatiorEngine::setRTDBManager(CMDRTDBManager *nsRTDBManager)
{
    m_mdRTDBManager = nsRTDBManager;
}
void CMDRTDBSimulatiorEngine::start()
{
    m_simulatiorThread=new CMDRTDBSimulatiorThread();
    if(NULL!=m_simulatiorThread)
    {
       m_simulatiorThread->setRTDBManager(m_mdRTDBManager);
       m_simulatiorThread->start();
    }
}
void CMDRTDBSimulatiorEngine::stop()
{
    if(NULL!=m_simulatiorThread)
    {
        m_simulatiorThread->stop();
        m_simulatiorThread->wait();
        m_simulatiorThread->deleteLater();
        m_simulatiorThread = NULL;
    }
}
