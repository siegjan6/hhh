#include "DrawTextCommand.h"
#include "MDDrawText.h"
////////////////////////////////////////////////////////////////////////
CDrawTextCommand::CDrawTextCommand(CMDDrawObj *obj, const QVariant &oldValue, const QVariant &newValue,
    int id, QUndoCommand *parent) :
    CMDDrawVectorCommand(obj, oldValue, newValue, id, parent)
{
}
CDrawTextCommand::~CDrawTextCommand()
{
    switch (id())
    {
    case (int)DrawTextId::TextBrush:
        delete reinterpret_cast<CMDBrushManager*>(m_oldValue.toULongLong());
        delete reinterpret_cast<CMDBrushManager*>(m_newValue.toULongLong());
        break;
    case (int)DrawTextId::Font:
        delete reinterpret_cast<QFont*>(m_oldValue.toULongLong());
        delete reinterpret_cast<QFont*>(m_newValue.toULongLong());
        break;
    default:
        break;
    }
}
bool CDrawTextCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;
    switch (id())
    {
    case (int)DrawTextId::TextBrush:
    case (int)DrawTextId::Font:
        return false;
    default:
        break;
    }

    const CDrawTextCommand *other = static_cast<const CDrawTextCommand*>(command);
    if (other->m_obj != m_obj)
        return false;

    m_newValue = other->m_newValue;
    return true;
}
void CDrawTextCommand::setValue(const QVariant& value)
{
    CMDDrawText* obj = dynamic_cast<CMDDrawText*>(m_obj);

    switch(id())
    {
    case (int)DrawTextId::Text:
        obj->setText(value.toString());
        break;
    case (int)DrawTextId::TextBrush:
        obj->undoTextBrush(reinterpret_cast<CMDBrushManager*>(value.toULongLong()));
        break;
    case (int)DrawTextId::Font:
        obj->setFont(reinterpret_cast<QFont*>(value.toULongLong()));
        break;
    case (int)DrawTextId::HAlignment:
        obj->setHAlignment((Qt::Alignment)value.toInt());
        break;
    case (int)DrawTextId::VAlignment:
        obj->setVAlignment((Qt::Alignment)value.toInt());
        break;
    case (int)DrawTextId::LineWrap:
        obj->setIsWrap(value.toBool());
        break;
    case (int)DrawTextId::ButtonFrame:
        obj->setButtonFrame(value.toBool());
        break;
    default:
        break;
    }
}
////////////////////////////////////////////////////////////////////////

