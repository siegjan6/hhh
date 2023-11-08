#include "DrawPipeCommand.h"
#include "MDDrawNode.h"
#include "MDNodeStream.h"

CDrawPipeCommand::CDrawPipeCommand(CMDDrawObj *obj, const QVariant &oldValue, const QVariant &newValue,
                                   int id, QUndoCommand *parent) :
    CMDDrawVectorCommand(obj, oldValue, newValue, id, parent)
{
}
CDrawPipeCommand::~CDrawPipeCommand()
{
    switch (id())
    {
    case (int)DrawPipeId::StreamPen:
        delete reinterpret_cast<CMDPenManager*>(m_oldValue.toULongLong());
        delete reinterpret_cast<CMDPenManager*>(m_newValue.toULongLong());
        break;
    default:
        break;
    }
}
bool CDrawPipeCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;
    switch (id())
    {
    case (int)DrawPipeId::StreamPen:
        return false;
    default:
        break;
    }

    const CDrawPipeCommand *other = static_cast<const CDrawPipeCommand*>(command);
    if (other->m_obj != m_obj)
        return false;

    m_newValue = other->m_newValue;
    return true;
}
void CDrawPipeCommand::setValue(const QVariant& value)
{
    CMDDrawNode* obj = dynamic_cast<CMDDrawNode*>(m_obj);

    switch(id())
    {
    case (int)DrawPipeId::PipeVisible:
        obj->setPipeBackGroundVisible(value.toBool());
        break;
    case (int)DrawPipeId::PipeWidth:
        if(value.toInt()>0)
        {
           obj->setPipewidth(value.toInt());
        }
        break;
    case (int)DrawPipeId::PipeBackColor:
        obj->setPipeClr2(value.value<QColor>());
        break;
    case (int)DrawPipeId::PipeHighColor:
         obj->setPipeClr1(value.value<QColor>());
        break;
    case (int)DrawPipeId::StreamLineVisible:
        obj->pLineStream()->setStreamLineVisible(value.toBool());
        break;
    case (int)DrawPipeId::StreamLineFlow:
        obj->pLineStream()->setStreamLineFlow(value.toBool());
        break;
    case (int)DrawPipeId::StreamPen:
        obj->pLineStream()->m_penManager = *reinterpret_cast<CMDPenManager*>(value.toULongLong());
        obj->pLineStream()->updatePen();
        break;
    default:
        break;
    }
}
