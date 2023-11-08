#ifndef MDCOMMONDEVICE_H
#define MDCOMMONDEVICE_H

#include <QString>
#include <QJsonObject>
#include <QList>
#include "MDCommonDeviceAlarm.h"
#include <QVariant>

class CMDAlarmCondition;
class CMDCommonDevice
{
public:
    CMDCommonDevice();
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void read(const QString& deviceID, const QVariantList &deviceAlarmList);
    void alarmToConditions(QList<CMDAlarmCondition*>& conditionList) const;
    QString helper_BuildAlarmExpression(const QString& deviceID, const QString& rawExpression) const;
public:
    QString m_deviceType;
    QString m_setupTime;
    QString m_longitude;
    QString m_latitude;
    QString m_radius;
    QString m_enable;
    QString m_deviceID;
    QString m_deviceName;
    QString m_version;
public:
    QList<CMDCommonDeviceAlarm> m_alarmList;
};

#endif // MDCOMMONDEVICE_H

