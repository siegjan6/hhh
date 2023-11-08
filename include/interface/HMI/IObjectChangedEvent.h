/*!
\author aGRw 2015.03.20
\section 窗体中对象发生变化后的事件集合,通知类
*/
#ifndef IOBJECTCHANGEDEVENT_H
#define IOBJECTCHANGEDEVENT_H

#include <QList>

class CMDDrawObj;

class IObjectChangedEvent
{
public:
    virtual ~IObjectChangedEvent(){}
public:
    virtual void add(QList<CMDDrawObj*>* list) = 0;
    virtual void dele(QList<CMDDrawObj*>* list) = 0;
    virtual void group(CMDDrawObj* obj) = 0;
    virtual void unGroup(CMDDrawObj* obj) = 0;
    virtual void combine(CMDDrawObj* obj) = 0;
    virtual void unCombine(CMDDrawObj* obj) = 0;

    virtual void top(CMDDrawObj* obj) = 0;
    virtual void last(CMDDrawObj* obj) = 0;
    virtual void front(CMDDrawObj* obj) = 0;
    virtual void back(CMDDrawObj* obj) = 0;

    virtual void select(QList<CMDDrawObj*>* list) = 0;

    virtual void nameChanged(CMDDrawObj* obj) = 0;

    virtual void reset() = 0;
};

#endif // IOBJECTCHANGEDEVENT_H
