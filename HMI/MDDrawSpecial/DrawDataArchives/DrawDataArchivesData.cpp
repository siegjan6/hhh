#include "DrawDataArchivesData.h"

CDrawDataArchivesData::CDrawDataArchivesData()
{
}

void CDrawDataArchivesData::operator =(const CDrawDataArchivesData &other)
{
    m_dataArchives = other.m_dataArchives;
}


void CDrawDataArchivesData::serialize(QJsonObject &json)
{
    m_dataArchives.serialize(json);
}

void CDrawDataArchivesData::deserialize(const QJsonObject &json)
{
    m_dataArchives.deserialize(json);
}
