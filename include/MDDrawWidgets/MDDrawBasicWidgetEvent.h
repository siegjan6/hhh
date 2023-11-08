/*!
\author bWVpeGw=
\section 脚本触发事件
*/
#ifndef CMDDRAWBASICWIDGETEVENT_H
#define CMDDRAWBASICWIDGETEVENT_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif
#include <QObject>
#include "MDDrawWidgetEvent.h"
#include <QMouseEvent>

class CMDDrawBasicWidget;
class MDDRAWWIDGETSSHARED_EXPORT CMDDrawBasicWidgetEvent : public CMDDrawWidgetEvent
{
     Q_OBJECT

public:
    CMDDrawBasicWidgetEvent();
    virtual ~CMDDrawBasicWidgetEvent(){}

public:
    void connectEvent();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    void onMouseDown(int x, int y);
    void onMouseUp(int x, int y);
    void onMouseMove(int x, int y);
    void onMouseDoubleClick(int x, int y);
    void onGotFocus();
    void onLostFocus();
    void onKeyDown(int key,int isModifiers);
    void onKeyUp(int key,int isModifiers);

    int modifierTypes(Qt::KeyboardModifiers modifiers);
};
#endif // CMDDRAWBASICWIDGETEVENT_H
