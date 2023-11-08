/*!

\author dGFuZ3p3

\section 功能

脚本包装类的管理类，用于简化 CMDProjectExpertImp 的实现代码

*/

#ifndef MDPROJECTWRAPPERSCOLLECTER_H
#define MDPROJECTWRAPPERSCOLLECTER_H

#include "../../include/MDRTDBInterfaces/IMDChangeVariable.h"

#include "../../include/MDScriptEngine/MDObjectWrapper.h"

class CMDProjectExpertImp;
class CMDProjectWrappersCollecter
{
public:
    CMDProjectWrappersCollecter();
/*!
\section 初始化与释放；wrapper 管理
*/
public:
    void create(CMDProjectExpertImp* projectExpertImp);
    void destroy();
    CMDObjectWrapperMgr m_wrappersMgr;
protected:
    CMDProjectExpertImp* m_projectExpertImp;

/*!
\section 事件触发
*/
public:
    void fireVariableEvent(IMDChangeVariable* changedVariable);
public:
    void fireProjectEvent_Loaded();
    void fireProjectEvent_Unloading();
public:
    void fireQueryEvent_Finished(const QString& queryID, int result);
public:
    void fireRcpEvent_BeginDownload(const QString &name, int total);
    void fireRcpEvent_EndDownload(const QString &name, int finishFlag);
    void fireRcpEvent_DownloadProcess(const QString &msg, int total, int completed);
public:
    void fireSecurityEvent_OnLogin(const QString& name, const int& resultCode, const QString& resultMsg);
    void fireSecurityEvent_OnLogout(const QString& name);
};

#endif // MDPROJECTWRAPPERSCOLLECTER_H
