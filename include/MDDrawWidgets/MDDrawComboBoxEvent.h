/*!
\author bWVpeGw=

\section 功能

组合框控件，事件绑定类
*/
#ifndef MDDRAWCOMBOBOXEVENT_H
#define MDDRAWCOMBOBOXEVENT_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>
#include "MDDrawBasicWidgetEvent.h"
class CMDDrawComboBox;

class MDDRAWWIDGETSSHARED_EXPORT CMDDrawComboBoxEvent : public CMDDrawBasicWidgetEvent
{
    Q_OBJECT
public:
    CMDDrawComboBoxEvent();
    virtual ~CMDDrawComboBoxEvent(){}
/*!
\section  绑定相关事件
*/
public:
    void connectEvent();  

private:
    void writeVariable();

//event
private slots:
    void currentIndexChanged(int index);
    void textChanged(QString str);
};

#endif//MDDRAWCOMBOBOXEVENT_H
