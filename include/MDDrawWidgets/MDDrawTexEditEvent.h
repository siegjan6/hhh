/*!
\author bWVpeGw=

\section 功能

编辑框控件,事件绑定类
*/
#ifndef MDDRAWTEXEDITEVENT_H
#define MDDRAWTEXEDITEVENT_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include "MDDrawBasicWidgetEvent.h"

class QLineEdit;
class CMDDrawTextEdit;
class CDrawTextEditData;
class MDDRAWWIDGETSSHARED_EXPORT CMDDrawTexEditEvent : public CMDDrawBasicWidgetEvent
{
    Q_OBJECT
public:
    CMDDrawTexEditEvent();
    virtual ~CMDDrawTexEditEvent();

/*!
\section  绑定文本变化事件
*/
public:
    void connectEvent();
private slots:
    void textChanged();

/*!
\section  绑定写变量事件
*/
protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    CMDDrawTextEdit* w;
    QLineEdit *m_lineEdit;
};

#endif // DRAWTEXEDITEVENT_H
