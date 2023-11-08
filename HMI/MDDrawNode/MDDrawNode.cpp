#include <QJsonArray>
#include "MDDrawNode.h"
#include "MDCalculation.h"
#include "MDPropertyDrawNode.h"
#include "CLineRegion.h"
#include "undo/UndoDataNode.h"
#include "MDExpInfo.h"
#include "MDNodeStream.h"
#include "MDJsonConvert.h"

CMDDrawNode::CMDDrawNode():CMDDrawVector()
{
    m_state = NodeState::Move;
    m_isNodeCreating = false;
    m_isNodeOperate = false;
    m_pipeBackGroundVisible = false;
    m_pLineStream = new CMDNodeStream(this);
    m_pipeClr1 = Qt::white;
    m_pipeClr2 = Qt::black;
    m_Density = 15;
    m_pipewidth = 20;
}

CMDDrawNode::~CMDDrawNode()
{
    if(m_pLineStream)
    {
        m_pLineStream->stop();
        delete m_pLineStream;
        m_pLineStream = NULL;
    }
}

void CMDDrawNode::onPaint(QPainter *p)
{
    drawPath(p);
}

void CMDDrawNode::onGeneratePath()
{
    if (!isNodeOperate()) //! 不是鼠标操作
    {
        scaleDatas(rect(), dataBk()->data()->rect(), m_nodes,m_nodesBk);
    }
    generateNodePath(basePath());

    setDirectRect(basePath()->boundingRect());
}
bool CMDDrawNode::isVisible(const QPointF &point)
{
    QPointF pf = CMDCalculation::invertPos(*matrix(), point);

    const int w = 6;
    qreal pw = widenWidth();
    pw = (pw >= w) ? pw : w;

    QRectF rf = rect();
    rf.adjust(-pw, -pw, pw, pw);
    if(!rf.contains(pf))
        return false;

    CLineRegion region;
    region.setRegion(path(),pw);
    bool result = region.contains(QPoint((int)point.x(),(int)point.y()));
    return result;
}

void CMDDrawNode::onInitialization()
{
    //if(!isStudioMode())
    m_pLineStream->start();
    onNodeChanged();
}

CMDPropertyDrawObj *CMDDrawNode::createPropertyDrawObj()
{
    return new CMDPropertyDrawNode(this);
}
CMDDrawObj *CMDDrawNode::clone()
{
    CMDDrawNode* obj = static_cast<CMDDrawNode*>(CMDDrawVector::clone());
    obj->m_state = this->m_state;
    obj->m_nodes.append(this->m_nodes);

    obj->m_pipeClr1 = this->m_pipeClr1;
    obj->m_pipeClr2 = this->m_pipeClr2;
    obj->m_pipeBackGroundVisible = this->m_pipeBackGroundVisible;
    obj->m_pipewidth = this->m_pipewidth;

    if(obj->m_pLineStream != NULL)
        delete obj->m_pLineStream;
    obj->m_pLineStream = this->m_pLineStream->clone();
    obj->m_pLineStream->setObject(obj);

    return (CMDDrawObj*)obj;
}

void CMDDrawNode::serialize(QJsonObject &json)
{
    CMDDrawVector::serialize(json);

    QJsonObject DrawNodeJson;

    DrawNodeJson.insert("Nodescount",m_nodes.count());
    int count = m_nodes.count();
    QJsonArray array;
    for(int i = 0; i < count; i++)
    {
        QJsonObject jsonItem;
        jsonItem.insert("node",CMDJsonConvert::toJson(m_nodes.at(i)));
        array.append(jsonItem);
    }
    DrawNodeJson.insert("Nodes",array);
    DrawNodeJson.insert("PipeClr1",(qreal)m_pipeClr1.rgba());
    DrawNodeJson.insert("PipeClr2",(qreal)m_pipeClr2.rgba());
    DrawNodeJson.insert("Density",m_Density);
    DrawNodeJson.insert("PipeBackGroundVisible",m_pipeBackGroundVisible);
    DrawNodeJson.insert("Pipewidth",m_pipewidth);
    m_pLineStream->serialize(DrawNodeJson);
    json.insert("DrawNode",DrawNodeJson);
}

