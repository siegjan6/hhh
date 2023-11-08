#ifndef DRAWLISTCOMMAND_H
#define DRAWLISTCOMMAND_H

#include "MDDrawObjCommand.h"

enum class DrawListId
{
    HasScrollBar = (int)DrawType::List << 16,
    LayoutDirection,
    BorderStyle,
    SelectionMode,
    Sorted
};

class DrawListCommand : public CMDDrawObjCommand
{
public:
    DrawListCommand(CMDDrawObj *obj, const QVariant& oldValue, const QVariant& newValue,
                    int id, QUndoCommand *parent = NULL);
    virtual ~DrawListCommand();

public:
    bool mergeWith(const QUndoCommand *command);
protected:
    void setValue(const QVariant& value) ;
};

#endif // DRAWLISTCOMMAND_H
