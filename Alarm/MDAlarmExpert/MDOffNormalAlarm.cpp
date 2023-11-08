#include "MDOffNormalAlarm.h"

#ifdef UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS

#include <QUaServer>

MDOffNormalAlarm::MDOffNormalAlarm(
	QUaServer* server
) : QUaOffNormalAlarm(server)
{
	
}

QString MDOffNormalAlarm::getExtProp() const
{
    //return this->browseChild<QUaProperty>("ExtProp");
    return const_cast<MDOffNormalAlarm*>(this)->extProp()->value<QString>();
//    return m_extProp;
}

void MDOffNormalAlarm::setExtProp(QString extProp)
{
    this->extProp()->setValue(extProp);
//    if (extProp == this->extProp())
//    {
//        return;
//    }
    m_extProp = extProp;
}

QUaProperty* MDOffNormalAlarm::extProp() const
{
    return const_cast<MDOffNormalAlarm*>(this)->browseChild<QUaProperty>("ExtProp");
}

//void MDOffNormalAlarm::setInputNode(QUaBaseVariable* inputNode)
//{
//        // call base implementation
//        QUaAlarmCondition::setInputNode(inputNode);
//        if (!inputNode)
//        {
//                return;
//        }
//        // subscribe to value changes
//        m_connections <<
//        QObject::connect(m_inputNode, &QUaBaseVariable::valueChanged, this,
//        [this](const QVariant& value) {
//                if (!m_normalValue.isValid())
//                {
//                        return;
//                }
//                this->setActive(value != m_normalValue, "$"+m_extProp);
//        });
//}

#endif // UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS
