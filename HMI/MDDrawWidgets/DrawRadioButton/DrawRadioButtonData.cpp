#include "DrawRadioButtonData.h"
#include "DrawObjDefine.h"

QMap<QString, QButtonGroup*> CDrawRadioButtonData::m_buttonGroups;
CDrawRadioButtonData::CDrawRadioButtonData()
{
    m_widget = NULL;
    m_event = NULL;

    m_checked = false;
    m_groupId = 0;
    m_text = "单选按钮";
    m_checkValue = qrand() % 100;
    m_checkExpression = NULL;
}

void CDrawRadioButtonData::operator=(const CDrawRadioButtonData &other)
{
    m_text = other.m_text;
    m_checked = other.m_checked;
    m_groupId = other.m_groupId;
    m_checkValue = other.m_checkValue;
}

void CDrawRadioButtonData::serialize(QJsonObject &json)
{
    json.insert("Text",m_text);
    json.insert("Checked",m_checked);
    json.insert("GroupId",m_groupId);
    json.insert("CheckValue",m_checkValue);
}

void CDrawRadioButtonData::deserialize(const QJsonObject &json)
{
    m_text = json.value("Text").toString();
    m_checked = json.value("Checked").toBool();
    m_groupId = json.value("GroupId").toInt();
    m_checkValue = json.value("CheckValue").toDouble();
}

