/*!

\author dGFuZ3p3

\section 功能

动作队列引擎类，管理需要执行的动作。

*/

#ifndef MDACTIONENGINE_H
#define MDACTIONENGINE_H

#include "../../include/MDCommandQueueEngine/MDCommandQueueEngine.h"

class CMDAction;
class CMDActionExpertImp;
class CMDActionEngine : public CMDCommandQueueEngine
{
public:
    explicit CMDActionEngine(CMDActionExpertImp* eventExpert);
    ~CMDActionEngine();
protected:
    CMDActionExpertImp* m_actionExpert;
public:
    bool asyncProcessAction(CMDAction* action);
public:
    virtual void onProcessCommand(CMDCommand* command);
};

#endif // MDACTIONENGINE_H
