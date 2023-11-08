#include <QToolTip>
#include <QDebug>

#include "MDDrawVector.h"
#include "MDDrawMulti.h"
#include "DrawVectorData.h"
#include "MDCalculation.h"
#include "MDPropertyDrawVector.h"
#include "INodeEdit.h"
#include "InputVariableDialog.h"
#include "DynamicColorDialog.h"
#include "MDUndoData.h"



//////////////////////////////////////////////////////////////////////////
CMDDrawVector::CMDDrawVector()
{
    m_vector = new CDrawVectorData;

    QGradientStops stops;
    stops.append(QGradientStop(0, QColor(255, 0 ,0, 200)));
    stops.append(QGradientStop(1, QColor(0, 0 ,255, 200)));
    m_vector->m_brushManager.setStops(stops);
    m_vector->m_brushManager.setAngle(45);
    m_vector->m_brushManager.setBrushType(CMDBrushManager::BrushType::LinearGradient);

    CMDBrushManager brush;
    brush.setBrushType(CMDBrushManager::BrushType::Solid);
    brush.setColor(QColor(Qt::black));
    m_vector->m_penManager.setBrushManager(brush);
    m_vector->m_penManager.setWidth(1);
}
CMDDrawVector::~CMDDrawVector()
{
    delete m_vector;
}
void CMDDrawVector::initCreate()
{
    CMDDrawObj::initCreate();
}
//////////////////////////////////////////////////////////////////////////
QPainterPath *CMDDrawVector::matrixPath()
{
    return &m_vector->m_matrixPath;
}
QPainterPath *CMDDrawVector::path()
{
    return &m_vector->m_path;
}
CMDPenManager *CMDDrawVector::pen()
{
    return &m_vector->m_penManager;
}
CMDPenManager *CMDDrawVector::factPen()
{
    if (m_vector->m_dynamicPen.pen() != NULL)
        return m_vector->m_dynamicPen.pen();

    return &m_vector->m_penManager;
}
CMDBrushManager *CMDDrawVector::brush()
{
    return &m_vector->m_brushManager;
}
CMDBrushManager *CMDDrawVector::factBrush()
{
    if (m_vector->m_dynamicBrush.brush() != NULL)
        return m_vector->m_dynamicBrush.brush();

    return &m_vector->m_brushManager;
}
void CMDDrawVector::setRotateAngle(qreal value)
{
    CMDCalculation::limitAngle(value);

    if (m_vector->m_dataVector.rotateAngle() == value)
		return;
    m_vector->m_dataVector.setRotateAngle(value);

    loadGeneratePathEvent();
}
qreal CMDDrawVector::rotateAngle()
{
    return m_vector->m_dataVector.rotateAngle();
}
void CMDDrawVector::setShear(qreal value)
{
	backupData();
    setFactShear(value);
}
qreal CMDDrawVector::shear()
{
    return m_vector->m_dataVector.shear();
}
void CMDDrawVector::setRotatePoint(const QPointF& value)
{
	backupData();
    setFactRotatePoint(value);
}

