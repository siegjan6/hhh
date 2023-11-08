#include "MDVariableArchivesEngine.h"

CMDVariableArchivesThread::CMDVariableArchivesThread()
{
    m_stopFlag = false;
    m_archivesVariableManager = NULL;
    m_time.start();
}
CMDVariableArchivesThread::~CMDVariableArchivesThread()
{
}
void CMDVariableArchivesThread::stop()
{
    QMutexLocker locker(&m_mutex);
    m_stopFlag = true;
}

void CMDVariableArchivesThread::setArchivesVariableManager(CMDArchivesVariableManager *archivesVariableManager )
{
    m_archivesVariableManager = archivesVariableManager;
}

void CMDVariableArchivesThread::run()
{
    if(m_archivesVariableManager->m_logSwitch != NULL && m_archivesVariableManager->m_logSwitch->isOn())
        m_archivesVariableManager->m_logSwitch->LOG_INFO("Enter ArchivesThread");

    m_time.start();
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

        int timeDiff=m_time.elapsed();//ms
        if(timeDiff>=50||isFirst)
        {
            isFirst = false;
            m_archivesVariableManager->doCycleDataArchives();
            m_archivesVariableManager->doChangeDataArchives();
            m_archivesVariableManager->doChangeDataPrjInitValueArchives();
            m_time.restart();
        }
        else
        {
             msleep(10);
        }
    }
    m_stopFlag = false;
    m_archivesVariableManager->stop();
    if(m_archivesVariableManager->m_logSwitch != NULL && m_archivesVariableManager->m_logSwitch->isOn())
        m_archivesVariableManager->m_logSwitch->LOG_INFO("Leave ArchivesThread");
}

//--------------------------------CMDVariableArchivesEngine-------------------------

CMDVariableArchivesEngine::CMDVariableArchivesEngine()
{
    m_archivesThread =NULL;
}

CMDVariableArchivesEngine::~CMDVariableArchivesEngine()
{
}

void CMDVariableArchivesEngine::start(CMDArchivesVariableManager *archivesVariableManager )
{
     m_archivesThread=new CMDVariableArchivesThread();
    if(NULL!=m_archivesThread)
    {
       m_archivesThread->setArchivesVariableManager(archivesVariableManager );
       m_archivesThread->start();
    }
}
void CMDVariableArchivesEngine::stop()
{
    if(NULL!=m_archivesThread)
    {
        m_archivesThread->stop();
        m_archivesThread->wait();
        m_archivesThread->deleteLater();
        m_archivesThread = NULL;
    }
}

bool CMDVariableArchivesEngine::isRunning()
{
    if(NULL!=m_archivesThread)
    {
       return true;
    }
   return false;
}
