#include <QtAlgorithms>

#include "FormPoint.h"
#include "MDHMIForm.h"
//////////////////////////////////////////////////////////////////////////
CFormPoint::CFormPoint(CMDHMIForm *container)
{
    m_container = container;
}
//////////////////////////////////////////////////////////////////////////
bool CFormPoint::canOperate(const QPointF &point, ControlState &state, int &/*index*/)
{
    const int interval = 5;
    QRect rect = m_container->rect();
    if (qAbs(point.y() - rect.height()) <= interval)
    {
        state = ControlState::FormHeight;
        return true;
    }
    if (qAbs(point.x() - rect.width()) <= interval)
    {
        state = ControlState::FormWidth;
        return true;
    }
    return false;
}
void CFormPoint::mouseDown(const QPointF &point)
{
    m_downMousePos = point;
    m_downRect = m_container->rect();
}
void CFormPoint::changeFormSize(const QPointF &point, ControlState state)
{
    QRect rect = m_downRect;

    switch (state) {
    case ControlState::FormWidth:
        rect.setWidth(rect.width() + point.x() - m_downMousePos.x());
        break;
    case ControlState::FormHeight:
        rect.setHeight(rect.height() + point.y() - m_downMousePos.y());
        break;
    default:
        break;
    }
    m_container->setPropertyRect(rect);
}
//////////////////////////////////////////////////////////////////////////
