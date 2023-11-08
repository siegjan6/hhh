#include "MDAccountPropertyData.h"
#include <QJsonArray>
#include <QJsonObject>

CMDAccountPropertyData_Config::CMDAccountPropertyData_Config()
{
    m_version = 0;

    m_name = "";
    m_password = "";
    m_comment = "";
    m_type = 0;

    m_enable = true;
    m_locked = false;
    m_logined = false;
    m_visible = false;
}

CMDAccountPropertyData_Config::~CMDAccountPropertyData_Config()
{
    m_spList.deleteAll();
}

CMDAccountPropertyData_Config::CMDAccountPropertyData_Config(const CMDAccountPropertyData_Config& obj)
{
    copy(&obj);
}

CMDAccountPropertyData_Config& CMDAccountPropertyData_Config::operator = (const CMDAccountPropertyData_Config& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDAccountPropertyData_Config::copy(const CMDAccountPropertyData_Config* source)
{
    m_version = source->m_version;

    m_name = source->m_name;
    m_password = source->m_password;
    m_comment = source->m_comment;
    m_type = source->m_type;

    m_enable = source->m_enable;
    m_locked = source->m_locked;
    m_visible = source->m_visible;

    m_spList = source->m_spList;

    return true;
}

// 按照版本序列化/反序列化
const quint8 CURRENT_VERSION_ACCOUNT_PDC = 1;
bool CMDAccountPropertyData_Config::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_ACCOUNT_PDC : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream, useCurrentVersion);
    else
        result = false;
    return result;
}
void CMDAccountPropertyData_Config::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

// 版本 1
void CMDAccountPropertyData_Config::serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion)
{
    stream << m_name << m_password << m_comment << m_type;
    stream << m_enable << m_locked << m_visible;
    m_spList.serialize(stream, useCurrentVersion);
}
void CMDAccountPropertyData_Config::deserializeOnVersion_1(QDataStream& stream)
{
    stream >> m_name >> m_password >> m_comment >> m_type;
    stream >> m_enable >> m_locked >> m_visible;
    m_spList.deserialize(stream);
}

void CMDAccountPropertyData_Config::serialize(QJsonObject& json)
{
    json.insert("Name",m_name);
    json.insert("Password", m_password);
    json.insert("Comment", m_comment);
    json.insert("Type", m_type);
    json.insert("Enable", m_enable);
    json.insert("Locked", m_locked);
    json.insert("Visible", m_visible);
    QJsonArray SPListJson;
    m_spList.serialize(SPListJson);
    json.insert("SPList", SPListJson);
}

void CMDAccountPropertyData_Config::deserialize(const QJsonObject& json)
{
    if(json.contains("Name"))
        m_name = json.value("Name").toString();
    if(json.contains("Password"))
        m_password = json.value("Password").toString();
    if(json.contains("Comment"))
        m_comment = json.value("Comment").toString();
    if(json.contains("Type"))
        m_type = json.value("Type").toInt();
    if(json.contains("Enable"))
        m_enable = json.value("Enable").toBool();
    if(json.contains("Locked"))
        m_locked = json.value("Locked").toBool();
    if(json.contains("Visible"))
        m_visible = json.value("Visible").toBool();
    if(json.contains("SPList")){
        QJsonArray SPListJson = json.value("SPList").toArray();
        m_spList.deserialize(SPListJson);
    }
}

// 运行时属性
CMDAccountPropertyData_Runtime::CMDAccountPropertyData_Runtime()
{
    m_loginCount = 0;
    m_lastLoginTime = QDateTime();
}

CMDAccountPropertyData_Runtime::~CMDAccountPropertyData_Runtime()
{

}

CMDAccountPropertyData_Runtime::CMDAccountPropertyData_Runtime(const CMDAccountPropertyData_Runtime& obj)
{
    m_loginCount = obj.m_loginCount;
    m_lastLoginTime = obj.m_lastLoginTime;
}

CMDAccountPropertyData_Runtime& CMDAccountPropertyData_Runtime::operator = (const CMDAccountPropertyData_Runtime& obj)
{
    if( this != &obj )
    {
        m_loginCount = obj.m_loginCount;
        m_lastLoginTime = obj.m_lastLoginTime;
    }
    return *this;
}
