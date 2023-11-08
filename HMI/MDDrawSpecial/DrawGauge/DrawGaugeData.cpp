#include "DrawGaugeData.h"
//////////////////////////////////////////////////////////////////////////
CDrawGaugeData::CDrawGaugeData()
{
}

CDrawGaugeData::~CDrawGaugeData()
{
}

void CDrawGaugeData::operator=(const CDrawGaugeData &other)
{
    m_gauge = other.m_gauge;
}
void CDrawGaugeData::serialize(QJsonObject &json)
{
    m_gauge.serialize(json);
}

void CDrawGaugeData::deserialize(const QJsonObject &json)
{
    m_gauge.deserialize(json);
}
//////////////////////////////////////////////////////////////////////////
