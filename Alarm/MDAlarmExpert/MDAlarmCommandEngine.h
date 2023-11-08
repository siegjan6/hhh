#ifndef MDALARMCOMMANDENGINE_H
#define MDALARMCOMMANDENGINE_H

#include <QVariantList>
#include "MDCommandQueueEngine/MDCommandQueueEngine.h"

class CMDAlarmExpertImp;
class CMDRawEvent;
class CMDRawVariableEvent;
class CMDAlarmCommandEngine : public CMDCommandQueueEngine
{
public:
    explicit CMDAlarmCommandEngine();
    ~CMDAlarmCommandEngine();
public:
    void setAlarmExpert(CMDAlarmExpertImp* alarmExpert);
protected:
    CMDAlarmExpertImp* m_alarmExpert ;// NULL;
public:
    bool submitDeviceAlarm(const QString& deviceID, const QVariantList &deviceAlarmList);
    bool submitRawAlarmEvent(CMDRawVariableEvent* rawEvent);
    bool submitAcknowledge(const QString& alarmID, const QString& comment = "");
    bool submitConfirm(const QString& alarmID, const QString& comment = "");
    bool submitSuppress(const QString& alarmID, const QString& comment = "");
    bool submitShelve(const QString& alarmID, const QString& comment = "");
    bool submitAddComment(const QString& alarmID, const QString& comment = "");
public:
    virtual void onProcessCommand(CMDCommand* command);
};

#endif // MDALARMCOMMANDENGINE_H
