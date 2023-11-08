#include <QStringList>
#include "DrawBasicArcData.h"
//////////////////////////////////////////////////////////////////////////
CDrawBasicArcData::CDrawBasicArcData()
{
    m_startAngle = 0;
    m_spanAngle = 225;
    m_startAngleBk = 0;
    m_spanAngleBk = 225;
}
void CDrawBasicArcData::operator=(const CDrawBasicArcData &other)
{
    if (this == &other)
        return;

    m_customDatas.clear();
    m_customDatas.append(other.m_customDatas);
    m_customCenter = other.m_customCenter;
    m_startAngle = other.m_startAngle;
    m_spanAngle = other.m_spanAngle;
}
void CDrawBasicArcData::serialize(QJsonObject &json)
{
    json.insert("StartAngle", m_startAngle);
    json.insert("SpanAngle", m_spanAngle);
}
void CDrawBasicArcData::deserialize(const QJsonObject &json)
{
    m_startAngle = json.value("StartAngle").toDouble();
    m_spanAngle = json.value("SpanAngle").toDouble();
}
//////////////////////////////////////////////////////////////////////////

