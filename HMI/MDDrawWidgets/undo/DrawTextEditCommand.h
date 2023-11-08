#ifndef DRAWTEXTEDITCOMMAND_H
#define DRAWTEXTEDITCOMMAND_H

#include "MDDrawObjCommand.h"

enum class DrawTextEditId
{
    HScrollBar = (int)DrawType::TextEdit << 16,
    VScrollBar,
    BorderStyle,
    LineWrap,
    ReadOnly,
    Alignment,
    Text,
    WriteVarEnable,
    EnterSetVarEnable,
    HasPassword
};


class DrawTextEditCommand : public CMDDrawObjCommand
{
public:
    DrawTextEditCommand(CMDDrawObj *obj, const QVariant& oldValue, const QVariant& newValue,
                        int id, QUndoCommand *parent = NULL);
    virtual ~DrawTextEditCommand();

public:
    bool mergeWith(const QUndoCommand *command);
protected:
    void setValue(const QVariant& value) ;
};

#endif // DRAWTEXTEDITCOMMAND_H
