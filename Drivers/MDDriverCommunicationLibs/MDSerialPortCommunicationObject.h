/*!
author enli 2015.01.08
\section 串口通信，由CMDCommunicationComponent对象创建，不具备主从切换功能
*/

#ifndef MDSERIALPORT_H
#define MDSERIALPORT_H
#include <QObject>
#include <QMutex>
#include <QMutexLocker>

//#include <QtSerialPort/QSerialPort>
#include <QThread>
#include "ICommunicationComponent.h"
#include "MDExtSerialPort/MDExtSerialPort.h"
/*
class CMDSerialPortCommunicationObject:public QObject,public ICommunicationObject
{
    Q_OBJECT
public:
    CMDSerialPortCommunicationObject();
    ~CMDSerialPortCommunicationObject();
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
    QSerialPort *m_pSerialPort;

    QString     m_commParaeter;
    QString     m_portName;
    QSerialPort::BaudRate m_baudRate;
    QSerialPort::DataBits m_dataBits;
    QSerialPort::StopBits m_stopBits;
    QSerialPort::Parity m_parity;

    qint16 m_responesTimeout;            // 服务器端响应超时

    qint16 m_recvLength;
    quint8 m_recvBuffer[4096];
    quint8 m_recvBufferTemp[4096];
    QMutex m_recvBufferMutex;

    CommunicationStatus m_commStatus;
    IEventCallback *m_pCurrentEventCallBack; //事件回调接口
private slots:
    void dataReceived();
};
*/

class CMDSerialPortCommunicationObject:public ICommunicationObject
{
    Q_OBJECT
public:
    CMDSerialPortCommunicationObject();
    ~CMDSerialPortCommunicationObject();

    /*!
     \section 通信状态
     */
    CommunicationStatus getCommStatus();
    void setCommStatus(CommunicationStatus commStatus);

    /*!
     \section 通信参数,通信参数（如串口：COM口，Baud等）
     */
    QString getCommParameter();
    void setCommParameter(QString commParameter);

    /*!
     \section 连接,同CMDCommunicationComponent接口功能
     */
    //启动通信连接
    bool connect(qint32 msecs);
    //断开通信连接
    bool disConnect(qint32 msecs);
    //发送数据
    bool writeData(quint8 *buffer, qint16 count,qint32 msecs);
    //接收数据
    //buffer:接收数据缓冲
    //startIndex:接收数据缓冲区的开始索引
    //bufferSize:接收数据缓冲区的大小
    //返回值：0：接收失败，大于0为收到的数据个数，-1为数据粘包
    qint64 readData(quint8 *buffer, qint16 startIndex, qint16 bufferSize,qint32 msecs);

    bool subscribes(const std::vector<std::string> &entityBrowseNames);

    bool registSubCallback(subCallbackFunc callback, void* contex);

    void asyncWriteVariable(QString address, MDDataType type, QVariant value);
private:

    //QSerialPort *m_pSerialPort;
    CMDExtSerialPort *m_pSerialPort;
    QString     m_commParaeter;
    QString     m_portName;
    /*
    QSerialPort::BaudRate m_baudRate;
    QSerialPort::DataBits m_dataBits;
    QSerialPort::StopBits m_stopBits;
    QSerialPort::Parity m_parity;
    */
    //qtexSerial
    FlowType m_qtexFlow;
    BaudRateType m_qtexBaud;
    ParityType m_qtexParity;
    StopBitsType m_qtexStopBit;
    DataBitsType m_qtexDataBit;

    qint16 m_recvLength;
    quint8 m_recvBuffer[4096];
    QMutex m_recvBufferMutex;

    CommunicationStatus m_commStatus;

    subCallbackFunc m_pSubCallback;
};
#endif // MDSERIALPORT_H
