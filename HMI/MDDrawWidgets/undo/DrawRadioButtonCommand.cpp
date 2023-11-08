#include "DrawRadioButtonCommand.h"
#include "MDDrawRadioButton.h"

DrawRadioButtonCommand::DrawRadioButtonCommand(CMDDrawObj *obj, const QVariant &oldValue, const QVariant &newValue, int id, QUndoCommand *parent):
    CMDDrawObjCommand(obj, oldValue, newValue, id, parent)
{

}

DrawRadioButtonCommand::~DrawRadioButtonCommand()
{

}

bool DrawRadioButtonCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;

    const DrawRadioButtonCommand *other = static_cast<const DrawRadioButtonCommand*>(command);
    if (other->m_obj != m_obj)
        return false;

    m_newValue = other->m_newValue;
    return true;
}

void DrawRadioButtonCommand::setValue(const QVariant &value)
{
    CMDDrawRadioButton* obj = dynamic_cast<CMDDrawRadioButton*>(m_obj);

    switch(id())
    {
    case (int)DrawRadioButtonId::Text:
        obj->setText(value.toString());
        break;
    case (int)DrawRadioButtonId::Checked:
        obj->setChecked(value.toBool());
        break;
    default:
        break;
    }
}
