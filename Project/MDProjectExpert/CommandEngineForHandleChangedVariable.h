/*!

\author dGFuZ3p3

\section 功能

变量变化队列引擎类，管理变量变化后，通知 HMI 和 脚本处理。
该引擎一定是在主线程处理命令，因为 HMI 涉及界面，而脚本也只能在主线程执行。

*/

#ifndef COMMANDENGINEFORHANDLECHANGEDVARIABLE_H
#define COMMANDENGINEFORHANDLECHANGEDVARIABLE_H

#include "../../include/MDCommandQueueEngine/MDCommandQueueEngine.h"
#include "../../include/MDRTDBInterfaces/IMDChangeVariable.h"

class CMDProjectExpertImp;
class CMDVariableChangedMsg;
class CCommandEngineForHandleChangedVariable : public CMDCommandQueueEngine
{
public:
    CCommandEngineForHandleChangedVariable();
    CMDProjectExpertImp* m_projectExpert ;// NULL;
    void submitVariableChangeMsg(IMDChangeVariable* changedVariable);
public:
    virtual void onProcessCommand(CMDCommand* command);
};

#endif // COMMANDENGINEFORHANDLECHANGEDVARIABLE_H
