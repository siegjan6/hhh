#ifndef DRAWCHECKBOXCOMMAND_H
#define DRAWCHECKBOXCOMMAND_H

#include "MDDrawObjCommand.h"

enum class DrawCheckBoxId
{
    Text = (int)DrawType::RadioButton << 16,
    Checked
};

class DrawCheckBoxCommand : public CMDDrawObjCommand
{
public:
    DrawCheckBoxCommand(CMDDrawObj *obj, const QVariant& oldValue, const QVariant& newValue,
                        int id, QUndoCommand *parent = NULL);
    virtual ~DrawCheckBoxCommand();

public:
    bool mergeWith(const QUndoCommand *command);
protected:
    void setValue(const QVariant& value) ;
};

#endif // DRAWCHECKBOXCOMMAND_H
