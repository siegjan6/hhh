#ifndef MDCOMMONDEVICEMGR_H
#define MDCOMMONDEVICEMGR_H

#include <QList>
#include "MDCommonDevice.h"

class CMDCommonDeviceMgr
{
public:
    CMDCommonDeviceMgr();
    bool load();
    void save();
public:
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
    void deviceAlarmToConditions(QList<CMDAlarmCondition*>& conditionList);
public:
    QList<CMDCommonDevice> m_devices;
};

#endif // MDCOMMONDEVICEMGR_H
