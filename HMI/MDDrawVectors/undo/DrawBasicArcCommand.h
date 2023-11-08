#ifndef DRAWBASICARCCOMMAND_H
#define DRAWBASICARCCOMMAND_H

#include "MDDrawObjCommand.h"

enum class DrawBasicArcId
{
    StartAngle = (int)DrawType::Arc << 16,
    SpanAngle,
};

class CDrawBasicArcCommand : public CMDDrawVectorCommand
{
public:
    CDrawBasicArcCommand(CMDDrawObj *obj, const QVariant& oldValue, const QVariant& newValue,
        int id, QUndoCommand *parent = NULL);
    virtual ~CDrawBasicArcCommand(){}
public:
    bool mergeWith(const QUndoCommand *command);
protected:
    void setValue(const QVariant& value) ;
};

#endif // DRAWBASICARCCOMMAND_H

