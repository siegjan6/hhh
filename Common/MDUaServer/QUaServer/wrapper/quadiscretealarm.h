#ifndef QUADISCRETEALARM_H
#define QUADISCRETEALARM_H

#include "../../include/MDUaServer/MDUaServer_Global.h"
#include <QUaAlarmCondition>

#ifdef UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS

class MDUASERVER_EXPORT QUaDiscreteAlarm : public QUaAlarmCondition
{
    Q_OBJECT

public:
	Q_INVOKABLE explicit QUaDiscreteAlarm(
		QUaServer* server
	);

	// nothing to do here

protected:


};

#endif // UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS

#endif // QUADISCRETEALARM_H

