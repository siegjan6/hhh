#include "CLineRegion.h"
#include "qbitmap.h"
CLineRegion::CLineRegion()
{
}
CLineRegion::~CLineRegion()
{
}
bool CLineRegion::contains(const QPoint &point)
{
    return m_rgn.contains(point);
}
void CLineRegion::setRegion(QPainterPath* path, qreal width)
{
    QPen pen(Qt::black);
    pen.setWidthF(width);

    QPainterPath pa = *path;
    QRectF rf = path->boundingRect();
    int w = rf.width() + width*2;
    int h = rf.height() + width*2;
    QBitmap bmp(w, h);
    pa.translate(-rf.x() + width, -rf.y() + width);

    QPainter p;
    p.begin(&bmp);
    p.fillRect(QRectF(0, 0, w, h), Qt::white);
    p.setPen(pen);
    p.drawPath(pa);
    p.end();

    m_rgn = QRegion(bmp);
    m_rgn.translate(rf.x() - width, rf.y() - width);
}


