#include "MDActionIDData.h"
#include <QJsonArray>
#include <QJsonObject>

CMDActionIDData::CMDActionIDData()
{
    m_objType = MDObjectType::null_obj;
    m_type = MDActionType::null_action;
    m_version = 0;
    m_customAction = "";
    m_customObjectName = "";
}

CMDActionIDData::~CMDActionIDData()
{

}

CMDActionIDData::CMDActionIDData(const CMDActionIDData& obj)
{
    copy(&obj);
}

CMDActionIDData& CMDActionIDData::operator = (const CMDActionIDData& obj)
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
bool CMDActionIDData::copy(const CMDActionIDData* source)
{
    m_version = source->m_version;
    m_type = source->m_type;
    m_objType = source->m_objType;
    m_objName = source->m_objName;
    m_srcState = source->m_srcState;
    m_destState = source->m_destState;
    m_customAction = source->m_customAction;
    m_customObjectName = source->m_customObjectName;

    return true;
}

MDObjectType CMDActionIDData::actionToObjType(MDActionType actionType)
{
    MDObjectType objType = MDObjectType::null_obj;
    switch (actionType)
    {
    case MDActionType::project_Load:
     case MDActionType::project_Unload:
     case MDActionType::project_ExitFS:
    {
        objType = MDObjectType::project;
    }
        break;
    case MDActionType::variable_ReadValue:
    case MDActionType::variable_WriteValue:
    {
        objType = MDObjectType::variable;
    }
        break;
    case MDActionType::window_Open:
    case MDActionType::window_Close:
    {
        objType = MDObjectType::window;
    }
        break;
    case MDActionType::recipe_Upload:
    case MDActionType::recipe_Download:
    case MDActionType::recipe_View:
    case MDActionType::recipe_Modify:
    case MDActionType::recipe_Delete:
    {
        objType = MDObjectType::recipe;
    }
        break;
    case MDActionType::timer_Delay:
    case MDActionType::timer_Start:
    case MDActionType::timer_Stop:
    {
        objType = MDObjectType::timer;
    }
        break;
    case MDActionType::account_Modify:
    case MDActionType::account_ModifyPassword:
    case MDActionType::account_Delete:
    {
        objType = MDObjectType::account;
    }
        break;
    case MDActionType::powerGroup_Modify:
    case MDActionType::powerGroup_Delete:
    {
        objType = MDObjectType::powerGroup;
    }
        break;
    case MDActionType::action_Execute:
    {
        objType = MDObjectType::action;
    }
        break;
    case MDActionType::alarm_Acknowledge:
    case MDActionType::alarm_Confirm:
    {
        objType = MDObjectType::alarm;
    }
        break;
    case MDActionType::historyGroup_Save:
    {
        objType = MDObjectType::historyGroup;
    }
        break;
    case MDActionType::tts_Play:
    {
        objType = MDObjectType::action;
    }
        break;
    case MDActionType::custom:
    {
        objType = MDObjectType::custom;
    }
        break;
    default:
        break;
    }
    return objType;
}

/*!
\section 序列化
*/
// 按照版本序列化/反序列化
const quint8 CURRENT_VERSION_AIDD = 1;
bool CMDActionIDData::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_AIDD : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream);
    else
        result = false;
    return result;
}
void CMDActionIDData::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

// 版本 1
void CMDActionIDData::serializeOnVersion_1(QDataStream& stream)
{
    quint16 type = (quint16)m_type;
    quint16 objType = (quint16)m_objType;

    stream << type << objType << m_objName;
}
void CMDActionIDData::deserializeOnVersion_1(QDataStream& stream)
{
    quint16 type;
    quint16 objType;

    stream >> type >> objType >> m_objName;

    m_type = (MDActionType)type;
    m_objType = (MDObjectType)objType;
}

void CMDActionIDData::serialize(QJsonObject& json)
{
    // Type
    json.insert("@T", (quint16)m_type);
    // ObjType
    json.insert("@OT", (quint16)m_objType);
    // ObjName
    json.insert("@N", m_objName);
    // Comment
    json.insert("@Comment", m_objComment);
}

void CMDActionIDData::deserialize(const QJsonObject& json)
{
    quint16 type = json.value("@T").toInt();
    m_type = (MDActionType)type;
    quint16 objType = json.value("@OT").toInt();
    m_objType = (MDObjectType)objType;
    m_objName = json.value("@N").toString();
    m_objComment = json.value("@Comment").toString();
}
