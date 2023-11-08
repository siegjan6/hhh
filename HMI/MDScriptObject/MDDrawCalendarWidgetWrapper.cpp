#include "MDDrawCalendarWidgetWrapper.h"
#include "MDDrawCalendarWidget.h"

CMDDrawCalendarWidgetWrapper::CMDDrawCalendarWidgetWrapper(void *object, const QString &name)
    :CMDDrawWidgetWrapper(object, name)
{
}

int CMDDrawCalendarWidgetWrapper::year()
{
    return object()->getYear();
}

void CMDDrawCalendarWidgetWrapper::setYear(int value)
{
    object()->setYear(value);
}

int CMDDrawCalendarWidgetWrapper::month()
{
    return object()->getMonth();
}

void CMDDrawCalendarWidgetWrapper::setMonth(int value)
{
    object()->setMonth(value);
}

int CMDDrawCalendarWidgetWrapper::day()
{
    return object()->getDay();
}

void CMDDrawCalendarWidgetWrapper::setDay(int value)
{
    object()->setDay(value);
}

QVariant CMDDrawCalendarWidgetWrapper::date()
{
    return QDateTime(object()->getScriptDate()).toMSecsSinceEpoch();
}

void CMDDrawCalendarWidgetWrapper::setDate(const QVariant &value)
{
    object()->setScriptDate(QDateTime::fromTime_t(value.toLongLong()/1000).date());
}

QVariant CMDDrawCalendarWidgetWrapper::maxDate()
{
    return QDateTime(object()->maxDate()).toMSecsSinceEpoch();
}

void CMDDrawCalendarWidgetWrapper::setMaxDate(const QVariant &value)
{
    object()->setMaxDate(QDateTime::fromTime_t(value.toLongLong()/1000).date());
}

QVariant CMDDrawCalendarWidgetWrapper::minDate()
{
    return QDateTime(object()->maxDate()).toMSecsSinceEpoch();
}

void CMDDrawCalendarWidgetWrapper::setMinDate(const QVariant &value)
{
    object()->setMinDate(QDateTime::fromTime_t(value.toLongLong()/1000).date());
}

CMDDrawCalendarWidget *CMDDrawCalendarWidgetWrapper::object()
{
    return reinterpret_cast<CMDDrawCalendarWidget*>(MDObject());
}
