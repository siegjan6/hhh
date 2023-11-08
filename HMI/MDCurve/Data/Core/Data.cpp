#include "Data.h"

QList<CPureValuePoint> &CCurveData::valueList()
{
    return m_valueList;
}

void CCurveData::setValueList(const QList<CPureValuePoint> &valueList)
{
    m_valueList = valueList;
}

XAxisValueType CCurveData::valueType() const
{
    return m_valueType;
}

void CCurveData::setValueType(XAxisValueType type)
{
    m_valueType = type;
}
QString CCurveData::name() const
{
    return m_name;
}

void CCurveData::setName(const QString &name)
{
    m_name = name;
}
QList<CTimeValuePoint> &CCurveData::timeList()
{
    return m_timeList;
}

void CCurveData::setTimeList(const QList<CTimeValuePoint> &timeList)
{
    m_timeList = timeList;
}
