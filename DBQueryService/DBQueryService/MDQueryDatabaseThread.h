#ifndef MDQUERYDATABASETHREAD_H
#define MDQUERYDATABASETHREAD_H

/*!

\author bHVvY2hhbw==

\section 功能
1.数据库查询线程--继承类CMDCommandQueueEngine
2.每一个查询是一个命令，将命令扔到线程队列中执行。
3.需要重新实现onProcessCommand方法，此方法为线程执行的空间。

*/

#include "MDCommandQueueEngine/MDCommandQueueEngine.h"

class CMDQueryDatabaseThread : public CMDCommandQueueEngine
{
public:
    CMDQueryDatabaseThread();

    /*!
    \section 回调函数，要在线程中处理，
             必须在这个回调函数内部完成，
             才能保证相应的处理是在当前类创建的线程内处理
    */
protected:
    virtual void onProcessCommand(CMDCommand* command);
};

#endif // MDQUERYDATABASETHREAD_H
