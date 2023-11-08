#include "CFeatureTagLine.h"
#include <QPainter>
#include <QJsonObject>
#include <QJsonArray>
#include "MDJsonConvert.h"

CFeatureTagLine::CFeatureTagLine():
    m_visible(true)
{
    setLineStyle(Qt::SolidLine);
}

CFeatureTagLine::~CFeatureTagLine()
{

}

bool CFeatureTagLine::visible() const
{
    return m_visible;
}

void CFeatureTagLine::setVisible(bool visible)
{
    m_visible = visible;
}

void CFeatureTagLine::appendLinePoints(const QPointF &p1, const QPointF &p2)
{
    m_linePoints << p1 << p2;
}

void CFeatureTagLine::clearLinePoints()
{
    m_linePoints.clear();
}

void CFeatureTagLine::draw(QPainter *p)
{
    p->save();
    if(m_visible){
        QPen pen;
        pen.setColor(lineColor());
        pen.setWidth(lineWidth());
        pen.setStyle(lineStyle());

        p->setPen(pen);
        p->drawLines(m_linePoints);
    }
    p->restore();
}
void CFeatureTagLine::serialize(QJsonObject &json)
{
    CBisectrixLine::serialize(json);
    QJsonObject tagLineJson;
    tagLineJson.insert("Visible",m_visible);
    json.insert("FeatureTagLine",tagLineJson);
}

void CFeatureTagLine::deserialize(const QJsonObject &json)
{
    m_linePoints.clear();
    CBisectrixLine::deserialize(json);
    QJsonObject tagLineJson = json.value("FeatureTagLine").toObject();
    m_visible = tagLineJson.value("Visible").toBool();
}

