#include "MDDriverInformation.h"

CMDDriverInformation::CMDDriverInformation()
{

}

CMDDriverInformation::~CMDDriverInformation()
{

}

QString CMDDriverInformation::id()
{
    return m_id;
}

void CMDDriverInformation::setID(QString id)
{
    m_id = id;
}
//名称
QString CMDDriverInformation::name()
{
    return m_name;
}

void CMDDriverInformation::setName(QString name)
{
    m_name = name;
}
QString CMDDriverInformation::description()
{
    return m_description;
}

void CMDDriverInformation::setDescription(QString description)
{
    m_description = description;
}
