#include "DrawBasicArcCommand.h"
#include "MDDrawBasicArc.h"
////////////////////////////////////////////////////////////////////////
CDrawBasicArcCommand::CDrawBasicArcCommand(CMDDrawObj *obj, const QVariant &oldValue, const QVariant &newValue,
    int id, QUndoCommand *parent) :
    CMDDrawVectorCommand(obj, oldValue, newValue, id, parent)
{
}
bool CDrawBasicArcCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;

    const CDrawBasicArcCommand *other = static_cast<const CDrawBasicArcCommand*>(command);
    if (other->m_obj != m_obj)
        return false;

    m_newValue = other->m_newValue;
    return true;
}
void CDrawBasicArcCommand::setValue(const QVariant& value)
{
    CMDDrawBasicArc* obj = dynamic_cast<CMDDrawBasicArc*>(m_obj);

    switch(id())
    {
    case (int)DrawBasicArcId::StartAngle:
        obj->setStartAngle(value.toDouble());
        break;
    case (int)DrawBasicArcId::SpanAngle:
        obj->setSpanAngle(value.toDouble());
        break;
    default:
        break;
    }
}
////////////////////////////////////////////////////////////////////////