void CMDDrawNode::deserialize(const QJsonObject &json)
{
    CMDDrawVector::deserialize(json);

    QJsonObject DrawNodeJson = json.value("DrawNode").toObject();

    m_nodes.clear();
    int count = DrawNodeJson.value("Nodescount").toInt();
    QJsonArray array = DrawNodeJson.value("Nodes").toArray();
    for(int i = 0; i < count; i++)
    {
        m_nodes.append(CMDJsonConvert::toQPointF(array.at(i).toObject().value("node")));
    }
    m_pipeClr1 = QColor::fromRgba(DrawNodeJson.value("PipeClr1").toDouble());
    m_pipeClr2 = QColor::fromRgba(DrawNodeJson.value("PipeClr2").toDouble());
    m_Density = DrawNodeJson.value("Density").toDouble();
    m_pipeBackGroundVisible = DrawNodeJson.value("PipeBackGroundVisible").toBool();
    m_pipewidth = DrawNodeJson.value("Pipewidth").toDouble();
    m_pLineStream->deserialize(DrawNodeJson);
}

void CMDDrawNode::setNodeState(NodeState value)
{
    m_state = value;
    invalidate();
}

NodeState CMDDrawNode::nodeState()
{
    return m_state;
}

void CMDDrawNode::setIsNodeCreating(bool value)
{
    m_isNodeCreating = value;
}

bool CMDDrawNode::isNodeCreating()
{
    return m_isNodeCreating;
}

bool CMDDrawNode::isNodeOperate()
{
    return m_isNodeOperate;
}

bool CMDDrawNode::createSuccess()
{
    return  m_nodes.count() >= minCount();
}

bool CMDDrawNode::createFinish()
{
    return  false;
}

int CMDDrawNode::minCount()
{
    return 2;
}

void CMDDrawNode::backupData()
{
    CMDDrawVector::backupData();
    m_nodesBk.clear();
    m_nodesBk.append(m_nodes);
}

QList<QPointF> *CMDDrawNode::nodeDatas()
{
    return &m_nodes;
}

void CMDDrawNode::moveNode(const QPointF &point, int pos)
{
    QPointF p = CMDCalculation::invertPos(*dataBk()->matrix(),point);
    QPointF mouse = CMDCalculation::invertPos(*dataBk()->matrix(), dataBk()->mousePos());
    QPointF off = QPointF(p.x() - mouse.x(), p.y() - mouse.y());
    m_isNodeOperate = true;
    onMove(p, off, pos);
    this->loadGeneratePathEvent();
    m_isNodeOperate = false;
}

void CMDDrawNode::addNode(const QPointF &point)
{
    backupData();

    QPointF p = CMDCalculation::invertPos(*dataBk()->matrix(),point);

    m_isNodeOperate = true;
    onAdd(p);
    loadGeneratePathEvent();
    m_isNodeOperate = false;
}

void CMDDrawNode::deleteNode(int pos)
{
    backupData();
    m_isNodeOperate = true;
    onDelete(pos);
    loadGeneratePathEvent();
    m_isNodeOperate = false;
}

void CMDDrawNode::creatingPaint(QPainter *p)
{
    p->drawLines(m_nodes.toVector());
}

qreal CMDDrawNode::widenWidth()
{
    qreal w1 = CMDDrawVector::widenWidth();
    qreal w2 = pipeBackGroundVisible() ? m_pipewidth : 0;
    qreal w3 = pLineStream()->streamLineVisible() ? pLineStream()->m_penManager.getPen().widthF() : 0;
    qreal w = qMax(w1, w2);
    w = qMax(w, w3);
    return w;
}

