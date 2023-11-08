#include "../../include/MDVariableExpert/MDVariableContacter.h"

CMDVariableContacter::CMDVariableContacter()
{
}

CMDVariableContacter::~CMDVariableContacter()
{

}

bool CMDVariableContacter::onVariableChanged(IMDChangeVariable* changedVariable)
{
    return false;
}

bool CMDVariableContacter::checkSP_WithUI(CMDActionSPSession* spSession)
{
    return false;
}

void CMDVariableContacter::onDeviceVariablesChange(const QList<QByteArray>& jsonMsgList)
{
}

void CMDVariableContacter::onDeviceVariablesBatchNotify(const QList<QByteArray>& redisCmdList)
{
}

void CMDVariableContacter::onVariablesBatchNotify(const QStringList& varNameList, const QList<QVariant>& varValueList)
{
}

