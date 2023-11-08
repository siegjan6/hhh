#include "MDAccountProperty_Runtime.h"
#include "MDAccountPropertyData.h"

CMDAccountProperty_Runtime::CMDAccountProperty_Runtime()
{
    m_data = new CMDAccountPropertyData_Runtime;
}

CMDAccountProperty_Runtime::~CMDAccountProperty_Runtime()
{
    if(m_data)
        delete m_data;
}

CMDAccountProperty_Runtime::CMDAccountProperty_Runtime(const CMDAccountProperty_Runtime& obj)
{
    if(m_data && obj.m_data)
        *m_data = *obj.m_data;
}

CMDAccountProperty_Runtime& CMDAccountProperty_Runtime::operator = (const CMDAccountProperty_Runtime& obj)
{
    if( this != &obj )
    {
        if(m_data && obj.m_data)
            *m_data = *obj.m_data;
    }
    return *this;
}

long CMDAccountProperty_Runtime::loginCount() const
{
    return (m_data ? m_data->m_loginCount : 0);
}
void CMDAccountProperty_Runtime::setLoginCount(long count)
{
    if(m_data)
        m_data->m_loginCount = count;
}

QDateTime CMDAccountProperty_Runtime::lastLoginTime() const
{
    return (m_data ? m_data->m_lastLoginTime : QDateTime());
}
void CMDAccountProperty_Runtime::setLastLoginTime(const QDateTime& dt)
{
    if(m_data)
        m_data->m_lastLoginTime = dt;
}
