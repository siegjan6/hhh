#include "MDAlarmCommandEngine.h"
#include "MDAlarmExpertImp.h"
#include "CommonDevice/MDCommonDevice.h"

// 仅供 CMDAlarmCommandEngine 使用的命令类
// ---------------------------------------------------------------------------------
class CMDRawEvent;
class CMDAlarmCommand : public CMDCommand
{
    friend class CMDAlarmCommandEngine;
protected:
    CMDAlarmCommand(void* para, qint16 type);
    ~CMDAlarmCommand();
protected:
    void deletePara();
};
CMDAlarmCommand::CMDAlarmCommand(void* para, qint16 type)
{
    setCmdPara(para);
    setCmdType(type);
}
CMDAlarmCommand::~CMDAlarmCommand()
{
    deletePara();
}
void CMDAlarmCommand::deletePara()
{
    if(0 == cmdType() || 1 == cmdType()|| 3 == cmdType()|| 4 == cmdType()|| 5 == cmdType()|| 6 == cmdType())
    {
        CMDRawEvent* para = (CMDRawEvent*)cmdPara();
        if(para)
            delete para;
    }
    else if(2 == cmdType())
    {
        CMDCommonDevice* para = (CMDCommonDevice*)cmdPara();
        if(para)
            delete para;
    }
}

// ---------------------------------------------------------------------------------

CMDAlarmCommandEngine::CMDAlarmCommandEngine()
{
    m_alarmExpert = NULL;
    setModeToHandleProcessed(MDModeToHandleProcessedCommand::mode_recycle);
    setComment("Alarm.AlarmEngine");
}

CMDAlarmCommandEngine::~CMDAlarmCommandEngine()
{

}

void CMDAlarmCommandEngine::setAlarmExpert(CMDAlarmExpertImp* alarmExpert)
{
    m_alarmExpert = alarmExpert;
}

void CMDAlarmCommandEngine::onProcessCommand(CMDCommand* command)
{
    if(0 == command->cmdType())
    {
        // raw
        CMDRawVariableEvent* para = (CMDRawVariableEvent*)command->cmdPara();
        m_alarmExpert->processRawAlarmEvent(para);
    }
    else if(1 == command->cmdType())
    {
        // confirm
        CMDRawEvent* para = (CMDRawEvent*)command->cmdPara();
        QString alarmID = para->rawObjName();
        QString comment = para->comment();
        m_alarmExpert->confirmDirect(alarmID, comment);
    }
    else if(2 == command->cmdType())
    {
        // cloud
        CMDCommonDevice* device = (CMDCommonDevice*)command->cmdPara();
        m_alarmExpert->buildConditionsFromCloud(device);
    }
    else if(3 == command->cmdType())
    {
        // ack
        CMDRawEvent* para = (CMDRawEvent*)command->cmdPara();
        QString alarmID = para->rawObjName();
        QString comment = para->comment();
        m_alarmExpert->acknowledgeDirect(alarmID, comment);
    }
    else if(4 == command->cmdType())
    {
        // suppress
        CMDRawEvent* para = (CMDRawEvent*)command->cmdPara();
        QString alarmID = para->rawObjName();
        QString comment = para->comment();
        m_alarmExpert->suppressDirect(alarmID, comment);
    }
    else if(5 == command->cmdType())
    {
        // shelve
        CMDRawEvent* para = (CMDRawEvent*)command->cmdPara();
        QString alarmID = para->rawObjName();
        QString comment = para->comment();
        m_alarmExpert->shelveDirect(alarmID, comment);
    }
    else if(6 == command->cmdType())
    {
        // add comment
        CMDRawEvent* para = (CMDRawEvent*)command->cmdPara();
        QString alarmID = para->rawObjName();
        QString comment = para->comment();
        m_alarmExpert->addCommentDirect(alarmID, comment);
    }
    else;
}

