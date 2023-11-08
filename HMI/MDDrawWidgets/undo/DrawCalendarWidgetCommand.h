#ifndef DRAWCALENDARWIDGETCOMMAND_H
#define DRAWCALENDARWIDGETCOMMAND_H

#include "MDDrawObjCommand.h"

enum class DrawCalendarWidgetId
{
    WeekNum = (int)DrawType::Calendar << 16,
    CurrentDateHighlight,
    BackToToday,
    CurrentDate,
    GridVisible,
    FirstDayofWeek,
    Enable
};

class DrawCalendarWidgetCommand : public CMDDrawObjCommand
{
public:
    DrawCalendarWidgetCommand(CMDDrawObj *obj, const QVariant& oldValue, const QVariant& newValue,
                              int id, QUndoCommand *parent = NULL);
    virtual ~DrawCalendarWidgetCommand();

public:
    bool mergeWith(const QUndoCommand *command);
protected:
    void setValue(const QVariant& value) ;
};

#endif // DRAWCALENDARWIDGETCOMMAND_H
