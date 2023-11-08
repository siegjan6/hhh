#include "MDLogMessage.h"
#include "MDLogMessageData.h"

CMDLogMessage::CMDLogMessage()
{
    m_data = new CMDLogMessageData();
}

CMDLogMessage::~CMDLogMessage()
{
    if(m_data)
        delete m_data;
}

CMDLogMessage::CMDLogMessage(const CMDLogMessage& obj)
{
    m_data = new CMDLogMessageData();

    copy(&obj);
}

CMDLogMessage& CMDLogMessage::operator = (const CMDLogMessage& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDLogMessage::copy(const CMDLogMessage* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

QString CMDLogMessage::msg() const
{
    return (m_data ? m_data->m_msg : "");
}
void CMDLogMessage::setMsg(const QString& msg)
{
    if(m_data)
        m_data->m_msg = msg;
}

QDateTime CMDLogMessage::dt() const
{
    return (m_data ? m_data->m_dt : QDateTime());
}
void CMDLogMessage::setDT(const QDateTime& dt)
{
    if(m_data)
        m_data->m_dt = dt;
}

MDLogType CMDLogMessage::type() const
{
    return (m_data ? m_data->m_type : MDLogType::info);
}
void CMDLogMessage::setType(MDLogType type)
{
    if(m_data)
        m_data->m_type = type;
}

QString CMDLogMessage::currentUser() const
{
    return (m_data ? m_data->m_currentUser : "");
}
void CMDLogMessage::setCurrentUser(const QString& currentUser)
{
    if(m_data)
        m_data->m_currentUser = currentUser;
}

QString CMDLogMessage::source() const
{
    return (m_data ? m_data->m_source : "");
}
void CMDLogMessage::setSource(const QString& source)
{
    if(m_data)
        m_data->m_source = source;
}

CMDObject* CMDLogMessage::object() const
{
    return (m_data ? &m_data->m_object : NULL);
}
void CMDLogMessage::setObject(const CMDObject* object)
{
    if(m_data)
        m_data->m_object = *object;
}

CMDActionSPSession* CMDLogMessage::spSession() const
{
    return (m_data ? &m_data->m_spSession : NULL);
}
void CMDLogMessage::setSpSession(const CMDActionSPSession* spSession)
{
    if(m_data)
        m_data->m_spSession = *spSession;
}
