#include "MDCommonDeviceAlarm.h"
#include <QVariant>

CMDCommonDeviceAlarm::CMDCommonDeviceAlarm()
{
}

void CMDCommonDeviceAlarm::read(const QJsonObject &json)
{
    m_id = json["@AlarmID"].toString();
    m_description = json["@AlarmDescription"].toString();
    m_condition =json["@AlarmCondition"].toString();
}

void CMDCommonDeviceAlarm::write(QJsonObject &json) const
{
    json["@AlarmID"] = m_id;
    json["@AlarmDescription"] = m_description;
    json["@AlarmCondition"] = m_condition;
}

void CMDCommonDeviceAlarm::read(const QVariant &json)
{
    QVariant prop;
    prop = json.toMap()["@AlarmID"];
    m_id = prop.toString();
    prop = json.toMap()["@AlarmDescription"];
    m_description = prop.toString();
    prop = json.toMap()["@AlarmCondition"];
    m_condition = prop.toString();
}
