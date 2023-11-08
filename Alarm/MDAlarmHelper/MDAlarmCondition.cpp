#include "MDAlarmCondition.h"
#include "MDAlarmConditionImp.h"
#include <QJsonDocument>

CMDAlarmCondition::CMDAlarmCondition()
{
    m_data = new CMDAlarmConditionImp;
    m_currentLimitAlarmMode = LimitAlarmMode::NONE;
    m_message = "";
}

CMDAlarmCondition::~CMDAlarmCondition()
{
    if(m_data)
        delete m_data;
}

CMDAlarmCondition::CMDAlarmCondition(const CMDAlarmCondition& obj)
{
    m_data = new CMDAlarmConditionImp(*obj.m_data);

    copy(&obj);
}

CMDAlarmCondition& CMDAlarmCondition::operator = (const CMDAlarmCondition& obj)
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
QString CMDAlarmCondition::name() const
{
    return (m_data ? m_data->m_name : "");
}
void CMDAlarmCondition::setName(const QString& name)
{
    if(m_data)
        m_data->m_name = name;
}

QString CMDAlarmCondition::message() const
{
    if(m_message.isEmpty())
        return comment();
    return m_message;
}
void CMDAlarmCondition::setMessage(const QString& message)
{
    m_message = message;
}

QString CMDAlarmCondition::comment() const
{
    return (m_data ? m_data->m_comment : "");
}
void CMDAlarmCondition::setComment(const QString& comment)
{
    if(m_data)
        m_data->m_comment = comment;
    if(m_message.isEmpty())
        m_message = comment;
}

QString CMDAlarmCondition::levelName() const
{
    return (m_data ? m_data->m_levelName : "");
}
void CMDAlarmCondition::setLevelName(const QString& levelName)
{
    if(m_data)
        m_data->m_levelName = levelName;
}

QString CMDAlarmCondition::expression() const
{
    return (m_data ? m_data->m_expression : "");
}
void CMDAlarmCondition::setExpression(const QString& expression)
{
    if(m_data)
        m_data->m_expression = expression;
}

QString CMDAlarmCondition::bindVar()
{
    return (m_data ? m_data->m_bindvar : "");
}
void CMDAlarmCondition::setBindVar(const QString& bindVar)
{
    if(m_data)
        m_data->m_bindvar = bindVar;
}

QString CMDAlarmCondition::customId() const
{
    return  (m_data ? m_data->m_customId : "");
}
void CMDAlarmCondition::setCustomId(const QString& customId)
{
    if(m_data)
        m_data->m_customId = customId;
}

int CMDAlarmCondition::serverity() const
{
    return  (m_data ? m_data->m_serverity : 0);
}
void CMDAlarmCondition::setServerity(const int& serverity)
{
    if(m_data)
        m_data->m_serverity = serverity;
}

bool CMDAlarmCondition::delayEnabled() const
{
    return  (m_data ? m_data->m_delayEnabled : false);
}
void CMDAlarmCondition::setDelayEnabled(const bool& delayEnabled)
{
    if(m_data)
        m_data->m_delayEnabled = delayEnabled;
}

QString CMDAlarmCondition::delayConditionExp()
{
    return  (m_data ? m_data->m_delayConditionExp : "");
}
void CMDAlarmCondition::setDelayConditionExp(const QString& delayConditionExp)
{
    if(m_data)
        m_data->m_delayConditionExp = delayConditionExp;
}

QString CMDAlarmCondition::enableAlarmExp()
{
    return  (m_data ? m_data->m_enableAlarmExp : "");
}
void CMDAlarmCondition::setEnableAlarmExp(const QString& enableAlarmExp)
{
    if(m_data)
        m_data->m_enableAlarmExp = enableAlarmExp;
}

QString CMDAlarmCondition::delayTimeExp()
{
    return  (m_data ? m_data->m_delayTimeExp : "");
}
void CMDAlarmCondition::setDelayTimeExp(const QString& delayTimeExp)
{
    if(m_data)
        m_data->m_delayTimeExp = delayTimeExp;
}

bool CMDAlarmCondition::latched() const
{
    return  (m_data ? m_data->m_latched : false);
}
void CMDAlarmCondition::setLatched(const bool& latched)
{
    if(m_data)
        m_data->m_latched = latched;
}

bool CMDAlarmCondition::acknowledgeRequired() const
{
    return  (m_data ? m_data->m_acknowledgeRequired : false);
}
void CMDAlarmCondition::setAcknowledgeRequired(const bool& acknowledgeRequired)
{
    if(m_data)
        m_data->m_acknowledgeRequired = acknowledgeRequired;
}

bool CMDAlarmCondition::enabled() const
{
    return  (m_data ? m_data->m_enabled : true);
}
void CMDAlarmCondition::setEnabled(const bool& enabled)
{
    if(m_data)
        m_data->m_enabled = enabled;
}


CMDAlarmCondition::LimitAlarmMode CMDAlarmCondition::currentLimitAlarmMode() const
{
    return m_currentLimitAlarmMode;
}

void CMDAlarmCondition::setCurrentLimitAlarmMode(const LimitAlarmMode& currentLimitAlarmMode)
{
    m_currentLimitAlarmMode = currentLimitAlarmMode;
}

CMDActionSPList* CMDAlarmCondition::spList()
{
    return (m_data ? &m_data->m_spList : NULL);
}
bool CMDAlarmCondition::setSPList(const CMDActionSPList* spList)
{
    if(!m_data || !spList)
        return false;

    m_data->m_spList = *spList;
    return true;
}

bool CMDAlarmCondition::copy(const CMDAlarmCondition* source)
{
    this->setCurrentLimitAlarmMode(source->currentLimitAlarmMode());
    return (m_data ? m_data->copy(source->m_data) : false);
}

/*!
\section 序列化
*/
bool CMDAlarmCondition::serialize(QDataStream& stream, bool useCurrentVersion)
{
    return (m_data ? m_data->serialize(stream, useCurrentVersion) : false);
}

void CMDAlarmCondition::deserialize(QDataStream& stream)
{
    if(m_data)
        m_data->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CMDAlarmCondition::serialize(QJsonObject& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CMDAlarmCondition::deserialize(const QJsonObject& json)
{
    if(m_data)
        m_data->deserialize(json);
}

QString CMDAlarmCondition::toJsonString()
{
    QJsonObject alarm;
    serialize(alarm);
    QJsonDocument doc;
    doc.setObject(alarm);
    return QString(doc.toJson(QJsonDocument::Compact).constData());
}
