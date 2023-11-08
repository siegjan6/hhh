#include "MDEventPropertyData.h"
#include <QJsonArray>
#include <QJsonObject>

/*!
 * \brief 事件数据基类的实现类
 */
CMDEventPropertyData_Config::CMDEventPropertyData_Config()
{
    m_enable = false;
    m_type = MDEventType::null_event;
    m_enableLog = false;
    m_version = 0;
}

CMDEventPropertyData_Config::CMDEventPropertyData_Config(MDEventType eventType)
{
    m_enable = false;
    m_enableLog = false;
    m_version = 0;

    m_type = eventType;
}

CMDEventPropertyData_Config::~CMDEventPropertyData_Config()
{

}

CMDEventPropertyData_Config::CMDEventPropertyData_Config(const CMDEventPropertyData_Config& obj)
{
    copy(&obj);
}

CMDEventPropertyData_Config& CMDEventPropertyData_Config::operator = (const CMDEventPropertyData_Config& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDEventPropertyData_Config::copy(const CMDEventPropertyData_Config* source)
{
    m_version = source->m_version;
    m_name = source->m_name;
    m_comment = source->m_comment;
    m_enable = source->m_enable;
    m_type = source->m_type;
    m_enableLog = source->m_enableLog;
    m_objName = source->m_objName;
    m_expressionString = source->m_expressionString;

    m_actionNameList.clear();
    m_actionNameList.append(source->m_actionNameList);

    return true;
}

const quint8 CURRENT_VERSION_EPDC = 1;
bool CMDEventPropertyData_Config::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_EPDC : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream);
    else
        result = false;
    return result;
}

void CMDEventPropertyData_Config::serializeOnVersion_1(QDataStream& stream)
{
    quint16 eventType = (quint16)m_type;

    stream << m_name << m_comment << m_enable;
    stream << m_enableLog << m_objName << m_expressionString;
    stream << eventType;
    stream << m_actionNameList;
}

void CMDEventPropertyData_Config::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

void CMDEventPropertyData_Config::deserializeOnVersion_1(QDataStream& stream)
{
    quint16 eventType;

    stream >> m_name >> m_comment >> m_enable;
    stream >> m_enableLog >> m_objName >> m_expressionString;
    stream >> eventType;
    stream >> m_actionNameList;

    m_type = (MDEventType)eventType;
}

void CMDEventPropertyData_Config::serialize(QJsonObject& json)
{
    QJsonObject MyJson;
    MyJson.insert("Name",m_name);
    MyJson.insert("Comment", m_comment);
    MyJson.insert("Enable", m_enable);
    MyJson.insert("EnableLog", m_enableLog);
    MyJson.insert("ObjName", m_objName);
    MyJson.insert("Expression", m_expressionString);
    MyJson.insert("Type", (int)m_type);
    QJsonArray actionArray;
    foreach (const QString& actionName, m_actionNameList)
        actionArray.append(actionName);
    MyJson.insert("ActionNameList", actionArray);

    json.insert("EventProperty", MyJson);
}

void CMDEventPropertyData_Config::deserialize(const QJsonObject& json)
{
    QJsonObject MyJson = json.value("EventProperty").toObject();

    m_name = MyJson.value("Name").toString();
    m_comment = MyJson.value("Comment").toString();
    m_enable = MyJson.value("Enable").toBool();
    m_enableLog = MyJson.value("EnableLog").toBool();
    m_objName = MyJson.value("ObjName").toString();
    m_expressionString = MyJson.value("Expression").toString();
    m_type = (MDEventType)MyJson.value("Type").toInt();
    QJsonArray actionArray = MyJson.value("ActionNameList").toArray();
    int opCount = actionArray.count();
    for(int i = 0; i < opCount; i++)
        m_actionNameList.append(actionArray.at(i).toString());
}

/*!
 * \brief 变量数值变化的实现类
 */
CEPCData_VariableValueChangedInRange::CEPCData_VariableValueChangedInRange()
{
    m_version = 0;
}

CEPCData_VariableValueChangedInRange::~CEPCData_VariableValueChangedInRange()
{

}

CEPCData_VariableValueChangedInRange::CEPCData_VariableValueChangedInRange(const CEPCData_VariableValueChangedInRange& obj)
{
    copy(&obj);
}

CEPCData_VariableValueChangedInRange& CEPCData_VariableValueChangedInRange::operator = (const CEPCData_VariableValueChangedInRange& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CEPCData_VariableValueChangedInRange::copy(const CEPCData_VariableValueChangedInRange* source)
{
    m_version = source->m_version;
    m_rangeCondition = source->m_rangeCondition;
    return true;
}

const quint8 CURRENT_VERSION_VVCIR = 1;
bool CEPCData_VariableValueChangedInRange::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_VVCIR : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream, useCurrentVersion);
    else
        result = false;
    return result;
}

