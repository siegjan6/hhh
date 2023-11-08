#ifndef MDRECIPEEVENT_H
#define MDRECIPEEVENT_H

#include "MDEvent.h"

#ifdef UA_ENABLE_SUBSCRIPTIONS_EVENTS

class MDRecipeEvent : public MDEvent
{
    Q_OBJECT
	
friend class QUaServer;

public:
    Q_INVOKABLE explicit MDRecipeEvent(QUaServer *server);
};

#endif // UA_ENABLE_SUBSCRIPTIONS_EVENTS

#endif // MDRECIPEEVENT_H

