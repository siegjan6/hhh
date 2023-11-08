/*!

\author dGFuZ3p3

\section 功能

帐户运行时属性数据类。

*/

#ifndef MDACCOUNTPROPERTY_RUNTIME_H
#define MDACCOUNTPROPERTY_RUNTIME_H

#include <QString>
#include <QDateTime>
#include "MDSecurityPolicyHelper_Global.h"

class CMDAccountPropertyData_Runtime;
class MDSECURITYPOLICYHELPERSHARED_EXPORT CMDAccountProperty_Runtime
{
public:
    CMDAccountProperty_Runtime();
    virtual ~CMDAccountProperty_Runtime();
   CMDAccountProperty_Runtime(const CMDAccountProperty_Runtime& obj);
   CMDAccountProperty_Runtime& operator = (const CMDAccountProperty_Runtime& obj);
public:
   long loginCount() const;
   void setLoginCount(long count);

   QDateTime lastLoginTime() const;
   void setLastLoginTime(const QDateTime& dt);
protected:
    CMDAccountPropertyData_Runtime* m_data;
};

#endif // MDACCOUNTPROPERTY_RUNTIME_H
