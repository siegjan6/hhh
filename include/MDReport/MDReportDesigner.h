/*!

\author bWVpeGw=

\section 功能

管理【报表设计器的导出】。

*/

#ifndef NCREPORTLIB_H
#define NCREPORTLIB_H

#include <QtCore/qglobal.h>
#include <QObject>
#include <list>
#include <iostream>
#include <qmutex.h>
#include <QMenu>
#include <QAction>

#include "MDMultiLanguage.h"
#include "IStudioModule.h"
#include "IStudioInterface.h"

#if defined(DESIGNER_LIBRARY)
#  define NCREPORTDESIGNER_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define NCREPORTDESIGNER_SHARED_EXPORT Q_DECL_IMPORT
#endif

class QTreeWidget;
class QTreeWidgetItem;
class CReport;
class CMDProjectConfiger;
class NCREPORTDESIGNER_SHARED_EXPORT CMDReportDesigner : public IStudioModule
{
public:
    CMDReportDesigner();
    ~CMDReportDesigner();

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
    CReport *m_imp; //! 报表设计器指针
};

#endif // NCREPORTLIB_H
