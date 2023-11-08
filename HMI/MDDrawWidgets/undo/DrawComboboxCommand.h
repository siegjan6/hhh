#ifndef DRAWCOMBOBOXCOMMAND_H
#define DRAWCOMBOBOXCOMMAND_H

#include "MDDrawObjCommand.h"

enum class DrawComboboxId
{
    LayoutDirection = (int)DrawType::ComboBox << 16,
    ItemStyle,
    Sorted
};

class DrawComboboxCommand : public CMDDrawObjCommand
{
public:
    DrawComboboxCommand(CMDDrawObj *obj, const QVariant& oldValue, const QVariant& newValue,
                        int id, QUndoCommand *parent = NULL);
    virtual ~DrawComboboxCommand();

public:
    bool mergeWith(const QUndoCommand *command);
protected:
    void setValue(const QVariant& value) ;
};

#endif // DRAWCOMBOBOXCOMMAND_H
