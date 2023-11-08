/*!
\author bWVpeGw=

\section 功能

复选框控件事件绑定类
*/
#ifndef MDDRAWCHECKBOXEVENT_H
#define MDDRAWCHECKBOXEVENT_H

#include <QtCore/qglobal.h>
/*!
\author luyj
\section 功能
复选框控件，事件绑定类
*/
#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include "MDDrawBasicWidgetEvent.h"
class CMDDrawCheckBox;

class MDDRAWWIDGETSSHARED_EXPORT CMDDrawCheckBoxEvent : public CMDDrawBasicWidgetEvent
{
    Q_OBJECT
public:
    CMDDrawCheckBoxEvent();
    virtual ~CMDDrawCheckBoxEvent(){}

/*!
\section 绑定选中状态改变事件
*/
public:
    void connectEvent();
private:
    void checkChanged(int value);

/*!
\section 绑定鼠标事件
*/
public:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    void sendMouseEvent(QEvent *event);
    void writeVariable();
};

#endif // MDDRAWCHECKBOXEVENT_H

