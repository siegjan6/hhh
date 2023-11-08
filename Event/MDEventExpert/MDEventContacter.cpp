#include "MDEventContacter.h"

CMDEventContacter::CMDEventContacter()
{
}

CMDEventContacter::~CMDEventContacter()
{
}

CMDAction* CMDEventContacter::findAction(const QString& actionName)
{
    return NULL;
}

bool CMDEventContacter::executeAction(CMDEvent* sourceEvent, CMDAction* action)
{
    return false;
}

bool CMDEventContacter::log(CMDLogMessage* msg)
{
    return false;
}
