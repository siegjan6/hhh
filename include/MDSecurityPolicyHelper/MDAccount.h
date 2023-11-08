/*!

\author dGFuZ3p3

\section 功能

帐户数据类，包含配置时属性 CMDAccountProperty_Config 和
                        运行时属性 CMDAccountProperty_Runtime 。

*/

#ifndef MDACCOUNT_H
#define MDACCOUNT_H

#include "MDSecurityPolicyHelper_Global.h"
#include "MDAccountProperty_Config.h"
#include "MDAccountProperty_Runtime.h"

class CMDAccountProperty_Config;
class CMDAccountProperty_Runtime;
class CMDAccountData;
class MDSECURITYPOLICYHELPERSHARED_EXPORT CMDAccount
{
public:
    CMDAccount();
    ~CMDAccount();
public:
    CMDAccountProperty_Config* configProperty() const;
    CMDAccountProperty_Runtime* runtimeProperty() const;
protected:
    CMDAccountData* m_data;
};

#endif // MDACCOUNT_H
