#include <math.h>
#include <QtCore/qmath.h>
#include "DrawBasicArcData.h"
#include "MDDrawBasicArc.h"
#include "MDCalculation.h"
#include "MDPropertyDrawBasicArc.h"
#include "UndoDataBasicArc.h"

//////////////////////////////////////////////////////////////////////////
CMDDrawBasicArc::CMDDrawBasicArc()
{
    m_data = new CDrawBasicArcData;

    int count = 2;
    for(int i = 0; i < count; i++)
        m_data->m_customDatas.append(QPointF());
}
CMDDrawBasicArc::~CMDDrawBasicArc()
{
    delete m_data;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawBasicArc::setStartAngle(qreal value)
{
    value = CMDCalculation::limitAngle(value);

    m_data->m_startAngle = value;
    loadGeneratePathEvent();
}
qreal CMDDrawBasicArc::startAngle()
{
    return m_data->m_startAngle;
}
void CMDDrawBasicArc::setSpanAngle(qreal value)
{
    value = CMDCalculation::limitAngle(value);

    m_data->m_spanAngle = value;
    loadGeneratePathEvent();
}
qreal CMDDrawBasicArc::spanAngle()
{
    return m_data->m_spanAngle;
}
//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawBasicArc::createPropertyDrawObj()
{
    return new CMDPropertyDrawBasicArc(this);
}
QList<QStringList> CMDDrawBasicArc::expNames()
{
    QList<QStringList> list;
    list << QStringList({"#layout"});
    list << QStringList({"drawBasicArc.startAngle",     QObject::tr("起始角度")});
    list << QStringList({"drawBasicArc.spanAngle",      QObject::tr("扫描角度")});

    return list;
}
void CMDDrawBasicArc::onDataChanged(CMDExpInfo *exp)
{
    switch (exp->index())
    {
    case 0: //startAngle
        setStartAngle(exp->decimalValue());
        break;
    case 1://spanAngle
        setSpanAngle(exp->decimalValue());
        break;
    default:
        break;
    }
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawBasicArc::backupData()
{
    CMDDrawVector::backupData();

    m_data->m_startAngleBk = m_data->m_startAngle;
    m_data->m_spanAngleBk = m_data->m_spanAngle;
}
//////////////////////////////////////////////////////////////////////////
QList<QPointF> *CMDDrawBasicArc::customDatas()
{
    return &m_data->m_customDatas;
}
QPointF CMDDrawBasicArc::customCenter()
{
    return m_data->m_customCenter;
}
void CMDDrawBasicArc::generateCustom()
{
    QRectF rf = rect();

    m_data->m_customCenter = rf.center();
    m_data->m_customDatas[0] = calculateAnglePoint(rf, m_data->m_startAngle+m_data->m_spanAngle);
    m_data->m_customDatas[1] = calculateAnglePoint(rf, m_data->m_startAngle);
}
void CMDDrawBasicArc::moveCustom(const QPointF &point, int pos)
{
    QPointF p = CMDCalculation::invertPos(*dataBk()->matrix(), point);
    QPointF center = rect().center();
    qreal y = (p.y() - center.y()) / rect().height();
    qreal x = (p.x() - center.x()) / rect().width();
    qreal angle = qAtan2(y, x);
    angle = angle * 180 / PI;

    if (pos == 0)
        setSpanAngle(angle - startAngle());
    else if (pos == 1)
        setStartAngle(angle);
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawBasicArc::serialize(QJsonObject &json)
{
    CMDDrawVector::serialize(json);

    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawBasicArc", js);
}
void CMDDrawBasicArc::deserialize(const QJsonObject &json)
{
    CMDDrawVector::deserialize(json);

    QJsonObject js = json.value("DrawBasicArc").toObject();
    m_data->deserialize(js);
}
CMDDrawObj *CMDDrawBasicArc::clone()
{
    CMDDrawBasicArc* obj = static_cast<CMDDrawBasicArc*>(CMDDrawVector::clone());
    *obj->m_data = *m_data;

    return obj;
}
//////////////////////////////////////////////////////////////////////////
QPointF CMDDrawBasicArc::calculateAnglePoint(const QRectF &rect, qreal angle)
{
    QPointF center = rect.center();
    angle = angle * PI / 180;
    qreal x = cos(angle)*rect.width()/2 + center.x();
    qreal y = sin(angle)*rect.height()/2 + center.y();
    return QPointF(x, y);
}
//////////////////////////////////////////////////////////////////////////
IUndoDataObj *CMDDrawBasicArc::createUndoData()
{
    return new CUndoDataBasicArc;
}
bool CMDDrawBasicArc::revertUndoData(IUndoDataObj *value)
{
    bool hasChange = CMDDrawVector::revertUndoData(value);

    CUndoDataBasicArc* u = dynamic_cast<CUndoDataBasicArc*>(value);
    if (!equal(u->m_startAngle, u->m_spanAngle))
    {
        m_data->m_startAngle = u->m_startAngle;
        m_data->m_spanAngle = u->m_spanAngle;
        hasChange = true;
    }

    return hasChange;
}
IUndoDataObj *CMDDrawBasicArc::fillUndoData(bool isOld)
{
    CUndoDataBasicArc* u = dynamic_cast<CUndoDataBasicArc*>(CMDDrawVector::fillUndoData(isOld));
    if (isOld)
    {
        u->m_startAngle = m_data->m_startAngleBk;
        u->m_spanAngle = m_data->m_spanAngleBk;
    }
    else
    {
        u->m_startAngle = m_data->m_startAngle;
        u->m_spanAngle = m_data->m_spanAngle;
    }

    return u;
}
bool CMDDrawBasicArc::undoEqual()
{
    return CMDDrawVector::undoEqual()
        && equal(m_data->m_startAngleBk, m_data->m_spanAngleBk);
}
bool CMDDrawBasicArc::equal(qreal startAngle, qreal spanAngle)
{
    return (startAngle == m_data->m_startAngle) && (spanAngle == m_data->m_spanAngle);
}
//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawBasicArc::createPropertyDrawObj(const QString &className)
{
    if(className.compare("DrawBasicArc", Qt::CaseInsensitive) == 0)
        return new CMDPropertyDrawBasicArc(this);

    return CMDDrawVector::createPropertyDrawObj(className);
}
void CMDDrawBasicArc::initClassInfo(QStringList &classInfo)
{
    CMDDrawVector::initClassInfo(classInfo);
    classInfo.append("DrawBasicArc");
}
//////////////////////////////////////////////////////////////////////////