void CEPCData_VariableValueChangedInRange::serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion)
{
    m_rangeCondition.serialize(stream, useCurrentVersion);
}

void CEPCData_VariableValueChangedInRange::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

void CEPCData_VariableValueChangedInRange::deserializeOnVersion_1(QDataStream& stream)
{
    m_rangeCondition.deserialize(stream);
}

void CEPCData_VariableValueChangedInRange::serialize(QJsonObject& json)
{
    QJsonObject MyJson;

    QJsonObject RangeConditionJson;
    m_rangeCondition.serialize(RangeConditionJson);
    MyJson.insert("RangeCondition",RangeConditionJson);

    json.insert("VariableValueChangedInRange", MyJson);
}

void CEPCData_VariableValueChangedInRange::deserialize(const QJsonObject& json)
{
    QJsonObject MyJson = json.value("VariableValueChangedInRange").toObject();

    QJsonObject RangeConditionJson = MyJson.value("RangeCondition").toObject();
    m_rangeCondition.deserialize(RangeConditionJson);
}

/*!
 * \brief 变量品质变化的实现类
 */
CEPCData_VariableQualityChanged::CEPCData_VariableQualityChanged()
{
    m_version = 0;
}

CEPCData_VariableQualityChanged::~CEPCData_VariableQualityChanged()
{

}

CEPCData_VariableQualityChanged::CEPCData_VariableQualityChanged(const CEPCData_VariableQualityChanged& obj)
{
    copy(&obj);
}

CEPCData_VariableQualityChanged& CEPCData_VariableQualityChanged::operator = (const CEPCData_VariableQualityChanged& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CEPCData_VariableQualityChanged::copy(const CEPCData_VariableQualityChanged* source)
{
    m_version = source->m_version;
    m_rangeCondition = source->m_rangeCondition;
    return true;
}

const quint8 CURRENT_VERSION_VQC = 1;
bool CEPCData_VariableQualityChanged::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_VQC : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream, useCurrentVersion);
    else
        result = false;
    return result;
}

void CEPCData_VariableQualityChanged::serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion)
{
    m_rangeCondition.serialize(stream, useCurrentVersion);
}

void CEPCData_VariableQualityChanged::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

void CEPCData_VariableQualityChanged::deserializeOnVersion_1(QDataStream& stream)
{
    m_rangeCondition.deserialize(stream);
}

void CEPCData_VariableQualityChanged::serialize(QJsonObject& json)
{
    QJsonObject MyJson;

    QJsonObject RangeConditionJson;
    m_rangeCondition.serialize(RangeConditionJson);
    MyJson.insert("RangeCondition",RangeConditionJson);

    json.insert("VariableQualityChanged", MyJson);
}

void CEPCData_VariableQualityChanged::deserialize(const QJsonObject& json)
{
    QJsonObject MyJson = json.value("VariableQualityChanged").toObject();

    QJsonObject RangeConditionJson = MyJson.value("RangeCondition").toObject();
    m_rangeCondition.deserialize(RangeConditionJson);
}

/*!
 * \brief 变量写的实现类
 */
CEPCData_VariableValueWrite::CEPCData_VariableValueWrite()
{
    m_version = 0;
    m_checkConditionBeforeOrAfterWrite = 0;
}

CEPCData_VariableValueWrite::~CEPCData_VariableValueWrite()
{

}

CEPCData_VariableValueWrite::CEPCData_VariableValueWrite(const CEPCData_VariableValueWrite& obj)
{
    copy(&obj);
}

CEPCData_VariableValueWrite& CEPCData_VariableValueWrite::operator = (const CEPCData_VariableValueWrite& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CEPCData_VariableValueWrite::copy(const CEPCData_VariableValueWrite* source)
{
    m_version = source->m_version;
    m_checkConditionBeforeOrAfterWrite = source->m_checkConditionBeforeOrAfterWrite;
    m_rangeCondition = source->m_rangeCondition;
    return true;
}

const quint8 CURRENT_VERSION_VVW = 1;
bool CEPCData_VariableValueWrite::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_VVW : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream, useCurrentVersion);
    else
        result = false;
    return result;
}

void CEPCData_VariableValueWrite::serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion)
{
    stream<< m_checkConditionBeforeOrAfterWrite;
    m_rangeCondition.serialize(stream, useCurrentVersion);
}

void CEPCData_VariableValueWrite::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

void CEPCData_VariableValueWrite::deserializeOnVersion_1(QDataStream& stream)
{
    stream>> m_checkConditionBeforeOrAfterWrite;
    m_rangeCondition.deserialize(stream);
}

