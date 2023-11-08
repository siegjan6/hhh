#ifndef MDOFFNORMALALARM_H
#define MDOFFNORMALALARM_H

#include <QUaOffNormalAlarm>

#ifdef UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS

class MDOffNormalAlarm : public QUaOffNormalAlarm
{
    Q_OBJECT

    Q_PROPERTY(QString ExtProp READ getExtProp WRITE setExtProp)

public:
	Q_INVOKABLE explicit MDOffNormalAlarm(
		QUaServer* server
	);

    //virtual void setInputNode(QUaBaseVariable* inputNode) override;
	// nothing to do here
    QString getExtProp() const;
    void setExtProp(QString extProp);

protected:

    QString m_extProp;

    QUaProperty* extProp() const;
};

#endif // UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS

#endif // QUADISCRETEALARM_H

