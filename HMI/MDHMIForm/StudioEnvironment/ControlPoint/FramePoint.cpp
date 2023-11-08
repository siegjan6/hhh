#include <QColor>
#include <QPainter>

#include "FramePoint.h"
#include "SelectObjectManager.h"

int CFramePoint::FrameCount = 8;

int CFramePoint::RotateCount = 4;

int CFramePoint::ShearCount = 2;
int CFramePoint::PointCount = 5;
int CFramePoint::FrameSize = 9.6;

//////////////////////////////////////////////////////////////////////////
CFramePoint::CFramePoint(CSelectObjectManager* objects)
{

    m_pointRect = QRect(0 , 0, FrameSize, FrameSize);


    m_objects = objects;
}
//////////////////////////////////////////////////////////////////////////
void CFramePoint::calculate(qreal scale, QRectF &invalidateRect, QRectF &rotateRect)
{
    generateData(scale);
    generatePath();
    generateRect(invalidateRect, rotateRect);
}
void CFramePoint::generateData(qreal scale)
{
    QPointF* pfs = m_frameArray;

    if (m_objects->isEmpty())
    {
        for (int i = 0; i < FrameCount; i++)
            pfs[i] = QPointF(0, 0);
    }
    else
    {
        qreal size = FrameSize / scale;
        QRectF rf = m_objects->rect();

        for (int i = 0; i < FrameCount; i++)
        {
            int j = (i < 4) ? i : i + 1;
            int row = j / 3;
            int col = j % 3;

            pfs[i].setX(rf.x() + rf.width() / 2 * col + size * (col - 1));
            pfs[i].setY(rf.y() + rf.height() / 2 * row + size * (row - 1));

            pfs[i] = CControlPointContainer::transFromData(*(m_objects->matrix()), scale, pfs[i]);
        }
    }
    //rotate center
    m_rotatePointPos = m_objects->rotatePointPos();
    m_rotatePointPos *= scale;
}
void CFramePoint::generatePath()
{
    //frame path
    QRectF rect = m_pointRect;
    QPainterPath* path = NULL;
    int sCount = 0;
    int rCount = 0;
    int fCount = 0;
    for (int i = 0; i < FrameCount; i++)
    {
        path = &m_framePath[i];
        *path = QPainterPath();

        //frame
        rect.moveTo(m_frameArray[i].x() - FrameSize / 2, m_frameArray[i].y() - FrameSize / 2);
        path->addRect(rect);

        switch (i)
        {
            //rotate
            case 0:
            case 2:
            case 5:
            case 7:
                path = &m_rotatePath[rCount++];
                m_framePoint[fCount++] = m_frameArray[i];
                break;
            //shear
            case 1:
            case 6:
                path = &m_shearPath[sCount++];
                break;
            default:
                path = NULL;
                break;
        }

        if (path != NULL)
        {
            *path = QPainterPath();
            path->addRect(QRectF(m_frameArray[i].x() - FrameSize, m_frameArray[i].y() - FrameSize, FrameSize*2, FrameSize*2));
        }

    }
    //改变矩形框点位置，方便画图
    QPointF p = m_framePoint[3];
    m_framePoint[3] = m_framePoint[2];
    m_framePoint[2] = p;
    m_framePoint[4] = m_framePoint[0];

    //rotate center
    m_rotateCenterPath = QPainterPath();
    if (m_objects->isVector())
    {
        m_rotateCenterPath.addEllipse(m_rotatePointPos.x() - FrameSize * 0.5f, m_rotatePointPos.y() - FrameSize * 0.5f,
            FrameSize, FrameSize);
    }
}
void CFramePoint::generateRect(QRectF &invalidateRect, QRectF &rotateRect)
{
    if (m_objects->isEmpty())
        return;

    QRectF rf;
    //frame
    for (int i = 0; i < FrameCount; i++)
    {
        rf = (rf.isEmpty())
            ? m_framePath[i].boundingRect() : rf.united(m_framePath[i].boundingRect());
    }
    invalidateRect = (invalidateRect.isEmpty())
        ? rf : rf.united(invalidateRect);

    //rotate center
    if (m_objects->isVector())
    {
        rf = m_rotateCenterPath.boundingRect();
        if (!invalidateRect.contains(rf))
            rotateRect = rf;
    }
}
//////////////////////////////////////////////////////////////////////////
bool CFramePoint::canOperate(const QPointF& pf, ControlState &state, int &index)
{
    if (m_rotateCenterPath.contains(pf))
    {
        //state = ControlState::Center;
        //return true;
    }

    for (int i = 0; i < FrameCount; i++)
    {
        if (m_framePath[i].contains(pf))
        {
            state = ControlState::FrameMove;
            index = i;
            return true;
        }
    }
    if (m_objects->isVector())
    {
        for (int i = 0; i < RotateCount; i++)
        {
            if (m_rotatePath[i].contains(pf))
            {
                state = ControlState::Rotate;
                index = i;
                return true;
            }
        }
        if (m_objects->isSingleVector())
        {
            for (int i = 0; i < ShearCount; i++)
            {
                if (m_shearPath[i].contains(pf))
                {
                    state = ControlState::Shear;
                    index = i;
                    return true;
                }
            }
        }
    }

    return false;
}
void CFramePoint::draw(QPainter *p)
{
    if (m_objects->isEmpty())
        return;

    //draw frame line
    QPen line(Qt::DashLine);
    line.setColor(QColor(0x94, 0x00, 0xD3));
    p->setPen(line);
    for (int i = 0; i < PointCount - 1; i++)
        p->drawLine(m_framePoint[i], m_framePoint[i+1]);

    //draw frame rect
    QBrush brush(Qt::black);
    for (int i = 0; i < FrameCount; i++)
    {
        p->fillPath(m_framePath[i],  brush);
        if (m_objects->isSingleVector() && (i == 1 || i == 6)) //shear
            p->setPen(Qt::yellow);
        else
           p->setPen(QColor(0x5F, 0x9E, 0xA0)); //normal
        p->drawPath(m_framePath[i]);
    }

    //draw rotate center
    p->setPen(Qt::blue);
    p->drawPath(m_rotateCenterPath);
    QRectF rf = m_rotateCenterPath.boundingRect();
    rf.adjust(1, 1, -1, -1);
    p->setPen(Qt::white);
    p->drawEllipse(rf);
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
