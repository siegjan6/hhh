/*!

\author dGFuZ3p3

\section 功能

报警等级界面类。

*/

#ifndef MDALARMLEVELUI_H
#define MDALARMLEVELUI_H

#include <QPoint>
#include <QStringList>
#include "MDAlarmLevelUI_Global.h"
#include "IStudioModule.h"
#include "IStudioInterface.h"

class QTreeWidget;
class QTreeWidgetItem;
class CMDAlarmLevelConfiger;
class CMDAlarmLevelUIImp;
class CMDAlarmConfiger;
class IStudioInterface;
class MDALARMLEVELUISHARED_EXPORT CMDAlarmLevelUI : public IStudioModule
{
public:
    CMDAlarmLevelUI();
     ~CMDAlarmLevelUI();

/*!
\section 显示对象选择界面
*/
public:
    bool select(const QString& source, QString& dest);

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
    CMDAlarmLevelUIImp* m_imp;
};

#endif // MDALARMLEVELUI_H
