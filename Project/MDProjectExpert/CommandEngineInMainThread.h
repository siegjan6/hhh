#ifndef COMMANDENGINEINMAINTHREAD_H
#define COMMANDENGINEINMAINTHREAD_H

#include "../../include/MDCommandQueueEngine/MDCommandQueueEngine.h"

class CMDProjectExpertImp;
class CMDAction;
class CCommandEngineInMainThread : public CMDCommandQueueEngine
{
public:
    CCommandEngineInMainThread();
    CMDProjectExpertImp* m_projectExpert ;// NULL;
    void submitQueryResult(const QString& queryID, int result);
public:
    virtual void onProcessCommand(CMDCommand* command);
};

#endif // COMMANDENGINEINMAINTHREAD_H
