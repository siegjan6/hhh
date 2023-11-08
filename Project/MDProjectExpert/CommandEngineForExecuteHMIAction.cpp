#include "CommandEngineForExecuteHMIAction.h"
#include "MDProjectExpertImp.h"

CCommandEngineForExecuteHMIAction::CCommandEngineForExecuteHMIAction()
{
    m_projectExpert = NULL;
    setComment("ProjectExpert.CommandEngineForExecuteHMIAction");
}

void CCommandEngineForExecuteHMIAction::submitHMIAction(CMDAction* action)
{
    CMDCommand* cmd = new CMDCommand(action);
    submitCommand(cmd);
}

void CCommandEngineForExecuteHMIAction::onProcessCommand(CMDCommand* command)
{
    CMDAction* action = (CMDAction*)command->cmdPara();

    if(m_projectExpert)
        m_projectExpert->onExecuteHMIAction(action);
}
