/*!
\author Y2hhaWhq

\section 功能

日历控件脚本类
*/
#ifndef MDDRAWCALENDARWIDGETWRAPPER_H
#define MDDRAWCALENDARWIDGETWRAPPER_H

#include <QVariant>
#include <QDate>
#include "MDDrawWidgetWrapper.h"
class CMDDrawCalendarWidget;

class MDHMIFORMSHARED_EXPORT CMDDrawCalendarWidgetWrapper : public CMDDrawWidgetWrapper
{
    Q_OBJECT

    Q_PROPERTY(int year READ year WRITE setYear)
    Q_PROPERTY(int month READ month WRITE setMonth)
    Q_PROPERTY(int day READ day WRITE setDay)
    Q_PROPERTY(QVariant date READ date WRITE setDate)
    Q_PROPERTY(QVariant maxDate READ maxDate WRITE setMaxDate)
    Q_PROPERTY(QVariant minDate READ minDate WRITE setMinDate)

public:
    CMDDrawCalendarWidgetWrapper(void* object, const QString& name);
    virtual ~CMDDrawCalendarWidgetWrapper(){}

 //! 属性
public Q_SLOTS:
    //! 年
     int year();
     void setYear(int value);
    //! 月
     int month();
     void setMonth(int value);
    //! 日
    int day();
     void setDay(int value);
    //! 日期
     QVariant date();
     void setDate(const QVariant &value);
    //! 日期上限
    QVariant maxDate();
     void setMaxDate(const QVariant &value);
    //! 日期下限
    QVariant minDate();
     void setMinDate(const QVariant &value);

public:
    CMDDrawCalendarWidget* object();
};

#endif // MDDRAWCALENDARWIDGETWRAPPER_H
