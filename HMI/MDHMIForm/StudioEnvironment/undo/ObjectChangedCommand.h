/*!
\author aGRw 2015.11.18
\section 控件添加删除，Undo指令类
*/
#ifndef OBJECTCHANGEDCOMMAND_H
#define OBJECTCHANGEDCOMMAND_H

#include <QUndoCommand>
#include "DrawObjDefine.h"

class CMDDrawObj;
class CStudio;

enum class ObjectChangedId
{
    Add = ((int)DrawType::Multi << 16) + 2000,
    Delete,
    Group,
    UnGroup,
    Combine,
    UnCombine,
    Index,
};

class CObjectChangedCommand : public QUndoCommand
{
public:
    CObjectChangedCommand(QList<CMDDrawObj*>* objs, QList<int>* indexs, CStudio* container, int id,
        QUndoCommand *parent = NULL);
    virtual ~CObjectChangedCommand(){}
public:
    void undo();
    void redo();
    int id() const{return m_id;}
private:
    QList<CMDDrawObj*> m_objs;
    QList<int> m_indexs;
    CStudio* m_container;
    int m_id;
};

#endif // OBJECTCHANGEDCOMMAND_H








