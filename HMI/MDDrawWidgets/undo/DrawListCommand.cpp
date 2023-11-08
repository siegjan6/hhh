#include "DrawListCommand.h"
#include "MDDrawList.h"

DrawListCommand::DrawListCommand(CMDDrawObj *obj, const QVariant &oldValue, const QVariant &newValue, int id, QUndoCommand *parent):
    CMDDrawObjCommand(obj, oldValue, newValue, id, parent)
{

}

DrawListCommand::~DrawListCommand()
{

}

bool DrawListCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;

    const DrawListCommand *other = static_cast<const DrawListCommand*>(command);
    if (other->m_obj != m_obj)
        return false;

    m_newValue = other->m_newValue;
    return true;
}

void DrawListCommand::setValue(const QVariant &value)
{
    CMDDrawList* obj = dynamic_cast<CMDDrawList*>(m_obj);

    switch(id())
    {
    case (int)DrawListId::HasScrollBar:
        obj->setHasScrollBar(value.toBool());
        break;
    case (int)DrawListId::LayoutDirection:
        obj->setLayoutDirection((Qt::LayoutDirection)value.toInt());
        break;
    case (int)DrawListId::BorderStyle:
        obj->setBorderStyle((QFrame::Shape)value.toInt());
        break;
    case (int)DrawListId::SelectionMode:
        obj->setSelectionMode((QAbstractItemView::SelectionMode)value.toInt());
        break;
    case (int)DrawListId::Sorted:
        obj->setSorted(value.toInt());
        break;
    default:
        break;
    }
}
