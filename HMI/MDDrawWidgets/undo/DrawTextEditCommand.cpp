#include "DrawTextEditCommand.h"
#include "MDDrawTextEdit.h"

DrawTextEditCommand::DrawTextEditCommand(CMDDrawObj *obj, const QVariant &oldValue, const QVariant &newValue, int id, QUndoCommand *parent):
    CMDDrawObjCommand(obj, oldValue, newValue, id, parent)
{

}

DrawTextEditCommand::~DrawTextEditCommand()
{

}

bool DrawTextEditCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;

    const DrawTextEditCommand *other = static_cast<const DrawTextEditCommand*>(command);
    if (other->m_obj != m_obj)
        return false;

    m_newValue = other->m_newValue;
    return true;
}

void DrawTextEditCommand::setValue(const QVariant &value)
{
    CMDDrawTextEdit* obj = dynamic_cast<CMDDrawTextEdit*>(m_obj);

    switch(id())
    {
    case (int)DrawTextEditId::HScrollBar:
        obj->setHorizontalScrollBar(value.toBool());
        break;
    case (int)DrawTextEditId::VScrollBar:
        obj->setVerticalScrollBar(value.toBool());
        break;
    case (int)DrawTextEditId::BorderStyle:
        obj->setBorderStyle((QFrame::Shape)value.toInt());
        break;
    case (int)DrawTextEditId::LineWrap:
        obj->setLineWrapMode((QTextEdit::LineWrapMode)value.toInt());
        break;
    case (int)DrawTextEditId::ReadOnly:
        obj->setReadOnly(value.toBool());
        break;
    case (int)DrawTextEditId::Alignment:
        obj->setAlignment((Qt::Alignment)value.toInt());
        break;
    case (int)DrawTextEditId::Text:
        obj->setText(value.toString());
        break;
    case (int)DrawTextEditId::WriteVarEnable:
        obj->setWriteVarEnable(value.toBool());
        break;
    case (int)DrawTextEditId::EnterSetVarEnable:
        obj->setEnterSetVarEnable(value.toBool());
        break;
    case (int)DrawTextEditId::HasPassword:
        obj->setHasPassword(value.toBool());
        break;
    default:
        break;
    }
}
