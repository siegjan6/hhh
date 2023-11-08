#include "../../include/MDProjectExpert/MDProjectContacter.h"

CMDProjectContacter::CMDProjectContacter()
{
}

CMDProjectContacter::~CMDProjectContacter()
{

}

bool CMDProjectContacter::onVariableChanged(IMDChangeVariable* changedVariable)
{
    return false;
}

qint8 CMDProjectContacter::onExecuteHMIAction(CMDAction* action)
{
    return 0;
}

bool CMDProjectContacter::loginByUI()
{
    return false;
}

bool CMDProjectContacter::checkSP_WithUI(CMDActionSPSession* spSession)
{
    return false;
}

void CMDProjectContacter::onLoadProgress(const QString& msg, bool finished, int finishCode)
{

}

void CMDProjectContacter::enterFullScreen()
{

}

void CMDProjectContacter::exitFullScreen(bool checkSP)
{

}

void CMDProjectContacter::showAccountAndPowerGroupManager()
{

}

void CMDProjectContacter::onDeviceDataReceived()
{

}

void CMDProjectContacter::onDeviceCloudStatusChanged(const QString& statusMsg)
{

}

void CMDProjectContacter::showRecipeRuntime()
{

}

void CMDProjectContacter::hideRecipeRuntime()
{

}

void CMDProjectContacter::showRuntime()
{

}
void CMDProjectContacter::hideRuntime()
{

}

bool CMDProjectContacter::isVirtualRunMode()
{
    return false;
}

void CMDProjectContacter::setVirtualRunMode(bool isVirtualRunMode)
{

}
