#include "MDPowerGroup.h"
#include "MDPowerGroupData.h"

const QStringList EMPTY_STRINGLIST;

CMDPowerGroup::CMDPowerGroup()
{
    m_data = new CMDPowerGroupData;
}

CMDPowerGroup::~CMDPowerGroup()
{
    if(m_data)
        delete m_data;
}

CMDPowerGroup::CMDPowerGroup(const CMDPowerGroup& obj)
{
    m_data = new CMDPowerGroupData();

    copy(&obj);
}

CMDPowerGroup& CMDPowerGroup::operator = (const CMDPowerGroup& obj)
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
QString CMDPowerGroup::name() const
{
    return (m_data ? m_data->m_name : "");
}
void CMDPowerGroup::setName(const QString& name)
{
    if(m_data)
        m_data->m_name = name;
}

QString CMDPowerGroup::comment() const
{
    return (m_data ? m_data->m_comment : "");
}
void CMDPowerGroup::setComment(const QString& comment)
{
    if(m_data)
        m_data->m_comment = comment;
}

int CMDPowerGroup::level() const
{
    return (m_data ? m_data->m_level : 0);
}
void CMDPowerGroup::setLevel(const int& level)
{
    if(m_data)
        m_data->m_level = level;
}

bool CMDPowerGroup::isEnable() const
{
    return (m_data ? m_data->m_enable : false);
}
void CMDPowerGroup::setEnable(bool enable)
{
    if(m_data)
        m_data->m_enable = enable;
}

bool CMDPowerGroup::isVisible() const
{
    return (m_data ? m_data->m_visible : false);
}
void CMDPowerGroup::setVisible(bool visible)
{
    if(m_data)
        m_data->m_visible = visible;
}

const QStringList& CMDPowerGroup::accountList() const
{
    return (m_data ? m_data->m_accountList : EMPTY_STRINGLIST);
}
void CMDPowerGroup::setAccountList(const QStringList& accountList)
{
    if(m_data)
    {
        m_data->m_accountList.clear();
        m_data->m_accountList.append(accountList);
    }
}

CMDActionSPList* CMDPowerGroup::spList()
{
    return (m_data ? &m_data->m_spList : NULL);
}
bool CMDPowerGroup::setSPList(const CMDActionSPList* spList)
{
    if(!m_data || !spList)
        return false;

    m_data->m_spList = *spList;
    return true;
}

bool CMDPowerGroup::copy(const CMDPowerGroup* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

/*!
\section 序列化
*/
bool CMDPowerGroup::serialize(QDataStream& stream, bool useCurrentVersion)
{
    return (m_data ? m_data->serialize(stream, useCurrentVersion) : false);
}

void CMDPowerGroup::deserialize(QDataStream& stream)
{
    if(m_data)
        m_data->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CMDPowerGroup::serialize(QJsonObject& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CMDPowerGroup::deserialize(const QJsonObject& json)
{
    if(m_data)
        m_data->deserialize(json);
}
