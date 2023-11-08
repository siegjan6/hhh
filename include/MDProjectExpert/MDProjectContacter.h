/*!

\author dGFuZ3p3

\section 功能

模块与外部联系的类，目的是降低模块与外部的耦合关系。

\section 用法

模块的使用者派生一个类，并将派生类的实例设置给模块，参考 CMDProjectExpert::load 。
模块的使用者必须实现所有的重载接口。

由于项目专家模块是后台模块，并不涉及任何界面，一旦需要与 HMI 或者其他界面库
交互，则必须通过此类实现。

*/

#ifndef MDPROJECTCONTACTER_H
#define MDPROJECTCONTACTER_H

#include <QString>
#include "MDProjectExpert_Global.h"
#include "../../include/MDRTDBInterfaces/IMDChangeVariable.h"

class CMDAction;
class CMDActionSPSession;
class MDPROJECTEXPERTSHARED_EXPORT CMDProjectContacter
{
public:
    CMDProjectContacter();
    virtual ~CMDProjectContacter();
public:
    virtual bool onVariableChanged(IMDChangeVariable* changedVariable);
    virtual qint8 onExecuteHMIAction(CMDAction* action);
public:
    virtual bool loginByUI();
    virtual bool checkSP_WithUI(CMDActionSPSession* spSession);
public:
    virtual void onLoadProgress(const QString& msg, bool finished, int finishCode);
public:
    virtual void enterFullScreen();
    virtual void exitFullScreen(bool checkSP);
public:
    virtual void showAccountAndPowerGroupManager();
public:
    virtual void onDeviceDataReceived();
    virtual void onDeviceCloudStatusChanged(const QString& statusMsg); 
public:
    virtual void showRecipeRuntime();
    virtual void hideRecipeRuntime();
    virtual void showRuntime();
    virtual void hideRuntime();
    virtual bool isVirtualRunMode();
    virtual void setVirtualRunMode(bool isVirtualRunMode);
};

#endif // MDPROJECTCONTACTER_H
