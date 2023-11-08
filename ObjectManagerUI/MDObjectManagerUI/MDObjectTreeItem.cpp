#include "MDObjectTreeItem.h"
#include "MDObjectTreeItemImp.h"

CMDObjectTreeItem::CMDObjectTreeItem()
{
    m_imp = new CMDObjectTreeItemImp;
}

CMDObjectTreeItem::CMDObjectTreeItem(qint16 type, void* ptr, CMDObjectTreeItem* parent)
{
    m_imp = new CMDObjectTreeItemImp(type, ptr, parent);
}

CMDObjectTreeItem::~CMDObjectTreeItem()
{
    if(m_imp)
        delete m_imp;
}

qint16 CMDObjectTreeItem::type() const
{
    return (m_imp ? m_imp->m_type : -1);
}
void CMDObjectTreeItem::setType(qint16 type)
{
    if(m_imp)
        m_imp->m_type = type;
}

void *CMDObjectTreeItem::ptr() const
{
    return (m_imp ? m_imp->m_ptr : NULL);
}
void CMDObjectTreeItem::setPtr(void *ptr)
{
    if(m_imp)
        m_imp->m_ptr = ptr;
}

CMDObjectTreeItem* CMDObjectTreeItem::parent() const
{
    return (m_imp ? m_imp->m_parent : NULL);
}
void CMDObjectTreeItem::setParent(CMDObjectTreeItem *parent)
{
    if(m_imp)
        m_imp->m_parent = parent;
}

CMDObjectTreeItem* CMDObjectTreeItem::child(int index) const
{
    if(!m_imp)
        return NULL;

    if(m_imp->m_childList.isEmpty())
        return NULL;

    return m_imp->m_childList.at(index);
}

int CMDObjectTreeItem::childCount() const
{
    return (m_imp ? m_imp->m_childList.size() : 0);
}

void CMDObjectTreeItem::appendChild(CMDObjectTreeItem* child)
{
    if(m_imp)
        m_imp->m_childList.append(child);
}

void CMDObjectTreeItem::releaseChild()
{
    if(!m_imp)
        return;

    QList<CMDObjectTreeItem*>::iterator i;
    for (i = m_imp->m_childList.begin(); i != m_imp->m_childList.end(); ++i)
        delete *i;
    m_imp->m_childList.clear();
}
