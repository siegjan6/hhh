#include "MDActionPropertyData.h"
#include <QJsonArray>
#include <QJsonObject>

CMDActionPropertyData_Config::CMDActionPropertyData_Config()
{
    m_enable = false;
    m_enableLog = false;
    m_enableDelay = false;
    m_delayLength = 1000;
    m_enableExecExp = false;
    m_execExp = "";
    m_version = 0;
}

CMDActionPropertyData_Config::CMDActionPropertyData_Config(MDActionType actionType)
{
    m_actionID.setType(actionType);
}

CMDActionPropertyData_Config::~CMDActionPropertyData_Config()
{
    m_spList.deleteAll();
}

CMDActionPropertyData_Config::CMDActionPropertyData_Config(const CMDActionPropertyData_Config& obj)
{
    copy(&obj);
}

CMDActionPropertyData_Config& CMDActionPropertyData_Config::operator = (const CMDActionPropertyData_Config& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDActionPropertyData_Config::copy(const CMDActionPropertyData_Config* source)
{
    m_version = source->m_version;
    m_name = source->m_name;
    m_comment = source->m_comment;
    m_enable = source->m_enable;
    m_enableLog = source->m_enableLog;
    m_actionID = source->m_actionID;
    m_enableDelay = source->m_enableDelay;
    m_delayLength = source->m_delayLength;
    m_enableExecExp = source->m_enableExecExp;
    m_execExp = source->m_execExp;
    m_spList = source->m_spList;

    return true;
}

// 按照版本序列化/反序列化
const quint8 CURRENT_VERSION_APDC = 1;
bool CMDActionPropertyData_Config::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_APDC : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream, useCurrentVersion);
    else
        result = false;
    return result;
}
void CMDActionPropertyData_Config::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

// 版本 1
void CMDActionPropertyData_Config::serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion)
{
    m_actionID.serialize(stream, useCurrentVersion);
    stream << m_name << m_comment << m_enable;
    stream << m_enableLog << m_enableDelay << m_delayLength;
    m_spList.serialize(stream, useCurrentVersion);
}
void CMDActionPropertyData_Config::deserializeOnVersion_1(QDataStream& stream)
{
    m_actionID.deserialize(stream);
    stream >> m_name >> m_comment >> m_enable;
    stream >> m_enableLog >> m_enableDelay >> m_delayLength;
    m_spList.deserialize(stream);
}

void CMDActionPropertyData_Config::serialize(QJsonObject& json)
{
    QJsonObject MyJson;

    QJsonObject actionIDJson;
    m_actionID.serialize(actionIDJson);
    MyJson.insert("ActionID", actionIDJson);
    MyJson.insert("Name",m_name);
    MyJson.insert("Comment", m_comment);
    MyJson.insert("Enable", m_enable);
    MyJson.insert("EnableLog", m_enableLog);
    MyJson.insert("EnableDelay", m_enableDelay);
    MyJson.insert("DelayLength", m_delayLength);
    MyJson.insert("EnableExecExp", m_enableExecExp);
    MyJson.insert("ExecExp", m_execExp);
    QJsonArray spListJson;
    m_spList.serialize(spListJson);
    MyJson.insert("SPList", spListJson);

    json.insert("ActionProperty", MyJson);
}

void CMDActionPropertyData_Config::deserialize(const QJsonObject& json)
{
    QJsonObject MyJson = json.value("ActionProperty").toObject();

    QJsonObject actionIDJson = MyJson.value("ActionID").toObject();
    m_actionID.deserialize(actionIDJson);
    m_name = MyJson.value("Name").toString();
    m_comment = MyJson.value("Comment").toString();
    m_enable = MyJson.value("Enable").toBool();
    m_enableLog = MyJson.value("EnableLog").toBool();
    m_enableDelay = MyJson.value("EnableDelay").toBool();
    m_delayLength = MyJson.value("DelayLength").toInt();
    if(MyJson.contains("EnableExecExp"))
        m_enableExecExp = MyJson.value("EnableExecExp").toBool();
    if(MyJson.contains("ExecExp"))
        m_execExp = MyJson.value("ExecExp").toString();
    QJsonArray spListJson = MyJson.value("SPList").toArray();
    m_spList.deserialize(spListJson);
}

