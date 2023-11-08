#include "MDExclusiveLevelAlarm.h"

#ifdef UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS

#include <QUaServer>

MDExclusiveLevelAlarm::MDExclusiveLevelAlarm(
	QUaServer* server
) : QUaExclusiveLevelAlarm(server)
{
	
}

QString MDExclusiveLevelAlarm::getExtProp() const
{
    //return this->browseChild<QUaProperty>("ExtProp");
    return const_cast<MDExclusiveLevelAlarm*>(this)->extProp()->value<QString>();
//    return m_extProp;
}

void MDExclusiveLevelAlarm::setExtProp(QString extProp)
{
    this->extProp()->setValue(extProp);
//    if (extProp == this->extProp())
//    {
//        return;
//    }
    m_extProp = extProp;
}

QUaProperty* MDExclusiveLevelAlarm::extProp() const
{
    return const_cast<MDExclusiveLevelAlarm*>(this)->browseChild<QUaProperty>("ExtProp");
}

#endif // UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS
