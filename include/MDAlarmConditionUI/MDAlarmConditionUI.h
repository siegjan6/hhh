/*!

\author dGFuZ3p3

\section 功能

报警条件界面类，管理报警条件界面，提供选择报警条件的界面。
报警条件界面与外界联系的类，通过此类可获取【报警等级】和【报警变量】。

*/

#ifndef MDALARMCONDITIONUI_H
#define MDALARMCONDITIONUI_H

#include <QPoint>
#include <QStringList>
#include "MDAlarmConditionUI_Global.h"
#include "IStudioModule.h"
#include "IStudioInterface.h"

class QTreeWidget;
class QTreeWidgetItem;
class CMDAlarmConditionConfiger;
class CMDAlarmConditionUIImp;
class CMDAlarmConditionUIContacter;
class CMDPowerGroupConfiger;
class IStudioInterface;
/*!
 * \brief 报警条件界面类，管理报警条件界面，提供选择报警条件的界面。
 */
class MDALARMCONDITIONUISHARED_EXPORT CMDAlarmConditionUI : public IStudioModule
{
public:
    CMDAlarmConditionUI();
     ~CMDAlarmConditionUI();
/*!
\section 初始化
*/
public:
    void setContacter(CMDAlarmConditionUIContacter* contacter);

/*!
\section 显示对象选择界面
*/
public:
    bool select(const QStringList& sourceList, QStringList& destList);

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
    CMDAlarmConditionUIImp* m_imp;
};

/*!
 * \brief 报警条件界面与外界联系的类，通过此类可获取【报警等级】和【报警变量】。
 */
class MDALARMCONDITIONUISHARED_EXPORT CMDAlarmConditionUIContacter
{
public:
    CMDAlarmConditionUIContacter();
    virtual ~CMDAlarmConditionUIContacter();
public:
    virtual bool onSelectAlarmLevel(const QString& source, QString& dest);
    virtual bool onSelectVariable(const QString& source, QString& dest);
};

#endif // MDALARMCONDITIONUI_H