// 写变量
CAPCData_VariableWriteValue::CAPCData_VariableWriteValue()
{
    m_mode = MDWriteMode::mode_fixed;
    m_valueSource = 0;
    m_version = 0;
}

CAPCData_VariableWriteValue::~CAPCData_VariableWriteValue()
{

}

CAPCData_VariableWriteValue::CAPCData_VariableWriteValue(const CAPCData_VariableWriteValue& obj)
{
    copy(&obj);
}

CAPCData_VariableWriteValue& CAPCData_VariableWriteValue::operator = (const CAPCData_VariableWriteValue& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CAPCData_VariableWriteValue::copy(const CAPCData_VariableWriteValue* source)
{
    m_version = source->m_version;
    m_mode = source->m_mode;
    m_value = source->m_value;
    m_valueSource = source->m_valueSource;
    m_sourceName = source->m_sourceName;

    return true;
}

const quint8 CURRENT_VERSION_VWV = 1;
bool CAPCData_VariableWriteValue::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_VWV : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream);
    else
        result = false;
    return result;
}

void CAPCData_VariableWriteValue::serializeOnVersion_1(QDataStream& stream)
{
    quint8 mode = (quint8)m_mode;

    stream << m_valueSource << m_sourceName;
    stream << mode << m_value;
}

void CAPCData_VariableWriteValue::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

void CAPCData_VariableWriteValue::deserializeOnVersion_1(QDataStream& stream)
{
    quint8 mode;

    stream >> m_valueSource >> m_sourceName;
    stream >> mode >> m_value;

    m_mode = (MDWriteMode)mode;
}

void CAPCData_VariableWriteValue::serialize(QJsonObject& json)
{
    QJsonObject MyJson;

    MyJson.insert("ValueSource", (int)m_valueSource);
    MyJson.insert("SourceName", m_sourceName);
    MyJson.insert("Mode", (int)m_mode);
    MyJson.insert("Value", m_value.toString());

    json.insert("VariableWriteValue", MyJson);
}

void CAPCData_VariableWriteValue::deserialize(const QJsonObject& json)
{
    QJsonObject MyJson = json.value("VariableWriteValue").toObject();

    m_valueSource = (qint8)MyJson.value("ValueSource").toInt();
    m_sourceName = MyJson.value("SourceName").toString();
    m_mode = (MDWriteMode)MyJson.value("Mode").toInt();
    m_value = MyJson.value("Value").toVariant();
}

// 窗口动作
CAPCData_WindowOperation::CAPCData_WindowOperation()
{
    m_left = 0;
    m_top = 0;
    m_width = 100;
    m_height = 100;
    m_pwid = 0;
    m_gRect = QRect(0,0,0,0);
    m_metadata = "";
    m_version = 0;
}

CAPCData_WindowOperation::~CAPCData_WindowOperation()
{

}

CAPCData_WindowOperation::CAPCData_WindowOperation(const CAPCData_WindowOperation& obj)
{
    copy(&obj);
}

CAPCData_WindowOperation& CAPCData_WindowOperation::operator = (const CAPCData_WindowOperation& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CAPCData_WindowOperation::copy(const CAPCData_WindowOperation* source)
{
    m_version = source->m_version;
    m_second = source->m_second;
    m_left = source->m_left;
    m_top = source->m_top;
    m_width = source->m_width;
    m_height = source->m_height;
    m_pwid = source->m_pwid;
    m_gRect = source->m_gRect;
    m_metadata = source->m_metadata;
    return true;
}

const quint8 CURRENT_VERSION_WOPER = 2;
bool CAPCData_WindowOperation::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_WOPER : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream);
    else if(actualVersion == 2)
        serializeOnVersion_2(stream);
    else
        result = false;
    return result;
}

void CAPCData_WindowOperation::serializeOnVersion_1(QDataStream& stream)
{
    stream << m_second;
}

void CAPCData_WindowOperation::serializeOnVersion_2(QDataStream& stream)
{
    serializeOnVersion_1(stream);
    stream << m_left << m_top << m_width << m_height;
}

void CAPCData_WindowOperation::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else if(m_version == 2)
        deserializeOnVersion_2(stream);
    else{}
}

