#ifndef NSCOMMUNICATIONCOMPONENT_H
#define NSCOMMUNICATIONCOMPONENT_H
#include "communicationlibs_global.h"
#include "ICommunicationComponent.h"
#include "ICommunicationObject.h"
#include "ICommunicationObjectEx.h"
#include <QString>
#include <QByteArray>
#include <QObject>
#include <QStringList>
#include "NSTcp.h"

class COMMUNICATIONLIBSSHARED_EXPORT NSCommunicationComponent : public ICommunicationComponent
  {
public:
      NSCommunicationComponent();
     ~NSCommunicationComponent();

      QString GetCommParameter();
      int SetCommParameter(QString value);

      Communication::CommunicationStatus GetCommStatus();

      int SetEventCallback(IEventCallback *OnEvent);
      int SwitchChannel();
      int Connect();
      int DisConnect();
      int SendData(QByteArray buffer);
      int SendData(QByteArray buffer,  const QObject *socketObj, int count);
      int GetData( QByteArray &buffer, int max_length);
      int GetData(QByteArray buffer, int startIndex, int bufferSize,const QObject &mSocket);
public:
     // Communication::CommunicationType CommType;
      QString _PrimaryCommParameter; /// 主通信参数
      QString _BackupCommParameter;  /// 备用通信参数
      int _ResponesTimeout;         //服务器端响应超时
private:
       ICommunicationObject *_CurrentInstance;//当前通信控件
       IEventCallback *_CurrentEventCallBack;


  };
#endif // NSCOMMUNICATIONCOMPONENT_H
