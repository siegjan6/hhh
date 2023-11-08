#include "DrawBasicWidgetData.h"
#include "MDJsonConvert.h"

CDrawBasicWidgetData::CDrawBasicWidgetData()
{
    m_textColor = Qt::black;
    m_backColor = Qt::white;
    m_disableBackColor = Qt::gray;
    m_font = QFont("Times", 10);
    m_enable = true;
}

void CDrawBasicWidgetData::operator =(const CDrawBasicWidgetData &other)
{
    m_font = other.m_font;
    m_enable = other.m_enable;
    m_backColor = other.m_backColor;
    m_disableBackColor = other.m_disableBackColor;
    m_textColor = other.m_textColor;
}


void CDrawBasicWidgetData::serialize(QJsonObject &json)
{
    json.insert("Font",CMDJsonConvert::toJson(m_font));
    json.insert("Enable",m_enable);
    json.insert("BackColor",(qreal)m_backColor.rgba());
    json.insert("DisableBackColor",(qreal)m_disableBackColor.rgba());
    json.insert("TextColor",(qreal)m_textColor.rgba());
}

void CDrawBasicWidgetData::deserialize(const QJsonObject &json)
{
    m_font = CMDJsonConvert::toQFont(json.value("Font"));
    m_enable = json.value("Enable").toBool();
    m_backColor = QColor::fromRgba(json.value("BackColor").toDouble());
    m_disableBackColor = QColor::fromRgba(json.value("DisableBackColor").toDouble());
    m_textColor = QColor::fromRgba(json.value("TextColor").toDouble());
}