void CMDDrawNode::scaleDatas(QRectF current, QRectF backup, QList<QPointF>& datas, QList<QPointF>& datasBk)
{
    if (backup.isEmpty()|| datasBk.count() == 0 || isNodeCreating())
        return;

    qreal xOff = current.x() - backup.x();
    qreal yOff = current.y() - backup.y();
    qreal xScale = current.width() / backup.width();
    qreal yScale = current.height() / backup.height();

    QPointF pf;
    for (int i = 0; i < datas.count(); i++)
    {
        pf.setX(backup.x() + (datasBk[i].x() - backup.x()) * xScale + xOff);
        pf.setY( backup.y() + (datasBk[i].y() - backup.y()) * yScale + yOff);
        datas[i] = pf;
    }

    onNodeChanged();
}

void CMDDrawNode::onMove(QPointF point, QPointF offset, int pos)
{
    QList<QPointF>& datas = m_nodes;
    QList<QPointF>& datasBk = m_nodesBk;

    if (isNodeCreating())
    {
        datas[pos] = point;
        onNodeChanged();
        return;
    }

    QSizeF sf;
    sf.setHeight(offset.y());
    sf.setWidth(offset.x());

    QList<QPointF> list;
    list.append(datasBk);
    list[pos] = QPointF(datasBk[pos].x() + sf.width(),datasBk[pos].y()+sf.height());

    offsetDatas(&datas, &datasBk, calculateOffset(list, datasBk));
    datas[pos] = QPointF(datas[pos].x() + sf.width(),datas[pos].y()+sf.height());
    onNodeChanged();
}

void CMDDrawNode::onAdd(QPointF point)
{
    QList<QPointF>& datas = m_nodes;
    QList<QPointF>& datasBk = m_nodesBk;

    if (isNodeCreating())
    {
        datas.append(point);
        onNodeChanged();
        return;
    }

    datas.append(point);
    offsetDatas(&datas, NULL, calculateOffset(datas, datasBk));

    onNodeChanged();
}

void CMDDrawNode::onDelete(int pos)
{
    QList<QPointF>& datas = m_nodes;
    //QList<QPointF>& datasBk = m_nodesBk;

    if (datas.count() < (minCount() + 1))
        return;

    datas.removeAt(pos);
    QPointF offset = calculateOffset(m_nodes, m_nodesBk);
    offsetDatas(&datas, NULL,offset);

    onNodeChanged();
}

QPointF CMDDrawNode::calculateOffset(QList<QPointF> current, QList<QPointF> backup)
{
    Q_UNUSED(backup);
    if (current.count() < minCount())
        return QPointF();

    //! Rect可能发生变化，需要计算偏移
    QPainterPath path;
    path.moveTo(current[0]);
    for(int i = 1;i<current.count();i++)
    {
        path.lineTo(current[i]);
    }
    QRectF rf = path.boundingRect();

    this->setNewRect( rf);

    QPointF p1 = CMDCalculation::calcMatrixPoint(this->dataBk()->data()->rect(),dataBk()->dataVector(), QPointF());
    QPointF p2 = CMDCalculation::calcMatrixPoint(this->newRect(), this->dataVector(),QPointF());

    return QPointF(p1.x() - p2.x(), p1.y() - p2.y());
}

//! 判断count 0
void CMDDrawNode::offsetDatas(QList<QPointF>* datas, QList<QPointF>* datasBk, QPointF offset)
{
    if (isNodeCreating())
        return;

    QSizeF sf;
    sf.setHeight(offset.y());
    sf.setWidth(offset.x());
    int count = datas->count();

    if (datasBk == NULL)
    {
        for (int i = 0; i < count; i++)
        {
            (*datas)[i].setX((*datas)[i].x()+sf.width());
            (*datas)[i].setY((*datas)[i].y()+sf.height());
        }
    }
    else
    {
        for (int i = 0; i < count; i++)
            (*datas)[i] = QPointF((*datasBk)[i].x()+sf.width(),(*datasBk)[i].y()+sf.height());
    }
}

bool CMDDrawNode::findIndex(QList<QPointF> datas, QPointF point, int &index)
{
    Q_UNUSED(point);
    int count = datas.count();
    const int width = 6;
    bool isVisible = false;
    QPainterPath path;
    QPen p(Qt::black,width);
    if(count < minCount())
        return false;
    path.moveTo(datas[0]);
    for (index = 1; index < count; index++)
    {
        path.lineTo(datas[index]);
        //if(path.IsOutlineVisible(point, p))
        {
            isVisible = true;
            break;
        }
    }

    return isVisible;
}