bool CMDAlarmCommandEngine::submitRawAlarmEvent(CMDRawVariableEvent* rawEvent)
{
    CMDAlarmCommand* newCommand = (CMDAlarmCommand*)fetchRecycleCommand();
    if(newCommand)
    {
        newCommand->deletePara();

        newCommand->setCmdPara(rawEvent);
        newCommand->setCmdType(0);
    }
    else
        newCommand = new CMDAlarmCommand(rawEvent, 0);

    return submitCommand(newCommand);
}

bool CMDAlarmCommandEngine::submitAcknowledge(const QString& alarmID, const QString& comment/* = ""*/)
{
    CMDRawEvent* para = new CMDRawEvent(alarmID, MDEventType::null_event);

    CMDAlarmCommand* newCommand = (CMDAlarmCommand*)fetchRecycleCommand();
    if(newCommand)
    {
        newCommand->deletePara();

        para->setComment(comment);
        newCommand->setCmdPara(para);
        newCommand->setCmdType(3);

    }
    else
        newCommand = new CMDAlarmCommand(para, 1);

    return submitCommand(newCommand);
}

bool CMDAlarmCommandEngine::submitConfirm(const QString& alarmID, const QString& comment/* = ""*/)
{
    CMDRawEvent* para = new CMDRawEvent(alarmID, MDEventType::null_event);

    CMDAlarmCommand* newCommand = (CMDAlarmCommand*)fetchRecycleCommand();
    if(newCommand)
    {
        newCommand->deletePara();

        para->setComment(comment);
        newCommand->setCmdPara(para);
        newCommand->setCmdType(1);
    }
    else
        newCommand = new CMDAlarmCommand(para, 1);

    return submitCommand(newCommand);
}

bool CMDAlarmCommandEngine::submitDeviceAlarm(const QString& deviceID, const QVariantList &deviceAlarmList)
{
    CMDCommonDevice* deviceAlarm = new CMDCommonDevice;
    deviceAlarm->read(deviceID, deviceAlarmList);

    CMDAlarmCommand* newCommand = (CMDAlarmCommand*)fetchRecycleCommand();
    if(newCommand)
    {
        newCommand->deletePara();

        newCommand->setCmdPara(deviceAlarm);
        newCommand->setCmdType(2);
    }
    else
        newCommand = new CMDAlarmCommand(deviceAlarm, 2);

    return submitCommand(newCommand);
}

bool CMDAlarmCommandEngine::submitSuppress(const QString& alarmID, const QString& comment/* = ""*/)
{
    CMDRawEvent* para = new CMDRawEvent(alarmID, MDEventType::null_event);

    CMDAlarmCommand* newCommand = (CMDAlarmCommand*)fetchRecycleCommand();
    if(newCommand)
    {
        newCommand->deletePara();

        para->setComment(comment);
        newCommand->setCmdPara(para);
        newCommand->setCmdType(4);
    }
    else
        newCommand = new CMDAlarmCommand(para, 4);

    return submitCommand(newCommand);
}

bool CMDAlarmCommandEngine::submitShelve(const QString& alarmID, const QString& comment/* = ""*/)
{
    CMDRawEvent* para = new CMDRawEvent(alarmID, MDEventType::null_event);

    CMDAlarmCommand* newCommand = (CMDAlarmCommand*)fetchRecycleCommand();
    if(newCommand)
    {
        newCommand->deletePara();

        para->setComment(comment);
        newCommand->setCmdPara(para);
        newCommand->setCmdType(5);
    }
    else
        newCommand = new CMDAlarmCommand(para, 5);

    return submitCommand(newCommand);
}

bool CMDAlarmCommandEngine::submitAddComment(const QString& alarmID, const QString& comment/* = ""*/)
{
    CMDRawEvent* para = new CMDRawEvent(alarmID, MDEventType::null_event);

    CMDAlarmCommand* newCommand = (CMDAlarmCommand*)fetchRecycleCommand();
    if(newCommand)
    {
        newCommand->deletePara();

        para->setComment(comment);
        newCommand->setCmdPara(para);
        newCommand->setCmdType(6);
    }
    else
        newCommand = new CMDAlarmCommand(para, 6);

    return submitCommand(newCommand);
}
