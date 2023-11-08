/*!
author enli 2015.01.08
\section 系统变量定时器管理类（CMDRTDBManager创建）, 每50ms刷新系统变量的值
*/
#ifndef MDSYSTEMVARIABLETIMER_H
#define MDSYSTEMVARIABLETIMER_H
#include <QTimer>
#include <QMutex>

class CMDVariableGroup;
class CMDSystemVariableTimer:QObject
{
    Q_OBJECT
public:
    CMDSystemVariableTimer();
    ~CMDSystemVariableTimer();
public:
    void start();
    void stop();
    void setSystemVariableGroup(CMDVariableGroup *systemVariableGroup);
private:
    QTimer m_timer;
    CMDVariableGroup *m_systemVariableGroup;
    QMutex m_timerMutex;
private slots:
      void onTimerOut();
};

#endif // MDSYSTEMVARIABLETIMER_H
