#include "timerthread.h"
#include <MDSysLogExpert/MDSysLogExpert.h>

TimerThread::TimerThread()
    :m_Stop(true),
     m_Millisecond(0),
     m_Callback(0)
{
}

TimerThread::~TimerThread()
{
    Stop();
    delete m_Callback;
}

void TimerThread::Start(ITimerCallback * callback, int millisecond)
{
    m_Callback = callback;
    m_Stop = false;
    m_Millisecond = millisecond;
    QThread::start();//启动线程
}

void TimerThread::Stop()
{
    m_StopLock.lock();
    m_Stop = true;
    m_StopConditon.wakeOne();
    m_StopLock.unlock();

    MDLOG_INFO_TO_LOGGER("TimerThread::Stop:Start");
    wait(); //等待run返回
    MDLOG_INFO_TO_LOGGER("TimerThread::Stop:End");
}

void TimerThread::run()
{
    while(!m_Stop)
    {
        m_StopLock.lock(); //为了调用Stop时及时退出
        if(!m_Stop)
        {
            if(m_Millisecond<1)
                m_StopConditon.wait(&m_StopLock);
            else
                m_StopConditon.wait(&m_StopLock, m_Millisecond);
        }
        m_StopLock.unlock();

        if(m_Callback)
            m_Callback->Call();
    } //end while
}
