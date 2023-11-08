/*!

\author dGFuZ3p3

\section 功能

签名输出信息类。

*/

#ifndef MDSIGOUTPUT_H
#define MDSIGOUTPUT_H

#include <QString>
#include <QDateTime>
#include "MDSecurityPolicyHelper_Global.h"
#include "MDSecurityPolicyConstant.h"

class CMDSigOutputData;
class MDSECURITYPOLICYHELPERSHARED_EXPORT CMDSigOutput
{
public:
    CMDSigOutput();
    virtual ~CMDSigOutput();
   CMDSigOutput(const CMDSigOutput& obj);
   CMDSigOutput& operator = (const CMDSigOutput& obj);
public:
   QDateTime sigTime() const;
   void setSigTime(const QDateTime& sigTime);
public:
    bool isSuccess() const;
    void setSuccess(bool result);

    MDSecurityResultCode resultCode() const;
    void setResultCode(MDSecurityResultCode resultCode);

    QString resultMsg() const;
    void setResultMsg(const QString& resultMsg);
public:
    virtual bool copy(const CMDSigOutput* source);
protected:
    CMDSigOutputData* m_data;
};

#endif // MDSIGOUTPUT_H
