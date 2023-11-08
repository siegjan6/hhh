#include "DrawCheckBoxData.h"

CDrawCheckBoxData::CDrawCheckBoxData()
{
    m_widget = NULL;
    m_event = NULL;

    m_checked = false;
    m_text = "复选框";
}

void CDrawCheckBoxData::operator =(const CDrawCheckBoxData &other)
{
    m_text = other.m_text;
    m_checked = other.m_checked;
}

void CDrawCheckBoxData::serialize(QJsonObject &json)
{
    json.insert("Text",m_text);
    json.insert("Checked",m_checked);
}

void CDrawCheckBoxData::deserialize(const QJsonObject &json)
{
    m_text = json.value("Text").toString();
    m_checked = json.value("Checked").toBool();
}

