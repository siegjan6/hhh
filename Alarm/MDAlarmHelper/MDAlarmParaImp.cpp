#include "MDAlarmParaImp.h"
#include <QVariant>
#include <QJsonArray>
#include <QJsonObject>

CMDAlarmParaImp::CMDAlarmParaImp()
{
    m_mode = MDAlarmMode::common;
    m_version = 0;
}

CMDAlarmParaImp::~CMDAlarmParaImp()
{
}

CMDAlarmParaImp::CMDAlarmParaImp(const CMDAlarmParaImp& obj)
{
    copy(&obj);
}

CMDAlarmParaImp& CMDAlarmParaImp::operator = (const CMDAlarmParaImp& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDAlarmParaImp::copy(const CMDAlarmParaImp* source)
{
    m_version = source->m_version;

    m_mode = source->m_mode;

    return true;
}

/*!
\section 序列化
*/
// 按照版本序列化/反序列化
const quint8 CURRENT_VERSION_AP = 1;
bool CMDAlarmParaImp::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_AP : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream);
    else
        result = false;
    return result;
}
void CMDAlarmParaImp::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

// 版本 1
void CMDAlarmParaImp::serializeOnVersion_1(QDataStream& stream)
{
    stream << (quint8)m_mode;
}
void CMDAlarmParaImp::deserializeOnVersion_1(QDataStream& stream)
{
    quint8 m;
    stream >> m;
    m_mode = (MDAlarmMode)m;
}

/*!
\section 序列化：JSON格式
*/
void CMDAlarmParaImp::serialize(QJsonObject& json)
{
    json.insert("Mode", (quint8)m_mode);
}

void CMDAlarmParaImp::deserialize(const QJsonObject& json)
{
    quint8 m = json.value("Mode").toInt();
    m_mode = (MDAlarmMode)m;
}


