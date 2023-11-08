#ifndef DRAWDATETIMECOMMAND_H
#define DRAWDATETIMECOMMAND_H

#include "MDDrawObjCommand.h"

enum class DrawDateTimeId
{
    ShowType = (int)DrawType::DateTime << 16,
    CalendarPop
};

class DrawDateTimeCommand : public CMDDrawObjCommand
{
public:
    DrawDateTimeCommand(CMDDrawObj *obj, const QVariant& oldValue, const QVariant& newValue,
                        int id, QUndoCommand *parent = NULL);
    virtual ~DrawDateTimeCommand();

public:
    bool mergeWith(const QUndoCommand *command);
protected:
    void setValue(const QVariant& value) ;
};

#endif // DRAWDATETIMECOMMAND_H
