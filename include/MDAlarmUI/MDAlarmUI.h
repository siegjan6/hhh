/*!

\author dGFuZ3p3

\section 功能

报警界面类，通过此类可访问【报警等级界面】和【报警条件界面】。

*/

#ifndef MDALARMUI_H
#define MDALARMUI_H

#include <QPoint>
#include <QStringList>
#include "MDAlarmUI_Global.h"

#include "IStudioModule.h"
#include "IStudioInterface.h"

class QTreeWidget;
class QTreeWidgetItem;
class CMDAlarmConfiger;
class CMDAlarmLevelUI;
class CMDAlarmConditionUI;
class CMDAlarmUIImp;
class CMDPowerGroupConfiger;
class MDALARMUISHARED_EXPORT CMDAlarmUI : public IStudioModule
{
public:
    CMDAlarmUI();
    ~CMDAlarmUI();

/*!
\section 访问【报警等级界面】和【报警条件界面】
*/
public:
    CMDAlarmLevelUI* levelUI();
    CMDAlarmConditionUI* conditionUI();

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
   CMDAlarmUIImp* m_imp;
};

#endif // MDALARMUI_H
