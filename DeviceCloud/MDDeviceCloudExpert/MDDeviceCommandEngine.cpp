#include "MDDeviceCommandEngine.h"
#include "MDDeviceCloudClient.h"

// 仅供 CMDDeviceCommandEngine 使用的命令类
// ---------------------------------------------------------------------------------
class CMDDeviceCommand : public CMDCommand
{
    friend class CMDDeviceCommandEngine;
protected:
    CMDDeviceCommand(const QString& deviceID, qint16 cmdType);
    ~CMDDeviceCommand();
protected:
    QString m_deviceID;
};
CMDDeviceCommand::CMDDeviceCommand(const QString& deviceID, qint16 cmdType)
{
    m_deviceID = deviceID;
    setCmdType(cmdType);
}
CMDDeviceCommand::~CMDDeviceCommand()
{
}

// ---------------------------------------------------------------------------------

CMDDeviceCommandEngine::CMDDeviceCommandEngine()
{
    m_cloudClient = NULL;
    setModeToHandleProcessed(MDModeToHandleProcessedCommand::mode_recycle);
    setComment("DeviceCloudExpert.DeviceEngine");
}

CMDDeviceCommandEngine::~CMDDeviceCommandEngine()
{

}

/*!
\section 初始化
*/
void CMDDeviceCommandEngine::setCloudClient(CMDDeviceCloudClient* cloudClient)
{
    m_cloudClient = cloudClient;
}

/*!
\section 提交命令
*/
bool CMDDeviceCommandEngine::submitDeviceID(const QString& deviceID)
{
    CMDDeviceCommand* newCommand = (CMDDeviceCommand*)fetchRecycleCommand();
    if(newCommand)
    {
        newCommand->setCmdType(1);
        newCommand->m_deviceID = deviceID;
    }
    else
        newCommand = new CMDDeviceCommand(deviceID, 1);

    return submitCommand(newCommand);
}

bool CMDDeviceCommandEngine::submitDeviceIDList()
{
    CMDDeviceCommand* newCommand = (CMDDeviceCommand*)fetchRecycleCommand();
    if(newCommand)
    {
        newCommand->setCmdType(0);
    }
    else
        newCommand = new CMDDeviceCommand("", 0);

    return submitCommand(newCommand);
}

/*!
\section 处理命令
*/
void CMDDeviceCommandEngine::onProcessCommand(CMDCommand* command)
{
    if(!m_cloudClient)
        return;

    if(0 == command->cmdType())
    {
        m_cloudClient->sendCommand_GetDeviceIDList();
    }
    else if(1 == command->cmdType())
    {
        m_cloudClient->sendCommand_GetDeviceInfo(((CMDDeviceCommand*)command)->m_deviceID);
    }
    else{}
}
