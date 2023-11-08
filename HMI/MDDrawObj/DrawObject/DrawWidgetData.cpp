#include "DrawWidgetData.h"
//////////////////////////////////////////////////////////////////////////
CDrawWidgetData::CDrawWidgetData()
{
    m_widget = NULL;
    m_event = NULL;
}

void CDrawWidgetData::operator=(const CDrawWidgetData &other)
{
    if (this == &other)
        return;
}
void CDrawWidgetData::serialize(QJsonObject &json)
{

}
void CDrawWidgetData::deserialize(const QJsonObject &json)
{

}
//////////////////////////////////////////////////////////////////////////

