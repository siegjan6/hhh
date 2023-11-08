/*!
\author aGRw 2015.12.09
\section Undo指令类
*/
#ifndef DRAWPIPECOMMAND_H
#define DRAWPIPECOMMAND_H

#include "MDDrawObjCommand.h"

enum class DrawPipeId
{
    PipeVisible,
    PipeWidth,
    PipeBackColor,
    PipeHighColor,
    StreamLineVisible,
    StreamLineFlow,
    StreamPen
};

class CDrawPipeCommand : public CMDDrawVectorCommand
{
public:
    CDrawPipeCommand(CMDDrawObj *obj, const QVariant& oldValue, const QVariant& newValue,
        int id, QUndoCommand *parent = NULL);
    virtual ~CDrawPipeCommand();
public:
    bool mergeWith(const QUndoCommand *command);
protected:
    void setValue(const QVariant& value) ;
};

#endif // DRAWPIPECOMMAND_H



