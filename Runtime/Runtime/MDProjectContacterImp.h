/*!

\author dGFuZ3p3

\section 功能

实现了项目专家联系人类。

*/

#ifndef MDPROJECTCONTACTERIMP_H
#define MDPROJECTCONTACTERIMP_H

#include "../../include/MDProjectExpert/MDProjectExpert.h"
#include "../../include/MDProjectExpert/MDProjectContacter.h"

class MainWindow;
class CMDProjectContacterImp : public CMDProjectContacter
{
public:
    CMDProjectContacterImp();
    ~CMDProjectContacterImp();

    MainWindow* m_mainWnd ;// NULL;
public:
    bool onVariableChanged(IMDChangeVariable* changedVariable);
    qint8 onExecuteHMIAction(CMDAction* action);
public:
    bool loginByUI();
    bool checkSP_WithUI(CMDActionSPSession* spSession);
public:
    void onLoadProgress(const QString& msg, bool finished, int finishCode);
public:
    void enterFullScreen();
    void exitFullScreen(bool checkSP);
public:
    void showAccountAndPowerGroupManager();
public:
    void onDeviceDataReceived();
    void onDeviceCloudStatusChanged(const QString& statusMsg);
public:
    void showRecipeRuntime();
    void hideRecipeRuntime();
    void showRuntime();
    void hideRuntime();
    bool isVirtualRunMode();
    void setVirtualRunMode(bool isVirtualRunMode);

private:
    bool m_isVirtualRunMode;
};

#endif // MDPROJECTCONTACTERIMP_H
