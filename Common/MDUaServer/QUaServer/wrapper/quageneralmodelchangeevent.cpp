#include "quageneralmodelchangeevent.h"

#ifdef UA_ENABLE_SUBSCRIPTIONS_EVENTS

#include <QUaServer>

QUaGeneralModelChangeEvent::QUaGeneralModelChangeEvent(
	QUaServer *server
) : QUaBaseModelChangeEvent(server)
{
#ifdef UA_ENABLE_HISTORIZING
	m_historizing = false;
#endif // UA_ENABLE_HISTORIZING
}

QUaChangesList QUaGeneralModelChangeEvent::changes() const
{
	QUaChangesList retList;
	QVariant varList = this->getChanges()->value();
	if (!varList.isValid() || !varList.canConvert<QVariantList>())
	{
		return retList;
	}
	auto iter = varList.value<QSequentialIterable>();
	for (const QVariant &v : iter)
	{
		retList << v.value<QUaChangeStructureDataType>();
	}
	return retList;
}

void QUaGeneralModelChangeEvent::setChanges(const QUaChangesList & listVerbs)
{
	this->getChanges()->setValue(listVerbs);
}

QUaProperty * QUaGeneralModelChangeEvent::getChanges() const
{
	return const_cast<QUaGeneralModelChangeEvent*>(this)->browseChild<QUaProperty>("Changes");
}

#endif // UA_ENABLE_SUBSCRIPTIONS_EVENTS