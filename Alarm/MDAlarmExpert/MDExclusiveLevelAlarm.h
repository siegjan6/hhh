#ifndef c
#define MDEXCLUSIVELEVELALARM_H

#include <QUaExclusiveLevelAlarm>

#ifdef UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS

class MDExclusiveLevelAlarm : public QUaExclusiveLevelAlarm
{
    Q_OBJECT

    Q_PROPERTY(QString ExtProp READ getExtProp WRITE setExtProp)

public:
	Q_INVOKABLE explicit MDExclusiveLevelAlarm(
		QUaServer* server
	);

	// nothing to do here
    QString getExtProp() const;
    void setExtProp(QString extProp);

protected:

    QString m_extProp;

    QUaProperty* extProp() const;
};

#endif // UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS

#endif // ExclusiveLevelAlarm

