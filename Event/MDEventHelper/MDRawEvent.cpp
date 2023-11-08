#include "MDRawEvent.h"
#include "MDEventHelperImp.h"

/*!
 * \brief 原始事件数据类
 */
CMDRawEvent::CMDRawEvent()
{
    m_data = new CMDRawEventData();
}

CMDRawEvent::CMDRawEvent(const QString& rawObjName, MDEventType eventType)
{
    m_data = new CMDRawEventData(rawObjName, eventType);
}

CMDRawEvent::~CMDRawEvent()
{
    if(m_data)
        delete m_data;
}

CMDRawEvent::CMDRawEvent(const CMDRawEvent& obj)
{
    if(obj.m_data)
        m_data = new CMDRawEventData(*obj.m_data);
}

CMDRawEvent& CMDRawEvent::operator = (const CMDRawEvent& obj)
{
    if( this != &obj )
    {
        if(m_data && obj.m_data)
            *m_data = *obj.m_data;
    }
    return *this;
}

QString CMDRawEvent::rawObjName() const
{
    return (m_data ? m_data->m_rawObjName : "");
}
void CMDRawEvent::setRawObjName(const QString& name)
{
    if(m_data)
        m_data->m_rawObjName = name;
}

MDEventType CMDRawEvent::eventType() const
{
    return (m_data ? m_data->m_eventType : MDEventType::null_event);
}
void CMDRawEvent::setEventType(const MDEventType& eventType)
{
    if(m_data)
        m_data->m_eventType = eventType;
}

QDateTime CMDRawEvent::timeStamp() const
{
    return (m_data ? m_data->m_timeStamp : QDateTime());
}
void CMDRawEvent::setTimeStamp(const QDateTime& dateTime)
{
    if(m_data)
        m_data->m_timeStamp = dateTime;
}

QString CMDRawEvent::comment() const
{
    return (m_data ? m_data->m_comment : "");
}

void CMDRawEvent::setComment(const QString& comment)
{
    if(m_data)
        m_data->m_comment = comment;
}

CMDLogSwitch* CMDRawEvent::logSwitch() const
{
    return m_data->m_logSwitch;
}
void CMDRawEvent::setLogSwitch(CMDLogSwitch* logSwitch)
{
    m_data->m_logSwitch = logSwitch;
}

/*!
 * \brief 原始变量事件数据类
 */
CMDRawVariableEvent::CMDRawVariableEvent()
{
    m_data1 = new CMDRawVariableEventData;
}

CMDRawVariableEvent::CMDRawVariableEvent(MDEventType eventType, IMDChangeVariable* changedVariable)
    : CMDRawEvent(changedVariable->name(), eventType)
{
    m_data1 = new CMDRawVariableEventData(changedVariable);
}

CMDRawVariableEvent::~CMDRawVariableEvent()
{
    if(m_data1)
        delete m_data1;
}

CMDRawVariableEvent::CMDRawVariableEvent(const CMDRawVariableEvent& obj)
    : CMDRawEvent(obj.rawObjName(), obj.eventType())
{
    if(m_data1 && obj.m_data1)
        *m_data1 = *obj.m_data1;
}

CMDRawVariableEvent& CMDRawVariableEvent::operator = (const CMDRawVariableEvent& obj)
{
    if( this != &obj )
    {
        *((CMDRawEvent*)this) = *((CMDRawEvent*)&obj);

        if(m_data1 && obj.m_data1)
            *m_data1 = *obj.m_data1;
    }
    return *this;
}

IMDChangeVariable* CMDRawVariableEvent::changedVariable() const
{
    return (m_data1 ? m_data1->m_changedVariable : NULL);
}
void CMDRawVariableEvent::setChangedVariable(IMDChangeVariable* changedVariable)
{
    if(m_data1)
    {
        m_data1->m_changedVariable->copy(changedVariable);
    }
//        *m_data1->m_changedVariable = *changedVariable;
}
