#include "MDActionSPSession.h"
#include "MDActionSPSessionData.h"

CMDActionSP DEFAULT_ACTION_SP;
CMDSigInput DEFAULT_SIG_INPUT;
CMDSigOutput DEFAULT_SIG_OUTPUT;

CMDActionSPSession::CMDActionSPSession()
{
    m_data = new CMDActionSPSessionData;
}

CMDActionSPSession::CMDActionSPSession(const CMDActionSP& actionSP)
{
    m_data = new CMDActionSPSessionData(actionSP);
}

CMDActionSPSession::~CMDActionSPSession()
{
    if(m_data)
        delete m_data;
}

CMDActionSPSession::CMDActionSPSession(const CMDActionSPSession& obj)
{
    m_data = new CMDActionSPSessionData();

    copy(&obj);
}

CMDActionSPSession& CMDActionSPSession::operator = (const CMDActionSPSession& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

CMDActionSP& CMDActionSPSession::actionSP() const
{
    return (m_data ? m_data->m_actionSP : DEFAULT_ACTION_SP);
}
void CMDActionSPSession::setActionSP(const CMDActionSP& actionSP)
{
    if(m_data)
        m_data->m_actionSP = actionSP;
}

// 操作签名
CMDSigInput& CMDActionSPSession::operatorInput() const
{
    return (m_data ? m_data->m_operatorInput : DEFAULT_SIG_INPUT);
}
void CMDActionSPSession::setOperatorInput(const CMDSigInput& input)
{
    if(m_data)
        m_data->m_operatorInput = input;
}

CMDSigOutput& CMDActionSPSession::operatorOutput() const
{
    return (m_data ? m_data->m_operatorOutput : DEFAULT_SIG_OUTPUT);
}
void CMDActionSPSession::setOperatorOutput(const CMDSigOutput& output)
{
    if(m_data)
        m_data->m_operatorOutput = output;
}

// 校验签名
CMDSigInput& CMDActionSPSession::verifierInput() const
{
    return (m_data ? m_data->m_verifierInput : DEFAULT_SIG_INPUT);
}
void CMDActionSPSession::setVerifierInput(const CMDSigInput& input)
{
    if(m_data)
        m_data->m_verifierInput = input;
}

CMDSigOutput& CMDActionSPSession::verifierOutput() const
{
    return (m_data ? m_data->m_verifierOutput : DEFAULT_SIG_OUTPUT);
}
void CMDActionSPSession::setVerifierOutput(const CMDSigOutput& output)
{
    if(m_data)
        m_data->m_verifierOutput = output;
}

bool CMDActionSPSession::copy(const CMDActionSPSession* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

