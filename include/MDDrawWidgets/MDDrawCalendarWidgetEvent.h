/*!
\author bWVpeGw=

\section 功能

日历控件事件绑定类
*/
#ifndef MDDRAWCALENDARWIDGETEVENT_H
#define MDDRAWCALENDARWIDGETEVENT_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include <QEvent>
#include "MDDrawWidgetEvent.h"

class CMDDrawCalendarWidget;

class MDDRAWWIDGETSSHARED_EXPORT CMDDrawCalendarWidgetEvent : public CMDDrawWidgetEvent
{
    Q_OBJECT
public:
    CMDDrawCalendarWidgetEvent();
    virtual ~CMDDrawCalendarWidgetEvent(){}

/*!
\section 绑定日期变化事件
*/
public:
    void connectEvent();
private:
    void dateChanged(const QDate &date);
    void writeVariable();
};

#endif // MDDRAWCALENDARWIDGETEVENT_H

