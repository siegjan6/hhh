#include "MDLogContacter.h"

CMDLogContacter::CMDLogContacter()
{
}

CMDLogContacter::~CMDLogContacter()
{

}

bool CMDLogContacter::variable(const QString& varName, IMDVariable* var)
{
    return false;
}

QVariant CMDLogContacter::variableValue(const QString& varName)
{
    return QVariant();
}

QString CMDLogContacter::currentAccount()
{
    return "";
}
