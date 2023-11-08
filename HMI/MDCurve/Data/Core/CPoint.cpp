#include "CPoint.h"
#include <QPainter>
#include <math.h>
#include <QJsonObject>
#include <QDebug>


CPoint::CPoint():
    m_visible(false),
    m_color(Qt::black),
    m_size(8),
    m_pointStyle(PointStyle::Circle),
    m_drawPath(new QPainterPath)
{
}

CPoint::~CPoint()
{
    if(m_drawPath){
        delete m_drawPath;
        m_drawPath = NULL;
    }
}
QColor CPoint::color() const
{
    return m_color;
}

void CPoint::setColor(const QColor &color)
{
    m_color = color;
}
int CPoint::size() const
{
    return m_size;
}

void CPoint::setSize(int size)
{
    enum { Min = 1 , Max = 10};
    if(size < Min )
        size = Min;
    else if (size > Max)
        size = Max;
    else
        m_size = size;
}
CPoint::PointStyle CPoint::pointStyle() const
{
    return m_pointStyle;
}

void CPoint::setPointStyle(const PointStyle &pointStyle)
{
    m_pointStyle = pointStyle;
}

bool CPoint::visible() const
{
    return m_visible;
}

void CPoint::setVisible(bool visible)
{
    m_visible = visible;
}


void CPoint::figurePoint(const QPointF &p)
{
    if(p.isNull())
        return;

    m_drawPath->moveTo(p);

    switch (m_pointStyle) {
    case PointStyle::Circle:
        figureCircle(p);
        break;
    case PointStyle::Square:
        figureSquare(p);
        break;
    case PointStyle::Triangle:
        figureTriangle(p);
        break;
    case PointStyle::Cross:
        figureCross(p);
        break;
    default:
        break;
    }
}

void CPoint::clearPath()
{
    if(m_drawPath){
        delete m_drawPath;
        m_drawPath = NULL;
    }
    m_drawPath = new QPainterPath;
}

void CPoint::draw(QPainter *p)
{
    if(!m_visible)
        return;
    p->save();
    p->setPen(m_color);
    p->drawPath(*m_drawPath);
    p->fillPath(*m_drawPath,QColor(m_color));
    p->restore();
}

void CPoint::figureCircle(const QPointF &p)
{
    QRectF circleRect;
    double x = p.x() - (double)m_size/2;
    double y = p.y() - (double)m_size/2;
    circleRect.setTopLeft(QPointF(x,y));
    circleRect.setSize(QSizeF(m_size,m_size));
    m_drawPath->addEllipse(circleRect);

}
// size 作为Cross外围正方形的边长
void CPoint::figureCross(const QPointF &p)
{
    QPointF topLeft = QPointF(p.x()-m_size*0.5,p.y()-m_size *0.5);
    QPointF bottomLeft = QPointF(p.x()-m_size*0.5,p.y()+m_size*0.5);
    QPointF topRight =  QPointF(p.x()+m_size*0.5,p.y()-m_size *0.5);
    QPointF bottomRigth =  QPointF(p.x()+m_size*0.5,p.y()+m_size *0.5);

    m_drawPath->moveTo(topLeft);
    m_drawPath->lineTo(bottomRigth);
    m_drawPath->moveTo(bottomLeft);
    m_drawPath->lineTo(topRight);
    m_drawPath->closeSubpath();
}

void CPoint::figureSquare(const QPointF &p)
{
    QRectF square;
    double x = p.x() - (double)m_size/2;
    double y = p.y() - (double)m_size/2;
    square.setTopLeft(QPointF(x,y));
    square.setSize(QSizeF(m_size,m_size));

    m_drawPath->addRect(square);
}
// 三角形的size属性为边长为size,数据点位于中心点
void CPoint::figureTriangle(const QPointF &p)
{
    double trHeight = m_size * 0.5 * sqrt(3);
    double startX = p.x() - m_size *0.5;
    double startY = p.y() + trHeight/3;
    m_drawPath->moveTo(startX,startY);
    m_drawPath->lineTo(p.x(),startY - trHeight);
    m_drawPath->lineTo(startX+m_size,startY);
    m_drawPath->closeSubpath();
}

void CPoint::serialize(QJsonObject &json)
{
    json.insert("Visible",m_visible);
    json.insert("Color",(double)m_color.rgba());
    json.insert("Size",m_size);
    json.insert("PointStyle",(int)m_pointStyle);
}

void CPoint::deserialize(const QJsonObject &json)
{
    m_visible = json.value("Visible").toBool();
    m_color = QColor(json.value("Color").toDouble());
    m_size = json.value("Size").toInt();
    int style = json.value("PointStyle").toInt();
    m_pointStyle = (PointStyle)style;
}

CPoint &CPoint::operator =(const CPoint &other)
{
    if(&other == this)
        return *this;

    this->m_visible = other.m_visible;
    this->m_color   = other.m_color;
    this->m_size    = other.m_size;
    this->m_pointStyle = other.m_pointStyle;

    return *this;
}



