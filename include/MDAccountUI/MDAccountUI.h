/*!

\author dGFuZ3p3

\section 功能

帐户界面类，管理帐户界面，提供选择帐户的界面。

*/

#ifndef MDACCOUNTUI_H
#define MDACCOUNTUI_H

#include <QPoint>
#include <QStringList>
#include "MDAccountUI_Global.h"
#include "IStudioModule.h"
#include "IStudioInterface.h"

class QTreeWidget;
class QTreeWidgetItem;
class CMDSecurityPolicyConfiger;
class CMDAccountUIImp;
class MDACCOUNTUISHARED_EXPORT CMDAccountUI : public IStudioModule
{
public:
    CMDAccountUI();
     ~CMDAccountUI();

/*!
\section 显示对象选择界面
*/
public:
    bool selectAccounts(const QStringList& sourceList, QStringList& destList);

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
    CMDAccountUIImp* m_imp;
};

#endif // MDACCOUNTUI_H
