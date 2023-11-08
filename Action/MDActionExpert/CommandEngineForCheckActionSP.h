/*!

\author dGFuZ3p3

\section 功能

动作队列引擎类，管理执行前需要进行安全检查的动作。
该引擎一定是在主线程处理动作，因为安全检查时会显示验证界面，只能在主线程执行。

*/

#ifndef COMMANDENGINEFORCHECKACTIONSP_H
#define COMMANDENGINEFORCHECKACTIONSP_H

#include "../../include/MDCommandQueueEngine/MDCommandQueueEngine.h"

class CMDActionExpertImp;
class CMDAction;
class CCommandEngineForCheckActionSP : public CMDCommandQueueEngine
{
public:
    CCommandEngineForCheckActionSP(CMDActionExpertImp* eventExpert);
protected:
    CMDActionExpertImp* m_actionExpert ;// NULL;
public:
    void submitActionWithSP(CMDAction* action);
public:
    virtual void onProcessCommand(CMDCommand* command);
};

#endif // COMMANDENGINEFORCHECKACTIONSP_H