void CEPCData_VariableValueWrite::serialize(QJsonObject& json)
{
    QJsonObject MyJson;

    MyJson.insert("CheckConditionBeforeOrAfterWrite",m_checkConditionBeforeOrAfterWrite);
    QJsonObject RangeConditionJson;
    m_rangeCondition.serialize(RangeConditionJson);
    MyJson.insert("RangeCondition",RangeConditionJson);

    json.insert("VariableQualityChanged", MyJson);
}

void CEPCData_VariableValueWrite::deserialize(const QJsonObject& json)
{
    QJsonObject MyJson = json.value("VariableQualityChanged").toObject();

    m_checkConditionBeforeOrAfterWrite = MyJson.value("CheckConditionBeforeOrAfterWrite").toInt();
    QJsonObject RangeConditionJson = MyJson.value("RangeCondition").toObject();
    m_rangeCondition.deserialize(RangeConditionJson);
}

/*!
 * \brief 周期的实现类
 */
CEPCData_TimeCycle::CEPCData_TimeCycle()
{
    m_timeCycle = 1000;
    m_version = 0;
}

CEPCData_TimeCycle::~CEPCData_TimeCycle()
{

}

CEPCData_TimeCycle::CEPCData_TimeCycle(const CEPCData_TimeCycle& obj)
{
    copy(&obj);
}

CEPCData_TimeCycle& CEPCData_TimeCycle::operator = (const CEPCData_TimeCycle& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CEPCData_TimeCycle::copy(const CEPCData_TimeCycle* source)
{
    m_version = source->m_version;
    m_timeCycle = source->m_timeCycle;
    return true;
}

const quint8 CURRENT_VERSION_TC = 1;
bool CEPCData_TimeCycle::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_TC : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream, useCurrentVersion);
    else
        result = false;
    return result;
}

void CEPCData_TimeCycle::serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion)
{
    stream<< m_timeCycle;
}

void CEPCData_TimeCycle::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

void CEPCData_TimeCycle::deserializeOnVersion_1(QDataStream& stream)
{
    stream>> m_timeCycle;
}

void CEPCData_TimeCycle::serialize(QJsonObject& json)
{
    QJsonObject MyJson;

    MyJson.insert("TimeCycle", (int)m_timeCycle);

    json.insert("TimeCycle", MyJson);
}

void CEPCData_TimeCycle::deserialize(const QJsonObject& json)
{
    QJsonObject MyJson = json.value("TimeCycle").toObject();

    m_timeCycle = MyJson.value("TimeCycle").toInt();
}

/*!
 * \brief 组合的实现类
 */
CEPCData_Combined::CEPCData_Combined()
{
    m_version = 0;
}

CEPCData_Combined::~CEPCData_Combined()
{

}

CEPCData_Combined::CEPCData_Combined(const CEPCData_Combined& obj)
{
    copy(&obj);
}

CEPCData_Combined& CEPCData_Combined::operator = (const CEPCData_Combined& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CEPCData_Combined::copy(const CEPCData_Combined* source)
{
    m_version = source->m_version;
    return true;
}

const quint8 CURRENT_VERSION_COMB = 1;
bool CEPCData_Combined::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_COMB : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream, useCurrentVersion);
    else
        result = false;
    return result;
}

void CEPCData_Combined::serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion)
{

}

void CEPCData_Combined::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

void CEPCData_Combined::deserializeOnVersion_1(QDataStream& stream)
{

}

void CEPCData_Combined::serialize(QJsonObject& json)
{
}

void CEPCData_Combined::deserialize(const QJsonObject& json)
{
}

/*!
 * \brief 事件运行时的属性类的实现类
 */
CMDEventPropertyData_Runtime::CMDEventPropertyData_Runtime()
{
    m_expFlag = false;
    m_autoResetExpFlag = false;
    m_triggerCount = 0;
}

CMDEventPropertyData_Runtime::~CMDEventPropertyData_Runtime()
{

}

CMDEventPropertyData_Runtime::CMDEventPropertyData_Runtime(const CMDEventPropertyData_Runtime& obj)
{
    m_expFlag = obj.m_expFlag;
    m_autoResetExpFlag = obj.m_autoResetExpFlag;
    m_triggerCount = obj.m_triggerCount;
    m_lastTriggerTime = obj.m_lastTriggerTime;
}

CMDEventPropertyData_Runtime& CMDEventPropertyData_Runtime::operator = (const CMDEventPropertyData_Runtime& obj)
{
    if( this != &obj )
    {
        m_expFlag = obj.m_expFlag;
        m_autoResetExpFlag = obj.m_autoResetExpFlag;
        m_triggerCount = obj.m_triggerCount;
        m_lastTriggerTime = obj.m_lastTriggerTime;
    }
    return *this;
}
