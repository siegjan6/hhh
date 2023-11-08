#include <QJsonArray>
#include "DrawComboBoxData.h"

CDrawComboBoxData::CDrawComboBoxData()
{
    m_comboBox = NULL;
    m_event = NULL;
    m_layoutDirection = false;
    m_itemStyle = 1;
    m_currentIndex = 0;
    m_sort = false;
}

void CDrawComboBoxData::operator =(const CDrawComboBoxData &other)
{
    m_itemAdd = other.m_itemAdd;
    m_itemStyle = other.m_itemStyle;
    m_layoutDirection = other.m_layoutDirection;
    m_sort = other.m_sort;
}

void CDrawComboBoxData::serialize(QJsonObject &json)
{
    json.insert("ItemStyle",m_itemStyle);
    json.insert("LayoutDirection",m_layoutDirection);
    json.insert("Sort",m_sort);
    QJsonArray array;
    for(int i = 0; i < m_itemAdd.count(); i++)
    {
        array.append(m_itemAdd.at(i));
    }
    json.insert("ItemAdd",array);
}

void CDrawComboBoxData::deserialize(const QJsonObject &json)
{
    m_itemStyle = json.value("ItemStyle").toInt();
    m_layoutDirection = json.value("LayoutDirection").toInt();
    m_sort = json.value("Sort").toBool();
    m_itemAdd.clear();
    QJsonArray array = json.value("ItemAdd").toArray();
    for(int i = 0; i < array.count(); i++)
    {
        m_itemAdd.append(array.at(i).toString());
    }
}
