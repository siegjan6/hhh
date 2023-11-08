#include <QDebug>

#include "SelectObjectManager.h"
#include "ControlPointContainer.h"
#include "MDDrawObj.h"
#include "MDDrawVector.h"
#include "MDCalculation.h"
#include "Studio.h"
#include "PropertySelectObject.h"

CSelectObjectManager::CSelectObjectManager(CControlPointContainer* controlPoint)
{
    m_isVector = false; //!是否是矢量控件
    m_lastSelectedObj = NULL; //!选择多个控件时，最后一个被选中的控件
    m_shear = 0;
    m_rotateAngle = 0;
    m_rotatePoint = QPointF(0.5, 0.5);
    m_propertyObject = NULL;


    Q_ASSERT(controlPoint != NULL);
    m_controlPoint = controlPoint;

    m_dataBk = CMDDrawObj::createCalcData();
}

CSelectObjectManager::~CSelectObjectManager()
{
    delete m_dataBk;
    if (m_propertyObject != NULL)
        delete m_propertyObject;
}
//////////////////////////////////////////////////////////////////////////
bool CSelectObjectManager::isSingle()
{
    return (m_list.count() == 1);
}
bool CSelectObjectManager::isSingleVector()
{
    return isSingle() & isVector();
}
bool CSelectObjectManager::isEmpty()
{
    return (m_list.count() == 0);
}
CMDDrawObj *CSelectObjectManager::obj()
{
    return isSingle() ? m_list.at(0) : NULL;
}
CMDDrawVector *CSelectObjectManager::vector()
{
    return isSingleVector() ? static_cast<CMDDrawVector*>(m_list.at(0)) : NULL;
}
EditMode CSelectObjectManager::studioMode()
{
    return isSingleVector() ? vector()->editMode() : EditMode::Normal;
}
bool CSelectObjectManager::isCutomEdit()
{
    return studioMode() == EditMode::Normal
            && isSingleVector()
            && dynamic_cast<ICustomEdit*>(vector()) != NULL;
}
ICustomEdit *CSelectObjectManager::custom()
{
    if (isCutomEdit())
        return dynamic_cast<ICustomEdit*>(vector());
    return NULL;
}
bool CSelectObjectManager::existGroup()
{
    for(int i = 0; i < m_list.count(); i++)
    {
        CMDDrawObj* obj = m_list.at(i);
        if (obj->type() == DrawType::Group)
            return true;
    }

    return false;
}
bool CSelectObjectManager::canCombine()
{
    int count = m_list.count();
    if(count < 2)
        return false;

    for(int i = 0; i < count; i++)
    {
        if(!m_list.at(i)->canCombine())
            return false;
    }
    return true;
}
bool CSelectObjectManager::existCombine()
{
    for(int i = 0; i < m_list.count(); i++)
    {
        if (m_list.at(i)->type() == DrawType::Combine)
            return true;
    }
    return false;
}
INodeEdit *CSelectObjectManager::node()
{
    if (isSingleVector())
        return dynamic_cast<INodeEdit*>(vector());
    return NULL;
}
bool CSelectObjectManager::isNodeEdit()
{
    return studioMode() == EditMode::Node;
}
bool CSelectObjectManager::isAddNode()
{
    return (isNodeEdit() && node()->nodeState() == NodeState::Add);
}
bool CSelectObjectManager::isDeleteNode()
{
    return (isNodeEdit() && node()->nodeState() == NodeState::Delete);
}
//////////////////////////////////////////////////////////////////////////
QRectF CSelectObjectManager::rect()
{
    return isSingle() ? obj()->rect() : m_rect;
}
void CSelectObjectManager::setRect(const QRectF &rf)
{
    backupData();
    invalidate();

    QRectF r = rf;
    CMDCalculation::limitRect(r);
    qreal x = 0, y = 0, width = 0, height = 0;
    x = r.x() - rect().x();
    y = r.y() - rect().y();
    width = r.width() - rect().width();
    height = r.height() - rect().height();

    qreal xOffRate = width / m_dataBk->data()->rect().width();
    qreal yOffRate = height / m_dataBk->data()->rect().height();

    for(int i = 0; i < m_list.count(); i++)
    {
        CMDDrawObj* obj = m_list.at(i);
        QRectF objRect = obj->dataBk()->bound();
        qreal objX = x + (objRect.x() - m_dataBk->data()->rect().x()) * xOffRate;
        qreal objW = objRect.width()*xOffRate;
        qreal objY = y + (objRect.y() - m_dataBk->data()->rect().y()) * yOffRate;
        qreal objH = objRect.height()*yOffRate;

        obj->boundMove(objX, objY, objW, objH);
    }

    resetProperty();
    invalidate();
}
void CSelectObjectManager::setRotateAngle(qreal value)
{
    CMDCalculation::limitAngle(value);

    if (m_rotateAngle == value)
        return;
    m_rotateAngle = value;

    m_matrix.reset();
    m_matrix.translate(rotatePointPos().x(), rotatePointPos().y());
    m_matrix.rotate(rotateAngle());
    m_matrix.translate(-rotatePointPos().x(), -rotatePointPos().y());
}
void CSelectObjectManager::setRotatePoint(const QPointF &value)
{
    if (m_rotatePoint == value)
        return;

    m_rotatePoint = value;
    setRotatePointPos();
}
QPointF CSelectObjectManager::rotatePointPos()
{
    return isSingleVector() ? vector()->rotatePointPos() : m_rotatePointPos;
}
QTransform *CSelectObjectManager::matrix()
{
    return isSingleVector() ? vector()->matrix() : &m_matrix;
}
void CSelectObjectManager::setVectorSign()
{
    m_isVector = true;

    int count = m_list.count();
    for (int i = 0; i < count; i++)
    {
        if (!m_list.at(i)->isVector())
        {
            m_isVector = false;
            break;
        }
    }
    if (isEmpty())
        m_isVector = false;
}
void CSelectObjectManager::setRotatePoint()
{
    m_rotatePoint = CMDCalculation::rotatePoint(QTransform(), rotatePointPos(), rect());
}
void CSelectObjectManager::setRotatePointPos()
{
    m_rotatePointPos.setX(rect().x() + rect().width() * rotatePoint().x());
    m_rotatePointPos.setY(rect().y() + rect().height() * rotatePoint().y());
}
void CSelectObjectManager::setListRect()
{
    QRectF rf;
    for(int i = 0; i < m_list.count(); i++)
        rf = (rf.isEmpty()) ? m_list.at(i)->bound() : rf.united(m_list.at(i)->bound());
    m_rect = rf;
}
void CSelectObjectManager::initProperty()
{
    if (isEmpty() || isSingle())
    {
        m_rotateAngle = 0;
        m_rotatePoint = QPointF(0.5, 0.5);
        m_shear = 0;
        m_matrix.reset();
    }
}
void CSelectObjectManager::resetProperty()
{
    initProperty();

    setListRect();
    setRotatePointPos();
}
void CSelectObjectManager::invalidate()
{
    m_controlPoint->invalidate();
}
void CSelectObjectManager::resetLastSelectedObject(QList<CMDDrawObj *> *objList)
{
    setLastSelectedObj(NULL);
    if (objList == NULL || objList->count() < 2)
        return;

    setLastSelectedObj(objList->at(objList->count() - 1));
}
void CSelectObjectManager::backupData()
{
    m_dataBk->data()->setRect(m_rect);

    for(int i = 0; i < m_list.count(); i++)
    {
        CMDDrawObj* obj = m_list.at(i);
        obj->backupData();
    }
}
//////////////////////////////////////////////////////////////////////////
void CSelectObjectManager::reset(QList<CMDDrawObj*> *objList)
{
    resetLastSelectedObject(objList);
    CStudio* studio = dynamic_cast<CStudio*>(m_controlPoint->container()->studio());

    m_list.clear();
    if (objList == NULL)
    {
        studio->resetSelectObjs();
        return;
    }

    m_list.append(*objList);

    setVectorSign();
    resetProperty();

    studio->resetSelectObjs();
}
bool CSelectObjectManager::isVisible(const QPointF &point)
{
    for(int i = 0; i < m_list.count(); i++)
    {
        CMDDrawObj* obj = m_list.at(i);
        if (obj->canSelect(point))
            return true;
    }

    return false;
}
void CSelectObjectManager::calculateProperty()
{
    setListRect();
    setRotatePointPos();
}
//////////////////////////////////////////////////////////////////////////
void CSelectObjectManager::mouseFrameMove(const QPointF &point, int pos, bool isOrtho)
{
    if (isSingle())
        obj()->mouseFrameMove(point, pos);
    else
    {
        invalidate();

        //正交模式
        qreal orthoTan = 0;
        if (isOrtho)
            orthoTan = CMDCalculation::calcOrthoTan(m_dataBk->data()->rect(), pos);

        qreal x = 0, y = 0, width = 0, height = 0;
        CMDCalculation::offset(m_dataBk->mousePos(), point, pos, x, y, width, height, orthoTan);
        CMDCalculation::limitOffset(m_dataBk->data()->rect(), x, y, width, height);

        qreal xOffRate = width / m_dataBk->data()->rect().width();
        qreal yOffRate = height / m_dataBk->data()->rect().height();

        for(int i = 0; i < m_list.count(); i++)
        {
            CMDDrawObj* obj = m_list.at(i);

            QRectF objRect = obj->dataBk()->bound();
            qreal objX = x + (objRect.x() - m_dataBk->data()->rect().x()) * xOffRate;
            qreal objW = objRect.width()*xOffRate;
            qreal objY = y + (objRect.y() - m_dataBk->data()->rect().y()) * yOffRate;
            qreal objH = objRect.height()*yOffRate;

            obj->boundMove(objX, objY, objW, objH);
        }

        resetProperty();
        invalidate();
    }
}
void CSelectObjectManager::mouseRotatePoint(const QPointF &point)
{
    if (!isVector())
        return;

    if (isSingle())
        vector()->mouseRotatePoint(point);
    else
    {
        invalidate();
        setRotatePoint(CMDCalculation::rotatePoint(*m_dataBk->matrix(), point, m_dataBk->data()->rect()));
        invalidate();
    }
}
void CSelectObjectManager::mouseRotate(const QPointF &point, bool isOrtho)
{
    if (!isVector())
        return;

    if (isSingle())
        vector()->mouseRotate(point, isOrtho);
    else
    {
        invalidate();

        int angle = (int)CMDCalculation::rotateAngle(m_dataBk, point, rotatePointPos());
        //取15的整数
        if (isOrtho)
            angle = angle/15*15;
        setRotateAngle(angle);

        for(int i = 0; i < m_list.count(); i++)
        {
            CMDDrawObj* obj = m_list.at(i);
            static_cast<CMDDrawVector*>(obj)->multiRotate(rotateAngle(), rotatePointPos());
        }

        invalidate();
    }
}
void CSelectObjectManager::mouseShear(const QPointF &point, int pos, bool isOrtho)
{
    if (!isSingleVector())
        return;

    vector()->mouseShear(point, pos, isOrtho);
}
void CSelectObjectManager::mouseCustom(const QPointF &point, int pos)
{
    if (isCutomEdit())
        custom()->moveCustom(point, pos);
}
void CSelectObjectManager::moveNode(const QPointF &point, int pos)
{
    if (!isNodeEdit())
        return;
    invalidate();
    node()->moveNode(point, pos);
    invalidate();
}
void CSelectObjectManager::addNode(const QPointF &point)
{
    if (!isNodeEdit())
        return;
    invalidate();
    node()->addNode(point);
    invalidate();
}
void CSelectObjectManager::deleteNode(int pos)
{
    if (!isNodeEdit())
        return;
    invalidate();
    node()->deleteNode(pos);
    invalidate();
}
void CSelectObjectManager::mouseDown(const QPointF &point, ControlState state)
{
    backupData();
    for(int i = 0; i < m_list.count(); i++)
    {
        CMDDrawObj* obj = m_list.at(i);
        obj->mouseDown(point);
    }

    m_dataBk->setState(state);
    m_dataBk->setMousePos(point);
}
void CSelectObjectManager::mouseMove(const QPointF &point)
{
    invalidate();

    for(int i = 0; i < m_list.count(); i++)
    {
        CMDDrawObj* obj = m_list.at(i);
        obj->mouseMove(point);
    }

     setListRect();
     setRotatePointPos();

     invalidate();
}
void CSelectObjectManager::mouseUp(const QPointF &/*point*/)
{
    invalidate();

    m_rotateAngle = 0;
    m_matrix.reset();

    if (m_dataBk->state() == ControlState::Rotate)
    {
        setListRect();
        setRotatePoint();
        invalidate();
    }

    resetProperty();
}
//////////////////////////////////////////////////////////////////////////
void CSelectObjectManager::flipX()
{
    if (isSingle())
        vector()->setIsFlipX(!vector()->isFlipX());
    else
    {
        invalidate();

        for(int i = 0; i < m_list.count(); i++)
        {
            CMDDrawObj* obj = m_list.at(i);
            CMDDrawVector* v = static_cast<CMDDrawVector*>(obj);
            v->setIsFlipX(!v->isFlipX());

            qreal offset = rect().left() + rect().right() - v->bound().left()*2 - v->bound().width();
            QRectF rf = v->rect();
            rf.translate(offset, 0);
            v->setRect(rf);
        }

        invalidate();
    }
}
void CSelectObjectManager::flipY()
{
    if (isSingle())
        vector()->setIsFlipY(!vector()->isFlipY());
    else
    {
        invalidate();

        for(int i = 0; i < m_list.count(); i++)
        {
            CMDDrawObj* obj = m_list.at(i);
            CMDDrawVector* v = static_cast<CMDDrawVector*>(obj);
            v->setIsFlipY(!v->isFlipY());

            qreal offset = rect().top() + rect().bottom() - v->bound().top()*2 - v->bound().height();
            QRectF rf = v->rect();
            rf.translate(0, offset);
            v->setRect(rf);
        }

        invalidate();
    }
}
//////////////////////////////////////////////////////////////////////////
QObject *CSelectObjectManager::propertyDrawObj()
{
    if (m_propertyObject == NULL)
        m_propertyObject = new CPropertySelectObject(this);
    return m_propertyObject;
}

void CSelectObjectManager::undoOperation()
{
    if (m_list.count() == 0)
        return;
    QUndoStack* undo = m_list.at(0)->parent()->studio()->undo();
    if (m_list.count() > 1)
        undo->beginMacro("Macro");
    for(int i = 0; i < m_list.count(); i++)
    {
        CMDDrawObj* obj = m_list.at(i);
        switch (m_dataBk->state()) {
        case ControlState::Move:
        case ControlState::FrameMove:
        {
            QRectF oldValue = obj->dataBk()->data()->rect();
            QRectF newValue = obj->rect();
            if (oldValue != newValue)
            {
                //undo->push(CDrawObjCommand(obj, oldValue, newValue, DrawCommandId::Rect));
            }
        }
            break;
        default:
            break;
        }

    }

    if (m_list.count() > 1)
        undo->endMacro();
}


//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

