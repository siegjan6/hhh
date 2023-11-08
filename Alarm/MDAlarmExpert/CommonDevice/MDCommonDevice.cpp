#include "MDCommonDevice.h"
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include "MDAlarmCondition.h"
#include "MDExpression.h"

CMDCommonDevice::CMDCommonDevice()
{
}

void CMDCommonDevice::read(const QJsonObject &json)
{
    m_deviceType = json["@DeviceType"].toString();
    m_setupTime = json["@SetupTime"].toString();
    m_longitude = json["@Longitude"].toString();
    m_latitude = json["@Latitude"].toString();
    m_radius = json["@Radius"].toString();
    m_enable = json["@Enable"].toString();
    m_deviceID = json["@DeviceID"].toString();
    m_deviceName = json["@DeviceName"].toString();
    m_version = json["@Version"].toString();

    m_alarmList.clear();
    QJsonArray alarmArray = json["MDDevicesAlarmInfo"].toArray();
    for (int index = 0; index < alarmArray.size(); ++index)
    {
        QJsonObject jsAlarm = alarmArray[index].toObject();
        CMDCommonDeviceAlarm deviceAlarm;
        deviceAlarm.read(jsAlarm);
        m_alarmList.append(deviceAlarm);
    }
}

void CMDCommonDevice::write(QJsonObject &json) const
{
    json["@DeviceType"] = m_deviceType;
    json["@SetupTime"] = m_setupTime;
    json["@Longitude"] = m_longitude;
    json["@Latitude"] = m_latitude;
    json["@Radius"] = m_radius;
    json["@Enable"] = m_enable;
    json["@DeviceID"] = m_deviceID;
    json["@DeviceName"] = m_deviceName;
    json["@Version"] = m_version;

    QJsonArray alarmArray;
    foreach (const CMDCommonDeviceAlarm deviceAlarm, m_alarmList)
    {
        QJsonObject jsAlarm;
        deviceAlarm.write(jsAlarm);
        alarmArray.append(jsAlarm);
    }
    json["MDDevicesAlarmInfo"] = alarmArray;
}

void CMDCommonDevice::read(const QString& deviceID, const QVariantList &deviceAlarmList)
{
    m_deviceID = deviceID;
    foreach (const QVariant alarm, deviceAlarmList)
    {
        CMDCommonDeviceAlarm deviceAlarm;
        deviceAlarm.read(alarm);
        m_alarmList.append(deviceAlarm);
    }
}

void CMDCommonDevice::alarmToConditions(QList<CMDAlarmCondition*>& conditionList) const
{
    foreach (const CMDCommonDeviceAlarm deviceAlarm, m_alarmList)
    {
        CMDAlarmCondition* ac = new CMDAlarmCondition;
        ac->setName(m_deviceID + "." + deviceAlarm.m_id);
        ac->setComment(deviceAlarm.m_description);
        ac->setExpression(helper_BuildAlarmExpression(m_deviceID, deviceAlarm.m_condition));

        conditionList.append(ac);
    }
}

QString CMDCommonDevice::helper_BuildAlarmExpression(const QString& deviceID, const QString& rawExpression) const
{
    QString alarmExpression = rawExpression;

    CMDExpression exp(rawExpression);
    exp.compile(rawExpression);
    QList<QString> refVarList = exp.referencedVarList();
    foreach (const QString varName, refVarList)
    {
        QString alarmVarName = deviceID + "." + varName;
        alarmExpression = alarmExpression.replace(varName, alarmVarName, Qt::CaseInsensitive);
    }

    return alarmExpression;
}
