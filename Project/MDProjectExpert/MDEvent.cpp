#include "MDEvent.h"

#ifdef UA_ENABLE_SUBSCRIPTIONS_EVENTS

MDEvent::MDEvent(QUaServer *server)
	: QUaBaseEvent(server)
{

}

QVariant MDEvent::meta() const
{
    return const_cast<MDEvent*>(this)->getMeta()->value();
}
QUaProperty * MDEvent::getMeta()
{
    return this->browseChild<QUaProperty>("meta");
}
void MDEvent::setMeta(const QVariant& meta)
{
    if (meta == this->meta())
    {
        return;
    }
    // set last meta before writing new

    // set new meta
    this->getMeta()->setValue(meta);
    // emit change
    emit this->metaChanged();
    // any change to comment, severity and quality will cause event.
    // trigger event
    auto time = QDateTime::currentDateTimeUtc();
    this->setTime(time);
    this->setReceiveTime(time);
    this->setMessage(tr("meta changed to %1.").arg(meta.toString()));
    //this->trigger();
}

QUaProperty* MDEvent::getStatus()
{
    return this->browseChild<QUaProperty>("status");
}

quint16 MDEvent::status() const
{
    return (quint16)const_cast<MDEvent*>(this)->getStatus()->value<quint16>();
}
void MDEvent::setStatus(const quint16& intStatus)
{
    if (intStatus == this->status())
    {
        return;
    }
    // set last status before writing new
	
    // set new status
    this->getStatus()->setValue(intStatus);
    // emit change
    emit this->statusChanged();
    // any change to comment, severity and quality will cause event.
    // trigger event
    auto time = QDateTime::currentDateTimeUtc();
    this->setTime(time);
    this->setReceiveTime(time);
    this->setMessage(tr("Status changed to %1.").arg(intStatus));
    //this->trigger();
}

#endif // UA_ENABLE_SUBSCRIPTIONS_EVENTS
