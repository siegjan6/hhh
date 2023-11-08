/*!
\author Y2hhaWhq

\section 功能

时间日期控件脚本类
*/
#ifndef MDDRAWDATETIMEEDITWRAPPER_H
#define MDDRAWDATETIMEEDITWRAPPER_H

#include <QVariant>
#include "MDDrawWidgetWrapper.h"
class CMDDrawDateTimeEdit;

class MDHMIFORMSHARED_EXPORT CMDDrawDateTimeEditWrapper : public CMDDrawWidgetWrapper
{
    Q_OBJECT

    Q_PROPERTY(int year READ year WRITE setYear)
    Q_PROPERTY(int month READ month WRITE setMonth)
    Q_PROPERTY(int day READ day WRITE setDay)
    Q_PROPERTY(int hour READ hour WRITE setHour)
    Q_PROPERTY(int minute READ minute WRITE setMinute)
    Q_PROPERTY(int second READ second WRITE setSecond)
    Q_PROPERTY(QVariant time READ time WRITE setTime)
    Q_PROPERTY(QVariant maxDateTime READ maxDateTime WRITE setMaxDateTime)
    Q_PROPERTY(QVariant minDateTime READ minDateTime WRITE setMinDateTime)

public:
    CMDDrawDateTimeEditWrapper(void* object, const QString& name);
    virtual ~CMDDrawDateTimeEditWrapper(){}

//!  property
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
    //! 时
    int hour();
    void setHour(int value);
    //! 分
    int minute();
    void setMinute(int value);
    //! 秒
    int second();
    void setSecond(int value);
    //! 日期时间
    QVariant time();
    void setTime(const QVariant& value);
    //! 日期时间上限
    QVariant maxDateTime();
    void setMaxDateTime(const QVariant &value);
    //! 日期时间下限
    QVariant minDateTime();
    void setMinDateTime(const QVariant& value);

public:
    CMDDrawDateTimeEdit* object();
};

#endif // MDDRAWDATETIMEEDITWRAPPER_H
