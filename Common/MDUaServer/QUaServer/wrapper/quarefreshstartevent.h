#ifndef QUAREFRESHSTARTEVENT_H
#define QUAREFRESHSTARTEVENT_H

#include "../../include/MDUaServer/MDUaServer_Global.h"
#include <QUaSystemEvent>

#ifdef UA_ENABLE_SUBSCRIPTIONS_EVENTS

class MDUASERVER_EXPORT QUaRefreshStartEvent : public QUaSystemEvent
{
    Q_OBJECT

friend class QUaServer;

public:
	Q_INVOKABLE explicit QUaRefreshStartEvent(
		QUaServer *server
	);

};

#endif // UA_ENABLE_SUBSCRIPTIONS_EVENTS

#endif // QUAREFRESHSTARTEVENT_H
