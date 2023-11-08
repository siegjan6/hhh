#include "MDObjectTreeItemImp.h"

CMDObjectTreeItemImp::CMDObjectTreeItemImp()
{
    m_type = -1;
    m_ptr = NULL;
    m_parent = NULL;
}

CMDObjectTreeItemImp::CMDObjectTreeItemImp(qint16 type, void* ptr, CMDObjectTreeItem* parent)
{
    m_type = type;
    m_ptr = ptr;
    m_parent = parent;
}

CMDObjectTreeItemImp::~CMDObjectTreeItemImp()
{
    releaseChild();
}

void CMDObjectTreeItemImp::releaseChild()
{
    QList<CMDObjectTreeItem*>::iterator i;
    for (i = m_childList.begin(); i != m_childList.end(); ++i)
        delete *i;
    m_childList.clear();
}
