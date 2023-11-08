#ifndef MDACTIOMDPSESSIONDATA_H
#define MDACTIOMDPSESSIONDATA_H

#include "MDActionSP.h"
#include "MDSigInput.h"
#include "MDSigOutput.h"

class CMDActionSPSessionData
{
public:
    CMDActionSPSessionData();
    CMDActionSPSessionData(const CMDActionSP& actionSP);
    virtual ~CMDActionSPSessionData();
   CMDActionSPSessionData(const CMDActionSPSessionData& obj);
   CMDActionSPSessionData& operator = (const CMDActionSPSessionData& obj);
public:
    bool copy(const CMDActionSPSessionData* source);
public:
    CMDActionSP m_actionSP;
public:
    CMDSigInput m_operatorInput;
    CMDSigOutput m_operatorOutput;
public:
    CMDSigInput m_verifierInput;
    CMDSigOutput m_verifierOutput;
};

class CMDSigInputData
{
public:
    CMDSigInputData();
    virtual ~CMDSigInputData();
   CMDSigInputData(const CMDSigInputData& obj);
   CMDSigInputData& operator = (const CMDSigInputData& obj);
public:
    bool copy(const CMDSigInputData* source);
public:
   QString m_sigID;
   QString m_sigPassword;
   QString m_sigComment;
public:
   bool m_noPasswordNeeded;
};

class CMDSigOutputData
{
public:
    CMDSigOutputData();
    virtual ~CMDSigOutputData();
   CMDSigOutputData(const CMDSigOutputData& obj);
   CMDSigOutputData& operator = (const CMDSigOutputData& obj);
public:
    bool copy(const CMDSigOutputData* source);
public:
    QDateTime m_sigTime;
    bool m_result;
    MDSecurityResultCode m_resultCode;
    QString m_resultMsg;
};

#endif // MDACTIOMDPSESSIONDATA_H
