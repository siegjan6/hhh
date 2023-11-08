#ifndef MDDATAARCHIVESUI_H
#define MDDATAARCHIVESUI_H


/*!

\author bHVvY2hhbw==

\section 功能
1.数据归档界面对外接口类。
2.加载数据归档配置界面即对数据组与数据源的配置。
3.处理相应树结点事件的处理。

*/

#include <QtCore/qglobal.h>
#include <QObject>
#include <list>
#include <iostream>
#include <qmutex.h>
#include <QMenu>
#include <QAction>
#include "IStudioModule.h"
#include "IStudioInterface.h"

#if defined(MDDATAARCHIVESUI_LIBRARY)
#  define MDDATAARCHIVESUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDATAARCHIVESUISHARED_EXPORT Q_DECL_IMPORT
#endif

class QTreeWidget;
class QTreeWidgetItem;
//================================
class CDataArchivesUI;
class IMDRTDBManagerUI;
class IMDDataArchives;

class MDDATAARCHIVESUISHARED_EXPORT CMDDataArchivesUI : public IStudioModule
{

public:
    CMDDataArchivesUI();
    virtual ~CMDDataArchivesUI();

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
    CDataArchivesUI *m_imp; //! 数据归档内部实现类
};

#endif // MDDATAARCHIVESUI_H

