#include "CAuxiliary.h"
#include "CFind.h"
#include "CCurveObjectManager.h"
#include "CReferenceCurveManager.h"
#include "CCurveObject.h"
#include "CXAxis.h"
#include "CYAxis.h"

#include <QDebug>

CAuxiliary::CAuxiliary():
    m_visible(true),
    m_mousePointX(0),
    m_curves(0),
    m_path(0),
    m_isCalculated(false)
{
}

CAuxiliary::~CAuxiliary()
{
    if(m_path)
    {
        delete m_path;
        m_path = NULL;
    }
}

void CAuxiliary::setCurves(CCurveObjectManager *curve)
{
    m_curves = curve;
}

void CAuxiliary::setReCurves(CReferenceCurveManager *reCurve)
{
    m_reCurves = reCurve;
}

void CAuxiliary::calculate()
{
    if(!m_curves || !m_visible)
    {
        m_isCalculated = false;
        return;
    }

    if(m_reCurves->list().isEmpty() && m_curves->list().isEmpty())
    {
        return;
    }

    if(m_path)
    {
        delete m_path;
        m_path = NULL;
    }
    m_path = new QPainterPath;

    m_curveArea = m_curves->curveArea();

    enum {SQUAREWIDTH = 5};

    for(int i = 0 ; i < m_curves->list().count();i++)
    {
        // jump the invisble curve,go to next loop
        if(!m_curves->list().at(i)->curveVisible())
            continue;

        CCurveObject *curve = m_curves->list().at(i);
        double xValue = m_mousePointX -m_curveArea.left();

        //jump the no points curve
        if(curve->totalVisblePoint().isEmpty())
            continue;
        double xPoint = 0;
        int index = 0;
        if(curve->xAxis()->valueType() == CXAxis::ValueType::Time)
        {
            xPoint = xValue;

//            double    xWidth = m_curveArea.width();
//            QDateTime timeMark  = curve->xAxis()->timeMark();
//            QDateTime endTime = curve->xAxis()->endTime();
//            qint64    valueLength = curve->xAxis()->valueLength();
//            QDateTime rawX =timeMark.addSecs(xPoint/xWidth *valueLength -
//                                             endTime.addSecs(-valueLength).secsTo(timeMark));
//            index = CFind::binarySearch(curve->rawData().timeList(),rawX);

            index = CFind::binarySearch(curve->totalVisblePoint(),xPoint);
        }

        else if(curve->xAxis()->valueType() == CXAxis::ValueType::Value)
        {
            xPoint = xValue*(curve->xAxis()->maxValue() - curve->xAxis()->minValue())/m_curveArea.width() +curve->xAxis()->minValue();
            index = CFind::binarySearch(curve->rawData().valueList(),xPoint);
        }
        if(index > curve->totalVisblePoint().count()-1)
            return;

        QPointF point = curve->totalVisblePoint().at(index);

        // Point translate according to the Curves drawing
        point.setX(m_curveArea.left() + point.x());
        point.setY(m_curveArea.bottom() + point.y());

        QRectF square;
        double x = point.x() - SQUAREWIDTH/2;
        double y = point.y() - SQUAREWIDTH/2;
        square.setTopLeft(QPointF(x,y));
        square.setSize(QSizeF(SQUAREWIDTH,SQUAREWIDTH));

        m_path->addRect(square);
    }
    m_isCalculated = true;
}

void CAuxiliary::draw(QPainter *p)
{
    if(!m_isCalculated || !m_curveArea.isValid() || !m_visible)
        return;

    p->save();
    QRegion region(m_curveArea.toRect());
    p->setClipRegion(region);
    p->drawPath(*m_path);
    drawMoveLine(p);
    p->restore();
}

void CAuxiliary::drawMoveLine(QPainter *p)
{
    p->save();

    p->setPen(Qt::blue);
    double top = m_curveArea.top();
    double bottom = m_curveArea.bottom();

    QPointF start(m_mousePointX,top);
    QPointF end(m_mousePointX,bottom);
    p->drawLine(start,end);

    p->restore();
}
bool CAuxiliary::visble() const
{
    return m_visible;
}

void CAuxiliary::setVisble(bool visble)
{
    m_visible = visble;
}

double CAuxiliary::mousePointX() const
{
    return m_mousePointX;
}

void CAuxiliary::setMousePointX(double mousePointX)
{
    m_mousePointX = mousePointX;
}

