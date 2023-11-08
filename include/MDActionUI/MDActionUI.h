/*!

\author dGFuZ3p3

\section 功能

动作界面类，管理动作界面，提供选择动作的界面。
动作界面与外界联系的类。

*/

#ifndef MDACTIONUI_H
#define MDACTIONUI_H

#include <QPoint>
#include <QStringList>
#include "MDActionUI_Global.h"
#include "../../include/MDActionHelper/MDActionConstant.h"
#include "IStudioModule.h"
#include "IStudioInterface.h"

class QTreeWidget;
class QTreeWidgetItem;
class CMDProjectConfiger;
class CMDActionUIImp;
class CMDActionUIContacter;
/*!
 * \brief 动作界面类。
 */
class MDACTIONUISHARED_EXPORT CMDActionUI : public IStudioModule
{
public:
    CMDActionUI();
     ~CMDActionUI();
/*!
\section 初始化
*/
public:
    void setProjectConfiger(CMDProjectConfiger* projectConfiger);
    void setContacter(CMDActionUIContacter* contacter);

/*!
\section 显示对象选择界面
*/
public:
    bool selectActions(const QStringList& sourceList, QStringList& destList);

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
    CMDActionUIImp* m_imp;
};

/*!
 * \brief 动作界面与外界联系的类。
 */
class MDACTIONUISHARED_EXPORT CMDActionUIContacter
{
public:
    CMDActionUIContacter();
    virtual ~CMDActionUIContacter();
public:
    virtual bool onSelectSingleObject(MDActionType type, const QString& source, QString& dest);
};

#endif // MDACTIONUI_H
