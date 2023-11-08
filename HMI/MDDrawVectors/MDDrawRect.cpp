#include <QtConfig>
#include "MDDrawRect.h"
#include "DrawRectData.h"
#include "MDCalculation.h"
#include "property/PropertyDrawRect.h"
#include "UndoDataRect.h"

CMDDrawRect::CMDDrawRect()
{
    m_data = new CDrawRectData;

    int count = 2;
    for(int i = 0; i < count; i++)
        m_data->m_customDatas.append(QPointF(0, 0));
}
CMDDrawRect::~CMDDrawRect()
{
    delete m_data;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawRect::setXRound(qreal value)
{
    setRound(value, true);
}
qreal CMDDrawRect::xRound()
{
    return m_data->m_xRound;
}
void CMDDrawRect::setYRound(qreal value)
{
    setRound(value, false);
}
qreal CMDDrawRect::yRound()
{
    return m_data->m_yRound;
}
//////////////////////////////////////////////////////////////////////////
DrawType CMDDrawRect::type()
{
    return DrawType::Rect;
}
//////////////////////////////////////////////////////////////////////////
CMDDrawObj *CMDDrawRect::createSelf()
{
    return new CMDDrawRect();
}
void CMDDrawRect::onGeneratePath()
{
    limitRound();
    generateRoundedPath(*basePath());

    generateCustom();
}
void CMDDrawRect::backupData()
{
    CMDDrawVector::backupData();

    m_data->m_xRoundBk = m_data->m_xRound;
    m_data->m_yRoundBk = m_data->m_yRound;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawRect::generateCustom()
{
    QRectF rf = rect();
    qreal x = xRound();
    qreal y = yRound();

    m_data->m_customDatas[0] = QPointF(rf.x()+x, rf.y());
    m_data->m_customDatas[1] = QPointF(rf.x(), rf.y()+y);

    m_data->m_customCenter = QPointF(rf.x()+x, rf.y()+y);
}
void CMDDrawRect::moveCustom(const QPointF& point, int pos)
{
    QPointF p = CMDCalculation::invertPos(*dataBk()->matrix(), point);
    QPointF mouse =CMDCalculation::invertPos(*dataBk()->matrix(), dataBk()->mousePos());
    QPointF off = p - mouse;

    onMouseMove(off, pos);
}
void CMDDrawRect::onMouseMove(const QPointF &offset, int pos)
{
    qreal x = m_data->m_xRoundBk;
    qreal y = m_data->m_yRoundBk;

    if (pos == 0)
        setRound(x + offset.x(), true);
    else
        setRound(y + offset.y(), false);
}
void CMDDrawRect::serialize(QJsonObject &json)
{
    CMDDrawVector::serialize(json);

    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawRect", js);
}
void CMDDrawRect::deserialize(const QJsonObject &json)
{
    CMDDrawVector::deserialize(json);

    QJsonObject js = json.value("DrawRect").toObject();
    m_data->deserialize(js);
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawRect::onInitialization()
{
    m_data->m_roundLimit = (m_data->m_xRound > m_data->m_yRound) ?
                m_data->m_xRound : m_data->m_yRound;
}
CMDDrawObj *CMDDrawRect::clone()
{
    CMDDrawRect* obj = static_cast<CMDDrawRect*>(CMDDrawVector::clone());
    *obj->m_data = *m_data;

    return obj;
}
//////////////////////////////////////////////////////////////////////////
QList<QPointF> *CMDDrawRect::customDatas()
{
    return &m_data->m_customDatas;
}
QPointF CMDDrawRect::customCenter()
{
    return m_data->m_customCenter;
}
void CMDDrawRect::generateRoundedPath(QPainterPath &path)
{
    qreal w = m_data->m_xRound;
    qreal h = m_data->m_yRound;
    bool hasArc = (w > 0 || h > 0);

    path = QPainterPath();
    if (hasArc)
        path.addRoundedRect(rect(), w, h);
    else
        path.addRect(rect());
}
void CMDDrawRect::limitRound()
{
    m_data->m_xRound = (m_data->m_xRound > m_data->m_roundLimit) ?
                m_data->m_xRound : m_data->m_roundLimit;
    m_data->m_yRound = (m_data->m_yRound > m_data->m_roundLimit) ?
                m_data->m_yRound : m_data->m_roundLimit;

    qreal wMax = rect().width() / 2.0;
    qreal hMax = rect().height() / 2.0;
    m_data->m_xRound = (m_data->m_xRound < wMax) ? m_data->m_xRound : wMax;
    m_data->m_yRound = (m_data->m_yRound < hMax) ? m_data->m_yRound : hMax;
}
void CMDDrawRect::setRound(qreal value, bool isX)
{
    const qreal min = 0;
    qreal wMax = rect().width() / 2.0;
    qreal hMax = rect().height() / 2.0;
    qreal max = isX ? wMax : hMax;

    value = (value > min) ? value : min;
    value = (value < max) ? value : max;

    m_data->m_xRound = (value < wMax) ? value : wMax;
    m_data->m_yRound = (value < hMax) ? value : hMax;

    m_data->m_roundLimit = (m_data->m_xRound > m_data->m_yRound) ?
                m_data->m_xRound : m_data->m_yRound;

    loadGeneratePathEvent();
}
//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawRect::createPropertyDrawObj()
{
    return new CPropertyDrawRect(this);
}
void CMDDrawRect::onPaint(QPainter *p)
{
    CMDDrawVector::onPaint(p);
}
//////////////////////////////////////////////////////////////////////////
IUndoDataObj *CMDDrawRect::createUndoData()
{
    return new CUndoDataRect;
}
bool CMDDrawRect::revertUndoData(IUndoDataObj *value)
{
    bool hasChange = CMDDrawVector::revertUndoData(value);

    CUndoDataRect* u = dynamic_cast<CUndoDataRect*>(value);
    if (!equal(u->m_xRound, u->m_yRound))
    {
        m_data->m_xRound = u->m_xRound;
        m_data->m_yRound = u->m_yRound;
        m_data->m_roundLimit = (m_data->m_xRound > m_data->m_yRound) ?
                    m_data->m_xRound : m_data->m_yRound;

        hasChange = true;
    }

    return hasChange;
}
IUndoDataObj *CMDDrawRect::fillUndoData(bool isOld)
{
    CUndoDataRect* u = dynamic_cast<CUndoDataRect*>(CMDDrawVector::fillUndoData(isOld));
    if (isOld)
    {
        u->m_xRound = m_data->m_xRoundBk;
        u->m_yRound = m_data->m_yRoundBk;
    }
    else
    {
        u->m_xRound = m_data->m_xRound;
        u->m_yRound = m_data->m_yRound;
    }

    return u;
}
bool CMDDrawRect::undoEqual()
{
    return CMDDrawVector::undoEqual()
        && equal(m_data->m_xRoundBk, m_data->m_yRoundBk);
}
bool CMDDrawRect::equal(qreal xRound, qreal yRound)
{
    return (xRound == m_data->m_xRound) && (yRound == m_data->m_yRound);
}
//////////////////////////////////////////////////////////////////////////



