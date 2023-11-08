#include "CommandEngineForCheckActionSP.h"
#include "MDActionExpertImp.h"

CCommandEngineForCheckActionSP::CCommandEngineForCheckActionSP(CMDActionExpertImp* eventExpert)
{
    m_actionExpert = eventExpert;

    setComment("ActionExpert.CommandEngineForCheckActionSP");
}

void CCommandEngineForCheckActionSP::submitActionWithSP(CMDAction* action)
{
    CMDCommand* cmd = new CMDCommand(action);
    submitCommand(cmd);
}

void CCommandEngineForCheckActionSP::onProcessCommand(CMDCommand* command)
{
    CMDAction* action = (CMDAction*)command->cmdPara();

    if(m_actionExpert)
        m_actionExpert->asyncProcessAction_AfterCheckSP(action);
}
