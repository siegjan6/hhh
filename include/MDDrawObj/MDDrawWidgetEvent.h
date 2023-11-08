/*!
\author aGRw 2015.04.15
\section CMDDrawWidget的事件处理类
*/
#ifndef MDDRAWWIDGETEVENT_H
#define MDDRAWWIDGETEVENT_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWOBJ_LIBRARY)
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>

class CMDDrawWidget;

class MDDRAWOBJSHARED_EXPORT CMDDrawWidgetEvent : public QObject
{
    Q_OBJECT
public:
    CMDDrawWidgetEvent();
    virtual ~CMDDrawWidgetEvent(){}
public:
    CMDDrawWidget* parent();
    void setParent(CMDDrawWidget* parent);
//////////////////////////////////////////////////////////////////////////
public:
    bool	eventFilter(QObject* watched, QEvent* event);
    virtual void connectEvent(){}
//////////////////////////////////////////////////////////////////////////
private:
    CMDDrawWidget* m_parent;

};

#endif // MDDRAWWIDGETEVENT_H
