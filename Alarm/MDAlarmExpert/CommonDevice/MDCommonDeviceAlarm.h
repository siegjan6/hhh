#ifndef MDCOMMONDEVICEALARM_H
#define MDCOMMONDEVICEALARM_H

#include <QString>
#include <QJsonObject>

class CMDCommonDeviceAlarm
{
public:
    CMDCommonDeviceAlarm();
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void read(const QVariant &json);
public:
    QString m_id;
    QString m_description;
    QString m_condition;
};

#endif // MDCOMMONDEVICEALARM_H
