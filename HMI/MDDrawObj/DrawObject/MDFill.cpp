#include "MDFill.h"
#include "DynamicColor.h"
#include "MDDrawObj.h"
//////////////////////////////////////////////////////////////////////////
CMDFill::CMDFill()
{
    m_scaleValue = 0;//填充值
    //!studio
    m_direction = FillDirection::Bottom;
    m_max = 100;//!最大值范围
    m_min = 0;//!最小值范围

    m_brushManager.setBrushType(CMDBrushManager::BrushType::Solid);
    m_brushManager.setColor(QColor(Qt::yellow));
    m_dynamicBrush = new CDynamicColorManager(DynamicType::Brush);
}
CMDFill::~CMDFill()
{
    delete m_dynamicBrush;
}
//////////////////////////////////////////////////////////////////////////
void CMDFill::draw(QPainter *painter, const QRectF &rect, QPainterPath *path)
{
    if (m_scaleValue == 0)
        return;

    QRectF rf = calculateRect(rect, m_scaleValue, m_direction);

    QRectF clipRect = painter->clipBoundingRect();
    painter->setClipRect(rf, Qt::IntersectClip);


    if (m_dynamicBrush->brush() != NULL)
        m_dynamicBrush->brush()->draw(painter, rect, path);
    else
        m_brushManager.draw(painter, rect, path);

    if(clipRect.isEmpty())
        painter->setClipRect(clipRect, Qt::NoClip);
    else
        painter->setClipRect(clipRect, Qt::ReplaceClip);
}
void CMDFill::operator=(const CMDFill &other)
{
    if (this == &other)
        return;

    m_direction = other.m_direction;
    m_brushManager = other.m_brushManager;
    m_max = other.m_max;
    m_min = other.m_min;
    *m_dynamicBrush = *other.m_dynamicBrush;
}
//////////////////////////////////////////////////////////////////////////
void CMDFill::serialize(QJsonObject &json)
{
    json.insert("Direction", (int)m_direction);
    json.insert("Max", m_max);
    json.insert("Min", m_min);

    QJsonObject jsBrushManager;
    m_brushManager.serialize(jsBrushManager);
    json.insert("BrushManager", jsBrushManager);
    QJsonObject jsDynamicBrush;
    m_dynamicBrush->serialize(jsDynamicBrush);
    json.insert("DynamicBrush", jsDynamicBrush);
}
void CMDFill::deserialize(const QJsonObject &json)
{
    m_direction = (FillDirection)json.value("Direction").toInt();
    m_max = json.value("Max").toDouble();
    m_min = json.value("Min").toDouble();
    m_brushManager.deserialize(json.value("BrushManager").toObject());
    m_dynamicBrush->deserialize(json.value("DynamicBrush").toObject());
}
//////////////////////////////////////////////////////////////////////////
void CMDFill::setScaleValue(qreal value)
{
    qreal range = m_max - m_min;
    range = (range == 0) ? 1 : range;

    m_scaleValue = (value - m_min) / range;
    m_scaleValue = (m_scaleValue < 0) ? 0 : m_scaleValue;
    m_scaleValue = (m_scaleValue > 1) ? 1 : m_scaleValue;
}

void CMDFill::setDynamicBrush(CDynamicColorManager *value)
{
    *m_dynamicBrush = *value;
}
CDynamicColorManager *CMDFill::dynamicBrush()
{
    return m_dynamicBrush;
}
void CMDFill::setContainer(CMDDrawObj *value)
{
    m_dynamicBrush->setContainer(value);
}
CMDDrawObj *CMDFill::container()
{
    return m_dynamicBrush->container();
}
QString CMDFill::text()
{
    return m_brushManager.text();
}
//////////////////////////////////////////////////////////////////////////
void CMDFill::getBrushes(QList<CMDBrushManager *> &list)
{
    list.append(&m_brushManager);
    m_dynamicBrush->getBrushes(list);
}
void CMDFill::generateContent(const QRectF &rect)
{
    if (m_dynamicBrush->brush() != NULL)
        m_dynamicBrush->brush()->generate(rect);
    else
        m_brushManager.generate(rect);
}
void CMDFill::loadDataChangedEvent(const QRectF &rect, CMDExpInfo *expression)
{
    m_dynamicBrush->resetValue(expression->decimalValue());
    generateContent(rect);
}
QRectF CMDFill::calculateRect(const QRectF &rect, qreal scaleValue, FillDirection direction)
{
    QRectF rf;
    switch (direction)
    {
    case FillDirection::Top:
        rf.setX(rect.x());
        rf.setY(rect.y());
        rf.setWidth(rect.width());
        rf.setHeight(rect.height()*scaleValue);
        break;
    case FillDirection::Bottom:
        rf.setX(rect.x());
        rf.setY(rect.y() + rect.height()*(1-scaleValue));
        rf.setWidth(rect.width());
        rf.setHeight(rect.height()*scaleValue);
        break;
    case FillDirection::Left:
        rf.setX(rect.x());
        rf.setY(rect.y());
        rf.setWidth(rect.width()*scaleValue);
        rf.setHeight(rect.height());
        break;
    case FillDirection::Right:
        rf.setX(rect.x() + rect.width()*(1-scaleValue));
        rf.setY(rect.y());
        rf.setWidth(rect.width()*scaleValue);
        rf.setHeight(rect.height());
        break;
    }

    return rf;
}
//////////////////////////////////////////////////////////////////////////

