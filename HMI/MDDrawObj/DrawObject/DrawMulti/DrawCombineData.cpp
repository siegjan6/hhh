#include "DrawCombineData.h"
#include "MDDrawObj.h"
//////////////////////////////////////////////////////////////////////////
CDrawCombineData::CDrawCombineData()
{
    m_connectLine = false;
    m_fillMode = Qt::OddEvenFill;
}
void CDrawCombineData::operator=(const CDrawCombineData &other)
{
    if (this == &other)
        return;

    m_connectLine = other.m_connectLine;
    m_fillMode = other.m_fillMode;
}
void CDrawCombineData::serialize(QJsonObject &json)
{
    json.insert("ConnectLine", m_connectLine);
    json.insert("FillMode", (int)m_fillMode);
}
void CDrawCombineData::deserialize(const QJsonObject &json)
{
    m_connectLine = json.value("ConnectLine").toBool();
    m_fillMode = (Qt::FillRule)json.value("FillMode").toInt();
}
//////////////////////////////////////////////////////////////////////////

