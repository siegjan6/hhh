/*!

\author dGFuZ3p3

\section 功能

HMI动作队列引擎类，管理 HMI 相关的动作，例如打开窗口、关闭窗口。
该引擎一定是在主线程处理动作，因为 HMI 动作都涉及界面，只能在主线程执行。

*/

#ifndef COMMANDENGINEFOREXECUTEHMIACTION_H
#define COMMANDENGINEFOREXECUTEHMIACTION_H

#include "../../include/MDCommandQueueEngine/MDCommandQueueEngine.h"

class CMDProjectExpertImp;
class CMDAction;
class CCommandEngineForExecuteHMIAction : public CMDCommandQueueEngine
{
public:
    CCommandEngineForExecuteHMIAction();
    CMDProjectExpertImp* m_projectExpert ;// NULL;
    void submitHMIAction(CMDAction* action);
public:
    virtual void onProcessCommand(CMDCommand* command);
};

#endif // COMMANDENGINEFOREXECUTEHMIACTION_H
