#ifndef DRAWTEXTCOMMAND_H
#define DRAWTEXTCOMMAND_H

#include "MDDrawObjCommand.h"

enum class DrawTextId
{
    Text = (int)DrawType::Text << 16,
    TextBrush,
    Font,
    HAlignment,
    VAlignment,
    LineWrap,
    ButtonFrame,
    TextFormat,
};

class CDrawTextCommand : public CMDDrawVectorCommand
{
public:
    CDrawTextCommand(CMDDrawObj *obj, const QVariant& oldValue, const QVariant& newValue,
        int id, QUndoCommand *parent = NULL);
    virtual ~CDrawTextCommand();
public:
    bool mergeWith(const QUndoCommand *command);
protected:
    void setValue(const QVariant& value) ;
};

#endif // DRAWTEXTCOMMAND_H






