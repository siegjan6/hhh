#ifndef TIMERTHREAD_H
#define TIMERTHREAD_H

#include <QThread>
#include <QWaitCondition>
#include <QMutex>

class ITimerCallback
{
public:
    virtual void Call() = 0;
};

class TimerThread : public QThread
{
    Q_OBJECT
public:
    TimerThread();
    ~TimerThread();
    //!开始定时器线程 callback为回调接口 millisecond为定时器间隔时间
    //!如果millisecond小于等于0 则认为时间间隔无限长
    void Start(ITimerCallback * callback, int millisecond);
    void Stop();
protected:
    virtual void run() Q_DECL_OVERRIDE;
private:
    volatile bool m_Stop;
    QMutex m_StopLock;
    QWaitCondition m_StopConditon;
    ITimerCallback * m_Callback;//自动释放内存
    volatile int m_Millisecond; //时间间隔 单位为毫秒
};

#endif // TIMERTHREAD_H
