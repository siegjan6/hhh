/*!

\author dGFuZ3p3

\section 功能

动作安全策略配置界面类。

*/

#ifndef MDACTIONSPUI_H
#define MDACTIONSPUI_H

#include "MDActionSPUI_Global.h"
#include "../../include/MDSecurityPolicyHelper/MDActionSP.h"

class CMDActionSPUIImp;
class CMDActionSP;
class CMDActionSPListDialog;
class CMDPowerGroupConfiger;
class MDACTIONSPUISHARED_EXPORT CMDActionSPUI
{
public:
    CMDActionSPUI();
    ~CMDActionSPUI();
/*!
\section 初始化
*/
public:
    bool init(CMDPowerGroupConfiger* configer);

/*!
\section 显示模态界面
*/
public:
    //! 显示一个模态界面，配置一个 CMDActionSP 对象
   bool doModalActionSPDialog(CMDActionSP *sourceProperty);
public:
   //! 显示一个模态界面，管理一个 CMDActionSP 链表
    bool doModalActionSPListDialog(CMDActionSPList& sourceList, CMDActionSPList& destList);
protected:
   CMDActionSPUIImp* m_imp;
};

#endif // MDACTIONSPUI_H
