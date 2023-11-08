/*!

\author dGFuZ3p3

\section 功能

脚本界面类，管理脚本界面。

*/

#ifndef MDSCRIPTUI_H
#define MDSCRIPTUI_H

#include <QPoint>
#include "MDScriptUI_Global.h"
#include "IStudioModule.h"
#include "IStudioInterface.h"

class QTreeWidget;
class QTreeWidgetItem;
class CMDProjectConfiger;
class CMDScriptUIImp;
class MDSCRIPTUISHARED_EXPORT CMDScriptUI : public IStudioModule
{
public:
    CMDScriptUI();
    ~CMDScriptUI();

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
    CMDScriptUIImp *m_imp;
};

#endif // MDSCRIPTUI_H
