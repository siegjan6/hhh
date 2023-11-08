/*!

\author dGFuZ3p3

\section 功能

事件和动作界面类，通过该类可以访问 【事件界面】 和 【动作界面】的实例。

*/

#ifndef MDEVENTACTIONUI_H
#define MDEVENTACTIONUI_H

#include <QPoint>
#include "MDEventActionUI_Global.h"
#include "../../include/MDEventUI/MDEventUI.h"
#include "../../include/MDActionUI/MDActionUI.h"
#include "IStudioModule.h"
#include "IStudioInterface.h"

class QTreeWidget;
class QTreeWidgetItem;
class IHMIFramework;
class CMDEventActionUIImp;
class MDEVENTACTIONUISHARED_EXPORT CMDEventActionUI : public IStudioModule
{
public:
    CMDEventActionUI();
     ~CMDEventActionUI();

/*!
\section 访问【事件界面】 和 【动作界面】
*/
public:
    CMDEventUI* eventUI();
    CMDActionUI* actionUI();

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

private:
    CMDEventActionUIImp* m_imp;
};

#endif // MDEVENTACTIONUI_H
