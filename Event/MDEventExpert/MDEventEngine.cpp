#include "MDEventEngine.h"
#include "MDEventExpertImp.h"
#include "MDRawEvent.h"

// 仅供 CMDEventEngine 使用的命令类，用于传递 CMDRawEvent 对象指针
// ---------------------------------------------------------------------------------
class CMDEventCommand : public CMDCommand
{
    friend class CMDEventEngine;
protected:
    CMDEventCommand(CMDRawEvent* rawEvent);
    ~CMDEventCommand();
    CMDRawEvent* m_rawEvent;
};
CMDEventCommand::CMDEventCommand(CMDRawEvent* rawEvent)
{
    m_rawEvent = rawEvent;
}
CMDEventCommand::~CMDEventCommand()
{
    if(m_rawEvent)
        delete m_rawEvent;
}
// ---------------------------------------------------------------------------------

CMDEventEngine::CMDEventEngine(CMDEventExpertImp* eventExpert)
{
    m_eventExpert = eventExpert;

    setModeToHandleProcessed(MDModeToHandleProcessedCommand::mode_recycle);
    setComment("Event.EventEngine");
}

CMDEventEngine::~CMDEventEngine()
{

}

bool CMDEventEngine::asyncProcessRawEvent(CMDRawEvent* rawEvent)
{
    CMDEventCommand* newCommand = (CMDEventCommand*)fetchRecycleCommand();
    if(newCommand)
    {
        delete newCommand->m_rawEvent;
        newCommand->m_rawEvent = rawEvent;
    }
    else
        newCommand = new CMDEventCommand(rawEvent);

    return submitCommand(newCommand);
}

void CMDEventEngine::onProcessCommand(CMDCommand* command)
{
    if(m_eventExpert)
        m_eventExpert->syncProcessRawEvent(((CMDEventCommand*)command)->m_rawEvent);
}
