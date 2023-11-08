#include "MDAccountProperty_Config.h"
#include "MDAccountPropertyData.h"

CMDAccountProperty_Config::CMDAccountProperty_Config()
{
    m_data = new CMDAccountPropertyData_Config;
}

CMDAccountProperty_Config::~CMDAccountProperty_Config()
{
    if(m_data)
        delete m_data;
}

CMDAccountProperty_Config::CMDAccountProperty_Config(const CMDAccountProperty_Config& obj)
{
    m_data = new CMDAccountPropertyData_Config();

    copy(&obj);
}

CMDAccountProperty_Config& CMDAccountProperty_Config::operator = (const CMDAccountProperty_Config& obj)
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
QString CMDAccountProperty_Config::name() const
{
    return (m_data ? m_data->m_name : "");
}
void CMDAccountProperty_Config::setName(const QString& name)
{
    if(m_data)
        m_data->m_name = name;
}

QString CMDAccountProperty_Config::password() const
{
    return (m_data ? m_data->m_password : "");
}
void CMDAccountProperty_Config::setPassword(const QString& password)
{
    if(m_data)
        m_data->m_password = password;
}

QString CMDAccountProperty_Config::comment() const
{
    return (m_data ? m_data->m_comment : "");
}
void CMDAccountProperty_Config::setComment(const QString& comment)
{
    if(m_data)
        m_data->m_comment = comment;
}

qint8 CMDAccountProperty_Config::type() const
{
    return (m_data ? m_data->m_type : 0);
}
void CMDAccountProperty_Config::setType(qint8 type)
{
    if(m_data)
        m_data->m_type = type;
}

bool CMDAccountProperty_Config::isEnable() const
{
    return (m_data ? m_data->m_enable : false);
}
void CMDAccountProperty_Config::setEnable(bool enable)
{
    if(m_data)
        m_data->m_enable = enable;
}

bool CMDAccountProperty_Config::isLocked() const
{
    return (m_data ? m_data->m_locked : false);
}
void CMDAccountProperty_Config::setLocked(bool locked)
{
    if(m_data)
        m_data->m_locked = locked;
}

bool CMDAccountProperty_Config::isLogin()
{
    return (m_data ? m_data->m_logined : false);
}
void CMDAccountProperty_Config::setLogined(bool logined)
{
    if(m_data)
        m_data->m_logined = logined;
}

bool CMDAccountProperty_Config::isVisible() const
{
    return (m_data ? m_data->m_visible : false);
}
void CMDAccountProperty_Config::setVisible(bool visible)
{
    if(m_data)
        m_data->m_visible = visible;
}

CMDActionSPList* CMDAccountProperty_Config::spList()
{
    return (m_data ? &m_data->m_spList : NULL);
}
bool CMDAccountProperty_Config::setSPList(const CMDActionSPList* spList)
{
    if(!m_data || !spList)
        return false;

    m_data->m_spList = *spList;
    return true;
}

bool CMDAccountProperty_Config::copy(const CMDAccountProperty_Config* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

/*!
\section 序列化
*/
bool CMDAccountProperty_Config::serialize(QDataStream& stream, bool useCurrentVersion)
{
    return (m_data ? m_data->serialize(stream, useCurrentVersion) : false);
}

void CMDAccountProperty_Config::deserialize(QDataStream& stream)
{
    if(m_data)
        m_data->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CMDAccountProperty_Config::serialize(QJsonObject& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CMDAccountProperty_Config::deserialize(const QJsonObject& json)
{
    if(m_data)
        m_data->deserialize(json);
}
