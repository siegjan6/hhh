#include "DrawSpinBoxData.h"

CDrawSpinBoxData::CDrawSpinBoxData()
{
    m_widget = NULL;
    m_event = NULL;

    m_singleStep = 1;
    m_maximum = 100;
    m_minimum = 0;
    m_initialValue = 10;
}

void CDrawSpinBoxData::operator=(const CDrawSpinBoxData &other)
{
    m_singleStep = other.m_singleStep;
    m_maximum = other.m_maximum;
    m_minimum = other.m_minimum;
    m_initialValue = other.m_initialValue;
}


void CDrawSpinBoxData::serialize(QJsonObject &json)
{
    json.insert("SingleStep",m_singleStep);
    json.insert("Maximum",m_maximum);
    json.insert("Minimum",m_minimum);
    json.insert("InitialValue",m_initialValue);
}

void CDrawSpinBoxData::deserialize(const QJsonObject &json)
{
    m_singleStep = json.value("SingleStep").toInt();
    m_maximum = json.value("Maximum").toInt();
    m_minimum = json.value("Minimum").toInt();
    m_initialValue = json.value("InitialValue").toInt();
}
