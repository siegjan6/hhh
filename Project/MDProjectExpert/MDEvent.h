#ifndef MDEVENT_H
#define MDEVENT_H

#include <QUaBaseEvent>
#include <QUaProperty>

#ifdef UA_ENABLE_SUBSCRIPTIONS_EVENTS

class MDEvent : public QUaBaseEvent
{
    Q_OBJECT

    Q_PROPERTY(QUaProperty *          status   READ getStatus)//   WRITE setResult   NOTIFY resultChanged)
    Q_PROPERTY(QUaProperty *          meta READ getMeta)

friend class QUaServer;

public:
    Q_INVOKABLE explicit MDEvent(QUaServer *server);

    QVariant meta() const;
    void setMeta(const QVariant& meta);

    quint16 status() const;
    void setStatus(const quint16& intStatus);

signals:
    void metaChanged();
    void statusChanged();

protected:
    QUaProperty         * getMeta();
    QUaProperty         * getStatus();
};

#endif // UA_ENABLE_SUBSCRIPTIONS_EVENTS

#endif // MDEVENT_H

