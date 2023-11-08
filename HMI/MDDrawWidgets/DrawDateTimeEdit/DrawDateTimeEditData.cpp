#include "DrawDateTimeEditData.h"

CDrawDateTimeEditData::CDrawDateTimeEditData()
{
    m_widget = NULL;
    m_event = NULL;

    m_showType = 0;
    m_calendarPop = true;
}

void CDrawDateTimeEditData::operator=(const CDrawDateTimeEditData &other)
{
    m_showType = other.m_showType;
    m_calendarPop = other.m_calendarPop;
}

void CDrawDateTimeEditData::serialize(QJsonObject &json)
{
    json.insert("ShowType",m_showType);
    json.insert("CalendarPop",m_calendarPop);
}

void CDrawDateTimeEditData::deserialize(const QJsonObject &json)
{
    m_showType = json.value("ShowType").toInt();
    m_calendarPop = json.value("CalendarPop").toBool();
}

