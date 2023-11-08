/*!
\author aGRw 2015.11.10
\section Undo指令类
*/
#ifndef MDDRAWOBJCOMMAND_H
#define MDDRAWOBJCOMMAND_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWOBJ_LIBRARY)
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QUndoCommand>
#include <QVariant>
#include "DrawObjDefine.h"

class CMDDrawObj;

enum class DrawObjId
{
    Name = (int)DrawType::Obj << 16,
    Rect,
    Layer,
    Variable,
    SupportGroupEvent,
    ShowSelectedFrame,
};

class MDDRAWOBJSHARED_EXPORT CMDDrawObjCommand : public QUndoCommand
{
public:
    CMDDrawObjCommand(CMDDrawObj *obj, const QVariant& oldValue, const QVariant& newValue,
        int id, QUndoCommand *parent = NULL);
    virtual ~CMDDrawObjCommand();
public:
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand *command);
    int id() const{return m_id;}
protected:
    virtual void setValue(const QVariant& value);
protected:
    QVariant m_oldValue;
    QVariant m_newValue;
    CMDDrawObj* m_obj;
private:
    int m_id;
};

class CMDDrawVector;

enum class DrawVectorId
{
    Angle = (int)DrawType::Vector << 16,
    RotatePoint,
    RotatePointPos,
    Shear,
    InputVariable,

    Brush,
    Pen,
    DynamicBrush,
    DynamicPen,
    Fill,

    MouseDownAction,
    MouseUpAction,
    RightMouseDownAction,
    RightMouseUpAction,
};

class MDDRAWOBJSHARED_EXPORT CMDDrawVectorCommand : public CMDDrawObjCommand
{
public:
    CMDDrawVectorCommand(CMDDrawObj *obj, const QVariant& oldValue, const QVariant& newValue,
        int id, QUndoCommand *parent = NULL);
    virtual ~CMDDrawVectorCommand();
public:
    bool mergeWith(const QUndoCommand *command);
protected:
    void setValue(const QVariant& value) ;
};


class CMDDrawGroup;

enum class DrawGroupId
{
    CustomProperty = (int)DrawType::Group << 16,
};

class MDDRAWOBJSHARED_EXPORT CMDDrawGroupCommand : public CMDDrawVectorCommand
{
public:
    CMDDrawGroupCommand(CMDDrawObj *obj, const QVariant& oldValue, const QVariant& newValue,
        int id, QUndoCommand *parent = NULL);
    virtual ~CMDDrawGroupCommand();
public:
    bool mergeWith(const QUndoCommand *command);
protected:
    void setValue(const QVariant& value) ;
};


enum class MouseOperateId
{
    Mouse = ((int)DrawType::Multi << 16) + 1000,
};

class MDDRAWOBJSHARED_EXPORT CMDMouseOperateCommand : public QUndoCommand
{
public:
    CMDMouseOperateCommand(const QVariant& oldValue, const QVariant& newValue,
        int id, QUndoCommand *parent = NULL);
    virtual ~CMDMouseOperateCommand();
public:
    void undo();
    void redo();
    bool mergeWith(const QUndoCommand *command);
    int id() const{return m_id;}
private:
    void setValue(const QVariant& value);
private:
    QVariant m_oldValue;
    QVariant m_newValue;
    int m_id;
};

#endif // MDDRAWOBJCOMMAND_H
