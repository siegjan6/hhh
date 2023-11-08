#include "NodePoint.h"
#include "SelectObjectManager.h"
//////////////////////////////////////////////////////////////////////////
CNodePoint::CNodePoint(CSelectObjectManager *objects)
{
    m_objects = objects;
}
//////////////////////////////////////////////////////////////////////////
bool CNodePoint::isVisible(const QPointF &point, ControlState &state, int &index, bool isDeleteNode)
{
    int len = m_datas.count();
    for(int i = 0; i < len; i++)
    {
        if (m_paths.at(i).contains(point))
        {
            state = isDeleteNode ? ControlState::DeleteNode : ControlState::MoveNode;
            index = i;
            return true;
        }
    }

    return false;
}
//////////////////////////////////////////////////////////////////////////
bool CNodePoint::isVisible(const QPointF &point)
{
    int count = m_datas.count();
    for (int i = 0; i < count; i++)
    {
        if (m_paths.at(i).contains(point))
            return true;
    }

    return false;
}
bool CNodePoint::canOperate(const QPointF &pf, ControlState &state, int &index)
{
    if (m_objects->isAddNode())
    {
        state = ControlState::AddNode;
        return true;
    }

    if (isVisible(pf, state, index, m_objects->isDeleteNode()))
        return true;
    if (m_objects->isVisible(pf))
    {
        state = ControlState::Move;
        return true;
    }

    return false;
}
void CNodePoint::draw(QPainter *p)
{
    //注意：len必须使用Datas，Paths的len可能比Datas的要大
    int len = m_datas.count();
    p->setPen(Qt::black);
    p->setBrush(QBrush(QColor(245, 245, 245)));
    for (int i = 0; i < len; i++)
        p->drawPath(m_paths.at(i));
}
//////////////////////////////////////////////////////////////////////////
void CNodePoint::calculate(qreal scale, QRectF &invalidateRect)
{
    generateData(scale);
    generatePath();
    generateRect(invalidateRect);
}
void CNodePoint::generateData(qreal scale)
{
    m_datas.clear();
    m_datas.append(*m_objects->node()->nodeDatas());

    int len = m_datas.count();
    for (int i = 0; i < len; i++)
        m_datas[i] = CControlPointContainer::transFromData(*m_objects->matrix(), scale, m_datas.at(i));
}
void CNodePoint::generatePath()
{
    const qreal size = CControlPointContainer::PointSize;
    int len = m_datas.count();
    //init Paths
    //没有，则创建;创建长度不够，重新创建
    if (m_paths.length() < len)
    {
        m_paths.clear();
        for (int i = 0; i < len; i++)
            m_paths.append(QPainterPath());
    }

    QPointF center;
    for (int i = 0; i < len; i++)
    {
        center = QPointF(m_datas.at(i).x() - size/2, m_datas.at(i).y() - size/2);
        QPainterPath path;
        path.addEllipse(QRectF(center, QSize(size, size)));
        m_paths[i] = path;
    }
}
void CNodePoint::generateRect(QRectF &invalidateRect)
{
    const qreal size = CControlPointContainer::PointSize;
    QRectF rf;
    for (int i = 0; i < m_datas.count(); i++)
    {
        QRectF r = QRectF(m_datas.at(i).x() - size/2.f, m_datas.at(i).y() - size/2.f, size, size);
        rf = rf.isEmpty() ? r : r.united(rf);
    }
    invalidateRect = invalidateRect.isEmpty() ? rf : rf.united(invalidateRect);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
