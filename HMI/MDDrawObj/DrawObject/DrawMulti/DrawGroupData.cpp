#include "DrawGroupData.h"
#include "MDDrawObj.h"
//////////////////////////////////////////////////////////////////////////
CDrawGroupData::CDrawGroupData()
{
    m_isVector = true;
}
void CDrawGroupData::operator=(const CDrawGroupData &other)
{
    if (this == &other)
        return;

    m_customPropertys = other.m_customPropertys;
}
void CDrawGroupData::serialize(QJsonObject &json)
{
    QJsonArray array;
    for(int i = 0; i < m_customPropertys.count(); i++)
    {
        QJsonObject property;
        property.insert("Name", m_customPropertys[i].name());
        property.insert("Expression", m_customPropertys[i].expression());
        array.append(property);
    }
    json.insert("CustomPropertys", array);
}
void CDrawGroupData::deserialize(QJsonObject &json)
{
    QJsonArray array = json.value("CustomPropertys").toArray();
    for(int i = 0; i < array.count(); i++)
    {
        QJsonObject property = array.at(i).toObject();
        QString name = property.value("Name").toString();
        QString expression = property.value("Expresssion").toString();
        m_customPropertys.append(CMDCustomProperty(name, expression));
    }
}
//////////////////////////////////////////////////////////////////////////

