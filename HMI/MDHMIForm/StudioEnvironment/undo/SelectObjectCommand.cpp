#include "SelectObjectCommand.h"
#include "SelectObjectManager.h"


CSelectObjectCommand::CSelectObjectCommand(CSelectObjectManager *obj,
    const QVariant &oldValue, const QVariant &newValue, int id, QUndoCommand *parent)
    :QUndoCommand(parent)
{
    Q_ASSERT(obj != NULL);

    m_obj = obj;
    m_oldValue = oldValue;
    m_newValue = newValue;
    m_id = (int)id;

    m_list = *obj->list();
}
////////////////////////////////////////////////////////////////////////
void CSelectObjectCommand::undo()
{
    setValue(m_oldValue);
}
void CSelectObjectCommand::redo()
{
    setValue(m_newValue);
}
bool CSelectObjectCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != m_id)
        return false;
    const CSelectObjectCommand *other = static_cast<const CSelectObjectCommand*>(command);
    //确保obj列表完全一致
    QList<CMDDrawObj*>* l = other->m_obj->list();
    if (m_list != *l)
        return false;

    m_newValue = other->m_newValue;
    return true;
}
////////////////////////////////////////////////////////////////////////
void CSelectObjectCommand::setValue(const QVariant &value)
{
    switch(m_id)
    {
    case (int)SelectObjectId::Rect:
        m_obj->reset(&m_list);
        m_obj->setRect(value.toRectF());
        break;
    default:
        break;
    }
}
////////////////////////////////////////////////////////////////////////
