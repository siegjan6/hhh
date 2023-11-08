#include "MDEventProperty_Config.h"
#include "MDEventPropertyData.h"
#include "MDStringResource.h"

const QStringList EMPTY_STRINGLIST;

CMDEventProperty_Config::CMDEventProperty_Config()
{
    m_data = new CMDEventPropertyData_Config;
}

CMDEventProperty_Config::CMDEventProperty_Config(MDEventType eventType)
{
    m_data = new CMDEventPropertyData_Config(eventType);
}

CMDEventProperty_Config::~CMDEventProperty_Config()
{
    if(m_data)
        delete m_data;
}

CMDEventProperty_Config::CMDEventProperty_Config(const CMDEventProperty_Config& obj)
{
    m_data = new CMDEventPropertyData_Config(obj.type());

    copy(&obj);
}

CMDEventProperty_Config& CMDEventProperty_Config::operator = (const CMDEventProperty_Config& obj)
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
QString CMDEventProperty_Config::name() const
{
    return (m_data ? m_data->m_name : "");
}
void CMDEventProperty_Config::setName(const QString& name)
{
    if(m_data)
        m_data->m_name = name;
}

QString CMDEventProperty_Config::comment() const
{
    return (m_data ? m_data->m_comment : "");
}
void CMDEventProperty_Config::setComment(const QString& comment)
{
    if(m_data)
        m_data->m_comment = comment;
}

bool CMDEventProperty_Config::isEnable() const
{
    return (m_data ? m_data->m_enable : false);
}
void CMDEventProperty_Config::setEnable(bool flag)
{
    if(m_data)
        m_data->m_enable = flag;
}

bool CMDEventProperty_Config::isEnableLog() const
{
    return (m_data ? m_data->m_enableLog : false);
}
void CMDEventProperty_Config::setEnableLog(bool flag)
{
    if(m_data)
        m_data->m_enableLog = flag;
}

MDEventType CMDEventProperty_Config::type() const
{
    return (m_data ? m_data->m_type : MDEventType::null_event);
}
void CMDEventProperty_Config::setType(MDEventType type)
{
    if(m_data)
        m_data->m_type = type;
}

QString CMDEventProperty_Config::objName() const
{
    return (m_data ? m_data->m_objName : "");
}
void CMDEventProperty_Config::setObjName(const QString& objName)
{
    if(m_data)
        m_data->m_objName = objName;
}

QString CMDEventProperty_Config::expressionString() const
{
    return (m_data ? m_data->m_expressionString : "");
}
void CMDEventProperty_Config::setExpressionString(const QString& expressionString)
{
    if(m_data)
        m_data->m_expressionString = expressionString;
}

const QStringList& CMDEventProperty_Config::actionNameList() const
{
    return (m_data ? m_data->m_actionNameList : EMPTY_STRINGLIST);
}
void CMDEventProperty_Config::setActionNameList(const QStringList& actionNameList)
{
    if(m_data)
    {
        m_data->m_actionNameList.clear();
        m_data->m_actionNameList.append(actionNameList);
    }
}

QString CMDEventProperty_Config::condition() const
{
    return "";
}

bool CMDEventProperty_Config::copy(const CMDEventProperty_Config* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

QString CMDEventProperty_Config::typeToString(MDEventType eventType)
{
    QString str;
    switch (eventType) {
    case MDEventType::combined: str = STRING_ET_COMBINED;
        break;
    case MDEventType::project_Loaded: str = STRING_ET_PROJECT_LOADED;
        break;
    case MDEventType::project_BeforeUnload: str = STRING_ET_PROJECT_BEFORE_UNLOAD;
        break;
    case MDEventType::variable_ValueChanged: str = STRING_ET_VARIABLE_VALUE_CHANGED;
        break;
    case MDEventType::variable_ValueChangedInRange: str = STRING_ET_VARIABLE_VALUE_CHANGED_IN_RANGE;
        break;
    case MDEventType::variable_QualityChanged: str = STRING_ET_VARIABLE_QUALITY_CHANGED;
        break;
    case MDEventType::variable_ValueRead: str = STRING_ET_VARIABLE_VALUE_READ;
        break;
    case MDEventType::variable_ValueWrite: str = STRING_ET_VARIABLE_VALUE_WRITE;
        break;
    case MDEventType::timer_Cycle: str = STRING_ET_TIMER_CYCLE;
        break;
    case MDEventType::timer_Moment: str = STRING_ET_TIMER_MOMENT;
        break;
    case MDEventType::recipe_Uploaded: str = STRING_ET_RECIPE_UPLOADED;
        break;
    case MDEventType::recipe_Downloaded: str = STRING_ET_RECIPE_DOWNLOADED;
        break;
    case MDEventType::recipe_BeforeView: str = STRING_ET_RECIPE_BEFORE_VIEW;
        break;
    case MDEventType::recipe_BeforeModify: str = STRING_ET_RECIPE_BEFORE_MODIFY;
        break;
    case MDEventType::recipe_BeforeDelete: str = STRING_ET_RECIPE_BEFORE_DELETE;
        break;
    case MDEventType::window_Opened: str = STRING_ET_WINDOW_OPENED;
        break;
    case MDEventType::window_Closed: str = STRING_ET_WINDOW_CLOSED;
        break;
    default:
        break;
    }
    return str;
}

/*!
\section 序列化
*/
bool CMDEventProperty_Config::serialize(QDataStream& stream, bool useCurrentVersion)
{
    return (m_data ? m_data->serialize(stream, useCurrentVersion) : false);
}

void CMDEventProperty_Config::deserialize(QDataStream& stream)
{
    if(m_data)
        m_data->deserialize(stream);
}

/*!
\section 序列化：JSON
*/
void CMDEventProperty_Config::serialize(QJsonObject& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CMDEventProperty_Config::deserialize(const QJsonObject& json)
{
    if(m_data)
        m_data->deserialize(json);
}
