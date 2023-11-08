#include "DrawObjData.h"
//////////////////////////////////////////////////////////////////////////
CDrawObjData::CDrawObjData()
{
    m_parent = NULL;
    m_multiParent = NULL;

    m_visible = true;
    m_locked = false;
    m_layer = 0;

    m_property = NULL;
    m_supportGroupEvent = false;
    m_showSelectedFrame = false;

    m_supportHidedEvent = false;
}

void CDrawObjData::operator=(const CDrawObjData &other)
{
    if (this == &other)
        return;

    m_newRect = other.m_newRect;
    m_bound = other.m_bound;
    m_data = other.m_data;
    m_dataBk = other.m_dataBk;
    m_name = other.m_name;
    m_visible = other.m_visible;
    m_locked = other.m_locked;
    m_layer = other.m_layer;
    m_supportGroupEvent = other.m_supportGroupEvent;
    m_showSelectedFrame = other.m_showSelectedFrame;
    m_storageImagePath = other.m_storageImagePath;
    m_toolTip = other.m_toolTip;
    m_supportHidedEvent = other.m_supportHidedEvent;
}
void CDrawObjData::serialize(QJsonObject &json)
{
    json.insert("Visible", m_visible);
    json.insert("Locked", m_locked);
    json.insert("Layer", m_layer);
    json.insert("Name", m_name);
    json.insert("SupportGroupEvent", m_supportGroupEvent);
    json.insert("ShowSelectedFrame", m_showSelectedFrame);
    json.insert("SupportHidedEvent", m_supportHidedEvent);
    json.insert("ToolTip", m_toolTip);
    QJsonObject jsData;
    m_data.serialize(jsData);
    json.insert("Data", jsData);
    QJsonObject jsExpManager;
    m_expManager.serialize(jsExpManager);
    json.insert("ExpManager", jsExpManager);
}
void CDrawObjData::deserialize(const QJsonObject &json)
{
    m_visible = json.value("Visible").toBool();
    m_locked = json.value("Locked").toBool();
    m_layer = json.value("Layer").toInt();
    m_name = json.value("Name").toString();
    m_supportGroupEvent = json.value("SupportGroupEvent").toBool();
    m_showSelectedFrame = json.value("ShowSelectedFrame").toBool();
    m_supportHidedEvent = json.value("SupportHidedEvent").toBool();
    m_toolTip = json.value("ToolTip").toString();

    m_data.deserialize(json.value("Data").toObject());
    m_expManager.deserialize(json.value("ExpManager").toObject());
}
//////////////////////////////////////////////////////////////////////////
