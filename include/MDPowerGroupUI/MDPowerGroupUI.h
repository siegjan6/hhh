/*!

\author dGFuZ3p3

\section 功能

权限组界面类，管理权限组界面，提供选择权限组的界面。

*/

#ifndef MDPOWERGROUPUI_H
#define MDPOWERGROUPUI_H

#include <QPoint>
#include <QStringList>
#include "MDPowerGroupUI_Global.h"
#include "IStudioModule.h"
#include "IStudioInterface.h"

class QTreeWidget;
class QTreeWidgetItem;
class CMDPowerGroupConfiger;
class CMDPowerGroupUIImp;
class CMDAccountUI;
class MDPOWERGROUPUISHARED_EXPORT CMDPowerGroupUI : public IStudioModule
{
public:
    CMDPowerGroupUI();
     ~CMDPowerGroupUI();
/*!
\section 初始化
*/
public:
    void setAccountUI(CMDAccountUI* accountUI);

/*!
\section 显示对象选择界面
*/
public:
    bool selectPowerGroups(const QStringList& sourceList, QStringList& destList);

/*!
\section 创建对象树管理界面，目前用于运行时管理帐户
*/
public:
    QWidget* createMgrWidget();

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
    CMDPowerGroupUIImp* m_imp;
};

#endif // MDPOWERGROUPUI_H
