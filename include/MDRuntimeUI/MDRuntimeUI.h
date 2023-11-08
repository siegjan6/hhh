/*!

\author dGFuZ3p3

\section 功能

运行时参数界面类，设置与运行有关的参数。

*/

#ifndef MDRUNTIMEUI_H
#define MDRUNTIMEUI_H

#include <QPoint>
#include <QStringList>
#include "MDRuntimeUI_Global.h"
#include "IStudioModule.h"
#include "IStudioInterface.h"

class QTreeWidget;
class QTreeWidgetItem;
class CMDProjectConfiger;
class IHMIFramework;
class CMDRuntimeUIImp;
class MDRUNTIMEUISHARED_EXPORT CMDRuntimeUI : public IStudioModule
{
public:
    CMDRuntimeUI();
    ~CMDRuntimeUI();
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
   CMDRuntimeUIImp* m_imp;
};

#endif // MDRUNTIMEUI_H
