#include "DrawAlarmData.h"

CDrawAlarmData::CDrawAlarmData()
{
}

void CDrawAlarmData::operator =(/*const*/ CDrawAlarmData &other)
{
    m_alarm = other.m_alarm;
}


void CDrawAlarmData::serialize(QJsonObject &json)
{
    m_alarm.serialize(json);
}

void CDrawAlarmData::deserialize(const QJsonObject &json)
{
    m_alarm.deserialize(json);
}

