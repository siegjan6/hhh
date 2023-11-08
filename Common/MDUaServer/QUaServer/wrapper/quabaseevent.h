#ifndef QUABASEEVENT_H
#define QUABASEEVENT_H

#include "../../include/MDUaServer/MDUaServer_Global.h"
// NOTE : this define needs to be out of UA_ENABLE_SUBSCRIPTIONS_EVENTS
//        otherwise we don't know if UA_ENABLE_SUBSCRIPTIONS_EVENTS is defined
#include <QUaBaseObject>

#ifdef UA_ENABLE_SUBSCRIPTIONS_EVENTS

class MDUASERVER_EXPORT QUaBaseEvent : public QUaBaseObject
{
    Q_OBJECT

friend class QUaServer;
friend class QUaNode;
friend class QUaBaseObject;
#ifdef UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS
friend class QUaConditionBranch;
#endif // UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS
#ifdef UA_ENABLE_HISTORIZING
	Q_PROPERTY(bool historizing READ historizing WRITE setHistorizing)
#endif // UA_ENABLE_HISTORIZING
public:
	// NOTE : Q_INVOKABLE removed on purpose because this class should not be instantiable
	explicit QUaBaseEvent(
		QUaServer *server
	);
	~QUaBaseEvent();

	// Event Properties API

	// Generated by the server to identify a particular *Event Notification*. 
	// The server is responsible to ensure the id is unique.
	QByteArray eventId() const;
	void       setEventId(const QByteArray& eventId);
	// Specific type of the event.
	QUaNodeId eventType() const;
	void      setEventType(const QUaNodeId& eventTypeNodeId);
	// Node that the Event originated from. If is not specific to a Node, then is set to null. 
	virtual QUaNodeId sourceNode() const;
	virtual void      setSourceNode(const QUaNodeId& sourceNodeId);
	// Description of the source of the Event.
	QString sourceName() const;
	void    setSourceName(const QString &strSourceName);
	// Helper that sets sourceNode and sourceName at once
	virtual void setSourceNode(const QUaNode* sourceNode);
	// Time (in UTC) the Event occurred. It comes from the underlying system or device.
	QDateTime time() const;
	void      setTime(const QDateTime &dateTime);
	// Time (in UTC) the OPC UA Server received the Event from the underlying device of another Server (ServerTimestamp).
	QDateTime receiveTime() const;
	void      setReceiveTime(const QDateTime& dateTime);

	// NOTE : optional; not created until one of these methods is called
	// TimeZone of the time the Event occurred.
	QTimeZone localTime() const;
	void      setLocalTime(const QTimeZone &localTimeZone);

	// Human-readable description of the Event.
	QUaLocalizedText message() const;
	void             setMessage(const QUaLocalizedText& message);
	// Urgency of the Event. Value from 1 to 1000, with 1 being the lowest severity and 1000 being the highest. 
	// A severity of 1 indicates an Event is informational, while 1000 would indicates catastrophic nature.
	virtual quint16 severity() const;
	virtual void setSeverity(const quint16 &intSeverity);

	// Custom Event API

	// Updates EventId and triggers the event and 
	void trigger();

#ifdef UA_ENABLE_HISTORIZING
	// Whether the Server is actively storing the history of the event (or alarm/condition)
	bool historizing() const;
	void setHistorizing(const bool& historizing);
#endif // UA_ENABLE_HISTORIZING

signals:
	void triggered();

protected:
#ifdef UA_ENABLE_HISTORIZING
	bool m_historizing;
#endif // UA_ENABLE_HISTORIZING
	// ByteString : 
	QUaProperty  * getEventId();
	// NodeId : 
	QUaProperty  * getEventType();
	// NodeId : 
	QUaProperty  * getSourceNode();
	// String : 
	QUaProperty  * getSourceName();
	// UtcTime :   
	QUaProperty  * getTime();
	// UtcTime : 
	QUaProperty  * getReceiveTime();
	// TimeZoneDataType : Offset and the DaylightSavingInOffset flag. 
	//                    Offset specifies the difference (in minutes) between the Time Property 
	//                    and the time at the location in which the event was issued.
	//                    If DaylightSavingInOffset is TRUE, then Daylight savings is in effect and Offset includes the correction.
	//                    If FALSE then the Offset does not include DST correction and DST may or may not have been in effect.
	QUaProperty  * getLocalTime();
	// LocalizedText : 
	QUaProperty  * getMessage();
	// UInt16 : 
	QUaProperty  * getSeverity();

	// Used to trigger the event
	UA_NodeId m_sourceNodeId;
	

	// helpers

	// Triggers the events (with last set EventId)
	void triggerInternal();
	// Overwrite if for some reason at application level, triggering should be disabled
	virtual bool shouldTrigger() const;

	static QByteArray    generateEventId();
	static UA_ByteString generateEventIdInternal();
	
};

#endif // UA_ENABLE_SUBSCRIPTIONS_EVENTS

#endif // QUABASEEVENT_H
