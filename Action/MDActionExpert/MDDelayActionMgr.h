/*!

\author dGFuZ3p3

\section 功能

延迟动作管理类，管理需要延时执行的动作。

*/

#ifndef MDDELAYACTIONMGR_H
#define MDDELAYACTIONMGR_H

#include <QLinkedList>
#include <QTimer>
#include <QMutex>
#include <QDateTime>

class CMDAction;
class CMDDelayAction
{
public:
    CMDDelayAction(CMDAction* action)
    {
        m_action = action;
        m_timeOnCreated = QDateTime::currentDateTime();
    }
    CMDAction* m_action ;// NULL;
    QDateTime m_timeOnCreated;
};

class CMDActionExpertImp;
class CMDDelayActionMgr : public QObject
{
    Q_OBJECT
public:
    CMDDelayActionMgr();
    virtual ~CMDDelayActionMgr();
/*!
\section 初始化与释放
*/
public:
    void init(CMDActionExpertImp* actionExpert);
    void release();
    CMDActionExpertImp* m_actionExpert ;// NULL;

/*!
\section 管理延时动作
*/
public:
    bool add(CMDAction* action);
protected:
    void removeAll();
protected:
    QLinkedList<CMDDelayAction*> m_delayActionQueue;
    QMutex m_delayActionQueueMutex;
protected:
    QTimer m_delayTimer; //! 延时定时器，精度为 100ms
private slots:
    void checkDelay(); //! 检测队列中动作的延时情况，延时已到的动作将立即执行
};

#endif // MDDELAYACTIONMGR_H
