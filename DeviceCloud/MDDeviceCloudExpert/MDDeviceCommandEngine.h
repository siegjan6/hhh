/*!

\author dGFuZ3p3

\section 功能

设备命令引擎类。

用途：避免在槽函数 CMDDeviceCloudClient::onSubscribeMessage 中发送数据；
为何需要：CMDDeviceCloudClient::onSubscribeMessage 是在
                 socket 的数据接收函数中被调用，此时发送数据可能引起 socket 出现问题
*/

#ifndef MDDEVICECOMMANDENGINE_H
#define MDDEVICECOMMANDENGINE_H

#include "MDCommandQueueEngine/MDCommandQueueEngine.h"

class CMDDeviceCloudClient;
class CMDDeviceCommandEngine  : public CMDCommandQueueEngine
{
public:
    CMDDeviceCommandEngine();
    ~CMDDeviceCommandEngine();
/*!
\section 初始化
*/
public:
    void setCloudClient(CMDDeviceCloudClient* cloudClient);
protected:
    CMDDeviceCloudClient* m_cloudClient ;// NULL;

/*!
\section 提交命令
*/
public:
    bool submitDeviceID(const QString& deviceID);
    bool submitDeviceIDList();

/*!
\section 处理命令
*/
public:
    virtual void onProcessCommand(CMDCommand* command);
};

#endif // MDDEVICECOMMANDENGINE_H
