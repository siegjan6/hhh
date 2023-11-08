#ifndef IEVENTCALLBACK_H
#define IEVENTCALLBACK_H


#include "communicationlibs_global.h"

 namespace CommunicationEventType
 {
  enum  _CommunicationEventType
     {
         ReceivedData = 0,   //收到数据
         Connected = 1,      //通信连接成功
         Disconnected = 2,   //网络收到关闭或发送失败自动关闭时，断开连接通知。上层无需重试
         Connecting = 3,     //正在连接
     };
 }
  class COMMUNICATIONLIBSSHARED_EXPORT IEventCallback
     {
      public:
         virtual void OnEvent(const CommunicationEventType::_CommunicationEventType iEventType)=0;
     };
#endif // IEVENTCALLBACK_H
