/*!
\author bWVpeGw=

\section 功能

时间日期控件，事件绑定类
*/
#ifndef MDDRAWDATETIMEEDITEVENT_H
#define MDDRAWDATETIMEEDITEVENT_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include "MDDrawBasicWidgetEvent.h"

class CMDDrawDateTimeEdit;

class MDDRAWWIDGETSSHARED_EXPORT CMDDrawDateTimeEditEvent : public CMDDrawBasicWidgetEvent
{
    Q_OBJECT
public:
    CMDDrawDateTimeEditEvent();
    virtual ~CMDDrawDateTimeEditEvent(){}

/*!
\section 绑定时间、日期变化事件
*/
public:
    void connectEvent();

private:
    void dateTimeChanged(const QDateTime &datetime);
    void writeVariable();
};

#endif // MDDRAWDATETIMEEDITEVENT_H

