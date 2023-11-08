/*!

\author dGFuZ3p3

\section 功能

事件队列引擎类，管理需要处理的事件。

*/

#ifndef MDEVENTENGINE_H
#define MDEVENTENGINE_H

#include "../../include/MDCommandQueueEngine/MDCommandQueueEngine.h"

class CMDRawEvent;
class CMDEventExpertImp;
class CMDEventEngine : public CMDCommandQueueEngine
{
    //Q_OBJECT

public:
    explicit CMDEventEngine(CMDEventExpertImp* eventExpert);
    ~CMDEventEngine();
protected:
    CMDEventExpertImp* m_eventExpert ;// NULL;
public:
    bool asyncProcessRawEvent(CMDRawEvent* rawEvent);
public:
    virtual void onProcessCommand(CMDCommand* command);
};

#endif // MDEVENTENGINE_H
