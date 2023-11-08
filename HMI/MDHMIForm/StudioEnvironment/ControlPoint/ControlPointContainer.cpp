#include "ControlPointContainer.h"
#include "FramePoint.h"
#include "CustomPoint.h"
#include "FormPoint.h"
#include "SelectObjectManager.h"
#include "MDDrawObj.h"
#include "IHMIForm.h"
#include "StudioEnvironment/Studio.h"
#include "MDHMIForm.h"
#include "Tool.h"
#include "NodePoint.h"
#include "MDDrawObjCommand.h"

int CControlPointContainer::PointSize = 8;

CControlPointContainer::CControlPointContainer(IHMIForm *container)
{
    Q_ASSERT(container != NULL);

    m_state = ControlState::None;
    m_pos = 0;
    //!是否在鼠标操作中
    m_mouseOperation = false;

    m_container = container;
    m_selectObjs = new CSelectObjectManager(this);

    m_frame = new CFramePoint(m_selectObjs);
    m_custom = new CCustomPoint(m_selectObjs);
    m_node = new CNodePoint(m_selectObjs);
    m_form = new CFormPoint(dynamic_cast<CMDHMIForm*>(m_container));
}
CControlPointContainer::~CControlPointContainer()
{
    delete m_frame;
    delete m_custom;
    delete m_node;
    delete m_form;
    delete m_selectObjs;
}
QPointF CControlPointContainer::formScale()
{
    return container()->common()->formScale();
}
//////////////////////////////////////////////////////////////////////////
QPointF CControlPointContainer::getGridPointF(ControlState state, const QPointF &point)
{
    QPointF pf = point;
    bool isGrid = dynamic_cast<CStudio*>(container()->studio())->isGrid();
    switch (state)
    {
        case ControlState::Move:
        case ControlState::FrameMove:
        case ControlState::Center:
        case ControlState::MoveNode:
        case ControlState::AddNode:
        case ControlState::FormWidth:
        case ControlState::FormHeight:
            if (isGrid)
                pf = CTool::gridPointF(pf);
            break;
        default:
            break;
    }

    return pf;
}
void CControlPointContainer::calculate()
{
    m_invalidateRect = QRectF();
    m_rotateCenterRect = QRectF();
    qreal scale = formScale().x();

    switch (m_selectObjs->studioMode())
    {
        case EditMode::Normal:
            m_frame->calculate(scale, m_invalidateRect, m_rotateCenterRect);
            if (m_selectObjs->isCutomEdit())
                m_custom->calculate(scale, m_invalidateRect);
            break;
        case EditMode::Node:
            m_node->calculate(scale, m_invalidateRect);
            break;
//        case EditMode.Segment:
//            _segment.Calculate(scale, ref m_invalidateRect);
//            break;
    default:
        break;
    }

    int w = 3;
    m_invalidateRect.adjust(-w, -w, w, w);
    m_rotateCenterRect.adjust(-w, -w, w, w);
}
void CControlPointContainer::mouseOperation(const QPointF &point)
{
    bool isOrtho = m_container->studio()->isOrtho();
    QPointF p = getGridPointF(m_state, point);

    switch (m_state)
    {
        case ControlState::Move:
            m_selectObjs->mouseMove(p);
            break;
        case ControlState::FrameMove:
            m_selectObjs->mouseFrameMove(p, m_pos, isOrtho);
            break;
        case ControlState::Center:
            m_selectObjs->mouseRotatePoint(p);
            break;
        case ControlState::Rotate:
            m_selectObjs->mouseRotate(p, isOrtho);
            break;
        case ControlState::Shear:
            m_selectObjs->mouseShear(p, m_pos, isOrtho);
            break;
        case ControlState::Custom:
            m_selectObjs->mouseCustom(p, m_pos);
            break;
        case ControlState::MoveNode:
            m_selectObjs->moveNode(p, m_pos);
            break;
        case ControlState::FormWidth:
        case ControlState::FormHeight:
            m_form->changeFormSize(p, m_state);
            break;
        default:
            break;
    }

    container()->setModify(true);
}
void CControlPointContainer::mouseDownOperation(const QPointF &point)
{
    QPointF p = getGridPointF(m_state, point);

    switch (m_state)
    {
    case ControlState::AddNode:
        //已有节点的点，不重复添加
        if (!m_node->isVisible(p))
            m_selectObjs->addNode(p);
        break;
    case ControlState::DeleteNode:
        m_selectObjs->deleteNode(m_pos);
        break;
    case ControlState::FormHeight:
    case ControlState::FormWidth:
        m_form->mouseDown(p);
        break;
    default:
        m_selectObjs->mouseDown(p, m_state);
        break;
    }
}
void CControlPointContainer::getState(const QPointF &point, ControlState &state, int &index)
{
    state = ControlState::None;
    index = 0;

    QPointF pf = point;
    pf *= formScale().x();

    //设置状态
//    if (_selectObjs.IsSegmentEdit)
//    {
//        state = ControlState.Segment;
//        return;
//    }
    if (m_selectObjs->isNodeEdit())
    {
        if (m_node->canOperate(pf, state, index))
            return;
    }
    if (m_frame->canOperate(pf, state, index))
        return;
    if (m_selectObjs->isCutomEdit())
    {
        if (m_custom->canOperate(pf, state, index))
            return;
    }
    if (m_selectObjs->isVisible(point))
    {
        state = ControlState::Move;
        return;
    }
    if (m_form->canOperate(point, state, index))
        return;
}
QPointF CControlPointContainer::transFromData(const QTransform &m, qreal scale, const QPointF &data)
{
    QPointF pf = m.map(data);
    pf *= scale;
    return pf;
}
//////////////////////////////////////////////////////////////////////////
bool CControlPointContainer::isSelected()
{
    return (!m_selectObjs->isEmpty());
}
bool CControlPointContainer::contains(CMDDrawObj *obj)
{
    return m_selectObjs->list()->contains(obj);
}
void CControlPointContainer::changeSelectObj(QList<CMDDrawObj *> *list)
{
    //选中同一个控件，不做操作
    if (list != NULL && list->count() == 1 && m_selectObjs->isSingle()
            && list->at(0) == m_selectObjs->obj())
        return;

    invalidate();
    m_selectObjs->reset(list);
    invalidate();
}
void CControlPointContainer::draw(QPainter *p)
{
    if (m_selectObjs->isEmpty())
        return;

    //LastSelectObj
    if (m_selectObjs->lastSelectedObj() != NULL)
    {
        QPen pen(QColor(255, 0, 0, 160));
        pen.setWidth(4);
        qreal scale = formScale().x();
        p->save();
        p->setPen(pen);
        p->scale(scale, scale);
        p->drawPath(*m_selectObjs->lastSelectedObj()->path());
        p->restore();
    }

//    if (_selectObjs.IsSegmentEdit)
//        _segment.Draw(g);
    /*else */if (m_selectObjs->isNodeEdit())
        m_node->draw(p);
    else
    {
        if (m_selectObjs->isCutomEdit())
            m_custom->draw(p);
        m_frame->draw(p);
    }
}
void CControlPointContainer::invalidate()
{
    calculate();
    static_cast<CStudio*>(m_container->studio())->invalidateControlPoint(this);
}
void CControlPointContainer::calculateAndInvalidate()
{
    m_selectObjs->calculateProperty();
    invalidate();
}
//////////////////////////////////////////////////////////////////////////
bool CControlPointContainer::mouseDown(const QPointF &point)
{
    getState(point, m_state, m_pos);

    m_mouseOperation = (m_state != ControlState::None);
    if (m_mouseOperation)
        mouseDownOperation(point);

    return (m_state != ControlState::None && m_state != ControlState::Move);
}
void CControlPointContainer::mouseUp(const QPointF &point)
{
    //undo
    if (m_mouseOperation)
    {
        bool undo = false;
        QList<CMDDrawObj*>* list = m_selectObjs->list();
        for(int i = 0; i < list->count(); i++)
        {
            if (!list->at(i)->undoEqual())
            {
                undo = true;
                break;
            }
        }

        if (undo)
        {
            QList<QVariant> olds;
            QList<QVariant> news;

            for(int i = 0; i < list->count(); i++)
            {
                CMDDrawObj* o = list->at(i);
                olds.append((qulonglong)o->fillUndoData(true));
                news.append((qulonglong)o->fillUndoData(false));
            }

            m_container->studio()->undo()->push(
                new CMDMouseOperateCommand(olds, news, (int)MouseOperateId::Mouse));
        }
    }

    m_mouseOperation = false;
    m_selectObjs->mouseUp(point);
}
void CControlPointContainer::mouseMove(const QPointF &point, ControlState &state, int &pos)
{
    if (!m_mouseOperation)
        getState(point, m_state, m_pos);

    state = m_state;
    pos = m_pos;

    if (m_mouseOperation)
        mouseOperation(point);
}
