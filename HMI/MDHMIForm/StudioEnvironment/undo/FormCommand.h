/*!
\author aGRw 2015.11.12
\section 窗体，Undo指令类
*/
#ifndef FORMCOMMAND_H
#define FORMCOMMAND_H

#include <QUndoCommand>
#include <QVariant>
#include "DrawObjDefine.h"

class CMDHMIForm;
class CStudio;

enum class FormId
{
    Rect = (int)DrawType::Form << 16,
    Brush,
    Layer,
    MouseDownAction,
    MouseUpAction,
    RightMosueDownAction,
    RightMouseUpAction,
    OpenAction,
    CloseAction,
    Text,
    FormType,
    Security,
};

class CFormCommand : public QUndoCommand
{
public:
    CFormCommand(CMDHMIForm *obj, const QVariant& oldValue, const QVariant& newValue,
        int id, QUndoCommand *parent = NULL);
    virtual ~CFormCommand();
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
    CMDHMIForm* m_form;
    CStudio* m_studio;
    int m_id;
};

#endif // FORMCOMMAND_H


