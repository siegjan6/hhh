#include "MDAlarmContacter.h"

CMDAlarmContacter::CMDAlarmContacter()
{
}

CMDAlarmContacter::~CMDAlarmContacter()
{

}

bool CMDAlarmContacter::variable(const QString& varName, IMDVariable* var)
{
    return false;
}

IMDRTDBVariable* CMDAlarmContacter::getVariable(const QString& varName)
{
    return NULL;
}

QVariant CMDAlarmContacter::variableValue(const QString& varName)
{
    return QVariant();
}

QString CMDAlarmContacter::currentAccount()
{
    return "";
}

bool CMDAlarmContacter::checkSP_WithUI(CMDActionSPSession* spSession)
{
    return false;
}

bool CMDAlarmContacter::handleAlarm(short type, const QString& name, const QString& msg, const QDateTime& startTime, const QDateTime& endTime)
{
    return false;
}
