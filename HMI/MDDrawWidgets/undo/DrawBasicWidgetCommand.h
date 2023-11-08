#ifndef DRAWBASICWIDGETCOMMAND_H
#define DRAWBASICWIDGETCOMMAND_H

#include "MDDrawObjCommand.h"

enum class DrawBasicWidgetId
{
    TextColor = (int)DrawType::Widget << 16,
    BackColor,
    DisableBackColor,
    Font,
    Enable
};

class CDrawBasicWidgetCommand : public CMDDrawObjCommand
{
public:
    CDrawBasicWidgetCommand(CMDDrawObj *obj, const QVariant& oldValue, const QVariant& newValue,
                            int id, QUndoCommand *parent = NULL);
    virtual ~CDrawBasicWidgetCommand();

public:
    bool mergeWith(const QUndoCommand *command);
protected:
    void setValue(const QVariant& value) ;
};

#endif // DRAWBASICWIDGETCOMMAND_H
