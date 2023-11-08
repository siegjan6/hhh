#include "SelectObjectManager.h"
#include "CustomPoint.h"
#include "ControlPointContainer.h"
//////////////////////////////////////////////////////////////////////////
CCustomPoint::CCustomPoint(CSelectObjectManager *objects)
{
    m_objects = objects;
}
CCustomPoint::~CCustomPoint()
{
    clearPath();
}
//////////////////////////////////////////////////////////////////////////
void CCustomPoint::draw(QPainter *p)
{
    if (m_datas.count() == 0)
        return;

    QPen pen(Qt::blue);
    pen.setStyle(Qt::DotLine);
    p->setPen(pen);
    for(int i = 0; i < m_datas.count(); i++)
        p->drawLine(m_datas.at(i), m_center);

    //cadetBlue
    p->setPen(QColor(95 ,158, 160));
    QBrush yellowBrush(Qt::yellow);

    for(int i = 0; i < m_paths.count(); i++)
    {
        p->fillPath(*m_paths.at(i), yellowBrush);
        p->drawPath(*m_paths.at(i));
    }
}
bool CCustomPoint::canOperate(const QPointF &point, ControlState &state, int &index)
{
    if (m_datas.count() == 0)
        return false;

    int len = m_datas.count();
    for(int i = 0; i < len; i++)
    {
        if (m_paths.at(i)->contains(point))
        {
            state = ControlState::Custom;
            index = i;
            return true;
        }
    }

    return false;
}
//////////////////////////////////////////////////////////////////////////
void CCustomPoint::calculate(qreal scale, QRectF &invalidateRect)
{
    generateData(scale);
    generatePath();
    generateRect(invalidateRect);
}
void CCustomPoint::generatePath()
{
    if (m_datas.count() == 0)
        return;

    const qreal size = CControlPointContainer::PointSize;
    int len = m_datas.count();
    //init Paths
    //没有，则创建;创建长度不够，重新创建
    if (m_paths.count() == 0)
    {
        for (int i = 0; i < len; i++)
            m_paths.append(new QPainterPath());
    }
    else if (m_paths.count() < len)
    {
        clearPath();

        for (int i = 0; i < len; i++)
            m_paths.append(new QPainterPath());
    }

    QPointF rf;
    QTransform m;
    for (int i = 0; i < len; i++)
    {
        rf = QPointF(m_datas.at(i).x() - size / 2, m_datas.at(i).y() - size / 2);
        *m_paths.at(i) = QPainterPath();
        m_paths.at(i)->addRect(QRectF(rf.x(), rf.y(), size, size));

        m.reset();
        m.translate(m_datas.at(i).x(), m_datas.at(i).y());
        m.rotate(45);
        m.translate(-m_datas.at(i).x(), -m_datas.at(i).y());
        *m_paths.at(i) = m.map(*m_paths.at(i));
    }
}
void CCustomPoint::generateRect(QRectF &invalidateRect)
{
    if (m_datas.count() == 0)
        return;

    int len = m_datas.count();
    QRectF rf;
    for (int i = 0; i < len; i++)
        rf = (rf.isEmpty()) ?
                m_paths.at(i)->boundingRect() : rf.united(m_paths.at(i)->boundingRect());

    invalidateRect = (invalidateRect.isEmpty()) ?
                rf : rf.united(invalidateRect);
}
void CCustomPoint::generateData(qreal scale)
{
    m_datas.clear();
    m_datas.append(*m_objects->custom()->customDatas());

    for(int i = 0; i < m_datas.count(); i++)
        m_datas[i] = CControlPointContainer::transFromData(*m_objects->matrix(), scale, m_datas.at(i));
    m_center = m_objects->custom()->customCenter();
    m_center = CControlPointContainer::transFromData(*m_objects->matrix(), scale, m_center);
}
//////////////////////////////////////////////////////////////////////////
void CCustomPoint::clearPath()
{
    for(int i = 0; i < m_paths.count(); i++)
        delete m_paths.at(i);
    m_paths.clear();
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////



