#include "MDActionSPSessionData.h"

CMDActionSPSessionData::CMDActionSPSessionData()
{
}

CMDActionSPSessionData::CMDActionSPSessionData(const CMDActionSP& actionSP)
{
   m_actionSP = actionSP;
}

CMDActionSPSessionData::~CMDActionSPSessionData()
{

}

CMDActionSPSessionData::CMDActionSPSessionData(const CMDActionSPSessionData& obj)
{
    copy(&obj);
}

CMDActionSPSessionData& CMDActionSPSessionData::operator = (const CMDActionSPSessionData& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDActionSPSessionData::copy(const CMDActionSPSessionData* source)
{
    m_actionSP = source->m_actionSP;

    m_operatorInput = source->m_operatorInput;
    m_verifierInput = source->m_verifierInput;

    m_operatorOutput = source->m_operatorOutput;
    m_verifierOutput = source->m_verifierOutput;

    return true;
}

// 签名输入
CMDSigInputData::CMDSigInputData()
{
    m_noPasswordNeeded = false;
}

CMDSigInputData::~CMDSigInputData()
{

}

CMDSigInputData::CMDSigInputData(const CMDSigInputData& obj)
{
    copy(&obj);
}

CMDSigInputData& CMDSigInputData::operator = (const CMDSigInputData& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDSigInputData::copy(const CMDSigInputData* source)
{
    m_sigID = source->m_sigID;
    m_sigPassword = source->m_sigPassword;
    m_sigComment = source->m_sigComment;

    m_noPasswordNeeded = source->m_noPasswordNeeded;

    return true;
}

// 签名输出
CMDSigOutputData::CMDSigOutputData()
{
    m_result = false;
    m_resultCode = MDSecurityResultCode::no_result;
    m_resultMsg = "";
}

CMDSigOutputData::~CMDSigOutputData()
{

}

CMDSigOutputData::CMDSigOutputData(const CMDSigOutputData& obj)
{
    copy(&obj);
}

CMDSigOutputData& CMDSigOutputData::operator = (const CMDSigOutputData& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDSigOutputData::copy(const CMDSigOutputData* source)
{
    m_sigTime = source->m_sigTime;
    m_result = source->m_result;

    m_resultCode = source->m_resultCode;
    m_resultMsg = source->m_resultMsg;

    return true;
}

