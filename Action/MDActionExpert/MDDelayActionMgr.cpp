#include "MDDelayActionMgr.h"
#include "MDAction.h"
#include "MDActionExpertImp.h"

CMDDelayActionMgr::CMDDelayActionMgr()
{

}

CMDDelayActionMgr::~CMDDelayActionMgr()
{
    release();
}

/*!
\section 初始化与释放
*/
void CMDDelayActionMgr::init(CMDActionExpertImp* actionExpert)
{
    m_actionExpert = actionExpert;

    m_delayTimer.setInterval(100);

    connect(&m_delayTimer, SIGNAL(timeout()), this, SLOT(checkDelay()));
}

void CMDDelayActionMgr::release()
{
    m_delayTimer.stop();

    removeAll();
}

/*!
\section 管理延时动作
*/
bool CMDDelayActionMgr::add(CMDAction* action)
{
    QMutexLocker locker(&m_delayActionQueueMutex);

    if(!m_delayTimer.isActive())
        m_delayTimer.start();

    if(m_delayTimer.isActive())
    {
        CMDDelayAction* delayAction = new CMDDelayAction(action);
        m_delayActionQueue.append(delayAction);
    }

    return (m_delayTimer.isActive());
}

void CMDDelayActionMgr::removeAll()
{
    QMutexLocker locker(&m_delayActionQueueMutex);

    QLinkedList<CMDDelayAction*>::iterator iPosition = m_delayActionQueue.begin();
    while(iPosition != m_delayActionQueue.end())
    {
        CMDDelayAction* delayAction = *iPosition;
        delete delayAction;
        iPosition ++;
    }
    m_delayActionQueue.clear();
}

void CMDDelayActionMgr::checkDelay()
{
    QMutexLocker locker(&m_delayActionQueueMutex);

    // 检查，延时已到的动作立即执行
    // -------------------------------------------------------------------------------------------------------------
    QLinkedList<CMDDelayAction*>::iterator iPosition = m_delayActionQueue.begin();
    while(iPosition != m_delayActionQueue.end())
    {
        CMDDelayAction* delayAction = *iPosition;

        qint64 timeSpan = delayAction->m_timeOnCreated.msecsTo(QDateTime::currentDateTime());
        if(timeSpan >= delayAction->m_action->configProperty()->delayLength())
        {
            iPosition = m_delayActionQueue.erase(iPosition);

            // 延时已到，执行动作
            // ------------------------------------------------------------------------------------------------
            m_actionExpert->asyncProcessAction_NoDelay(delayAction->m_action);

            delete delayAction;
        }
        else iPosition ++;
    }

    // 延时队列已空，停止定时器
    if(m_delayActionQueue.isEmpty())
        m_delayTimer.stop();
}
