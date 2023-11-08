#ifndef DRAWRADIOBUTTONCOMMAND_H
#define DRAWRADIOBUTTONCOMMAND_H

#include "MDDrawObjCommand.h"

enum class DrawRadioButtonId
{
    Text = (int)DrawType::RadioButton << 16,
    Checked
};

class DrawRadioButtonCommand : public CMDDrawObjCommand
{
public:
    DrawRadioButtonCommand(CMDDrawObj *obj, const QVariant& oldValue, const QVariant& newValue,
                           int id, QUndoCommand *parent = NULL);
    virtual ~DrawRadioButtonCommand();

public:
    bool mergeWith(const QUndoCommand *command);
protected:
    void setValue(const QVariant& value) ;
};

#endif // DRAWRADIOBUTTONCOMMAND_H
