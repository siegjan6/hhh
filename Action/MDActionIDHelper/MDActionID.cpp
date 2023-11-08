#include "MDActionID.h"
#include "MDActionIDData.h"
#include "MDStringResource.h"

static CMDActionID DEFAULT_ACTION_ID;

CMDActionID::CMDActionID()
{
    m_data = new CMDActionIDData;
}

CMDActionID::CMDActionID(MDActionType type, const QString& name)
{
    m_data = new CMDActionIDData;
    if(m_data)
    {
        setType(type);
        m_data->m_objName = name;
    }
}

CMDActionID::~CMDActionID()
{
    if(m_data)
        delete m_data;
}

CMDActionID& CMDActionID::operator = (const CMDActionID& obj)
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
MDObjectType CMDActionID::actionToObjType(MDActionType actionType)
{
    return CMDActionIDData::actionToObjType(actionType);
}

CMDActionID& CMDActionID::theDefault()
{
    return DEFAULT_ACTION_ID;
}

MDActionType CMDActionID::type() const
{
    return (m_data ? m_data->m_type : MDActionType::null_action);
}
void CMDActionID::setType(MDActionType type)
{
    if(m_data)
    {
        m_data->m_type = type;
        m_data->m_objType = actionToObjType(type);
    }
}

MDObjectType CMDActionID::objType() const
{
    return (m_data ? m_data->m_objType : MDObjectType::null_obj);
}

QString CMDActionID::objName() const
{
    return (m_data ? m_data->m_objName : "");
}
void CMDActionID::setObjName(const QString& name)
{
    if(m_data)
        m_data->m_objName = name;
}

QString CMDActionID::customAction() const
{
    return (m_data ? m_data->m_customAction : "");
}
void CMDActionID::setCustomAction(const QString& action)
{
    if(m_data)
        m_data->m_customAction = action;
}

QString CMDActionID::customObjectName() const
{
    return (m_data ? m_data->m_customObjectName : "");
}
void CMDActionID::setCustomObjectName(const QString& name)
{
    if(m_data)
        m_data->m_customObjectName = name;
}

QString CMDActionID::customComment() const
{
    return (m_data ? m_data->m_customComment : "");
}
void CMDActionID::setCustomComment(const QString& comment)
{
    if(m_data)
        m_data->m_customComment = comment;
}

QVariant CMDActionID::srcState() const
{
    return (m_data ? m_data->m_srcState : QVariant());
}
void CMDActionID::setSrcState(const QVariant& state)
{
    if(m_data)
        m_data->m_srcState = state;
}

QVariant CMDActionID::destState() const
{
    return (m_data ? m_data->m_destState : QVariant());
}
void CMDActionID::setDestState(const QVariant& state)
{
    if(m_data)
        m_data->m_destState = state;
}

bool CMDActionID::copy(const CMDActionID* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

QString CMDActionID::typeToString(MDActionType actionType)
{
    QString str;
    switch (actionType) {
    case MDActionType::project_Load: str = STRING_AT_PROJECT_LOAD;
        break;
    case MDActionType::project_Unload: str = STRING_AT_PROJECT_UNLOAD;
        break;
    case MDActionType::project_ExitFS: str = STRING_AT_PROJECT_EXITFS;
        break;

    case MDActionType::variable_WriteValue: str = STRING_AT_VARIABLE_WRITE_VALUE;
        break;
    case MDActionType::variable_ReadValue: str = STRING_AT_VARIABLE_READ_VALUE;
        break;

    case MDActionType::timer_Delay: str = STRING_AT_TIMER_DELAY;
        break;
    case MDActionType::timer_Start: str = STRING_AT_TIMER_START;
        break;
    case MDActionType::timer_Stop: str = STRING_AT_TIMER_STOP;
        break;

    case MDActionType::recipe_Upload: str = STRING_AT_RECIPE_UPLOAD;
        break;
    case MDActionType::recipe_Download: str = STRING_AT_RECIPE_DOWNLOAD;
        break;
    case MDActionType::recipe_View: str = STRING_AT_RECIPE_VIEW;
        break;
    case MDActionType::recipe_Modify: str = STRING_AT_RECIPE_MODIFY;
        break;
    case MDActionType::recipe_Delete: str = STRING_AT_RECIPE_DELETE;
        break;

    case MDActionType::window_Open: str = STRING_AT_WINDOW_OPEN;
        break;
    case MDActionType::window_Close: str = STRING_AT_WINDOW_CLOSE;
        break;

    case MDActionType::account_Modify: str = STRING_AT_ACCOUNT_MODIFY;
        break;
    case MDActionType::account_ModifyPassword: str = STRING_AT_ACCOUNT_MODIFY_PASSWORD;
        break;
    case MDActionType::account_Delete: str = STRING_AT_ACCOUNT_DELETE;
        break;

    case MDActionType::powerGroup_Modify: str = STRING_AT_POWER_GROUP_MODIFY;
        break;
    case MDActionType::powerGroup_Delete: str = STRING_AT_POWER_GROUP_DELETE;
        break;

    case MDActionType::action_Execute: str = STRING_AT_ACTION_EXECUTE;
        break;
    case MDActionType::alarm_Acknowledge: str = STRING_AT_ALARM_ACKNOWLEDGE;
        break;
    case MDActionType::alarm_Confirm: str = STRING_AT_ALARM_CONFIRM;
        break;
    case MDActionType::tts_Play: str = STRING_AT_TTS_PLAY;
        break;
    case MDActionType::historyGroup_Save: str = STRING_AT_HISTORY_GROUP_SAVE;
        break;
    case MDActionType::account_powergroup_Manage: str = STRING_AT_ACCOUNT_POWER_GROUP_MANAGER;
        break;
    default:
        break;
    }
    return str;
}

QString CMDActionID::getLogMsg()
{
    if(type() == MDActionType::null_action)
        return "";
    QString _actionName = CMDActionID::typeToString(type());
    QString _objName = objName();
    if(!customAction().isEmpty())
        _actionName = customAction();
    if(!customObjectName().isEmpty())
        _objName = customObjectName();
    QString msg = QString("%1 %2").arg(_actionName).arg(_objName);
    switch(type())
    {
    case MDActionType::variable_WriteValue:
        msg += QString("[从 %1 更改为 %2]").arg(srcState().toString()).arg(destState().toString());
        break;
    default:
        break;
    }

    if(!customComment().isEmpty())
        msg = customComment();
    return msg;
}

/*!
\section 序列化
*/
bool CMDActionID::serialize(QDataStream& stream, bool useCurrentVersion)
{
    return (m_data ? m_data->serialize(stream, useCurrentVersion) : false);
}

void CMDActionID::deserialize(QDataStream& stream)
{
    if(m_data)
        m_data->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CMDActionID::serialize(QJsonObject& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CMDActionID::deserialize(const QJsonObject& json)
{
    if(m_data)
        m_data->deserialize(json);
}
