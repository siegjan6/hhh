/*!

\author dGFuZ3p3

\section 功能

安全策略界面类，通过此类可访问【帐户界面】和【权限组界面】。

*/

#ifndef MDSECURITYPOLICYUI_H
#define MDSECURITYPOLICYUI_H

#include <QPoint>
#include <QStringList>
#include "MDSecurityPolicyUI_Global.h"
#include "../../include/MDAccountUI/MDAccountUI.h"
#include "../../include/MDPowerGroupUI/MDPowerGroupUI.h"
#include "MDActionSP.h"
#include "IStudioModule.h"
#include "IStudioInterface.h"

class QTreeWidget;
class QTreeWidgetItem;
class CMDSecurityPolicyConfiger;
class CMDSecurityPolicyExpert;
class CMDSecurityPolicyUIImp;
class CMDActionSP;
class QDialog;
class CMDActionSPSession;
class MDSECURITYPOLICYUISHARED_EXPORT CMDSecurityPolicyUI : public IStudioModule
{
public:
    CMDSecurityPolicyUI();
    ~CMDSecurityPolicyUI();
/*!
\section 初始化
*/
public:
   void setConfiger(CMDSecurityPolicyConfiger* configer);
   void setSecurityPolicyExpert(CMDSecurityPolicyExpert* spExpert);

/*!
\section 访问【帐户界面】和【权限组界面】
*/
public:
    CMDAccountUI m_accountUI;
    CMDPowerGroupUI m_powerGroupUI;

/*!
\section 显示登录界面、安全验证界面；帐户和权限组管理界面
*/
public:
   bool login();
   bool checkSP(CMDActionSPSession* spSession);
   bool showMgrDialog();

/*!
\section property
*/
public:
   //根节点名称
   virtual QString rootName();
   //根节点图标
   virtual QIcon* rootIcon();
   //根节点
   virtual QTreeWidgetItem* rootItem();
   virtual void setRootItem(QTreeWidgetItem* value);
   //设置Studio接口
   virtual void setStudioInterface(IStudioInterface* value);
/*!
\section function
*/
public:
   //加载
   virtual void load();
   //卸载
   virtual void unload();

   //多语言
   virtual void changeLanguage();
   virtual void collectWords();

protected:
   CMDSecurityPolicyUIImp* m_imp;
};

#endif // MDSECURITYPOLICYUI_H
