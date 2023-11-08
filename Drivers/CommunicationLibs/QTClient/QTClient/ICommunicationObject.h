#ifndef ICOMMUNICATIONOBJECT_H
#define ICOMMUNICATIONOBJECT_H
#include "communicationlibs_global.h"
#include "QString"
#include "IEventCallback.h"
#include "ICommunicationComponent.h"


class COMMUNICATIONLIBSSHARED_EXPORT ICommunicationObject
   {

public:       
      virtual  bool SetCommParameter(QString value)=0;
      virtual  QString GetCommParameter()=0;

       virtual int Connect()=0;
       virtual int DisConnect()=0;
       virtual int SendData(QByteArray buffer)=0;
       virtual int GetData(QByteArray &buffer, int max_length)=0;

       Communication::CommunicationStatus CommStatus;
       IEventCallback *EventCallBack;

   };
#endif // ICOMMUNICATIONOBJECT_H
