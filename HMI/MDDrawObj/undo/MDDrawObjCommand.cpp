#include "MDDrawObjCommand.h"
#include "MDDrawVector.h"
#include "MDDrawGroup.h"
#include "IObjectChangedEvent.h"
#include "IHMIStudio.h"
#include "IObjName.h"
#include "DrawData.h"
#include "MDUndoData.h"
////////////////////////////////////////////////////////////////////////
CMDDrawObjCommand::CMDDrawObjCommand(CMDDrawObj *obj, const QVariant &oldValue, const QVariant &newValue,
    int id, QUndoCommand *parent) :
    QUndoCommand(parent)
{
    Q_ASSERT(obj != NULL);

    m_obj = obj;
    m_oldValue = oldValue;
    m_newValue = newValue;
    m_id = (int)id;
}
CMDDrawObjCommand::~CMDDrawObjCommand()
{
    switch (m_id)
    {
    case (int)DrawObjId::Variable:
        delete reinterpret_cast<QList<QString>*>(m_oldValue.toULongLong());
        delete reinterpret_cast<QList<QString>*>(m_newValue.toULongLong());
        break;
    default:
        break;
    }
}
////////////////////////////////////////////////////////////////////////
void CMDDrawObjCommand::undo()
{
    setValue(m_oldValue);
}
void CMDDrawObjCommand::redo()
{
    setValue(m_newValue);
}
bool CMDDrawObjCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != m_id)
        return false;   
    switch (m_id)
    {
    case (int)DrawObjId::Variable:
        return false;
    default:
        break;
    }

    const CMDDrawObjCommand *other = static_cast<const CMDDrawObjCommand*>(command);
    if (other->m_obj != m_obj)
        return false;

    m_newValue = other->m_newValue;
    return true;
}
////////////////////////////////////////////////////////////////////////
void CMDDrawObjCommand::setValue(const QVariant &value)
{
    switch(m_id)
    {
    case (int)DrawObjId::Name:
    {
        QString s = value.toString();
        m_obj->parent()->common()->nameManager()->dynamicContainsName(s);
        m_obj->setName(s);
        m_obj->parent()->framework()->manager()->objectChangedEvent()->nameChanged(m_obj);
    }
        break;
    case (int)DrawObjId::Rect:
        m_obj->setDynamicRect(value.toRectF());
        break;
    case (int)DrawObjId::Layer:
        m_obj->setLayer(value.toInt());
        break;
    case (int)DrawObjId::Variable:
    {
        QList<QString>* list = reinterpret_cast<QList<QString>*>(value.toULongLong());
        int index = 0;
        for (int i = 0; i < list->count(); i++)
            m_obj->expManager()->list()->at(index++)->setExpression(list->at(i));
    }
        break;
    case (int)DrawObjId::SupportGroupEvent:
        m_obj->setSupportGroupEvent(value.toBool());
        break;
    case (int)DrawObjId::ShowSelectedFrame:
        m_obj->setShowSelectedFrame(value.toBool());
        break;
    default:
        break;
    }
}
////////////////////////////////////////////////////////////////////////
CMDDrawVectorCommand::CMDDrawVectorCommand(CMDDrawObj *obj, const QVariant &oldValue, const QVariant &newValue,
    int id, QUndoCommand *parent) :
    CMDDrawObjCommand(obj, oldValue, newValue, id, parent)
{

}
CMDDrawVectorCommand::~CMDDrawVectorCommand()
{
    switch (id())
    {
    case (int)DrawVectorId::Brush:
        delete reinterpret_cast<CMDBrushManager*>(m_oldValue.toULongLong());
        delete reinterpret_cast<CMDBrushManager*>(m_newValue.toULongLong());
        break;
    case (int)DrawVectorId::Pen:
        delete reinterpret_cast<CMDPenManager*>(m_oldValue.toULongLong());
        delete reinterpret_cast<CMDPenManager*>(m_newValue.toULongLong());
        break;
    case (int)DrawVectorId::DynamicBrush:
    case (int)DrawVectorId::DynamicPen:
        delete reinterpret_cast<CDynamicColorManager*>(m_oldValue.toULongLong());
        delete reinterpret_cast<CDynamicColorManager*>(m_newValue.toULongLong());
        break;
    case (int)DrawVectorId::Fill:
        delete reinterpret_cast<CMDFill*>(m_oldValue.toULongLong());
        delete reinterpret_cast<CMDFill*>(m_newValue.toULongLong());
        break;
    case (int)DrawVectorId::MouseDownAction:
    case (int)DrawVectorId::MouseUpAction:
    case (int)DrawVectorId::RightMouseDownAction:
    case (int)DrawVectorId::RightMouseUpAction:
        delete reinterpret_cast<CMDHMIActionManager*>(m_oldValue.toULongLong());
        delete reinterpret_cast<CMDHMIActionManager*>(m_newValue.toULongLong());
        break;
    default:
        break;
    }
}
bool CMDDrawVectorCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;
    switch (id())
    {
    case (int)DrawVectorId::Brush:
    case (int)DrawVectorId::Pen:
    case (int)DrawVectorId::DynamicBrush:
    case (int)DrawVectorId::DynamicPen:
    case (int)DrawVectorId::Fill:
    case (int)DrawVectorId::MouseDownAction:
    case (int)DrawVectorId::MouseUpAction:
    case (int)DrawVectorId::RightMouseDownAction:
    case (int)DrawVectorId::RightMouseUpAction:
        return false;
    default:
        break;
    }

    const CMDDrawVectorCommand *other = static_cast<const CMDDrawVectorCommand*>(command);
    if (other->m_obj != m_obj)
        return false;

    m_newValue = other->m_newValue;
    return true;
}
void CMDDrawVectorCommand::setValue(const QVariant& value)
{
    CMDDrawVector* obj = dynamic_cast<CMDDrawVector*>(m_obj);

    switch(id())
    {
    case (int)DrawVectorId::Angle:
        obj->setDynamicRotateAngle(value.toDouble());
        break;
    case (int)DrawVectorId::RotatePoint:
        obj->setRotatePoint(value.toPointF());
        break;
    case (int)DrawVectorId::RotatePointPos:
        obj->setRotatePointPos(value.toPointF());
        break;
    case (int)DrawVectorId::Shear:
        obj->setDynamicShear(value.toDouble());
        break;
    case (int)DrawVectorId::InputVariable:
        obj->setInputVariable(value.toString());
        break;

    case (int)DrawVectorId::Brush:
        obj->undoBrush(reinterpret_cast<CMDBrushManager*>(value.toULongLong()));
        break;
    case (int)DrawVectorId::Pen:
        obj->undoPen(reinterpret_cast<CMDPenManager*>(value.toULongLong()));
        break;
    case (int)DrawVectorId::DynamicBrush:
        obj->setDynamicBrush(reinterpret_cast<CDynamicColorManager*>(value.toULongLong()));
        break;
    case (int)DrawVectorId::DynamicPen:
        obj->setDynamicPen(reinterpret_cast<CDynamicColorManager*>(value.toULongLong()));
        break;
    case (int)DrawVectorId::Fill:
        obj->setFill(reinterpret_cast<CMDFill*>(value.toULongLong()));
        break;

    case (int)DrawVectorId::MouseDownAction:
    case (int)DrawVectorId::MouseUpAction:
    case (int)DrawVectorId::RightMouseDownAction:
    case (int)DrawVectorId::RightMouseUpAction:
        obj->undoAction(id(), reinterpret_cast<CMDHMIActionManager*>(value.toULongLong()));
        break;
    default:
        break;
    }
}
////////////////////////////////////////////////////////////////////////
CMDMouseOperateCommand::CMDMouseOperateCommand(
        const QVariant &oldValue, const QVariant &newValue, int id, QUndoCommand *parent)
        : QUndoCommand(parent)
{
    m_oldValue = oldValue;
    m_newValue = newValue;
    m_id = id;
}
CMDMouseOperateCommand::~CMDMouseOperateCommand()
{
    QList<QVariant> l = m_oldValue.toList();
    for(int i = 0; i < l.count(); i++)
        delete reinterpret_cast<IUndoDataObj*>(l.at(i).toULongLong());
    l = m_newValue.toList();
    for(int i = 0; i < l.count(); i++)
        delete reinterpret_cast<IUndoDataObj*>(l.at(i).toULongLong());
}
void CMDMouseOperateCommand::undo()
{
    setValue(m_oldValue);
}
void CMDMouseOperateCommand::redo()
{
    setValue(m_newValue);
}
bool CMDMouseOperateCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != m_id)
        return false;

    qulonglong c = (qulonglong)command;
    CMDMouseOperateCommand *other = reinterpret_cast<CMDMouseOperateCommand*>(c);
    QList<QVariant> l = m_newValue.toList();
    QList<QVariant> lOther = other->m_newValue.toList();
    int count = l.count();
    if (count != lOther.count())
        return false;

    for (int i = 0; i < count; i++)
    {
        IUndoDataObj* u = reinterpret_cast<IUndoDataObj*>(l.at(i).toULongLong());
        IUndoDataObj* uOther = reinterpret_cast<IUndoDataObj*>(lOther.at(i).toULongLong());
        if (u->obj() != uOther->obj())
            return false;
    }

    //move data
    for(int i = 0; i < l.count(); i++)
        delete reinterpret_cast<IUndoDataObj*>(l.at(i).toULongLong());
    m_newValue = other->m_newValue;
    other->m_newValue = QList<QVariant>();

    return true;
}
void CMDMouseOperateCommand::setValue(const QVariant &value)
{
    switch(id())
    {
    case (int)MouseOperateId::Mouse:
    {
        QList<QVariant> l = value.toList();
        for(int i = 0; i < l.count(); i++)
        {
            IUndoDataObj* u = reinterpret_cast<IUndoDataObj*>(l.at(i).toULongLong());
            u->obj()->undoOperate(u);
        }
    }
        break;
    default:
        break;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////
CMDDrawGroupCommand::CMDDrawGroupCommand(CMDDrawObj *obj, const QVariant &oldValue, const QVariant &newValue, int id, QUndoCommand *parent)
    :CMDDrawVectorCommand(obj, oldValue, newValue, id, parent)
{
}
CMDDrawGroupCommand::~CMDDrawGroupCommand()
{
    switch (id())
    {
    case (int)DrawGroupId::CustomProperty:
        delete reinterpret_cast<QList<CMDCustomProperty>*>(m_oldValue.toULongLong());
        delete reinterpret_cast<QList<CMDCustomProperty>*>(m_newValue.toULongLong());
        break;
    default:
        break;
    }
}
bool CMDDrawGroupCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;

    return false;
}
void CMDDrawGroupCommand::setValue(const QVariant &value)
{
    CMDDrawGroup* obj = dynamic_cast<CMDDrawGroup*>(m_obj);

    switch(id())
    {
    case (int)DrawGroupId::CustomProperty:
        *obj->customPropertys() = *reinterpret_cast<QList<CMDCustomProperty>*>(value.toULongLong());
        break;
    default:
        break;
    }
}