void CMDDrawNode::onNodeChanged()
{

}

void CMDDrawNode::generateNodePath(QPainterPath* path)
{
    if(m_nodes.count() > 0)
    {
        path->moveTo(m_nodes.at(0));
        for(int i = 1;i < m_nodes.count();i++)
            path->lineTo(m_nodes.at(i));
    }
}

void CMDDrawNode::drawPath(QPainter *painter)
{
    if(pipeBackGroundVisible())
    {
        drawPipe(painter);
    }
    if(pLineStream()->streamLineVisible())
    {
        drawStreamLine(painter);
    }
    if(isStudioMode() && pLineStream()->streamLineVisible())
    {
        drawDirection(painter);
    }
}

void CMDDrawNode::generateBound()
{
    QRectF rf = path()->boundingRect();
    qreal penWidth = pen()->width();
    rf.adjust(-penWidth, -penWidth, penWidth, penWidth);
    setBound(rf);
}

IUndoDataObj *CMDDrawNode::createUndoData()
{
    return new CUndoDataNode;
}
bool CMDDrawNode::revertUndoData(IUndoDataObj *value)
{
    bool hasChange = CMDDrawVector::revertUndoData(value);

    CUndoDataNode* u = dynamic_cast<CUndoDataNode*>(value);
    if (u->m_nodes != m_nodes)
    {
        m_nodes = u->m_nodes;
        hasChange = true;
    }

    return hasChange;
}
IUndoDataObj *CMDDrawNode::fillUndoData(bool isOld)
{
    CUndoDataNode* u = dynamic_cast<CUndoDataNode*>(CMDDrawVector::fillUndoData(isOld));
    if (isOld)
        u->m_nodes = m_nodesBk;
    else
        u->m_nodes = m_nodes;

    return u;
}
bool CMDDrawNode::undoEqual()
{
    return CMDDrawVector::undoEqual()
            && (m_nodesBk == m_nodes);
}
void CMDDrawNode::undoOperate(IUndoDataObj *value)
{
    if (revertUndoData(value))
    {
        m_isNodeOperate = true;
        loadGeneratePathEvent();
        m_isNodeOperate = false;
    }
}

QList<QStringList> CMDDrawNode::expNames()
{
    QList<QStringList> list;
    list << QStringList({"#stream",         QObject::tr("流动")});
    list << QStringList({"drawNode.enable",   QObject::tr("显示流动")});
    list << QStringList({"drawNode.isFlow",   QObject::tr("是否流动")});
    list << QStringList({"drawNode.percent",   QObject::tr("流动比例(0~1)")});
    list << QStringList({"drawNode.Direction",   QObject::tr("流动方向")});

    return list;
}

void CMDDrawNode::onDataChanged(CMDExpInfo *exp)
{
    switch (exp->index())
    {
    case 0: //! 是否显示流动线条
        if(m_pLineStream)
            m_pLineStream->setStreamLineVisible(exp->decimalValue());
        invalidate();
        break;
    case 1: //! 是否流动
        if(m_pLineStream)
            m_pLineStream->setStreamLineFlow(exp->decimalValue());
        invalidate();
        break;
    case 2: //! 流动百分比
        if(m_pLineStream)
            m_pLineStream->setPercent(exp->decimalValue());
        invalidate();
        break;
    case 3: //! 线条流动方向
        if(m_pLineStream)
        {
            double v = exp->decimalValue();
            if(v != 0)
                m_pLineStream->setDirection(CMDNodeStream::Direction::forward);
            else
                m_pLineStream->setDirection(CMDNodeStream::Direction::reverse);
        }
        invalidate();
        break;
    default:
        break;
    }
}

void CMDDrawNode::getPens(QList<CMDPenManager *> &list)
{
    CMDDrawVector::getPens(list);
    list.append(&m_pLineStream->m_penManager);
}

