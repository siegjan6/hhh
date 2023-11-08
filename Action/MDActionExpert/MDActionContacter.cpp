#include "MDActionContacter.h"
#include "MDAction.h"
#include "MDRTDBInterfaces/IMDVariable.h"

CMDActionContacter::CMDActionContacter()
{
}

CMDActionContacter::~CMDActionContacter()
{
}

CMDProjectModuleExpert* CMDActionContacter::findActionExecuter(CMDAction* action)
{
    return NULL;
}

bool CMDActionContacter::executeAction(CMDAction* action)
{
    return false;
}

bool CMDActionContacter::checkSP_WithUI(CMDActionSPSession* spSession)
{
    return false;
}

bool CMDActionContacter::log(CMDLogMessage* msg)
{
    return false;
}

bool CMDActionContacter::variable(const QString& varName, IMDVariable* var)
{
    return false;
}

QVariant CMDActionContacter::variableValue(const QString& varName)
{
    return QVariant();
}
