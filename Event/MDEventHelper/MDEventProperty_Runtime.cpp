#include "MDEventProperty_Runtime.h"
#include "MDEventPropertyData.h"

CMDEventProperty_Runtime::CMDEventProperty_Runtime()
{
    m_data = new CMDEventPropertyData_Runtime;
}

CMDEventProperty_Runtime::~CMDEventProperty_Runtime()
{
    if(m_data)
        delete m_data;
}

CMDEventProperty_Runtime::CMDEventProperty_Runtime(const CMDEventProperty_Runtime& obj)
{
    if(m_data && obj.m_data)
        *m_data = *obj.m_data;
}

CMDEventProperty_Runtime& CMDEventProperty_Runtime::operator = (const CMDEventProperty_Runtime& obj)
{
    if( this != &obj )
    {
        if(m_data && obj.m_data)
            *m_data = *obj.m_data;
    }
    return *this;
}

/*!
\section 属性
*/
bool CMDEventProperty_Runtime::expFlag() const
{
    return (m_data ? m_data->m_expFlag : false);
}
void CMDEventProperty_Runtime::setExpFlag(bool flag)
{
    if(m_data)
        m_data->m_expFlag = flag;
}

bool CMDEventProperty_Runtime::autoResetExpFlag() const
{
    return (m_data ? m_data->m_autoResetExpFlag : false);
}
void CMDEventProperty_Runtime::setAutoResetExpFlag(bool flag)
{
    if(m_data)
        m_data->m_autoResetExpFlag = flag;
}

long CMDEventProperty_Runtime::triggerCount() const
{
    return (m_data ? m_data->m_triggerCount : 0);
}
void CMDEventProperty_Runtime::setTriggerCount(long count)
{
    if(m_data)
        m_data->m_triggerCount = count;
}

QDateTime CMDEventProperty_Runtime::lastTriggerTime() const
{
    return (m_data ? m_data->m_lastTriggerTime : QDateTime());
}
void CMDEventProperty_Runtime::setLastTriggerTime(const QDateTime& lastTriggerTime)
{
    if(m_data)
        m_data->m_lastTriggerTime = lastTriggerTime;
}
