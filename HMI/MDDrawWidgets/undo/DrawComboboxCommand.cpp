#include "DrawComboboxCommand.h"
#include "MDDrawComboBox.h"

DrawComboboxCommand::DrawComboboxCommand(CMDDrawObj *obj, const QVariant &oldValue, const QVariant &newValue, int id, QUndoCommand *parent):
    CMDDrawObjCommand(obj, oldValue, newValue, id, parent)
{

}

DrawComboboxCommand::~DrawComboboxCommand()
{

}

bool DrawComboboxCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;

    const DrawComboboxCommand *other = static_cast<const DrawComboboxCommand*>(command);
    if (other->m_obj != m_obj)
        return false;

    m_newValue = other->m_newValue;
    return true;
}

void DrawComboboxCommand::setValue(const QVariant &value)
{
    CMDDrawComboBox* obj = dynamic_cast<CMDDrawComboBox*>(m_obj);

    switch(id())
    {
    case (int)DrawComboboxId::LayoutDirection:
        obj->setLayoutDirection(value.toBool());
        break;
    case (int)DrawComboboxId::ItemStyle:
        obj->setItemStyle(value.toInt());
        break;
    case (int)DrawComboboxId::Sorted:
        obj->setSorted(value.toBool());
        break;
    default:
        break;
    }
}

