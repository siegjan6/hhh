#include "MDRTDBManagerUIFactory.h"
#include "MDRTDBManagerUI.h"
#include "MDRTDBRuntimeUI.h"
CMDRTDBManagerUIFactory::CMDRTDBManagerUIFactory()
{

}

CMDRTDBManagerUIFactory::~CMDRTDBManagerUIFactory()
{

}
//!配置管理类
IMDRTDBManagerUI *CMDRTDBManagerUIFactory::createMDRTDBManagerUI()
{
    CMDRTDBManagerUI *pMDRTDBManagerUI=new CMDRTDBManagerUI();
    return (IMDRTDBManagerUI*)pMDRTDBManagerUI;
}
//!运行管理类
IMDRTDBRuntimeUI *CMDRTDBManagerUIFactory::createMDRTDBRuntimeUI()
{
    CMDRTDBRuntimeUI *pMDRTDBRuntimeUI=new CMDRTDBRuntimeUI();
    return (IMDRTDBRuntimeUI*)pMDRTDBRuntimeUI;
}