CMDNodeStream *CMDDrawNode::pLineStream() const
{
    return m_pLineStream;
}

void CMDDrawNode::setPLineStream(CMDNodeStream *pLineStream)
{
    m_pLineStream = pLineStream;
}

void CMDDrawNode::drawPipe(QPainter *painter)
{
    if(!createSuccess() || this->pen() == NULL )
        return;

    int clr1R,clr1G,clr1B,clr2R,clr2G,clr2B,alpha1,alpha2;
    m_pipeClr1.getRgb(&clr1R,&clr1G,&clr1B,&alpha1);
    m_pipeClr2.getRgb(&clr2R,&clr2G,&clr2B,&alpha2);


    qreal wInterval = m_pipewidth / m_Density;
    qreal rInterval = (qreal)(clr1R - clr2R) / m_Density;
    qreal gInterval = (qreal)(clr1G - clr2G) / m_Density;
    qreal bInterval = (qreal)(clr1B - clr2B)  / m_Density;
    qreal aInterval = (qreal)(alpha1 - alpha2)  / m_Density;

    qreal r = clr2R - rInterval;
    qreal g = clr2G - gInterval;
    qreal b = clr2B - bInterval;
    qreal a = alpha2 - aInterval;

    qreal width = m_pipewidth + wInterval;

    for (int i = 0; i < m_Density; i++)
    {
        QRgb rgba = qRgba(r+= rInterval,g+=gInterval,b+=bInterval,a+=aInterval);
        QPen qpen(QColor::fromRgba(rgba));
        qpen.setCapStyle(Qt::PenCapStyle::RoundCap);
        qpen.setJoinStyle(Qt::PenJoinStyle::RoundJoin);
        qpen.setWidthF(width -= wInterval);

        painter->setPen(qpen);

        painter->drawPath(*basePath());
    }
}

void CMDDrawNode::drawStreamLine(QPainter *painter)
{
    if(pLineStream())
    {
        pLineStream()->drawStreamLine(painter);
    }
}

void CMDDrawNode::drawLine(QPainter *painter)
{
    Q_UNUSED(painter);
}

bool CMDDrawNode::pipeBackGroundVisible() const
{
    return m_pipeBackGroundVisible;
}

void CMDDrawNode::setPipeBackGroundVisible(bool visible)
{
    m_pipeBackGroundVisible = visible;
}
QColor CMDDrawNode::pipeClr2() const
{
    return m_pipeClr2;
}

void CMDDrawNode::setPipeClr2(const QColor &pipeClr2)
{
    m_pipeClr2 = pipeClr2;
}

QColor CMDDrawNode::pipeClr1() const
{
    return m_pipeClr1;
}

void CMDDrawNode::setPipeClr1(const QColor &pipeClr1)
{
    m_pipeClr1 = pipeClr1;
}

qreal CMDDrawNode::pipewidth() const
{
    return m_pipewidth;
}

void CMDDrawNode::setPipewidth(const qreal &pipewidth)
{
    m_pipewidth = pipewidth;
}


void CMDDrawNode::drawDirection(QPainter *p)
{
    if(nodeDatas()->isEmpty())
        return;

    QPointF pf;
    if(pLineStream()->direction() == CMDNodeStream::Direction::forward)
        pf = nodeDatas()->first();
    else
        pf = nodeDatas()->last();

    QPainterPath path;
    path.addEllipse(pf, 3, 3);
    p->fillPath(path, QBrush(Qt::yellow));
}

void CMDDrawNode::generateContent()
{
    CMDDrawVector::generateContent();

    m_pLineStream->updatePen();
}

//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawNode::createPropertyDrawObj(const QString &className)
{
    if(className.compare("DrawNode", Qt::CaseInsensitive) == 0)
        return new CMDPropertyDrawNode(this);

    return CMDDrawVector::createPropertyDrawObj(className);
}

void CMDDrawNode::initClassInfo(QStringList &classInfo)
{
    CMDDrawVector::initClassInfo(classInfo);
    classInfo.append("DrawNode");
}
//////////////////////////////////////////////////////////////////////////
