#include "DrawBasicWidgetCommand.h"
#include "MDDrawBasicWidget.h"

CDrawBasicWidgetCommand::CDrawBasicWidgetCommand(CMDDrawObj *obj, const QVariant &oldValue, const QVariant &newValue, int id, QUndoCommand *parent):
    CMDDrawObjCommand(obj, oldValue, newValue, id, parent)
{

}

CDrawBasicWidgetCommand::~CDrawBasicWidgetCommand()
{
    switch (id())
    {
    case (int)DrawBasicWidgetId::Font:
        delete reinterpret_cast<QFont*>(m_oldValue.toULongLong());
        delete reinterpret_cast<QFont*>(m_newValue.toULongLong());
        break;
    default:
        break;
    }
}

bool CDrawBasicWidgetCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;
    switch (id())
    {
    case (int)DrawBasicWidgetId::Font:
        return false;
    default:
        break;
    }

    const CDrawBasicWidgetCommand *other = static_cast<const CDrawBasicWidgetCommand*>(command);
    if (other->m_obj != m_obj)
        return false;

    m_newValue = other->m_newValue;
    return true;
}

void CDrawBasicWidgetCommand::setValue(const QVariant &value)
{
    CMDDrawBasicWidget* obj = dynamic_cast<CMDDrawBasicWidget*>(m_obj);

    switch(id())
    {
    case (int)DrawBasicWidgetId::TextColor:
        obj->setTextColor(value.value<QColor>());
        break;
    case (int)DrawBasicWidgetId::BackColor:
        obj->setBackColor(value.value<QColor>());
        break;
    case (int)DrawBasicWidgetId::DisableBackColor:
        obj->setDisableBackColor(value.value<QColor>());
        break;
    case (int)DrawBasicWidgetId::Font:
        obj->setFont(reinterpret_cast<QFont*>(value.toULongLong()));
        break;
    case (int)DrawBasicWidgetId::Enable:
        obj->setEnable(value.toBool());
        break;
    default:
        break;
    }
}
