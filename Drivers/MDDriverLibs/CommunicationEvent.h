/*!
\author enli 2015.01.08
\section 目前未使用
*/
#ifndef COMMUNICATIONEVENT_H
#define COMMUNICATIONEVENT_H


#include "MDDriverLibs_global.h"
#include "IEventCallback.h"
class CBaseChannel;

class MDDRIVERLIBSSHARED_EXPORT CCommunicationEvent : public IEventCallback
{
public:
    CCommunicationEvent();
    ~CCommunicationEvent();
//事件通知接口
public:
    void onEvent(const CommunicationEventType eventType);
//关联Channel
public:
    CBaseChannel *getChannel();
    void setChannel(CBaseChannel *channel);
private:
    //关联Channel
    CBaseChannel *m_channel;
};

#endif // COMMUNICATIONEVENT_H
