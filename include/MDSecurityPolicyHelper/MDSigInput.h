/*!

\author dGFuZ3p3

\section 功能

签名输入信息类。

*/

#ifndef MDSIGINPUT_H
#define MDSIGINPUT_H

#include <QString>
#include "MDSecurityPolicyHelper_Global.h"

class CMDSigInputData;
class MDSECURITYPOLICYHELPERSHARED_EXPORT CMDSigInput
{
public:
    CMDSigInput();
    virtual ~CMDSigInput();
   CMDSigInput(const CMDSigInput& obj);
   CMDSigInput& operator = (const CMDSigInput& obj);
public:
    QString sigID() const;
    void setSigID(const QString& sigID);

    QString sigPassword() const;
    void setSigPassword(const QString& sigPassword);

    QString sigComment() const;
    void setSigComment(const QString& sigComment);
public:
    bool isNoPasswordNeeded() const;
    void setNoPasswordNeeded(bool noPassword);
public:
    virtual bool copy(const CMDSigInput* source);
protected:
    CMDSigInputData* m_data;
};

#endif // MDSIGINPUT_H
