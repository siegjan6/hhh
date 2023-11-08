#ifndef QUAREFRESHREQUIREDEVENT_H
#define QUAREFRESHREQUIREDEVENT_H

#include "../../include/MDUaServer/MDUaServer_Global.h"
#include <QUaSystemEvent>

#ifdef UA_ENABLE_SUBSCRIPTIONS_EVENTS

class MDUASERVER_EXPORT QUaRefreshRequiredEvent : public QUaSystemEvent
{
    Q_OBJECT

friend class QUaServer;

public:
	Q_INVOKABLE explicit QUaRefreshRequiredEvent(
		QUaServer *server
	);

};

#endif // UA_ENABLE_SUBSCRIPTIONS_EVENTS

#endif // QUAREFRESHREQUIREDEVENT_H
