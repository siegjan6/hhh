#include "DrawRectData.h"
//////////////////////////////////////////////////////////////////////////
CDrawRectData::CDrawRectData()
{
    m_xRoundBk = 0;
    m_yRoundBk = 0;
    m_roundLimit = 0;
    m_xRound = 0;
    m_yRound = 0;
}

void CDrawRectData::operator=(const CDrawRectData &other)
{
    if (this == &other)
        return;

    m_customDatas.clear();
    m_customDatas.append(other.m_customDatas);
    m_xRound = other.m_xRound;
    m_yRound = other.m_yRound;
}
void CDrawRectData::serialize(QJsonObject &json)
{
    json.insert("XRound", m_xRound);
    json.insert("YRound", m_yRound);
}
void CDrawRectData::deserialize(const QJsonObject &json)
{
    m_xRound = json.value("XRound").toDouble();
    m_yRound = json.value("YRound").toDouble();
}
//////////////////////////////////////////////////////////////////////////