void CAPCData_WindowOperation::deserializeOnVersion_1(QDataStream& stream)
{
    stream >> m_second;
}

void CAPCData_WindowOperation::deserializeOnVersion_2(QDataStream& stream)
{
    deserializeOnVersion_1(stream);
    stream >> m_left >> m_top >> m_width >> m_height;
}

void CAPCData_WindowOperation::serialize(QJsonObject& json)
{
    QJsonObject MyJson;

    MyJson.insert("Second", m_second);
    MyJson.insert("Left", m_left);
    MyJson.insert("Top", m_top);
    MyJson.insert("Width", m_width);
    MyJson.insert("Height", m_height);

    json.insert("WindowOperation", MyJson);
}

void CAPCData_WindowOperation::deserialize(const QJsonObject& json)
{
    QJsonObject MyJson = json.value("WindowOperation").toObject();

    m_second = MyJson.value("Second").toString();
    m_left = MyJson.value("Left").toInt();
    m_top = MyJson.value("Top").toInt();
    m_width = MyJson.value("Width").toInt();
    m_height = MyJson.value("Height").toInt();
}

// 播放语音
CAPCData_PlayTTS::CAPCData_PlayTTS()
{
    m_mode = 0;
    m_repeat = 1;
    m_version = 0;
}

CAPCData_PlayTTS::~CAPCData_PlayTTS()
{

}

CAPCData_PlayTTS::CAPCData_PlayTTS(const CAPCData_PlayTTS& obj)
{
    copy(&obj);
}

CAPCData_PlayTTS& CAPCData_PlayTTS::operator = (const CAPCData_PlayTTS& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CAPCData_PlayTTS::copy(const CAPCData_PlayTTS* source)
{
    m_version = source->m_version;
    m_mode = source->m_mode;
    m_repeat = source->m_repeat;
    m_ttsText = source->m_ttsText;
    m_ttsTextFile = source->m_ttsTextFile;

    return true;
}

const quint8 CURRENT_VERSION_PTTS = 1;
bool CAPCData_PlayTTS::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_PTTS : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream);
    else
        result = false;
    return result;
}

void CAPCData_PlayTTS::serializeOnVersion_1(QDataStream& stream)
{
    stream << m_mode << m_repeat << m_ttsText << m_ttsTextFile;
}

void CAPCData_PlayTTS::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

void CAPCData_PlayTTS::deserializeOnVersion_1(QDataStream& stream)
{
    stream >> m_mode >> m_repeat >> m_ttsText >> m_ttsTextFile;
}

void CAPCData_PlayTTS::serialize(QJsonObject& json)
{
    QJsonObject MyJson;

    MyJson.insert("Mode", m_mode);
    MyJson.insert("Repeat", m_repeat);
    MyJson.insert("TTSText", m_ttsText);
    MyJson.insert("TTSTextFile", m_ttsTextFile);

    json.insert("PlayTTS", MyJson);
}

void CAPCData_PlayTTS::deserialize(const QJsonObject& json)
{
    QJsonObject MyJson = json.value("PlayTTS").toObject();

    m_mode = MyJson.value("Mode").toInt();
    m_repeat = MyJson.value("Repeat").toInt();
    m_ttsText = MyJson.value("TTSText").toInt();
    m_ttsTextFile = MyJson.value("TTSTextFile").toInt();
}

CMDActionPropertyData_Runtime::CMDActionPropertyData_Runtime()
{
    m_exeCount = 0;
    m_lastExeTime;
    m_enabledExeSP = NULL;
}

CMDActionPropertyData_Runtime::~CMDActionPropertyData_Runtime()
{

}

CMDActionPropertyData_Runtime::CMDActionPropertyData_Runtime(const CMDActionPropertyData_Runtime& obj)
{
    m_exeCount = obj.m_exeCount;
    m_lastExeTime = obj.m_lastExeTime;
    m_enabledExeSP = obj.m_enabledExeSP;
}

CMDActionPropertyData_Runtime& CMDActionPropertyData_Runtime::operator = (const CMDActionPropertyData_Runtime& obj)
{
    if( this != &obj )
    {
        m_exeCount = obj.m_exeCount;
        m_lastExeTime = obj.m_lastExeTime;
        m_enabledExeSP = obj.m_enabledExeSP;
    }
    return *this;
}
