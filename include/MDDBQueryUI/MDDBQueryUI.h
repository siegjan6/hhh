
/*!

\author bWVpeGw=

\section 功能

管理【数据查询UI模块的导出】。

*/
#ifndef MDDBQUERYUI_H
#define MDDBQUERYUI_H

#include <QtCore/qglobal.h>
#include <QObject>

#include "MDMultiLanguage.h"
#include "IStudioModule.h"
#include "IStudioInterface.h"
#include "../include/MDDBQueryConfigure/MDDBQueryConfigure.h"

#if defined(DBQUERYUI_LIBRARY)
#  define DBQUERYUI_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define DBQUERYUI_SHARED_EXPORT Q_DECL_IMPORT
#endif

class IMDDBQueryEngine;
class DBQueryUI;

class DBQUERYUI_SHARED_EXPORT MDDBQueryUI : public IStudioModule
{
public:
    MDDBQueryUI();
    ~MDDBQueryUI();

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
    DBQueryUI *m_DBQueryUI; //! 数据查询UI指针
};

namespace DBQUI
{
//视图选择模式
enum VIEW_SELECTION_MODE
{
    VIEW_SINGLE_SELECTION = 0,
    VIEW_MULTIPLE_SELECTION
};

enum VIEW_FIELD_RETURN_TYPE
{
    VIEW_FIELD_RETURN_TYPE_ALL= 0,
    VIEW_FIELD_RETURN_TYPE_DATETIME,
    VIEW_FIELD_RETURN_TYPE_VARIABLE
};

DBQUERYUI_SHARED_EXPORT QStringList GetViewNameSelected(const QString & viewPath, VIEW_SELECTION_MODE mode);
DBQUERYUI_SHARED_EXPORT QList<DBQ::ViewField *> GetFieldsFromViewSelected(const QString & viewPath, const QString & viewName, VIEW_FIELD_RETURN_TYPE returnType);
//函数功能:调用标准的运行时参数配置界面
DBQUERYUI_SHARED_EXPORT void CallStandardRumtimeParmatersConfigUI(IMDDBQueryEngine * pEngine);
}
#endif // MDDBQUERYUI_H
