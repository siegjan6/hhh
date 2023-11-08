/*!

\author dGFuZ3p3

\section 功能

对象树的节点类，凡是用树控件管理的对象都可使用此类。

*/

#ifndef MDOBJECTTREEITEM_H
#define MDOBJECTTREEITEM_H

#include <QList>
#include "MDObjectManagerUI_Global.h"

class CMDObjectTreeItemImp;
class MDOBJECTMANAGERUISHARED_EXPORT CMDObjectTreeItem
{
public:
    CMDObjectTreeItem();
    CMDObjectTreeItem(qint16 type, void* ptr, CMDObjectTreeItem* parent);
    virtual ~CMDObjectTreeItem();
public:
    //! 类型，自定义
    qint16 type() const;
    void setType(qint16 type);
public:
    //! 一个指针，一般用来存放与树节点对应的对象指针
    void *ptr() const;
    void setPtr(void *ptr);
public:
    //! 唯一的父对象
    CMDObjectTreeItem* parent() const;
    void setParent(CMDObjectTreeItem* parent);
public:
    //! 管理子对象（多个）
    CMDObjectTreeItem* child(int index) const;
    int childCount() const;
    void appendChild(CMDObjectTreeItem* child);
    void releaseChild();
protected:
    CMDObjectTreeItemImp* m_imp;
};

#endif // MDOBJECTTREEITEM_H
