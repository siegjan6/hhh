#include "MDActionProperty_Runtime.h"
#include "MDActionPropertyData.h"

CMDActionProperty_Runtime::CMDActionProperty_Runtime()
{
    m_data = new CMDActionPropertyData_Runtime;
}

CMDActionProperty_Runtime::~CMDActionProperty_Runtime()
{
    if(m_data)
        delete m_data;
}

CMDActionProperty_Runtime::CMDActionProperty_Runtime(const CMDActionProperty_Runtime& obj)
{
    if(m_data && obj.m_data)
        *m_data = *obj.m_data;
}

CMDActionProperty_Runtime& CMDActionProperty_Runtime::operator = (const CMDActionProperty_Runtime& obj)
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
long CMDActionProperty_Runtime::exeCount() const
{
    return (m_data ? m_data->m_exeCount : 0);
}
void CMDActionProperty_Runtime::setExeCount(long exeCount)
{
    if(m_data)
        m_data->m_exeCount = exeCount;
}

QDateTime CMDActionProperty_Runtime::lastExeTime() const
{
    return (m_data ? m_data->m_lastExeTime : QDateTime());
}
void CMDActionProperty_Runtime::setLastExeTime(const QDateTime& lastExeTime)
{
    if(m_data)
        m_data->m_lastExeTime = lastExeTime;
}

CMDActionSP* CMDActionProperty_Runtime::enabledExeSP() const
{
    return (m_data ? m_data->m_enabledExeSP : NULL);
}
void CMDActionProperty_Runtime::setEnabledExeSP(CMDActionSP* sp)
{
    if(m_data)
        m_data->m_enabledExeSP = sp;
}
