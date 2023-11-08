#include "CommandEngineForHandleChangedVariable.h"
#include "MDProjectExpertImp.h"

CCommandEngineForHandleChangedVariable::CCommandEngineForHandleChangedVariable()
{
    m_projectExpert = NULL;
    setComment("ProjectExpert.EngineForHandleChangedVariable");
}

void CCommandEngineForHandleChangedVariable::submitVariableChangeMsg(IMDChangeVariable* changedVariable)
{
    CMDCommand* cmd = new CMDCommand(changedVariable);
    submitCommand(cmd);
}

void CCommandEngineForHandleChangedVariable::onProcessCommand(CMDCommand* command)
{
    IMDChangeVariable* changedVariable = (IMDChangeVariable*)command->cmdPara();

    if(m_projectExpert)
    {
        m_projectExpert->onVariableChanged_HMI_Script(changedVariable);
    }

    delete changedVariable;
}
