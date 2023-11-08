#include "DrawCalendarWidgetData.h"

CDrawCalendarWidgetData::CDrawCalendarWidgetData()
{
    m_widget = NULL;
    m_event = NULL;

    m_weekNum = false;
    m_currentDateHighlight = false;
    m_backToToday = false;
    m_showToday = false;
    m_gridVisible = false;
    m_firstDayofWeek = Qt::Sunday;
}

void CDrawCalendarWidgetData::operator =(const CDrawCalendarWidgetData &other)
{
    m_weekNum = other.m_weekNum;
    m_gridVisible = other.m_gridVisible;
    m_backToToday = other.m_backToToday;
    m_currentDateHighlight = other.m_currentDateHighlight;
    m_firstDayofWeek = other.m_firstDayofWeek;
    m_showToday = other.m_showToday;
}

void CDrawCalendarWidgetData::serialize(QJsonObject &json)
{
    json.insert("WeekNum",m_weekNum);
    json.insert("CurrentDateHighlight",m_currentDateHighlight);
    json.insert("BackToToday",m_backToToday);
    json.insert("ShowToday",m_showToday);
    json.insert("GridVisible",m_gridVisible);
    json.insert("FirstDayofWeek",m_firstDayofWeek);
}

void CDrawCalendarWidgetData::deserialize(const QJsonObject &json)
{
    m_weekNum = json.value("WeekNum").toBool();
    m_currentDateHighlight = json.value("CurrentDateHighlight").toBool();
    m_backToToday = json.value("BackToToday").toBool();
    m_showToday = json.value("ShowToday").toBool();
    m_gridVisible = json.value("GridVisible").toBool();
    m_firstDayofWeek = (Qt::DayOfWeek)json.value("FirstDayofWeek").toInt();
}

