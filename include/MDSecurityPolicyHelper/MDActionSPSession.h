/*!

\author dGFuZ3p3

\section 功能

安全策略会话类，包含了安全验证时的输入输出信息。
输入：用户名、密码等；
输出：针对输入的验证结果。

*/

#ifndef MDACTIONSPSESSION_H
#define MDACTIONSPSESSION_H

#include "MDActionSP.h"
#include "MDSigInput.h"
#include "MDSigOutput.h"
#include "MDSecurityPolicyHelper_Global.h"

class CMDActionSPSessionData;
class MDSECURITYPOLICYHELPERSHARED_EXPORT CMDActionSPSession
{
public:
    CMDActionSPSession();
    CMDActionSPSession(const CMDActionSP& actionSP);
    virtual ~CMDActionSPSession();
   CMDActionSPSession(const CMDActionSPSession& obj);
   CMDActionSPSession& operator = (const CMDActionSPSession& obj);
public:
   //! 发起会话的安全策略
    CMDActionSP& actionSP() const;
    void setActionSP(const CMDActionSP& actionSP);
public:
    //! 操作者输入
    CMDSigInput& operatorInput() const;
    void setOperatorInput(const CMDSigInput& input);

    //! 操作者输出
    CMDSigOutput& operatorOutput() const;
    void setOperatorOutput(const CMDSigOutput& output);
public:
    //! 校验者输入
    CMDSigInput& verifierInput() const;
    void setVerifierInput(const CMDSigInput& input);

    //! 校验者输出
    CMDSigOutput& verifierOutput() const;
    void setVerifierOutput(const CMDSigOutput& output);
public:
    virtual bool copy(const CMDActionSPSession* source);
protected:
    CMDActionSPSessionData* m_data;
};

#endif // MDACTIONSPSESSION_H
