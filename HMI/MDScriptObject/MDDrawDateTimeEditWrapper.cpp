#include "MDDrawDateTimeEditWrapper.h"
#include "MDDrawDateTimeEdit.h"

CMDDrawDateTimeEditWrapper::CMDDrawDateTimeEditWrapper(void *object, const QString &name)
    :CMDDrawWidgetWrapper(object, name)
{
}

int CMDDrawDateTimeEditWrapper::year()
{
    return object()->getYear();
}

void CMDDrawDateTimeEditWrapper::setYear(int value)
{
    object()->setYear(value);
}

int CMDDrawDateTimeEditWrapper::month()
{
    return object()->getMonth();
}

void CMDDrawDateTimeEditWrapper::setMonth(int value)
{
    object()->setMonth(value);
}

int CMDDrawDateTimeEditWrapper::day()
{
    return object()->getDay();
}

void CMDDrawDateTimeEditWrapper::setDay(int value)
{
    object()->setDay(value);
}

int CMDDrawDateTimeEditWrapper::hour()
{
    return object()->getHour();
}

void CMDDrawDateTimeEditWrapper::setHour(int value)
{
    object()->setHour(value);
}

int CMDDrawDateTimeEditWrapper::minute()
{
    return object()->getMinute();
}

void CMDDrawDateTimeEditWrapper::setMinute(int value)
{
    object()->setMinute(value);
}

int CMDDrawDateTimeEditWrapper::second()
{
    return object()->getSecond();
}

void CMDDrawDateTimeEditWrapper::setSecond(int value)
{
    object()->setSecond(value);
}

QVariant CMDDrawDateTimeEditWrapper::time()
{
    return QVariant(object()->getTime().toMSecsSinceEpoch());
}

void CMDDrawDateTimeEditWrapper::setTime(const QVariant &value)
{
    object()->setTime(QDateTime::fromTime_t(value.toLongLong()/1000));
}

QVariant CMDDrawDateTimeEditWrapper::maxDateTime()
{
    return QVariant(object()->maxDateTime().toMSecsSinceEpoch());
}

void CMDDrawDateTimeEditWrapper::setMaxDateTime(const QVariant &value)
{
    object()->setMaxDateTime(QDateTime::fromTime_t(value.toLongLong()/1000));
}

QVariant CMDDrawDateTimeEditWrapper::minDateTime()
{
    return QVariant(object()->maxDateTime().toMSecsSinceEpoch());
}

void CMDDrawDateTimeEditWrapper::setMinDateTime(const QVariant &value)
{
    object()->setMinDateTime(QDateTime::fromTime_t(value.toLongLong()/1000));
}

CMDDrawDateTimeEdit *CMDDrawDateTimeEditWrapper::object()
{
    return reinterpret_cast<CMDDrawDateTimeEdit*>(MDObject());
}
