#include "MDSecurityPolicyContacter.h"

CMDSecurityPolicyContacter::CMDSecurityPolicyContacter()
{
}

CMDSecurityPolicyContacter::~CMDSecurityPolicyContacter()
{

}

void CMDSecurityPolicyContacter::onLogin(const QString& name, const int& resultCode, const QString& resultMsg)
{

}

void CMDSecurityPolicyContacter::onLogout(const QString& name)
{

}

bool CMDSecurityPolicyContacter::log(CMDLogMessage* msg)
{
    return false;
}
