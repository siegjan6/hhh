#include <QJsonArray>
#include "DrawListData.h"

CDrawListData::CDrawListData()
{
    m_widget = NULL;
    m_event = NULL;

    m_hasScrollBar = true;
    m_borderStyle = QFrame::NoFrame;
    m_selectionMode = QAbstractItemView::SingleSelection;
    m_layoutDirection = Qt::LayoutDirectionAuto;
    m_sorted = 0;
}

void CDrawListData::operator =(const CDrawListData &other)
{
    m_hasScrollBar = other.m_hasScrollBar;
    m_itemAdd = other.m_itemAdd;
    m_borderStyle = other.m_borderStyle;
    m_selectionMode = other.m_selectionMode;
    m_layoutDirection = other.m_layoutDirection;
    m_sorted = other.m_sorted;
}

void CDrawListData::serialize(QJsonObject &json)
{
    json.insert("HasScrollBar",m_hasScrollBar);
    json.insert("BorderStyle",m_borderStyle);
    json.insert("SelectionMode",m_selectionMode);
    json.insert("LayoutDirection",m_layoutDirection);
    json.insert("Sorted",m_sorted);
    QJsonArray array;
    for(int i = 0; i < m_itemAdd.count(); i++)
    {
        array.append(m_itemAdd.at(i));
    }
    json.insert("ItemAdd",array);
}

void CDrawListData::deserialize(const QJsonObject &json)
{
    m_hasScrollBar = json.value("HasScrollBar").toBool();
    m_borderStyle = (QFrame::Shape)json.value("BorderStyle").toInt();
    m_selectionMode = (QAbstractItemView::SelectionMode)json.value("SelectionMode").toInt();
    m_layoutDirection = (Qt::LayoutDirection)json.value("LayoutDirection").toInt();
    m_sorted = json.value("Sorted").toInt();
    QJsonArray array = json.value("ItemAdd").toArray();
    m_itemAdd.clear();
    for(int i = 0; i < array.count(); i++)
    {
        m_itemAdd.append(array.at(i).toString());
    }
}
