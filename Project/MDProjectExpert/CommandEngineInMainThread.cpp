#include "CommandEngineInMainThread.h"
#include "MDProjectExpertImp.h"

CCommandEngineInMainThread::CCommandEngineInMainThread()
{
    m_projectExpert = NULL;
    setComment("ProjectExpert.CommandEngineInMainThread");
}

void CCommandEngineInMainThread::submitQueryResult(const QString& queryID, int result)
{
    CMDAction* qResultAction = new CMDAction;
    qResultAction->configProperty()->setName(queryID);
    qResultAction->configProperty()->setDelayLength(result);

    CMDCommand* cmd = new CMDCommand(qResultAction);
    cmd->setCmdType(0);
    submitCommand(cmd);
}

void CCommandEngineInMainThread::onProcessCommand(CMDCommand* command)
{
    CMDAction* action = (CMDAction*)command->cmdPara();

    if(0 == command->cmdType())
    {
        if(m_projectExpert)
            m_projectExpert->onQueryResult(action->configProperty()->name(),
                                                                        action->configProperty()->delayLength());

        command->setCmdPara(NULL);
        delete action;
    }
}
