/*!
\author enli
\section 以太通信，由CMDCommunicationComponent对象创建，目前支持TCP客户端
*/
#ifndef MDTCPCOMMUNICATIONOBJECT_H
#define MDTCPCOMMUNICATIONOBJECT_H


#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QTcpSocket>
#include "ICommunicationComponent.h"
/*
class CMDTCPCommunicationObject:public QObject,public ICommunicationObject
{
    Q_OBJECT
public:
    CMDTCPCommunicationObject();
    ~CMDTCPCommunicationObject();

    //服务器端响应超时
    qint16 getResponesTimeout();
    void setResponesTimeout(qint16 timeOut);

    //通信状态
    CommunicationStatus getCommStatus();
    void setCommStatus(CommunicationStatus commStatus);

    //通信参数（如串口：COM口，Baud等）
    QString getCommParameter();
    void setCommParameter(QString commParameter);

    //启动通信连接
    qint16 startConnect();
    //停止通信连接
    qint16 stopConnect();

    //发送数据
    qint16 sendData(quint8 *buffer, qint16 count);


    //接收数据
    //buffer:接收数据缓冲
    //startIndex:接收数据缓冲区的开始索引
    //bufferSize:接收数据缓冲区的大小
    //返回值：收到的数据个数
    qint16 getData(quint8 *buffer, qint16 startIndex, qint16 bufferSize);

    //设置事件通知接口
    qint16 setEventCallback( IEventCallback *onEvent);

private:
   void onEvent(const CommunicationEventType eventType);
private:
    QTcpSocket *m_pTcpSocket;

    QString    m_commParaeter;
    QString    m_localIP;
    qint16     m_localPort;
    QString    m_serverIP;
    qint16     m_serverPort;
    QString    m_mode;

    qint16 m_responesTimeout;            // 服务器端响应超时

    qint16 m_recvLength;
    quint8 m_recvBuffer[65535];
    QMutex m_recvBufferMutex;

    CommunicationStatus m_commStatus;
    IEventCallback *m_pCurrentEventCallBack; //事件回调接口
public slots:
    //void slotConnected();
    void slotDisconnected();
    void dataReceived();
};*/

class CMDTCPCommunicationObject:public ICommunicationObject//public QObject,public ICommunicationObject//,public ICommunicationObjectEx
{
   // Q_OBJECT
public:
    CMDTCPCommunicationObject();
    ~CMDTCPCommunicationObject();
    //通信状态
    CommunicationStatus getCommStatus();
    void setCommStatus(CommunicationStatus commStatus);

    //通信参数（如IP,Port等）
    QString getCommParameter();
    void setCommParameter(QString commParameter);


    /*!
    \section 启动通信连接
    \msecs:连接超时时间
    */
    bool connect(qint32 msecs);

    /*!
    \section 断开通信连接
    \msecs:断开超时时间
    */
    bool disConnect(qint32 msecs);
    //发送数据
    bool writeData(quint8 *buffer, qint16 count,qint32 msecs);
    /*!
    \section 接收数据
    \buffer:接收数据缓冲
    \startIndex:接收数据缓冲区的开始索引
    \bufferSize:接收数据缓冲区的大小
    \返回值：0：接收失败，大于0为收到的数据个数，-1为数据粘包    */

    qint64 readData(quint8 *buffer, qint16 startIndex, qint16 bufferSize,qint32 msecs);

    bool subscribes(const std::vector<std::string> &entityBrowseNames);

    bool registSubCallback(subCallbackFunc callback, void* contex);

    void asyncWriteVariable(QString address, MDDataType type, QVariant value);
private:
    QTcpSocket *m_pTcpSocket;

    QString    m_commParaeter;
    QString    m_localIP;
    qint16     m_localPort;
    QString    m_serverIP;
    qint16     m_serverPort;
    QString    m_mode;

    qint16 m_recvLength;
    quint8 m_recvBuffer[65535];
    QMutex m_recvBufferMutex;

    CommunicationStatus m_commStatus;

    subCallbackFunc m_pSubCallback;
};
#endif // MDTCPCOMMUNICATIONOBJECT_H
