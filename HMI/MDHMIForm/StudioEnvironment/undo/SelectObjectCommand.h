/*!
\author aGRw 2015.11.12
\section 多个选择控件，Undo指令类
*/
#ifndef SELECTOBJECTCOMAND_H
#define SELECTOBJECTCOMAND_H

#include <QUndoCommand>
#include <QVariant>
#include "DrawObjDefine.h"

class CSelectObjectManager;
class CMDDrawObj;

enum class SelectObjectId
{
    Rect = ((int)DrawType::Multi << 16),
};

class CSelectObjectCommand : public QUndoCommand
{
public:
    CSelectObjectCommand(CSelectObjectManager *obj, const QVariant& oldValue, const QVariant& newValue,
        int id, QUndoCommand *parent = NULL);
    virtual ~CSelectObjectCommand(){}
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
    CSelectObjectManager* m_obj;
    QList<CMDDrawObj*> m_list;
    int m_id;
};

#endif // SELECTOBJECTCOMAND_H



