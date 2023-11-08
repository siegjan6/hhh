#include "CWarningLine.h"
#include "MDJsonConvert.h"
#include <QPainter>
#include <QJsonObject>
#include <QDebug>
CWarningLine::CWarningLine():
    m_value(0),
    m_visible(false),
    m_yPoint(0)
{
    setLineStyle(Qt::SolidLine);
    CBisectrixLine::setLineColor(Qt::red);
}

CWarningLine::~CWarningLine()
{

}
double CWarningLine::value() const
{
    return m_value;
}

void CWarningLine::setValue(double value)
{
    m_value = value;
}
bool CWarningLine::visible() const
{
    return m_visible;
}

void CWarningLine::setVisible(bool visible)
{
    m_visible = visible;
}
double CWarningLine::yPoint() const
{
    return m_yPoint;
}

void CWarningLine::setYPoint(double yPoint)
{
    m_yPoint = yPoint;
}

void CWarningLine::setLinePoint(const QPointF &start, const QPointF &end)
{
    m_startPoint = start;
    m_endPoint = end;
}

void CWarningLine::draw(QPainter *p)
{
    if(!m_visible)
        return;

    p->save();
    QPen linePen;
    linePen.setWidth(lineWidth());
    linePen.setColor(lineColor());
    linePen.setStyle(lineStyle());
    p->setPen(linePen);
    p->drawLine(m_startPoint,m_endPoint);
    p->restore();
}

void CWarningLine::serialize(QJsonObject &json)
{
    CBisectrixLine::serialize(json);
    QJsonObject warningJson;
    warningJson.insert("Value",m_value);
    warningJson.insert("Visible",m_visible);
    warningJson.insert("YPoint",m_yPoint);
    warningJson.insert("StartPoint",CMDJsonConvert::toJson(m_startPoint));
    warningJson.insert("EndPoint",CMDJsonConvert::toJson(m_endPoint));
    json.insert("WarningLine",warningJson);
}

void CWarningLine::deserialize(const QJsonObject &json)
{
    CBisectrixLine::deserialize(json);
    QJsonObject warningJson = json.value("WarningLine").toObject();
    m_value = warningJson.value("Value").toDouble();
    m_visible = warningJson.value("Visible").toBool();
    m_yPoint = warningJson.value("YPoint").toDouble();
    m_startPoint = CMDJsonConvert::toQPointF(warningJson.value("StartPoint"));
    m_endPoint = CMDJsonConvert::toQPointF(warningJson.value("EndPoint"));
}


