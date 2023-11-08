#include "DrawCurveData.h"
//////////////////////////////////////////////////////////////////////////
CDrawCurveData::CDrawCurveData()
{
}

void CDrawCurveData::setIsRealTime(bool isRealTime)
{
    m_curve.setIsRealTime(isRealTime);
}

void CDrawCurveData::setDataSourceParameter(int type, const QString &parameter)
{
    m_curve.setDataSourceParameter(type,parameter);
}
void CDrawCurveData::setFilter(const QString &filter)
{
    m_curve.setHistoryDataSearchFilter(filter);
}

void CDrawCurveData::setTagFilter(const QString &filter)
{
    m_curve.setHistoryTagSearchFilter(filter);
}

void CDrawCurveData::query()
{
    m_curve.query();
}

CDrawCurveData& CDrawCurveData::operator=(const CDrawCurveData &other)
{
    this->m_curve = other.m_curve;
    return *this;
}
void CDrawCurveData::serialize(QJsonObject &json)
{
    m_curve.serialize(json);
}

void CDrawCurveData::deserialize(const QJsonObject &json)
{
    m_curve.deserialize(json);
}

void CDrawCurveData::changeLanguage()
{
   m_curve.changeLanguage();
}

void CDrawCurveData::collectWords()
{
    m_curve.collectWords();
}
//////////////////////////////////////////////////////////////////////////
