/*!
\author enli
\section 通信接口基类，管理通信接口参数，数据的收发，主从通信方式的切换，由channel对象创建.根据通信介质类型分别创建窗口或以太通信对象
*/

#ifndef MDCOMMUNICATIONCOMPONENT_H
#define MDCOMMUNICATIONCOMPONENT_H

#include <QString>
#include <QByteArray>
#include <QObject>
#include <QStringList>

#include "IEventCallback.h"
#include "ICommunicationComponent.h"


/*
class CMDCommunicationComponent  : public ICommunicationComponent
{
public:
      CMDCommunicationComponent();
      virtual ~CMDCommunicationComponent();
public:
      //服务器端响应超时
      qint16 getResponesTimeout();
      void setResponesTimeout(qint16 timeOut);

      //通信类型
      CommunicationType getCommType();
      void setCommType(CommunicationType commType);

      //通信状态
      CommunicationStatus getCommStatus();
      void setCommStatus(CommunicationStatus commStatus);

      //通信参数（如串口：COM口，Baud等）
      QString getCommParameter();
      void setCommParameter(QString commParameter);

      // 主从通道切换
      // 通信不正常时:
      //  1.驱动模板多次重试后仍无法恢复通信时触发Switch
      //  2.通信组件主从切换时新通道也无法通信，自动触发Switch一次
      qint16 switchChannel();

      //启动通信连接
      qint16 startConnect();
      //停止通信连接
      qint16 stopConnect();

      //发送数据
      qint16 sendData(quint8 *buffer, qint16 count);
      //发送数据(tcp 服务器端)
      qint16 sendData(quint8 *buffer, qint16 count,const QObject *socketObj);


      //接收数据
      //buffer:接收数据缓冲
      //startIndex:接收数据缓冲区的开始索引
      //bufferSize:接收数据缓冲区的大小
      //返回值：收到的数据个数
      qint16 getData(quint8 *buffer, qint16 startIndex, qint16 bufferSize);

      //接收数据
      //buffer:接收数据缓冲
      //startIndex:接收数据缓冲区的开始索引
      //bufferSize:接收数据缓冲区的大小
      //socket:socket对象
      //返回值：收到的数据个数
      qint16 getData(quint8 *buffer, qint16 startIndex, qint16 bufferSize,const QObject &socket);

      //设置事件通知接口
      qint16 setEventCallback( IEventCallback *onEvent);

private:
    CommunicationStatus m_commStatus;
    CommunicationType m_commType;
    QString m_commParameter;          // 通信参数
    QString m_primaryCommParameter;   // 主通信参数
    QString m_backupCommParameter;    // 备用通信参数
    qint16 m_responesTimeout;            // 服务器端响应超时
    ICommunicationObject *m_pCurrentInstance;//当前通信控件
    IEventCallback *m_pCurrentEventCallBack; //事件回调接口

};*/


class CMDCommunicationComponent  : public ICommunicationComponent
{
    Q_OBJECT
public:
      CMDCommunicationComponent();
      virtual ~CMDCommunicationComponent();
public:
      /*!
      \section 通信类型：串口、以太、USB
      */
      CommunicationType getCommType();
      void setCommType(CommunicationType commType);

      /*!
      \section 当前链路通信状态
      */
      CommunicationStatus getCommStatus();
      void setCommStatus(CommunicationStatus commStatus);

      /*!
      \section 通信参数（如串口：COM口，Baud等）
      */
      QString getCommParameter();
      void setCommParameter(QString commParameter);
      QString getCurCommParam();
      /*!
      \section 主从通道切换
      通信不正常时:
      1.驱动模板重试次数后仍无法恢复通信时触发Switch
      2.通信组件主从切换时新通道也无法通信，自动触发Switch一次
      */
      void switchChannel();
      /*!
      \section 启动通信连接
      \根据通信接口创建不同的通信object
      \msecs：以太连接超时时间
      */
      bool connect(qint32 msecs);

      /*!
      \section 断开通信连接
      \ msecs：以太断开连接超时时间
      */
      bool disConnect(qint32 msecs);

      /*!
      \section 发送数据
      */
      bool writeData(quint8 *buffer, qint16 count,qint32 msecs);

      /*!
      \section 接收数据
      \buffer:接收数据缓冲
      \startIndex:接收数据缓冲区的开始索引
      \bufferSize:接收数据缓冲区的大小
      \返回值：0：接收失败，大于0为收到的数据个数，-1为数据粘包
      */
      qint64 readData(quint8 *buffer, qint16 startIndex, qint16 bufferSize,qint32 msecs);

      bool subscribes(const std::vector<std::string> &entityBrowseNames);

      bool registSubCallback(subCallbackFunc callback, void* contex);

      void asyncWriteVariable(QString address, MDDataType type, QVariant value);

      void asyncWriteVariables(QList<QString> addresses, QList<MDDataType> types, QList<QVariant> values);
private:
    CommunicationStatus m_commStatus;
    CommunicationType m_commType;
    QString m_commParameter;          // 通信参数，以逗号隔开
    QString m_primaryCommParameter;   // 主通信参数
    QString m_backupCommParameter;    // 备用通信参数
    ICommunicationObject *m_pCurrentInstance;//当前通信控件
};
#endif // MDCOMMUNICATIONCOMPONENT_H
