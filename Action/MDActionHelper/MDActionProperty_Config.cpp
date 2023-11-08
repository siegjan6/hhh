#include "MDActionProperty_Config.h"
#include "MDActionPropertyData.h"

CMDActionProperty_Config::CMDActionProperty_Config()
{
    m_data = new CMDActionPropertyData_Config;
}

CMDActionProperty_Config::CMDActionProperty_Config(MDActionType actionType)
{
    m_data = new CMDActionPropertyData_Config(actionType);
}

CMDActionProperty_Config::~CMDActionProperty_Config()
{
    if(m_data)
        delete m_data;
}

CMDActionProperty_Config::CMDActionProperty_Config(const CMDActionProperty_Config& obj)
{
    m_data = new CMDActionPropertyData_Config(obj.actionID().type());

    copy(&obj);
}

CMDActionProperty_Config& CMDActionProperty_Config::operator = (const CMDActionProperty_Config& obj)
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
QString CMDActionProperty_Config::name() const
{
    return (m_data ? m_data->m_name : "");
}
void CMDActionProperty_Config::setName(const QString& name)
{
    if(m_data)
        m_data->m_name = name;
}

QString CMDActionProperty_Config::comment() const
{
    return (m_data ? m_data->m_comment : "");
}
void CMDActionProperty_Config::setComment(const QString& comment)
{
    if(m_data)
        m_data->m_comment = comment;
}

bool CMDActionProperty_Config::isEnable() const
{
    return (m_data ? m_data->m_enable : false);
}
void CMDActionProperty_Config::setEnable(bool enable)
{
    if(m_data)
        m_data->m_enable = enable;
}

bool CMDActionProperty_Config::isEnableLog() const
{
    return (m_data ? m_data->m_enableLog : false);
}
void CMDActionProperty_Config::setEnableLog(bool enable)
{
    if(m_data)
        m_data->m_enableLog = enable;
}

CMDActionID& CMDActionProperty_Config::actionID() const
{
    return (m_data ? m_data->m_actionID : CMDActionID::theDefault());
}
void CMDActionProperty_Config::setActionID(const CMDActionID& actionID)
{
    if(m_data)
        m_data->m_actionID = actionID;
}

bool CMDActionProperty_Config::isEnableDelay() const
{
    return (m_data ? m_data->m_enableDelay : false);
}
void CMDActionProperty_Config::setEnableDelay(bool enable)
{
    if(m_data)
        m_data->m_enableDelay = enable;
}

qint32 CMDActionProperty_Config::delayLength() const
{
    return (m_data ? m_data->m_delayLength : 0);
}
void CMDActionProperty_Config::setDelayLength(qint32 delayLength)
{
    if(m_data)
        m_data->m_delayLength = delayLength;
}

bool CMDActionProperty_Config::isEnableExecExp() const
{
    return (m_data ? m_data->m_enableExecExp : false);
}
void CMDActionProperty_Config::setEnableExecExp(bool enable)
{
    if(m_data)
        m_data->m_enableExecExp = enable;
}

QString CMDActionProperty_Config::execExp() const
{
    return (m_data ? m_data->m_execExp : 0);
}
void CMDActionProperty_Config::setExecExp(QString exp)
{
    if(m_data)
        m_data->m_execExp = exp;
}

CMDActionSPList* CMDActionProperty_Config::spList()
{
    return (m_data ? &m_data->m_spList : NULL);
}
bool CMDActionProperty_Config::setSPList(const CMDActionSPList* spList)
{
    if(!m_data || !spList)
        return false;

    m_data->m_spList = *spList;
    return true;
}

bool CMDActionProperty_Config::copy(const CMDActionProperty_Config* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

/*!
\section 序列化
*/
bool CMDActionProperty_Config::serialize(QDataStream& stream, bool useCurrentVersion)
{
    return (m_data ? m_data->serialize(stream, useCurrentVersion) : false);
}

void CMDActionProperty_Config::deserialize(QDataStream& stream)
{
    if(m_data)
        m_data->deserialize(stream);
}

/*!
\section 序列化：JSON
*/
void CMDActionProperty_Config::serialize(QJsonObject& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CMDActionProperty_Config::deserialize(const QJsonObject& json)
{
    if(m_data)
        m_data->deserialize(json);
}