QPointF CMDDrawVector::rotatePoint()
{
    return m_vector->m_dataVector.rotatePoint();
}
void CMDDrawVector::setRotatePointPos(const QPointF& value)
{
	backupData();
    mouseRotatePoint(value);
}
QPointF CMDDrawVector::rotatePointPos()
{
    return m_vector->m_dataVector.rotatePointPos();
}
void CMDDrawVector::setScalePoint(const QPointF& value)
{
    QPointF v = value;
    CMDCalculation::limitScalePoint(v);

    if (m_vector->m_dataVector.scalePoint() == v)
		return;
    m_vector->m_dataVector.setScalePoint(v);
    loadGeneratePathEvent();
}
QPointF CMDDrawVector::scalePoint()
{
    return m_vector->m_dataVector.scalePoint();
}
void CMDDrawVector::setXScale(qreal value)
{
	backupData();
    setFactXScale(value);
}
qreal CMDDrawVector::xScale()
{
    return m_vector->m_dataVector.xScale();
}
void CMDDrawVector::setYScale(qreal value)
{
	backupData();
    setFactYScale(value);
}
qreal CMDDrawVector::yScale()
{
    return m_vector->m_dataVector.yScale();
}
void CMDDrawVector::setEditMode(EditMode value)
{
    invalidate();
    if (value == EditMode::Normal)
        m_vector->m_editMode = value;
//    else if (value == EditMode::Segment && this is ISegmentEdit)
//        _editMode = value;
    else if (value == EditMode::Node && dynamic_cast<INodeEdit*>(this) != NULL)
        m_vector->m_editMode = value;
    invalidate();
}
EditMode CMDDrawVector::editMode()
{
    return m_vector->m_editMode;
}
void CMDDrawVector::setFillValue(qreal value)
{
    m_vector->m_fill.setScaleValue(value);
    invalidate();
}
void CMDDrawVector::setFill(CMDFill *value)
{
    m_vector->m_fill = *value;
}
CMDFill *CMDDrawVector::fill()
{
    return &m_vector->m_fill;
}
IDataVector *CMDDrawVector::dataVector()
{
    return &m_vector->m_dataVector;
}
CMDHMIActionManager *CMDDrawVector::mouseDownActions()
{
    return &m_vector->m_mouseDownActions;
}
CMDHMIActionManager *CMDDrawVector::mouseUpActions()
{
    return &m_vector->m_mouseUpActions;
}
CMDHMIActionManager *CMDDrawVector::rightMouseDownActions()
{
    return &m_vector->m_rightMouseDownActions;
}
CMDHMIActionManager *CMDDrawVector::rightMouseUpActions()
{
    return &m_vector->m_rightMouseUpActions;
}
void CMDDrawVector::setInputVariable(const QString &value)
{
    m_vector->m_inputVariable = value;
}
QString CMDDrawVector::inputVariable()
{
    return m_vector->m_inputVariable;
}
void CMDDrawVector::setDynamicBrush(CDynamicColorManager *value)
{
    m_vector->m_dynamicBrush = *value;
}
CDynamicColorManager* CMDDrawVector::dynamicBrush()
{
    return &m_vector->m_dynamicBrush;
}
void CMDDrawVector::setDynamicPen(CDynamicColorManager *value)
{
    m_vector->m_dynamicPen = *value;
}
CDynamicColorManager* CMDDrawVector::dynamicPen()
{
    return &m_vector->m_dynamicPen;
}
void CMDDrawVector::setFactShear(qreal value)
{
    CMDCalculation::limitShear(value);

    if (m_vector->m_dataVector.shear() == value)
		return;
    dataBk()->setState(ControlState::Shear);
    m_vector->m_dataVector.setShear(value);
    setFactRect(calculateOffset());
}
void CMDDrawVector::setFactRotatePoint(const QPointF& value)
{
    if (m_vector->m_dataVector.rotatePoint() == value)
		return;
    dataBk()->setState(ControlState::Center);
    m_vector->m_dataVector.setRotatePoint(value);
    setFactRect(calculateOffset());
}
void CMDDrawVector::setFactXScale(qreal value)
{
    CMDCalculation::limitScale(value);

    if (m_vector->m_dataVector.xScale() == value)
		return;
    dataBk()->setState(ControlState::XScale);
    m_vector->m_dataVector.setXScale(value);
    setFactRect(calculateOffset());
}
void CMDDrawVector::setFactYScale(qreal value)
{
    CMDCalculation::limitScale(value);

    if (m_vector->m_dataVector.yScale() == value)
		return;
    dataBk()->setState(ControlState::YScale);
    m_vector->m_dataVector.setYScale(value);
    setFactRect(calculateOffset());
}
bool CMDDrawVector::isVector()
{
    return true;
}
bool CMDDrawVector::isWidget()
{
    return false;
}
qreal CMDDrawVector::widenWidth()
{
    qreal w = factPen()->getPen().widthF();
    return (w >= CMDDrawObj::widenWidth()) ? w : CMDDrawObj::widenWidth();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawVector::mouseFrameMove(const QPointF& point, int pos)
{
    CMDDrawObj::mouseFrameMove(point, pos);
}
bool CMDDrawVector::canSelect(const QRectF& rect)
{
    if (CMDDrawObj::canSelect())
    {
        QRectF rf = path()->boundingRect();
        rf = CMDCalculation::limitRect(rf);
        return rect.intersects(rf);
    }

	return false;
}
void CMDDrawVector::draw(QPainter* painter, bool isPrinting/* = false*/)
{
    if (!finalVisible())
        return;

    painter->save();
    painter->setTransform((*matrix()) * painter->transform());
    CMDDrawObj::draw(painter, isPrinting);
	painter->restore();
}
void CMDDrawVector::boundMove(qreal xOff, qreal yOff, qreal wOff, qreal hOff)
{
    setFactRect(baseBoundMove(xOff, yOff, wOff, hOff));
}
bool CMDDrawVector::isVisible(const QPointF& point)
{
	return path()->contains(point);
}
QRectF CMDDrawVector::baseMultiRotate(qreal angle, const QPointF& center)
{
    dataBk()->setState(ControlState::MultiRotate);
    dataBk()->setOffset(CMDCalculation::rotateAtOffset(dataBk(), center, angle));
    m_vector->m_dataVector.setRotateAngle(dataBk()->dataVector()->rotateAngle() + angle);
    qreal a = m_vector->m_dataVector.rotateAngle();
    CMDCalculation::limitAngle(a);
    m_vector->m_dataVector.setRotateAngle(a);
	return calculateOffset();
}
void CMDDrawVector::multiRotate(qreal angle, const QPointF &center)
{
    setFactRect(baseMultiRotate(angle, center));
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawVector::generateBound()
{
    const qreal penMax = 2.f;

    QPen p = factPen()->getPen();
    //画笔的宽度较大时，转角长度会很长，需要特别计算Bound
    if (p.widthF() > penMax)
    {
        QRectF rf = basePath()->boundingRect();
        qreal w = p.widthF() / 2;
        rf.adjust(-w, -w, w, w);
        rf = matrix()->mapRect(rf);
        setBound(rf);
    }
    else
        setBound(m_vector->m_path.boundingRect());
}
void CMDDrawVector::backupData()
{
    CMDDrawObj::backupData();

    if (!isVector())
        return;

    dataBk()->setMatrixBound(m_vector->m_baseMatrix.mapRect(rect()));

    dataBk()->dataVector()->setRotateAngle(rotateAngle());
    dataBk()->dataVector()->setShear(shear());
    dataBk()->dataVector()->setRotatePoint(rotatePoint());
    dataBk()->dataVector()->setXScale(xScale());
    dataBk()->dataVector()->setYScale(yScale());
    dataBk()->dataVector()->setScalePoint(scalePoint());

    dataBk()->setMatrix(matrix());
    dataBk()->setBound(bound());

    dataBk()->dataVector()->setIsFlipX(isFlipX());
    dataBk()->dataVector()->setIsFlipY(isFlipY());
}
CMDPropertyDrawObj *CMDDrawVector::createPropertyDrawObj()
{
    return new CMDPropertyDrawVector(this);
}
void CMDDrawVector::getBrushes(QList<CMDBrushManager *> &list)
{
    CMDDrawObj::getBrushes(list);
    list.append(&m_vector->m_brushManager);
    m_vector->m_dynamicBrush.getBrushes(list);
    fill()->getBrushes(list);
}
void CMDDrawVector::getPens(QList<CMDPenManager *> &list)
{
    CMDDrawObj::getPens(list);
    list.append(&m_vector->m_penManager);
    m_vector->m_dynamicPen.getPens(list);
}
void CMDDrawVector::showToolTip()
{
    QString tip = toolTip();
    if(!tip.isEmpty())
        QToolTip::showText(QCursor::pos(), tip);
}
QPointF CMDDrawVector::vectorPoint(const QPointF& point)
{
    return CMDCalculation::invertPos(m_vector->m_matrix, point);
}
void CMDDrawVector::frameMove(qreal xOff, qreal yOff, qreal wOff, qreal hOff)
{
    dataBk()->setState(ControlState::FrameMove);
    dataBk()->setFixRate(CMDCalculation::fixPointRate(xOff, yOff, wOff, hOff));

    setNewRect(CMDCalculation::offsetRect(dataBk()->data()->rect(), xOff, yOff, wOff, hOff));
    setFactRect(calculateOffset());
}
QRectF CMDDrawVector::baseBoundMove(qreal xOff, qreal yOff, qreal wOff, qreal hOff)
{
	//计算边框移动后的数据
	QRectF rf;
    qreal rotateAngle = m_vector->m_dataVector.rotateAngle();
    qreal shear = m_vector->m_dataVector.shear();
    CMDCalculation::boundMoveData(dataBk()->data()->rect(), dataBk()->dataVector(), wOff, hOff,
        rotateAngle, shear, rf);
    m_vector->m_dataVector.setRotateAngle(rotateAngle);
    m_vector->m_dataVector.setShear(shear);
	setNewRect(rf);

    dataBk()->setState(ControlState::BoundMove);
    dataBk()->setOffset(QPointF(xOff, yOff));
    return calculateOffset();
}
void CMDDrawVector::loadInitalizationEvent()
{
    m_vector->m_mouseDownActions.setContainer(parent());
    m_vector->m_mouseUpActions.setContainer(parent());
    m_vector->m_rightMouseDownActions.setContainer(parent());
    m_vector->m_rightMouseUpActions.setContainer(parent());

    CMDDrawObj::loadInitalizationEvent();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawVector::onMouseDown(Qt::MouseButton button, qreal x, qreal y)
{
    //script
    QList<QVariant> paras;
    paras << x << y;
    if(button == Qt::MouseButton::LeftButton)
        parent()->run()->fireEvent(name(), m_vector->sMouseDown, paras);
    else if(button == Qt::MouseButton::RightButton)
        parent()->run()->fireEvent(name(), m_vector->sRMouseDown, paras);

    //action
    if (button == Qt::MouseButton::LeftButton)
        mouseDownActions()->execute();
    else if (button = Qt::MouseButton::RightButton)
        rightMouseDownActions()->execute();

    //inputVariable
    if (m_vector->m_inputVariable.count() > 0)
    {
        CInputVariableDialog dlg(dynamic_cast<QWidget*>(parent()));
        dlg.init(m_vector->m_inputVariable, parent()->framework());
        dlg.exec();
    }
}
void CMDDrawVector::onMouseUp(Qt::MouseButton button, qreal x, qreal y)
{
    //script
    QList<QVariant> paras;
    paras << x << y;
    if(button == Qt::MouseButton::LeftButton)
        parent()->run()->fireEvent(name(), m_vector->sMouseUp, paras);
    else if(button == Qt::MouseButton::RightButton)
        parent()->run()->fireEvent(name(), m_vector->sRMouseUp, paras);

    //action
    if (button == Qt::MouseButton::LeftButton)
        mouseUpActions()->execute();
    else if (button = Qt::MouseButton::RightButton)
        rightMouseUpActions()->execute();
}
void CMDDrawVector::onMouseMove(Qt::MouseButton button, qreal x, qreal y)
{
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawVector::loadMouseEvent(Qt::MouseButton button, const QPointF &location, MouseType type)
{
    if (isStudioMode())
        return;

    QPointF pf = location;
    if (isVector())
        pf = vectorPoint(location);

    switch (type)
    {
    case MouseType::Down:
        onMouseDown(button, pf.x()-rect().x(), pf.y()-rect().y());
        break;
    case MouseType::Up:
        onMouseUp(button, pf.x()-rect().x(), pf.y()-rect().y());
        break;
    case MouseType::Move:
        onMouseMove(button, pf.x()-rect().x(), pf.y()-rect().y());
        break;
    default:
        break;
    }
}
void CMDDrawVector::onMouseEnter()
{
    CMDDrawObj::onMouseEnter();
}
void CMDDrawVector::onMouseLeave()
{
    CMDDrawObj::onMouseLeave();
}
void CMDDrawVector::initContent()
{
    resetImagePath(getImagePath());

    m_vector->m_dynamicBrush.setContainer(this);
    m_vector->m_dynamicPen.setContainer(this);
    fill()->setContainer(this);

    generateContent();
}
void CMDDrawVector::generateContent()
{
    if (m_vector->m_dynamicBrush.brush() == NULL)
        m_vector->m_brushManager.generate(rect());
    else
        m_vector->m_dynamicBrush.brush()->generate(rect());

    if (m_vector->m_dynamicPen.pen() == NULL)
        m_vector->m_penManager.generate(rect());
    else
        m_vector->m_dynamicPen.pen()->generate(rect());

    m_vector->m_fill.generateContent(rect());
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawVector::generateMatrix()
{
	if (basePath()->isEmpty())
		return;

    QPointF rotatePointPos = m_vector->m_dataVector.rotatePointPos();
    CMDCalculation::calcMatrix(rect(), &m_vector->m_dataVector, m_vector->m_baseMatrix, rotatePointPos);
    m_vector->m_dataVector.setRotatePointPos(rotatePointPos);
	//成组时，叠加矩阵
    CMDDrawMulti* multi = multiParent();
    if (multi != NULL)
    {
        if (multi->matrix() == NULL)
            m_vector->m_multiMatrix = m_vector->m_baseMatrix;
        else
            m_vector->m_multiMatrix = m_vector->m_baseMatrix * (*(multi->matrix()));
    }

    m_vector->m_matrixPath = m_vector->m_baseMatrix.map(*basePath());
    m_vector->m_path = matrix()->map(*basePath());
}
QTransform* CMDDrawVector::matrix()
{
    if (multiParent() != NULL)
        return &m_vector->m_multiMatrix;

    return &m_vector->m_baseMatrix;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawVector::mouseRotatePoint(const QPointF &point)
{
    QPointF pf = CMDCalculation::rotatePoint(*dataBk()->matrix(), point, dataBk()->data()->rect());
    if (m_vector->m_dataVector.isFlipX())
        pf.setX(1 - pf.x());
    if (m_vector->m_dataVector.isFlipY())
        pf.setY(1 - pf.y());
    setFactRotatePoint(pf);
}
void CMDDrawVector::mouseRotate(const QPointF& point, bool isOrtho)
{
    int angle = CMDCalculation::rotateAngle(dataBk(), point, rotatePointPos());
    if (isOrtho)
        angle = angle/15*15;
    setRotateAngle(angle);
}
void CMDDrawVector::mouseShear(const QPointF& point, int pos, bool isOrtho)
{
    qreal shear = CMDCalculation::shear(dataBk(), dataBk()->dataVector(), point, pos);
    if (isOrtho)
        shear = (int)shear;
    setFactShear(shear);
}
//////////////////////////////////////////////////////////////////////////
QRectF CMDDrawVector::calculateOffset()
{
    bool isBound = (dataBk()->state() == ControlState::BoundMove);

    QPointF oldPoint = CMDCalculation::calcFixPoint(dataBk()->data()->rect(), dataBk()->dataVector(), dataBk()->fixRate(), isBound);
    QPointF newPoint = CMDCalculation::calcFixPoint(newRect(), &m_vector->m_dataVector, dataBk()->fixRate(), isBound);

    qreal xOff = oldPoint.x() - newPoint.x() + dataBk()->offset().x();
    qreal yOff = oldPoint.y() - newPoint.y() + dataBk()->offset().y();

    return CMDCalculation::offsetRect(newRect(), xOff, yOff, 0, 0);
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawVector::serialize(QJsonObject &json)
{
   CMDDrawObj::serialize(json);

   QJsonObject js;
   m_vector->serialize(js);
   json.insert("DrawVector", js);
}
void CMDDrawVector::deserialize(const QJsonObject &json)
{
    CMDDrawObj::deserialize(json);

    QJsonObject js = json.value("DrawVector").toObject();
    m_vector->deserialize(js);
}
CMDDrawObj* CMDDrawVector::clone()
{
    CMDDrawVector* obj = static_cast<CMDDrawVector*>(CMDDrawObj::clone());
    *obj->m_vector = *m_vector;

    return obj;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawVector::unMultiOper()
{
    if (isVector())
    {
        m_vector->m_dataVector.setIsFlipX(m_vector->m_dataVector.isFlipX() ^ multiParent()->isFlipX());
        m_vector->m_dataVector.setIsFlipY(m_vector->m_dataVector.isFlipY() ^ multiParent()->isFlipY());
        QRectF rf;
        CMDCalculation::matrixData(rect(), &m_vector->m_dataVector, *matrix(), rf);
        //上一行需要multiParent，下一行需要multiParent为null，所以这一行千万不能删
        setMultiParent(NULL);
        setFactRect(rf);
    }

    CMDDrawObj::unMultiOper();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawVector::onPaint(QPainter *p)
{
    factBrush()->draw(p, rect(), basePath());
    m_vector->m_fill.draw(p, rect(), basePath());
    factPen()->draw(p, rect(), basePath());
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawVector::setIsFlipX(bool value)
{
    if (m_vector->m_dataVector.isFlipX() == value)
        return;

    backupData();
    setFlip(value, true);
}
bool CMDDrawVector::isFlipX()
{
    return m_vector->m_dataVector.isFlipX();
}
void CMDDrawVector::setIsFlipY(bool value)
{
    if (m_vector->m_dataVector.isFlipY() == value)
        return;

    backupData();
    setFlip(value, false);
}
bool CMDDrawVector::isFlipY()
{
    return m_vector->m_dataVector.isFlipY();
}
void CMDDrawVector::setFlip(bool value, bool isX)
{
    if (isX)
        m_vector->m_dataVector.setIsFlipX(value);
    else
        m_vector->m_dataVector.setIsFlipY(value);

    m_vector->m_dataVector.setRotateAngle(-m_vector->m_dataVector.rotateAngle());
    qreal a = m_vector->m_dataVector.rotateAngle();
    CMDCalculation::limitAngle(a);
    m_vector->m_dataVector.setRotateAngle(a);
    m_vector->m_dataVector.setShear(-m_vector->m_dataVector.shear());
    QPointF rotatePoint = m_vector->m_dataVector.rotatePoint();
    if (isX)
        rotatePoint.setX(1 - m_vector->m_dataVector.rotatePoint().x());
    else
        rotatePoint.setY(1 - m_vector->m_dataVector.rotatePoint().y());
    m_vector->m_dataVector.setRotatePoint(rotatePoint);

    dataBk()->setState(ControlState::Center);
    setFactRect(calculateOffset());
}
QList<QStringList> CMDDrawVector::expNames()
{
    QList<QStringList> list;
    list << QStringList({"#layout"});
    list << QStringList({"drawvector.angle",        QObject::tr("旋转角度")});
    list << QStringList({"drawvector.shear",        QObject::tr("倾斜")});

    list << QStringList({"#appearance"});
    list << QStringList({"drawvector.fill",         QObject::tr("填充值")});
    list << QStringList({"drawvector.fillBrush",    QObject::tr("填充背景切换")});
    list << QStringList({"drawvector.brush",        QObject::tr("背景切换")});
    list << QStringList({"drawvector.pen",          QObject::tr("线条切换")});
    return list;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawVector::loadDataChangedEvent(CMDExpInfo *exp)
{
    CMDDrawObj::loadDataChangedEvent(exp);

    if(exp->type() == (int)DrawType::Vector)
    {
        switch (exp->index())
        {
        case 0:     //angle
            setDynamicRotateAngle(exp->decimalValue());
            break;
        case 1:     //shear
            setDynamicShear(exp->decimalValue());
            break;
        case 2://fill
            setFillValue(exp->decimalValue());
            break;
        case 3://fillBrush
            m_vector->m_fill.loadDataChangedEvent(rect(), exp);
            invalidate();
            break;
        case 4://brush
            m_vector->m_dynamicBrush.resetValue(exp->decimalValue());
            generateContent();
            invalidate();
            break;
        case 5://pen
            m_vector->m_dynamicPen.resetValue(exp->decimalValue());
            generateContent();
            invalidate();
            break;
        default:
            break;
        }
    }
}
//////////////////////////////////////////////////////////////////////////
IUndoDataObj *CMDDrawVector::createUndoData()
{
    return new CMDUndoDataVector;
}
bool CMDDrawVector::revertUndoData(IUndoDataObj *value)
{
    bool hasChange = CMDDrawObj::revertUndoData(value);

    CMDUndoDataVector* u = dynamic_cast<CMDUndoDataVector*>(value);
    CDataVector* uDataVector = dynamic_cast<CDataVector*>(u->dataVector());
    if (!m_vector->m_dataVector.equal(uDataVector))
    {
        m_vector->m_dataVector = *uDataVector;
        hasChange = true;
    }

    return hasChange;
}
IUndoDataObj *CMDDrawVector::fillUndoData(bool isOld)
{
    CMDUndoDataVector* u = dynamic_cast<CMDUndoDataVector*>(CMDDrawObj::fillUndoData(isOld));
    if (isOld)
        u->setDataVector(dynamic_cast<CDataVector*>(dataBk()->dataVector()));
    else
        u->setDataVector(&m_vector->m_dataVector);

    return u;
}
bool CMDDrawVector::undoEqual()
{
    return CMDDrawObj::undoEqual()
            && dynamic_cast<CDataVector*>(dataBk()->dataVector())->equal(&m_vector->m_dataVector);
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawVector::undoBrush(CMDBrushManager *value)
{
    *brush() = *value;
    generateContent();
}
void CMDDrawVector::undoPen(CMDPenManager *value)
{
    *pen() = *value;
    generateContent();
}
void CMDDrawVector::undoAction(int id, CMDHMIActionManager* value)
{
    CMDHMIActionManager* manager = NULL;
    switch (id)
    {
    case (int)DrawVectorId::MouseDownAction:
        manager = mouseDownActions();
        break;
    case (int)DrawVectorId::MouseUpAction:
        manager = mouseUpActions();
        break;
    case (int)DrawVectorId::RightMouseDownAction:
        manager = rightMouseDownActions();
        break;
    case (int)DrawVectorId::RightMouseUpAction:
        manager = rightMouseUpActions();
        break;
    default:
        break;
    }

    *manager = *value;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawVector::setDynamicRotateAngle(qreal value)
{
    setRotateAngle(value);
    generateGroupPath();
}
void CMDDrawVector::setDynamicShear(qreal value)
{
    setShear(value);
    generateGroupPath();
}
//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawVector::createPropertyDrawObj(const QString &className)
{
    if(className.compare("DrawVector", Qt::CaseInsensitive) == 0)
        return new CMDPropertyDrawVector(this);

    return CMDDrawObj::createPropertyDrawObj(className);
}
void CMDDrawVector::initClassInfo(QStringList &classInfo)
{
    CMDDrawObj::initClassInfo(classInfo);
    classInfo.append("DrawVector");
}
//////////////////////////////////////////////////////////////////////////



