#ifndef QUAEXCLUSIVELEVELALARM_H
#define QUAEXCLUSIVELEVELALARM_H

#include "../../include/MDUaServer/MDUaServer_Global.h"
#include <QUaExclusiveLimitAlarm>

#ifdef UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS

class MDUASERVER_EXPORT QUaExclusiveLevelAlarm : public QUaExclusiveLimitAlarm
{
    Q_OBJECT

public:
	Q_INVOKABLE explicit QUaExclusiveLevelAlarm(
		QUaServer* server
	);

	// nothing to do here

protected:


};

#endif // UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS

#endif // QUAEXCLUSIVELEVELALARM_H

