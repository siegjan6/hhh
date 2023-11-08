#include "DrawLogData.h"

CDrawLogData::CDrawLogData()
{
}

CDrawLogData& CDrawLogData::operator =(const CDrawLogData &other)
{
    if(this == &other)
    {
        return *this;
    }
    this->m_log = other.m_log;
    return *this;
}


void CDrawLogData::serialize(QJsonObject &json)
{
    m_log.serialize(json);
}

void CDrawLogData::deserialize(const QJsonObject &json)
{   
    m_log.deserialize(json);
}
