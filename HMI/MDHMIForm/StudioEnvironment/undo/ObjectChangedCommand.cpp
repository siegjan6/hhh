#include "ObjectChangedCommand.h"
#include "MDDrawObj.h"
#include "Studio.h"
/////////////////////////////////////////////////////////////////
CObjectChangedCommand::CObjectChangedCommand(QList<CMDDrawObj *> *objs, QList<int>* indexs, CStudio *container,
    int id, QUndoCommand *parent) : QUndoCommand(parent)
{
    m_objs.append(*objs);
    m_indexs.append(*indexs);
    m_container = container;
    m_id = id;
}
void CObjectChangedCommand::undo()
{
    switch (m_id)
    {
    case (int)ObjectChangedId::Add:
        m_container->undoDelete(&m_objs, &m_indexs);
        break;
    case (int)ObjectChangedId::Delete:
        m_container->undoAdd(&m_objs, &m_indexs);
        break;
    case (int)ObjectChangedId::Group:
        m_container->undoUnGroup(&m_objs, &m_indexs);
        break;
    case (int)ObjectChangedId::UnGroup:
        m_container->undoGroup(&m_objs, &m_indexs);
        break;
    case (int)ObjectChangedId::Combine:
        m_container->undoUnCombine(&m_objs, &m_indexs);
        break;
    case (int)ObjectChangedId::UnCombine:
        m_container->undoCombine(&m_objs, &m_indexs);
        break;
    case (int)ObjectChangedId::Index:
        m_container->undoIndex(m_objs.at(0), m_indexs.at(0));
        break;
    default:
        break;
    }
}
void CObjectChangedCommand::redo()
{
    switch (m_id)
    {
    case (int)ObjectChangedId::Add:
        m_container->undoAdd(&m_objs, &m_indexs);
        break;
    case (int)ObjectChangedId::Delete:
        m_container->undoDelete(&m_objs, &m_indexs);
        break;
    case (int)ObjectChangedId::Group:
        m_container->undoGroup(&m_objs, &m_indexs);
        break;
    case (int)ObjectChangedId::UnGroup:
        m_container->undoUnGroup(&m_objs, &m_indexs);
        break;
    case (int)ObjectChangedId::Combine:
        m_container->undoCombine(&m_objs, &m_indexs);
        break;
    case (int)ObjectChangedId::UnCombine:
        m_container->undoUnCombine(&m_objs, &m_indexs);
        break;
    case (int)ObjectChangedId::Index:
        m_container->undoIndex(m_objs.at(0), m_indexs.at(1));
        break;
    default:
        break;
    }
}
//////////////////////////////////////////////////////////////////////
