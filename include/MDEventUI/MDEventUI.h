/*!

\author dGFuZ3p3

\section 功能

事件界面类，管理事件界面，提供选择事件的界面。
事件界面与外界联系的类。

*/

#ifndef MDEVENTUI_H
#define MDEVENTUI_H

#include <QPoint>
#include <QStringList>
#include "MDEventUI_Global.h"
#include "../../include/MDEventHelper/MDEventConstant.h"
#include "IStudioModule.h"
#include "IStudioInterface.h"

class QTreeWidget;
class QTreeWidgetItem;
class CMDProjectConfiger;
class CMDEventUIContacter;
class CMDEventUIImp;
/*!
 * \brief 事件界面类。
 */
class MDEVENTUISHARED_EXPORT CMDEventUI : public IStudioModule
{
public:
    CMDEventUI();
     ~CMDEventUI();

/*!
\section 初始化
*/
public:
    void setContacter(CMDEventUIContacter* contacter);

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
    CMDEventUIImp* m_imp;
};

/*!
 * \brief 事件界面与外界联系的类。
 */
class MDEVENTUISHARED_EXPORT CMDEventUIContacter
{
public:
    CMDEventUIContacter();
    virtual ~CMDEventUIContacter();
public:
    virtual bool onSelectActions(const QStringList& sourceList, QStringList& destList);
    virtual bool onSelectSingleObject(MDEventType type, const QString& source, QString& dest);
};

#endif // MDEVENTUI_H
