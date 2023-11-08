/*!
\author bWVpeGw=

\section 功能

微调按钮，事件绑定类
*/

#ifndef MDDRAWSPINBOXEVENT_H
#define MDDRAWSPINBOXEVENT_H

#include <QtCore/qglobal.h>
#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include "MDDrawBasicWidgetEvent.h"
class CMDDrawSpinBox;

class MDDRAWWIDGETSSHARED_EXPORT CMDDrawSpinBoxEvent : public CMDDrawBasicWidgetEvent
{
    Q_OBJECT
public:
    CMDDrawSpinBoxEvent();
    virtual ~CMDDrawSpinBoxEvent(){}

/*!
\section 绑定变量改变事件
*/
public:
    void connectEvent();

private slots:
    void valueChanged(int value);

private:
    void writeVariable();
};

#endif // MDDRAWSPINBOXEVENT_H

