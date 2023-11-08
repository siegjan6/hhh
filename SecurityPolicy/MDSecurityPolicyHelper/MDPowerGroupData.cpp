#include "MDPowerGroupData.h"
#include <QJsonArray>
#include <QJsonObject>

CMDPowerGroupData::CMDPowerGroupData()
{
    m_version = 0;

    m_name = "";
    m_comment = "";
    m_level = 0;

    m_enable = false;
    m_visible = false;
}

CMDPowerGroupData::~CMDPowerGroupData()
{
    m_spList.deleteAll();
}

CMDPowerGroupData::CMDPowerGroupData(const CMDPowerGroupData& obj)
{
    copy(&obj);
}

CMDPowerGroupData& CMDPowerGroupData::operator = (const CMDPowerGroupData& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDPowerGroupData::copy(const CMDPowerGroupData* source)
{
    m_version = source->m_version;

    m_name = source->m_name;
    m_comment = source->m_comment;
    m_level = source->m_level;
    m_accountList.clear();
    m_accountList.append(source->m_accountList);

    m_enable = source->m_enable;
    m_visible = source->m_visible;
    m_spList = source->m_spList;

    return true;
}

// 按照版本序列化/反序列化
const quint8 CURRENT_VERSION_PGPD = 1;
bool CMDPowerGroupData::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_PGPD : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream, useCurrentVersion);
    else
        result = false;
    return result;
}
void CMDPowerGroupData::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

// 版本 1
void CMDPowerGroupData::serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion)
{
    stream << m_name << m_comment;
    stream << m_accountList;
    stream << m_enable << m_visible;
    m_spList.serialize(stream, useCurrentVersion);
}
void CMDPowerGroupData::deserializeOnVersion_1(QDataStream& stream)
{
    stream >> m_name >> m_comment;
    stream >> m_accountList;
    stream >> m_enable >> m_visible;
    m_spList.deserialize(stream);
}

void CMDPowerGroupData::serialize(QJsonObject& json)
{
    json.insert("Name",m_name);
    json.insert("Comment", m_comment);
    json.insert("Level", m_level);

    QJsonArray accountArray;
    foreach (const QString& account, m_accountList)
        accountArray.append(account);
    json.insert("AccountList", accountArray);

    json.insert("Enable", m_enable);
    json.insert("Visible", m_visible);
    QJsonArray SPListJson;
    m_spList.serialize(SPListJson);
    json.insert("SPList", SPListJson);
}

void CMDPowerGroupData::deserialize(const QJsonObject& json)
{
    m_name = json.value("Name").toString();
    m_comment = json.value("Comment").toString();
    if(json.contains("Level"))
        m_level = json.value("Level").toInt();

    QJsonArray accountArray = json.value("AccountList").toArray();
    int opCount = accountArray.count();
    for(int i = 0; i < opCount; i++)
        m_accountList.append(accountArray.at(i).toString());

    m_enable = json.value("Enable").toBool();
    m_visible = json.value("Visible").toBool();
    QJsonArray SPListJson = json.value("SPList").toArray();
    m_spList.deserialize(SPListJson);
}
