#include "MDSystemVariableTimer.h"
#include "MDVariableGroup.h"
CMDSystemVariableTimer::CMDSystemVariableTimer()
{
    m_systemVariableGroup = NULL;
    m_timer.setInterval(50);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
 }
CMDSystemVariableTimer::~CMDSystemVariableTimer()
{

 }
void CMDSystemVariableTimer::setSystemVariableGroup(CMDVariableGroup *systemVariableGroup)
{
    QMutexLocker locker(&m_timerMutex);
    m_systemVariableGroup = systemVariableGroup;
}
void CMDSystemVariableTimer::start()
{
    QMutexLocker locker(&m_timerMutex);
    m_timer.start();
}
void CMDSystemVariableTimer::stop()
{
    QMutexLocker locker(&m_timerMutex);
    m_timer.stop();
}
/*!
\section 每50ms刷新系统的变量的值
*/
void CMDSystemVariableTimer::onTimerOut()
{
    QMutexLocker locker(&m_timerMutex);
    if(m_systemVariableGroup == NULL)
    {
        return;
    }
    m_systemVariableGroup->updateSystemVariable();
 }
