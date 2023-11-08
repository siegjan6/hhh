/*!

\author dGFuZ3p3

\section 功能

对象树的节点类的实现类。

*/

#ifndef MDOBJECTTREEITEMIMP_H
#define MDOBJECTTREEITEMIMP_H

#include <QList>

class CMDObjectTreeItem;
class CMDObjectTreeItemImp
{
public:
    CMDObjectTreeItemImp();
    CMDObjectTreeItemImp(qint16 type, void* ptr, CMDObjectTreeItem* parent);
    ~CMDObjectTreeItemImp();
public:
    void releaseChild();
public:
    QList<CMDObjectTreeItem*> m_childList;
public:
    qint16 m_type ;// -1;
    void* m_ptr ;// NULL;
    CMDObjectTreeItem* m_parent ;// NULL;
};

#endif // MDOBJECTTREEITEMIMP_H
