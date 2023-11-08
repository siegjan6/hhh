#include <QJsonArray>
#include "MDHMIActionManager.h"
#include "MDProjectExpert/IMDProjectExpert.h"
//////////////////////////////////////////////////////////////////////////
CMDHMIActionManager::CMDHMIActionManager()
{
    m_container = NULL;
}
void CMDHMIActionManager::serialize(QJsonObject &json)
{
    QJsonArray array;
    foreach(QString s, m_list)
        array.append(s);
    json.insert("List", array);
}
void CMDHMIActionManager::deserialize(const QJsonObject &json)
{
    QJsonArray array = json.value("List").toArray();
    foreach (QJsonValue v, array)
        m_list.append(v.toString());
}
void CMDHMIActionManager::operator=(const CMDHMIActionManager &other)
{
    if (this == &other)
        return;

    m_list = other.m_list;
}
void CMDHMIActionManager::execute()
{
    for(int i = 0; i < m_list.count(); i++)
        m_container->framework()->projectExpert()->executeAction(m_list.at(i));
}
//////////////////////////////////////////////////////////////////////////
