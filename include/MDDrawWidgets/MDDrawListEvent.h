/*!
\author bWVpeGw=

\section 功能

列表控件，事件绑定类
*/
#ifndef MDDRAWLISTEVENT_H
#define MDDRAWLISTEVENT_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include "MDDrawBasicWidgetEvent.h"
class CMDDrawList;
class MDDRAWWIDGETSSHARED_EXPORT CMDDrawListEvent : public CMDDrawBasicWidgetEvent
{
    Q_OBJECT
public:
    CMDDrawListEvent();
    virtual ~CMDDrawListEvent(){}

/*!
\section  绑定当前索引改变事件
*/
public:
    void connectEvent();

private:
    void currentIndexChanged(int value);
};
#endif // MDDRAWLISTEVENT_H
