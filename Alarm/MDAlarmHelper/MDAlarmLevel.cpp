#include "MDAlarmLevel.h"
#include "MDAlarmLevelImp.h"

/*!
 * \brief 报警颜色数据类
 */
CMDAlarmColor::CMDAlarmColor()
{
    m_data = new CMDAlarmColorImp;
}

CMDAlarmColor::CMDAlarmColor(QRgb f, QRgb b)
{
    m_data = new CMDAlarmColorImp(f, b);
}

CMDAlarmColor::~CMDAlarmColor()
{
    if(m_data)
        delete m_data;
}

CMDAlarmColor::CMDAlarmColor(const CMDAlarmColor& obj)
{
    m_data = new CMDAlarmColorImp(*obj.m_data);

    copy(&obj);
}

CMDAlarmColor& CMDAlarmColor::operator = (const CMDAlarmColor& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

/*!
\section 属性
*/
QRgb CMDAlarmColor::f() const
{
    return (m_data ? m_data->m_f : qRgb(255, 255, 255));
}
void CMDAlarmColor::setF(QRgb f)
{
    if(m_data)
        m_data->m_f = f;
}

QRgb CMDAlarmColor::b() const
{
    return (m_data ? m_data->m_b : qRgb(255, 255, 255));
}
void CMDAlarmColor::setB(QRgb b)
{
    if(m_data)
        m_data->m_b = b;
}

bool CMDAlarmColor::copy(const CMDAlarmColor* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

/*!
\section 序列化
*/
bool CMDAlarmColor::serialize(QDataStream& stream, bool useCurrentVersion)
{
    return (m_data ? m_data->serialize(stream, useCurrentVersion) : false);
}

void CMDAlarmColor::deserialize(QDataStream& stream)
{
    if(m_data)
        m_data->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CMDAlarmColor::serialize(QJsonObject& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CMDAlarmColor::deserialize(const QJsonObject& json)
{
    if(m_data)
        m_data->deserialize(json);
}

/*!
 * \brief 报警等级数据类
 */
CMDAlarmLevel::CMDAlarmLevel()
{
    m_data = new CMDAlarmLevelImp;
}

CMDAlarmLevel::~CMDAlarmLevel()
{
    if(m_data)
        delete m_data;
}

CMDAlarmLevel::CMDAlarmLevel(const CMDAlarmLevel& obj)
{
    m_data = new CMDAlarmLevelImp(*obj.m_data);

    copy(&obj);
}

CMDAlarmLevel& CMDAlarmLevel::operator = (const CMDAlarmLevel& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

/*!
\section 属性
*/
QString CMDAlarmLevel::name() const
{
    return (m_data ? m_data->m_name : "");
}
void CMDAlarmLevel::setName(const QString& name)
{
    if(m_data)
        m_data->m_name = name;
}

QString CMDAlarmLevel::comment() const
{
    return (m_data ? m_data->m_comment : "");
}
void CMDAlarmLevel::setComment(const QString& comment)
{
    if(m_data)
        m_data->m_comment = comment;
}

qint32 CMDAlarmLevel::delayLength() const
{
    return (m_data ? m_data->m_delayLength : 0);
}
void CMDAlarmLevel::setDelayLength(qint32 delayLength)
{
    if(m_data)
        m_data->m_delayLength = delayLength;
}

CMDAlarmColor& CMDAlarmLevel::color_occur() const
{
    return m_data->m_color_occur;
}
void CMDAlarmLevel::setColor_occur(const CMDAlarmColor& color)
{
    if(m_data)
        m_data->m_color_occur = color;
}

CMDAlarmColor& CMDAlarmLevel::color_confirm_not_end() const
{
    return m_data->m_color_confirm_not_end;
}
void CMDAlarmLevel::setColor_confirm_not_end(const CMDAlarmColor& color)
{
    if(m_data)
        m_data->m_color_confirm_not_end = color;
}

CMDAlarmColor& CMDAlarmLevel::color_confirm_end() const
{
    return m_data->m_color_confirm_end;
}
void CMDAlarmLevel::setColor_confirm_end(const CMDAlarmColor& color)
{
    if(m_data)
        m_data->m_color_confirm_end = color;
}

CMDAlarmColor& CMDAlarmLevel::color_end_not_confirm() const
{
    return m_data->m_color_end_not_confirm;
}
void CMDAlarmLevel::setColor_end_not_confirm(const CMDAlarmColor& color)
{
    if(m_data)
        m_data->m_color_end_not_confirm = color;
}

CMDAlarmColor& CMDAlarmLevel::color_end_confirm() const
{
    return m_data->m_color_end_confirm;
}
void CMDAlarmLevel::setColor_end_confirm(const CMDAlarmColor& color)
{
    if(m_data)
        m_data->m_color_end_confirm = color;
}

bool CMDAlarmLevel::copy(const CMDAlarmLevel* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

/*!
\section 序列化
*/
bool CMDAlarmLevel::serialize(QDataStream& stream, bool useCurrentVersion)
{
    return (m_data ? m_data->serialize(stream, useCurrentVersion) : false);
}

void CMDAlarmLevel::deserialize(QDataStream& stream)
{
    if(m_data)
        m_data->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CMDAlarmLevel::serialize(QJsonObject& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CMDAlarmLevel::deserialize(const QJsonObject& json)
{
    if(m_data)
        m_data->deserialize(json);
}
