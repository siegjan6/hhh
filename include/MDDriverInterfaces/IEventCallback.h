/*!
\author zyb 2015.01.08
\section 通信事件通知，目前未使用
*/
#ifndef IEVENTCALLBACK_H
#define IEVENTCALLBACK_H

enum class CommunicationEventType
{
    ReceivedData = 0,   //收到数据
    Connected = 1,      //通信连接成功
    DisConnected = 2,   //网络收到关闭或发送失败自动关闭时，断开连接通知。上层无需重试
    Connecting = 3,     //正在连接
};
class IEventCallback
{
public:
    virtual ~IEventCallback(){}
public:
    virtual void onEvent(CommunicationEventType eventType)=0;

};
#endif // IEVENTCALLBACK_H
