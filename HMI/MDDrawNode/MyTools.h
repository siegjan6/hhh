#ifndef MYTOOLS_H
#define MYTOOLS_H
#include <qlist.h>
#include <qpoint.h>

class MyTools
{
public:
    MyTools();
    static QPointF getCbyPoint2Dis1(QPointF a,QPointF b,qreal d);
    static qreal distance(const QPointF& a, const QPointF& b);
    static QList<QPointF> convertBeziers(const QList<QPointF>& nodeList, bool close);
    static qreal getLineHorizontalAngle(const QPointF& pointA,const  QPointF& pointB);
    static QPointF centerRadiusPoint(const QPointF& center, qreal angle, qreal radius);
};

#endif // MYTOOLS_H
