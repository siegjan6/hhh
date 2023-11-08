#include "DynamicColor.h"
#include "MDDrawObj.h"

CDynamicColor::CDynamicColor(DynamicType type)
{
    m_brush = NULL;
    m_pen = NULL;
    m_min = 0;
    m_max = 100;

    if (type == DynamicType::Brush)
        m_brush = new CMDBrushManager;
    else if (type == DynamicType::Pen)
        m_pen = new CMDPenManager;
}
CDynamicColor::~CDynamicColor()
{
    release();
}
void CDynamicColor::initColor(const QColor &color)
{
    if (m_brush != NULL)
        m_brush->init(color);
    else if (m_pen != NULL)
        m_pen->init(color, 1);
}
//////////////////////////////////////////////////////////////////////////
void CDynamicColor::getBrushes(QList<CMDBrushManager *> &list)
{
    if(m_brush != NULL)
        list.append(m_brush);
}
void CDynamicColor::getPens(QList<CMDPenManager *> &list)
{
    if(m_pen != NULL)
        list.append(m_pen);
}
void CDynamicColor::setImagePath(const QString &value)
{
    if (m_brush != NULL)
        m_brush->setProjectPath(value);
    else if (m_pen != NULL)
        m_pen->setProjectPath(value);
}
bool CDynamicColor::isInRange(double value)
{
    return (value >= m_min && value <= m_max);
}
void CDynamicColor::operator=(const CDynamicColor &other)
{
    if (this == &other)
        return;

    release();

    m_min = other.m_min;
    m_max = other.m_max;
    if (other.m_brush != NULL)
    {
        m_brush = new CMDBrushManager;
        *m_brush = *other.m_brush;
    }
    if (other.m_pen != NULL)
    {
        m_pen = new CMDPenManager;
        *m_pen = *other.m_pen;
    }

}
void CDynamicColor::serialize(QJsonObject &json)
{
    json.insert("Max", m_max);
    json.insert("Min", m_min);
    QJsonObject jsBrush;
    if(m_brush != NULL)
        m_brush->serialize(jsBrush);
    json.insert("Brush", jsBrush);
    QJsonObject jsPen;
    if(m_pen != NULL)
        m_pen->serialize(jsPen);
    json.insert("Pen", jsPen);
}
void CDynamicColor::deserialize(const QJsonObject &json)
{
    m_max = json.value("Max").toDouble();
    m_min = json.value("Min").toDouble();
    if(m_brush != NULL)
        m_brush->deserialize(json.value("Brush").toObject());
    if(m_pen != NULL)
        m_pen->deserialize(json.value("Pen").toObject());
}
void CDynamicColor::release()
{
    if (m_brush != NULL)
    {
        delete m_brush;
        m_brush = NULL;
    }
    if (m_pen != NULL)
    {
        delete m_pen;
        m_pen = NULL;
    }
}
//////////////////////////////////////////////////////////////////////////
CDynamicColorManager::CDynamicColorManager()
{
    m_brush = NULL;
    m_pen = NULL;
    m_type = DynamicType::Brush;
    m_container = NULL;
}
CDynamicColorManager::CDynamicColorManager(DynamicType type)
{
    m_brush = NULL;
    m_pen = NULL;
    m_type = DynamicType::Brush;

    m_type = type;
}
CDynamicColorManager::~CDynamicColorManager()
{
    release();
}
CDynamicColor *CDynamicColorManager::insert(int index)
{
    CDynamicColor* color = new CDynamicColor(m_type);
    color->setImagePath(m_container->getImagePath());
    m_list.insert(index, color);
    return color;
}
void CDynamicColorManager::showDialog(int index)
{
    if (index >= 0 && index <= m_list.count())
    {
        if (m_type == DynamicType::Brush)
            m_list.at(index)->brush()->showDialog();
        else if (m_type == DynamicType::Pen)
            m_list.at(index)->pen()->showDialog();
    }
}
void CDynamicColorManager::setContainer(CMDDrawObj *value)
{
    m_container = value;
}
CMDDrawObj *CDynamicColorManager::container()
{
    return m_container;
}
//////////////////////////////////////////////////////////////////////////
void CDynamicColorManager::getBrushes(QList<CMDBrushManager *> &list)
{
    for(int i = 0; i < m_list.count(); i++)
        m_list.at(i)->getBrushes(list);
}
void CDynamicColorManager::getPens(QList<CMDPenManager *> &list)
{
    for(int i = 0; i < m_list.count(); i++)
        m_list.at(i)->getPens(list);
}
void CDynamicColorManager::resetValue(double value)
{
    int count = m_list.count();
    for (int i = 0; i < count; i++)
    {
        if (m_list.at(i)->isInRange(value))
        {
            if (m_type == DynamicType::Brush)
                m_brush = m_list.at(i)->brush();
            else if (m_type == DynamicType::Pen)
                m_pen = m_list.at(i)->pen();

            return;
        }
    }

    m_brush = NULL;
    m_pen = NULL;
}
void CDynamicColorManager::operator=(const CDynamicColorManager &other)
{
    if (this == &other)
        return;

    release();

    m_type = other.m_type;
    for (int i = 0; i < other.m_list.count(); i++)
    {
        CDynamicColor* color = new CDynamicColor(m_type);
        *color = *other.m_list.at(i);
        m_list.append(color);
    }

    m_container = other.m_container;
}
void CDynamicColorManager::serialize(QJsonObject &json)
{
    json.insert("Type", (int)m_type);
    QJsonArray array;
    foreach(CDynamicColor* d, m_list)
    {
        QJsonObject jsD;
        d->serialize(jsD);
        array.append(jsD);
    }
    json.insert("List", array);
}
void CDynamicColorManager::deserialize(const QJsonObject &json)
{
    m_type = (DynamicType)json.value("Type").toInt();

    QJsonArray array = json.value("List").toArray();
    foreach(QJsonValue v, array)
    {
        CDynamicColor* color = new CDynamicColor(m_type);
        color->deserialize(v.toObject());
        m_list.append(color);
    }
}
//////////////////////////////////////////////////////////////////////////
void CDynamicColorManager::release()
{
    for(int i = 0; i < m_list.count(); i++)
        delete m_list.at(i);
    m_list.clear();
}
//////////////////////////////////////////////////////////////////////////
