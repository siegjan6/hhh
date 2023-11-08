#include "MDSigInput.h"
#include "MDActionSPSessionData.h"

CMDSigInput::CMDSigInput()
{
    m_data = new CMDSigInputData;
}

CMDSigInput::~CMDSigInput()
{
    if(m_data)
        delete m_data;
}

CMDSigInput::CMDSigInput(const CMDSigInput& obj)
{
    m_data = new CMDSigInputData();

    copy(&obj);
}

CMDSigInput& CMDSigInput::operator = (const CMDSigInput& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

QString CMDSigInput::sigID() const
{
    return (m_data ? m_data->m_sigID : "");
}
void CMDSigInput::setSigID(const QString& sigID)
{
    if(m_data)
        m_data->m_sigID = sigID;
}

QString CMDSigInput::sigPassword() const
{
    return (m_data ? m_data->m_sigPassword : "");
}
void CMDSigInput::setSigPassword(const QString& sigPassword)
{
    if(m_data)
        m_data->m_sigPassword = sigPassword;
}

QString CMDSigInput::sigComment() const
{
    return (m_data ? m_data->m_sigComment : "");
}
void CMDSigInput::setSigComment(const QString& sigComment)
{
    if(m_data)
        m_data->m_sigComment = sigComment;
}

bool CMDSigInput::isNoPasswordNeeded() const
{
    return (m_data ? m_data->m_noPasswordNeeded : false);
}
void CMDSigInput::setNoPasswordNeeded(bool noPassword)
{
    if(m_data)
        m_data->m_noPasswordNeeded = noPassword;
}

bool CMDSigInput::copy(const CMDSigInput* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}
