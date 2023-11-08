#include "MDProjectContacterImp.h"
#include "mainwindow.h"
#include "MDProjectConfiger.h"
#include "MDSecurityPolicyConfiger.h"
#include "MDSecurityPolicyUI.h"

CMDProjectContacterImp::CMDProjectContacterImp()
{
    m_mainWnd = NULL;
    m_isVirtualRunMode = false;
}

CMDProjectContacterImp::~CMDProjectContacterImp()
{

}

bool CMDProjectContacterImp::onVariableChanged(IMDChangeVariable* changedVariable)
{
    if(!m_mainWnd)
        return false;

    return  m_mainWnd->onVariableChanged(changedVariable);
}

qint8 CMDProjectContacterImp::onExecuteHMIAction(CMDAction* action)
{
    if(!m_mainWnd)
        return -1;

    return  m_mainWnd->onExecuteHMIAction(action);
}

bool CMDProjectContacterImp::loginByUI()
{
    if(!m_mainWnd)
        return false;

    CMDSecurityPolicyUI securityUI;
    securityUI.setSecurityPolicyExpert(m_mainWnd->m_projectExpert.securityExpert());
    return securityUI.login();
}

bool CMDProjectContacterImp::checkSP_WithUI(CMDActionSPSession* spSession)
{
    if(!m_mainWnd)
        return false;

    if(!m_mainWnd->m_projectExpert.projectConfiger()->securityConfiger()->para()->isEnableSP())
        return true;

    //if(m_mainWnd->m_projectExpert.securityExpert()->isCurrentAccountHasPower(spSession))
    //    return true;

    CMDSecurityPolicyUI securityUI;
    securityUI.setSecurityPolicyExpert(m_mainWnd->m_projectExpert.securityExpert());
    return securityUI.checkSP(spSession);
}

void CMDProjectContacterImp::onLoadProgress(const QString& msg, bool finished, int finishCode)
{
    if(m_mainWnd)
    {
        m_mainWnd->showLoadProgress(msg, finished, finishCode);
    }
}

void CMDProjectContacterImp::enterFullScreen()
{
    if(m_mainWnd)
    {
        m_mainWnd->enterFS();
    }
}

void CMDProjectContacterImp::exitFullScreen(bool checkSP)
{
    if(m_mainWnd)
    {
        if(checkSP)
            m_mainWnd->exitFSWithSecurityCheck();
        else m_mainWnd->exitFS();
    }
}

void CMDProjectContacterImp::showAccountAndPowerGroupManager()
{
    if(m_mainWnd)
    {
        m_mainWnd->showSPMgr();
    }
}

void CMDProjectContacterImp::onDeviceDataReceived()
{
    if(m_mainWnd)
    {
        m_mainWnd->closeRTDBWindow();
    }
}

void CMDProjectContacterImp::onDeviceCloudStatusChanged(const QString& statusMsg)
{
    if(m_mainWnd)
    {
        m_mainWnd->onDeviceCloudStatusChanged(statusMsg);
    }
}

void CMDProjectContacterImp::showRecipeRuntime()
{
    if(m_mainWnd)
    {
        m_mainWnd->showRecipeRuntime();
    }
}

void CMDProjectContacterImp::hideRecipeRuntime()
{
    if(m_mainWnd)
    {
        m_mainWnd->hideRecipeRuntime();
    }
}

void CMDProjectContacterImp::showRuntime()
{
    if(m_mainWnd)
    {
        m_mainWnd->showRuntime();
    }
}
void CMDProjectContacterImp::hideRuntime()
{
    if(m_mainWnd)
    {
        m_mainWnd->hideRuntime();
    }
}

bool CMDProjectContacterImp::isVirtualRunMode()
{
    return m_isVirtualRunMode;
}
void CMDProjectContacterImp::setVirtualRunMode(bool isVirtualRunMode)
{
    m_isVirtualRunMode = isVirtualRunMode;
}
