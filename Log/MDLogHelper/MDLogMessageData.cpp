#include "MDLogMessageData.h"

CMDLogMessageData::CMDLogMessageData()
{
    m_dt = QDateTime::currentDateTime();
    m_type = MDLogType::info;
}

CMDLogMessageData::~CMDLogMessageData()
{

}

CMDLogMessageData::CMDLogMessageData(const CMDLogMessageData& obj)
{
    copy(&obj);
}

CMDLogMessageData& CMDLogMessageData::operator = (const CMDLogMessageData& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDLogMessageData::copy(const CMDLogMessageData* source)
{
    m_msg = source->m_msg;
    m_dt = source->m_dt;
    m_type = source->m_type;
    m_source = source->m_source;
    m_currentUser = source->m_currentUser;

    m_object = source->m_object;
    m_spSession = source->m_spSession;

    return true;
}
