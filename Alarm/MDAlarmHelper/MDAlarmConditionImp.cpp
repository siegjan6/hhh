#include "MDAlarmConditionImp.h"
#include <QJsonArray>
#include <QJsonObject>

CMDAlarmConditionImp::CMDAlarmConditionImp()
{
    m_version = 0;

    m_name = "";
    m_comment = "";
    m_levelName = "";
    m_expression = "";
    m_bindvar = "";
    m_enabled = true;
    m_enableAlarmExp = "";
    m_customId = "";
    m_serverity = 0;
    m_delayEnabled = false;
    m_delayConditionExp = "";
    m_delayTimeExp = "";
    m_latched = false;
    m_acknowledgeRequired = false;
}

CMDAlarmConditionImp::~CMDAlarmConditionImp()
{
    m_spList.deleteAll();
}

CMDAlarmConditionImp::CMDAlarmConditionImp(const CMDAlarmConditionImp& obj)
{
    copy(&obj);
}

CMDAlarmConditionImp& CMDAlarmConditionImp::operator = (const CMDAlarmConditionImp& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDAlarmConditionImp::copy(const CMDAlarmConditionImp* source)
{
    m_version = source->m_version;

    m_name = source->m_name;
    m_comment = source->m_comment;
    m_levelName = source->m_levelName;
    m_expression = source->m_expression;
    m_bindvar = source->m_bindvar;
    m_spList = source->m_spList;
    m_customId = source->m_customId;
    m_serverity = source->m_serverity;
    m_delayEnabled = source->m_delayEnabled;
    m_delayConditionExp = source->m_delayConditionExp;
    m_delayTimeExp = source->m_delayTimeExp;
    m_latched = source->m_latched;
    m_acknowledgeRequired = source->m_acknowledgeRequired;
    m_enabled = source->m_enabled;
    m_enableAlarmExp = source->m_enableAlarmExp;
    return true;
}

/*!
\section 序列化
*/
// 按照版本序列化/反序列化
const quint8 CURRENT_VERSION_ACOND = 2;
bool CMDAlarmConditionImp::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_ACOND : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream, useCurrentVersion);
    else if(actualVersion == 2)
        serializeOnVersion_2(stream, useCurrentVersion);
    else
        result = false;
    return result;
}
void CMDAlarmConditionImp::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else if(m_version == 2)
        deserializeOnVersion_2(stream);
    else{}
}

// 版本 1
void CMDAlarmConditionImp::serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion)
{
    CValueRangeCondition condition;
    QString objName;

    stream << m_name << m_comment << m_levelName << objName;
    condition.serialize(stream, useCurrentVersion);
    m_spList.serialize(stream, useCurrentVersion);
}
void CMDAlarmConditionImp::deserializeOnVersion_1(QDataStream& stream)
{
    CValueRangeCondition condition;
    QString objName;

    stream >> m_name >> m_comment >> m_levelName >> objName;
    condition.deserialize(stream);
    m_spList.deserialize(stream);

    // 将原有信息转换到 m_expression
    m_expression = condition.toExpression(objName);
}

// 版本 2：
// 取消了版本1中的 m_condition【CValueRangeCondition】 和 m_objName【QString】;
// 增加了 m_expression【QString】
void CMDAlarmConditionImp::serializeOnVersion_2(QDataStream& stream, bool useCurrentVersion)
{
    stream << m_name << m_comment << m_levelName << m_expression;
    m_spList.serialize(stream, useCurrentVersion);
}
void CMDAlarmConditionImp::deserializeOnVersion_2(QDataStream& stream)
{
    stream >> m_name >> m_comment >> m_levelName >> m_expression;
    m_spList.deserialize(stream);
}

void CMDAlarmConditionImp::serialize(QJsonObject& json)
{
    json.insert("Name", m_name);
    json.insert("Comment", m_comment);
    json.insert("LevelName", m_levelName);
    json.insert("Expression", m_expression);
    json.insert("BindVar", m_bindvar);
    json.insert("Enabled", m_enabled);
    json.insert("EnableAlarmExp", m_enableAlarmExp);
    json.insert("CustomId", m_customId);
    json.insert("Serverity", m_serverity);
    json.insert("DelayEnabled", m_delayEnabled);
    json.insert("DelayConditionExp", m_delayConditionExp);
    json.insert("DelayTimeExp", m_delayTimeExp);
    json.insert("Latched", m_latched);
    json.insert("AcknowledgeRequired", m_acknowledgeRequired);
    QJsonArray SPListJson;
    m_spList.serialize(SPListJson);
    json.insert("SPList", SPListJson);
}

void CMDAlarmConditionImp::deserialize(const QJsonObject& json)
{
    m_name = json.value("Name").toString();
    m_comment = json.value("Comment").toString();
    m_levelName = json.value("LevelName").toString();
    m_expression = json.value("Expression").toString();
    if(json.contains("BindVar"))
        m_bindvar = json.value("BindVar").toString();
    if(json.contains("Enabled"))
        m_enabled = json.value("Enabled").toBool();
    if(json.contains("EnableAlarmExp"))
        m_enableAlarmExp = json.value("EnableAlarmExp").toString();
    if(json.contains("CustomId"))
        m_customId = json.value("CustomId").toString();
    if(json.contains("Serverity"))
        m_serverity = json.value("Serverity").toInt();
    if(json.contains("DelayEnabled"))
        m_delayEnabled = json.value("DelayEnabled").toBool();
    if(json.contains("DelayConditionExp"))
        m_delayConditionExp = json.value("DelayConditionExp").toString();
    if(json.contains("DelayTimeExp"))
        m_delayTimeExp = json.value("DelayTimeExp").toString();
    if(json.contains("Latched"))
        m_latched = json.value("Latched").toBool();
    if(json.contains("AcknowledgeRequired"))
        m_acknowledgeRequired = json.value("AcknowledgeRequired").toBool();
    QJsonArray SPListJson = json.value("SPList").toArray();
    m_spList.deserialize(SPListJson);
}
