#include "DrawDateTimeCommand.h"
#include "MDDrawDateTimeEdit.h"

DrawDateTimeCommand::DrawDateTimeCommand(CMDDrawObj *obj, const QVariant &oldValue, const QVariant &newValue, int id, QUndoCommand *parent):
    CMDDrawObjCommand(obj, oldValue, newValue, id, parent)
{

}

DrawDateTimeCommand::~DrawDateTimeCommand()
{

}

bool DrawDateTimeCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;

    const DrawDateTimeCommand *other = static_cast<const DrawDateTimeCommand*>(command);
    if (other->m_obj != m_obj)
        return false;

    m_newValue = other->m_newValue;
    return true;
}

void DrawDateTimeCommand::setValue(const QVariant &value)
{
    CMDDrawDateTimeEdit* obj = dynamic_cast<CMDDrawDateTimeEdit*>(m_obj);

    switch(id())
    {
    case (int)DrawDateTimeId::ShowType:
        obj->setShowType(value.toInt());
        break;
    case (int)DrawDateTimeId::CalendarPop:
        obj->setCalendarPop(value.toBool());
        break;
    default:
        break;
    }
}
