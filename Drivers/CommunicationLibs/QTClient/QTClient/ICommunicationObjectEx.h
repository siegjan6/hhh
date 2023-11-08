#ifndef ICOMMUNICATIONOBJECTEX_H
#define ICOMMUNICATIONOBJECTEX_H
#include "communicationlibs_global.h"
#include <QByteArray>
#include <QObject>

   /// TCP服务通信对象接口
class COMMUNICATIONLIBSSHARED_EXPORT ICommunicationObjectEx
   {

public:
      int ResponesTimeout;
      virtual int SendData(QByteArray buffer,  const QObject *socketObj, int count)=0;
      virtual int GetData(QByteArray buffer, int startIndex, int bufferSize,const QObject &mSocket)=0;

   };

#endif // ICOMMUNICATIONOBJECTEX_H
