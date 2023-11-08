#include "MDAlarmLevelImp.h"
#include <QJsonArray>
#include <QJsonObject>

// 颜色
CMDAlarmColorImp::CMDAlarmColorImp()
{
    m_version = 0;

    m_f = qRgb(255, 0, 0);
    m_b = qRgb(0, 0, 0);
}

CMDAlarmColorImp::CMDAlarmColorImp(quint32 f, quint32 b)
{
    m_version = 0;

    m_f = f;
    m_b = b;
}

CMDAlarmColorImp::~CMDAlarmColorImp()
{
}

CMDAlarmColorImp::CMDAlarmColorImp(const CMDAlarmColorImp& obj)
{
    copy(&obj);
}

CMDAlarmColorImp& CMDAlarmColorImp::operator = (const CMDAlarmColorImp& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDAlarmColorImp::copy(const CMDAlarmColorImp* source)
{
    m_version = source->m_version;
    m_f = source->m_f;
    m_b = source->m_b;

    return true;
}

// 按照版本序列化/反序列化
const quint8 CURRENT_VERSION_AC = 1;
bool CMDAlarmColorImp::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_AC : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream);
    else
        result = false;
    return result;
}
void CMDAlarmColorImp::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

// 版本 1
void CMDAlarmColorImp::serializeOnVersion_1(QDataStream& stream)
{
    stream << (quint32)m_f << (quint32)m_b;
}
void CMDAlarmColorImp::deserializeOnVersion_1(QDataStream& stream)
{
    quint32 f, b;
    stream >> f >> b;
    m_f = f;
    m_b = b;
}

/*!
\section 序列化：JSON格式
*/
void CMDAlarmColorImp::serialize(QJsonObject& json)
{
    json.insert("Front", (double)m_f);
    json.insert("Back", (double)m_b);
}

void CMDAlarmColorImp::deserialize(const QJsonObject& json)
{
    quint32 f = (quint32)json.value("Front").toDouble();
    m_f = f;
    quint32 b = (quint32)json.value("Back").toDouble();
    m_b = b;
}

// 等级
CMDAlarmLevelImp::CMDAlarmLevelImp()
{
    m_version = 0;

    m_name = "";
    m_comment = "";
    m_level = 0;
    m_delayLength = 0;

    m_color_occur.setF(qRgb(255,0,0));
    m_color_occur.setB(qRgb(242,242,42));

    m_color_confirm_not_end.setF(qRgb(0,0,0));
    m_color_confirm_not_end.setB(qRgb(7,230,199));

    m_color_confirm_end.setF(qRgb(0,0,0));
    m_color_confirm_end.setB(qRgb(7,230,199));

    m_color_end_not_confirm.setF(qRgb(0,0,0));
    m_color_end_not_confirm.setB(qRgb(255,0,0));

    m_color_end_confirm.setF(qRgb(0,0,0));
    m_color_end_confirm.setB(qRgb(255,0,0));
}

CMDAlarmLevelImp::~CMDAlarmLevelImp()
{
}

CMDAlarmLevelImp::CMDAlarmLevelImp(const CMDAlarmLevelImp& obj)
{
    copy(&obj);
}

CMDAlarmLevelImp& CMDAlarmLevelImp::operator = (const CMDAlarmLevelImp& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDAlarmLevelImp::copy(const CMDAlarmLevelImp* source)
{
    m_version = source->m_version;
    m_name = source->m_name;
    m_comment = source->m_comment;
    m_delayLength = source->m_delayLength;

    m_color_occur = source->m_color_occur;
    m_color_confirm_not_end = source->m_color_confirm_not_end;
    m_color_confirm_end = source->m_color_confirm_end;
    m_color_end_not_confirm = source->m_color_end_not_confirm;
    m_color_end_confirm = source->m_color_end_confirm;

    return true;
}

// 按照版本序列化/反序列化
const quint8 CURRENT_VERSION_AL = 1;
bool CMDAlarmLevelImp::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_AL : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream, useCurrentVersion);
    else
        result = false;
    return result;
}
void CMDAlarmLevelImp::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

// 版本 1
void CMDAlarmLevelImp::serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion)
{
    stream << m_name << m_comment << m_delayLength;
    m_color_occur.serialize(stream, useCurrentVersion);
    m_color_confirm_not_end.serialize(stream, useCurrentVersion);
    m_color_confirm_end.serialize(stream, useCurrentVersion);
    m_color_end_not_confirm.serialize(stream, useCurrentVersion);
    m_color_end_confirm.serialize(stream, useCurrentVersion);
}
void CMDAlarmLevelImp::deserializeOnVersion_1(QDataStream& stream)
{
    stream >> m_name >> m_comment >> m_delayLength;
    m_color_occur.deserialize(stream);
    m_color_confirm_not_end.deserialize(stream);
    m_color_confirm_end.deserialize(stream);
    m_color_end_not_confirm.deserialize(stream);
    m_color_end_confirm.deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CMDAlarmLevelImp::serialize(QJsonObject& json)
{
    json.insert("Name", m_name);
    json.insert("Comment", m_comment);
    json.insert("DelayLength", (qint64)m_delayLength);

    QJsonObject color_occur_JSON;
    m_color_occur.serialize(color_occur_JSON);
    json.insert("Color_Occur", color_occur_JSON);

    QJsonObject color_confirm_not_end_JSON;
    m_color_confirm_not_end.serialize(color_confirm_not_end_JSON);
    json.insert("Color_Confirm_Not_End", color_confirm_not_end_JSON);

    QJsonObject color_confirm_end_JSON;
    m_color_confirm_end.serialize(color_confirm_end_JSON);
    json.insert("Color_Confirm_End", color_confirm_end_JSON);

    QJsonObject color_end_not_confirm_JSON;
    m_color_end_not_confirm.serialize(color_end_not_confirm_JSON);
    json.insert("Color_End_Not_Confirm", color_end_not_confirm_JSON);

    QJsonObject color_end_confirm_JSON;
    m_color_end_confirm.serialize(color_end_confirm_JSON);
    json.insert("Color_End_Confirm", color_end_confirm_JSON);
}

void CMDAlarmLevelImp::deserialize(const QJsonObject& json)
{
    m_name = json.value("Name").toString();
    m_comment = json.value("Comment").toString();
    m_delayLength = json.value("DelayLength").toInt();

    QJsonObject color_occur_JSON = json.value("Color_Occur").toObject();
    m_color_occur.deserialize(color_occur_JSON);

    QJsonObject color_confirm_not_end_JSON = json.value("Color_Confirm_Not_End").toObject();
    m_color_confirm_not_end.deserialize(color_confirm_not_end_JSON);

    QJsonObject color_confirm_end_JSON = json.value("Color_Confirm_End").toObject();
    m_color_confirm_end.deserialize(color_confirm_end_JSON);

    QJsonObject color_end_not_confirm_JSON = json.value("Color_End_Not_Confirm").toObject();
    m_color_end_not_confirm.deserialize(color_end_not_confirm_JSON);

    QJsonObject color_end_confirm_JSON = json.value("Color_End_Confirm").toObject();
    m_color_end_confirm.deserialize(color_end_confirm_JSON);
}

