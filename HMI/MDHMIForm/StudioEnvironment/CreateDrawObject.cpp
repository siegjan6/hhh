#include <QtGlobal>
#include <QtCore/qmath.h>
#include "MDCalculation.h"
#include "INodeEdit.h"
#include "ControlPointContainer.h"
#include "Tool.h"
#include "Studio.h"
#include "CreateDrawObject.h"
#include "DrawCreator.h"
#include "MDHMIForm.h"
#include "ObjectChangedCommand.h"
#include "IHMIStudio.h"
//////////////////////////////////////////////////////////////////////////
CCreateDrawObject::CCreateDrawObject(CStudio *container)
{
    Q_ASSERT(container != NULL);
    m_studio = container;

    //!控件创建过程状态
    m_state = CreateState::NotCreate;
    //!创建控件类型
    m_type = DrawType::Rect;
    //!节点控件,默认为null
    m_nodeObject = NULL;
    //!是否是以节点控件方式创建
    m_isNodeCreate = false;
    m_orthoState = OrthoMode::Invalid;
}
//////////////////////////////////////////////////////////////////////////
QPointF CCreateDrawObject::orthoSquarePoint(const QPointF &begin, const QPointF &end)
{
    if (m_orthoState != OrthoMode::Square)
        return end;

    qreal w = end.x()- begin.x();
    qreal h = end.y() - begin.y();
    qreal absW = qAbs(w);
    qreal absH = qAbs(h);
    qreal dis = (absW < absH) ? absW : absH;

    QPointF pf = begin;
    if (w > 0)
        pf.setX(pf.x() + dis);
    else
        pf.setX(pf.x() - dis);
    if (h > 0)
        pf.setY(pf.y() + dis);
    else
        pf.setY(pf.y() - dis);

    return pf;
}
QPointF CCreateDrawObject::orthoHoriorVert(const QPointF &begin, const QPointF &end)
{
    if (m_orthoState != OrthoMode::HoriOrVert)
        return end;

    qreal w = end.x() - begin.x();
    qreal h = end.y() - begin.y();
    qreal absW = qAbs(w);
    qreal absH = qAbs(h);
    qreal absW1 = absW*qTan(PI*30/180);
    qreal absW2 = absW*qTan(PI*60/180);

    QPointF pf = begin;
    if (absH <= absW1)
        pf.setX(end.x());
    else if (absH > absW1 && absH < absW2)
    {
        pf.setX(pf.x() + w);
        if (h >= 0)
            pf.setY(pf.y() + absW);
        else
            pf.setY(pf.y() - absW);
    }
    else if (absH >= absW2)
        pf.setY(end.y());

    return pf;
}
//////////////////////////////////////////////////////////////////////////
void CCreateDrawObject::begin(DrawType type)
{
    end();

    m_state = CreateState::BeginCreate;
    m_type = type;
    m_isNodeCreate = CDrawCreator::isDrawNode(type);

    //ortho
    m_orthoState = OrthoMode::Invalid;
    if (m_studio->isOrtho())
        m_orthoState = CDrawCreator::drawOrthoMode(type);
}
void CCreateDrawObject::end()
{
    if (m_nodeObject != NULL)
    {
        INodeEdit* node = dynamic_cast<INodeEdit*>(m_nodeObject);

        //最后一个点为动态移动点，需要删除
        int count = node->nodeDatas()->count();
        if (count > 0)
            node->deleteNode(count - 1);

        //创建成功
        if (node->createSuccess())
        {
            node->setIsNodeCreating(false);
            QList<CMDDrawObj*> list;
            list.append(m_nodeObject);

            //先删除，否则会重复添加
            m_studio->objs()->removeOne(m_nodeObject);
            QList<int> indexs;
            indexs.append(m_studio->objs()->count());
            m_studio->undo()->push(new CObjectChangedCommand(
                &list, &indexs, m_studio, (int)ObjectChangedId::Add));

            QList<CMDDrawObj*> objs;
            objs.append(m_nodeObject);
            m_studio->addToObjectTree(&objs);

            m_studio->selectObjs()->clear();
            m_studio->selectObjs()->append(m_nodeObject);

            m_studio->controlPoint()->changeSelectObj(&list);
            m_nodeObject->invalidate();
        }
        else
        {
            m_studio->objs()->removeOne(m_nodeObject);
            delete m_nodeObject;
        }


        m_nodeObject = NULL;
    }

    m_studio->window()->setCursor(Qt::ArrowCursor);
    m_state = CreateState::NotCreate;
    m_orthoState = OrthoMode::Invalid;

    m_studio->container()->framework()->manager()->resetToolKit();
}
bool CCreateDrawObject::mouseDown(Qt::MouseButton button, const QPointF &location, const QPointF &revertPoint)
{
    QPointF revert = revertPoint;
    if (m_studio->isGrid())
        revert = CTool::gridPointF(revert);

    if (m_state == CreateState::BeginCreate)
    {
        m_state = CreateState::HasCreate;

        if (!m_isNodeCreate)
        {
            m_studio->rubberBand()->setGeometry(QRect(location.toPoint(), QSize()));
            m_studio->rubberBand()->show();
        }

        if (m_nodeObject != NULL)
        {
            end();
            return true;
        }

        if (m_isNodeCreate)
        {
            m_nodeObject = dynamic_cast<CMDDrawVector*>(CDrawCreator::createDrawObj(m_type));
            m_nodeObject->setParent(m_studio->container());
            static_cast<CObjName*>(m_studio->nameManager())->createName(m_nodeObject);

            m_studio->objs()->append(m_nodeObject);
            INodeEdit* node = dynamic_cast<INodeEdit*>(m_nodeObject);
            node->setIsNodeCreating(true);
            node->addNode(revert);
            node->addNode(revert);
            m_nodeObject->loadInitalizationEvent();
        }

        return true;
    }

    if (m_state == CreateState::HasCreate)
    {
        if (m_isNodeCreate && m_nodeObject != NULL)
        {
            INodeEdit* node = dynamic_cast<INodeEdit*>(m_nodeObject);
            int count = node->nodeDatas()->count();
            QPointF lastPoint = node->nodeDatas()->at(count - 2);

            revert = orthoHoriorVert(lastPoint, revert);
            //预防在同一位置重复添加点
            if (lastPoint.toPoint() == revert.toPoint())
                return true;

            node->moveNode(revert, count - 1);
            node->addNode(revert);

            //添加点后创建成功，则直接结束
            if (node->createFinish())
            {
                end();
                return true;
            }
        }

        return true;
    }

    return false;
}
bool CCreateDrawObject::mouseUp(Qt::MouseButton button, const QPointF &location, const QPointF &revertPoint, const QPointF &revertDownPoint)
{
    QPointF revert = revertPoint;
    QPointF reverDown = revertDownPoint;
    if (m_studio->isGrid())
    {
        revert = CTool::gridPointF(revert);
        reverDown = CTool::gridPointF(reverDown);
    }

    if (m_state != CreateState::NotCreate)
    {
        if (!m_isNodeCreate)
        {
            if (m_state == CreateState::HasCreate)
            {
                revert = orthoSquarePoint(reverDown, revert);
                m_studio->rubberBand()->hide();

                QRectF rf(reverDown, revert);
                rf = rf.normalized();
                CMDDrawObj* obj = m_studio->studioCreateDrawObj(m_type, rf);
                if (obj != NULL)
                {
                    QList<CMDDrawObj*> list;
                    list.append(obj);

                    m_studio->selectObjs()->clear();
                    m_studio->selectObjs()->append(obj);

                    m_studio->controlPoint()->changeSelectObj(&list);
                    obj->invalidate();
                }

                end();
            }
        }

        return true;
    }

    return false;
}
bool CCreateDrawObject::mouseMove(Qt::MouseButton button, const QPointF &location, const QPointF &revertPoint, const QPointF &downPoint)
{
    QPointF revert = revertPoint;
    QPointF loca = location;
    if (m_studio->isGrid())
        revert = CTool::gridPointF(revert);

    if (m_state != CreateState::NotCreate)
    {
        m_studio->window()->setCursor(Qt::CrossCursor);

        if (m_isNodeCreate)
        {
            if (m_nodeObject != NULL)
            {
                INodeEdit* node = dynamic_cast<INodeEdit*>(m_nodeObject);
                int count = node->nodeDatas()->count();
                QPointF lastPoint  = node->nodeDatas()->at(count - 2);

                revert = orthoHoriorVert(lastPoint, revert);
                node->moveNode(revert, count - 1);
            }
        }
        else
        {
            loca = orthoSquarePoint(downPoint, loca);
            if (m_state == CreateState::HasCreate)
            {
                QRectF rf(downPoint, loca);
                rf = rf.normalized();
                m_studio->rubberBand()->setGeometry(rf.toRect());
            }
        }

        return true;

    }

    return false;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

