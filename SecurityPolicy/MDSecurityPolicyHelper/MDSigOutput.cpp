#include "MDSigOutput.h"
#include "MDActionSPSessionData.h"

CMDSigOutput::CMDSigOutput()
{
    m_data = new CMDSigOutputData;
}

CMDSigOutput::~CMDSigOutput()
{
    if(m_data)
        delete m_data;
}

CMDSigOutput::CMDSigOutput(const CMDSigOutput& obj)
{
    m_data = new CMDSigOutputData();

    copy(&obj);
}

CMDSigOutput& CMDSigOutput::operator = (const CMDSigOutput& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

QDateTime CMDSigOutput::sigTime() const
{
    return (m_data ? m_data->m_sigTime : QDateTime());
}
void CMDSigOutput::setSigTime(const QDateTime& sigTime)
{
    if(m_data)
        m_data->m_sigTime = sigTime;
}

MDSecurityResultCode CMDSigOutput::resultCode() const
{
    return (m_data ? m_data->m_resultCode : MDSecurityResultCode::no_result);
}
void CMDSigOutput::setResultCode(MDSecurityResultCode resultCode)
{
    if(m_data)
        m_data->m_resultCode = resultCode;
}

QString CMDSigOutput::resultMsg() const
{
    return (m_data ? m_data->m_resultMsg : "");
}
void CMDSigOutput::setResultMsg(const QString& resultMsg)
{
    if(m_data)
        m_data->m_resultMsg = resultMsg;
}

bool CMDSigOutput::isSuccess() const
{
    return (m_data ? m_data->m_result : false);
}
void CMDSigOutput::setSuccess(bool result)
{
    if(m_data)
        m_data->m_result = result;
}

bool CMDSigOutput::copy(const CMDSigOutput* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}
