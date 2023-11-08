#include "DrawCheckBoxCommand.h"
#include "MDDrawCheckBox.h"

DrawCheckBoxCommand::DrawCheckBoxCommand(CMDDrawObj *obj, const QVariant &oldValue, const QVariant &newValue, int id, QUndoCommand *parent):
    CMDDrawObjCommand(obj, oldValue, newValue, id, parent)
{

}

DrawCheckBoxCommand::~DrawCheckBoxCommand()
{

}

bool DrawCheckBoxCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;

    const DrawCheckBoxCommand *other = static_cast<const DrawCheckBoxCommand*>(command);
    if (other->m_obj != m_obj)
        return false;

    m_newValue = other->m_newValue;
    return true;
}

void DrawCheckBoxCommand::setValue(const QVariant &value)
{
    CMDDrawCheckBox* obj = dynamic_cast<CMDDrawCheckBox*>(m_obj);

    switch(id())
    {
    case (int)DrawCheckBoxId::Text:
        obj->setText(value.toString());
        break;
    case (int)DrawCheckBoxId::Checked:
        obj->setChecked(value.toBool());
        break;
    default:
        break;
    }
}
