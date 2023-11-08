/*!
\author bWVpeGw=

\section 功能

单选按钮控件，事件绑定类
*/
#ifndef MDDRAWRADIOBUTTONEVENT_H
#define MDDRAWRADIOBUTTONEVENT_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include "MDDrawBasicWidgetEvent.h"
class CMDDrawRadioButton;

class MDDRAWWIDGETSSHARED_EXPORT CMDDrawRadioButtonEvent : public CMDDrawBasicWidgetEvent
{
    Q_OBJECT
public:
    CMDDrawRadioButtonEvent();
    virtual ~CMDDrawRadioButtonEvent(){}

/*!
\section  绑定选中状态事件
*/
public:
    void connectEvent();
private:
    void checkChanged(bool value);
/*!
\section  绑定鼠标事件
*/
public:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    void sendMouseEvent(QEvent *event);
};

#endif // MDDRAWRADIOBUTTONEVENT_H
