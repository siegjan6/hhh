#include "MDActionEngine.h"
#include "MDActionExpertImp.h"

// 仅供 CMDActionEngine 使用的命令类，由于传递 CMDAction 对象指针
// ---------------------------------------------------------------------------------
class CMDActionCommand : public CMDCommand
{
    friend class CMDActionEngine;
protected:
    CMDActionCommand(CMDAction* action);
    void setAction(CMDAction* action);
    CMDAction* m_action;
};
CMDActionCommand::CMDActionCommand(CMDAction* action)
{
    m_action = action;
}
void CMDActionCommand::setAction(CMDAction* action)
{
    m_action = action;
}

// ---------------------------------------------------------------------------------

CMDActionEngine::CMDActionEngine(CMDActionExpertImp* actionExpert)
{
    m_actionExpert = actionExpert;

    setComment("ActionExpert.ActionEngine");
    setMaxRecycleQueueLength(10 * 1024);
}

CMDActionEngine::~CMDActionEngine()
{

}

bool CMDActionEngine::asyncProcessAction(CMDAction* action)
{
    CMDActionCommand* recycleCmd = (CMDActionCommand*)fetchRecycleCommand();
    if(recycleCmd)
        recycleCmd->setAction(action);
    CMDActionCommand* newCmd = (recycleCmd ? recycleCmd : new CMDActionCommand(action));
    return submitCommand(newCmd);
}

void CMDActionEngine::onProcessCommand(CMDCommand* command)
{
    if(m_actionExpert)
        m_actionExpert->syncProcessAction(((CMDActionCommand*)command)->m_action);
}
