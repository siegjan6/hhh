#include "Tool.h"
//////////////////////////////////////////////////////////////////////////
QPointF CTool::gridPointF(const QPointF &point)
{
    QPointF p = point;
    qreal value = p.x();
    p.setX((int)value - ((int)value) % 10);
    value = p.y();
    p.setY((int)value - ((int)value) % 10);

    return p;
}
QRectF CTool::gridRect(const QRect &rect)
{
    QRectF rf = rect;
    rf.moveLeft(rect.x() - rect.x() % 10);
    rf.moveTop(rect.y() - rect.y() % 10);
    rf.setWidth(rect.width() - rect.width() % 10);
    rf.setHeight(rect.height() - rect.height() % 10);

    return rf;
}
