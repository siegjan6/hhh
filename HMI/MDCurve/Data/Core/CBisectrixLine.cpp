#include "CBisectrixLine.h"
#include <QColor>
#include <QJsonObject>

#define MINVALUE 1
#define MAXVALUE 10

CBisectrixLine::CBisectrixLine():
    m_lineColor(Qt::black),
    m_lineWidth(1),
    m_lineStyle(Qt::DotLine)
{
}

CBisectrixLine::~CBisectrixLine()
{
}

void CBisectrixLine::setLineColor(const QColor &color)
{
    m_lineColor = color;
}

QColor CBisectrixLine::lineColor() const
{
    return m_lineColor;
}

void CBisectrixLine::setLineStyle(Qt::PenStyle style)
{
    m_lineStyle = style;
}

Qt::PenStyle CBisectrixLine::lineStyle() const
{
    return m_lineStyle;
}

void CBisectrixLine::setLineWidth(int wid)
{
    if(wid < MINVALUE)
        m_lineWidth = MINVALUE;
    else if(wid > MAXVALUE)
        m_lineWidth = MAXVALUE;
    else
        m_lineWidth = wid;
}

int CBisectrixLine::lineWidth() const
{
    return m_lineWidth;
}

void CBisectrixLine::serialize(QJsonObject &json)
{
    json.insert("LineColor",(double)m_lineColor.rgba());
    json.insert("LineWidth",m_lineWidth);
    json.insert("LineStyle",(int)m_lineStyle);
}

void CBisectrixLine::deserialize(const QJsonObject &json)
{
    m_lineColor = QColor::fromRgba(json.value("LineColor").toDouble());
    m_lineWidth = json.value("LineWidth").toInt();
    int style = json.value("LineStyle").toInt();
    m_lineStyle = (Qt::PenStyle)style;
}

