#ifndef COMMUNICATIONCOMPONENT_H
#define COMMUNICATIONCOMPONENT_H

#include "communicationlibs_global.h"
#include "IEventCallback.h"
#include <QString>
#include <QByteArray>
#include <QObject>
#include "NSEnums.h"

class COMMUNICATIONLIBSSHARED_EXPORT ICommunicationComponent:public QObject
{
   Q_OBJECT
public:
       int ResponesTimeout;
       virtual int SwitchChannel()=0;
       virtual int Connect()=0;
       virtual int DisConnect()=0;
       virtual int SendData(QByteArray buffer)=0;
       virtual int SendData(QByteArray buffer, const QObject *socketObj, int count)=0;
       virtual int GetData(QByteArray &buffer, int max_length)=0;
       virtual int GetData(QByteArray buffer, int startIndex, int bufferSize,const QObject &mSocket)=0;
       virtual int SetEventCallback( IEventCallback *OnEvent)=0;

       virtual QString GetCommParameter()=0;
       virtual int SetCommParameter(QString value)=0;
       virtual Communication::CommunicationStatus GetCommStatus()=0;

       Communication::CommunicationType CommType;
       Communication::CommunicationStatus CommStatus;
       QString CommParameter;


};

#endif // COMMUNICATIONCOMPONENT_H
