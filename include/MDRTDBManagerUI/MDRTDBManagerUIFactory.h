/*!
\author enli 2015.01.08
\section 类厂：创建实时数据库配置或运行对象
*/
#ifndef CMDRTDBMANAGERUIFACTORY_H
#define CMDRTDBMANAGERUIFACTORY_H


#include "mdrtdbmanagerui_global.h"
#include "IMDRTDBManagerUI.h"
#include "IMDRTDBRuntimeUI.h"

class MDRTDBMANAGERUISHARED_EXPORT CMDRTDBManagerUIFactory
{
public:
    CMDRTDBManagerUIFactory();
    ~CMDRTDBManagerUIFactory();
public:
    static IMDRTDBManagerUI *createMDRTDBManagerUI();
    static IMDRTDBRuntimeUI *createMDRTDBRuntimeUI();
};

#endif // CMDRTDBMANAGERUIFACTORY_H
