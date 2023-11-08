/*!

\author bosmutus

\section 功能

Ua包装类的管理类，用于简化 CMDProjectExpertImp 的实现代码

*/

#ifndef MDPROJECTWRAPPERSUACOLLECTER_H
#define MDPROJECTWRAPPERSUACOLLECTER_H

#include "../../include/MDRTDBInterfaces/IMDChangeVariable.h"

#include "../../include/MDScriptEngine/MDObjectWrapper.h"

class CMDProjectExpertImp;
class MDRecipeEvent;
class CMDProjectWrappersUaCollecter
{
public:
    CMDProjectWrappersUaCollecter();
/*!
\section 初始化与释放；wrapper 管理
*/
public:
    void create(CMDProjectExpertImp* projectExpertImp);
    void destroy();
    //CMDObjectWrapperMgr m_wrappersMgr;
protected:
    CMDProjectExpertImp* m_projectExpertImp;

///*!
//\section 事件触发
//*/
//public:
//    void fireVariableEvent(IMDChangeVariable* changedVariable);
//    void fireProjectEvent_Loaded();
//    void fireProjectEvent_Unloading();
//public:
//    void fireQueryEvent_Finished(const QString& queryID, int result);
public:
    void fireRcpEvent_BeginDownload(const QString &name, int total);
    void fireRcpEvent_EndDownload(const QString &name, int finishFlag);
    void fireRcpEvent_DownloadProcess(const QString &msg, int total, int completed);

private:
    MDRecipeEvent* g_Recipe_ua_event;
};

#endif // MDPROJECTWRAPPERSUACOLLECTER_H
