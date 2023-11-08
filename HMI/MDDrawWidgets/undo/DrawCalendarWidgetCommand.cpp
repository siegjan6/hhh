#include "DrawCalendarWidgetCommand.h"
#include "MDDrawCalendarWidget.h"

DrawCalendarWidgetCommand::DrawCalendarWidgetCommand(CMDDrawObj *obj, const QVariant &oldValue, const QVariant &newValue, int id, QUndoCommand *parent):
    CMDDrawObjCommand(obj, oldValue, newValue, id, parent)
{

}

DrawCalendarWidgetCommand::~DrawCalendarWidgetCommand()
{

}

bool DrawCalendarWidgetCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != id())
        return false;

    const DrawCalendarWidgetCommand *other = static_cast<const DrawCalendarWidgetCommand*>(command);
    if (other->m_obj != m_obj)
        return false;

    m_newValue = other->m_newValue;
    return true;
}

void DrawCalendarWidgetCommand::setValue(const QVariant &value)
{
    CMDDrawCalendarWidget* obj = dynamic_cast<CMDDrawCalendarWidget*>(m_obj);

    switch(id())
    {
    case (int)DrawCalendarWidgetId::WeekNum:
        obj->setWeekNum(value.toBool());
        break;
    case (int)DrawCalendarWidgetId::CurrentDateHighlight:
        obj->setCurrentDateHighlight(value.toBool());
        break;
    case (int)DrawCalendarWidgetId::BackToToday:
        obj->setBackToToday(value.toBool());
        break;
    case (int)DrawCalendarWidgetId::CurrentDate:
        obj->setCurrentDate(value.toBool());
        break;
    case (int)DrawCalendarWidgetId::GridVisible:
        obj->setGridVisible(value.toBool());
        break;
    case (int)DrawCalendarWidgetId::FirstDayofWeek:
        obj->setFirstDayofWeek((Qt::DayOfWeek)value.toInt());
        break;
    case (int)DrawCalendarWidgetId::Enable:
        obj->setEnable(value.toBool());
        break;
    default:
        break;
    }
}

