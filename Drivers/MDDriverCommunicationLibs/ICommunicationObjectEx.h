/*!

\author zyb

\section 功能

串口或以太通信方式的通用接口类，管理数据的收发

\section 线程特性

此类提供的接口都是线程安全的。

*/
#ifndef ICOMMUNICATIONOBJECTEX_H
#define ICOMMUNICATIONOBJECTEX_H
#include <QByteArray>
#include <QObject>

// TCP服务通信对象接口
class ICommunicationObjectEx
{
public:

    /*!
    \section 发送数据
    buffer:发送数据的存放buff
    count:实际需发送的字节数
    */
    virtual qint16 sendData(quint8 *buffer, qint16 count,const QObject *socketObj)=0;

    /*!
    \section 接收数据
    buffer:接收数据缓冲
    startIndex:接收数据缓冲区的开始索引
    bufferSize:接收数据缓冲区的大小
    socket:socket对象
    return:返回实际收到的字节数
    */
    virtual qint16 getData(quint8 *buffer, qint16 startIndex, qint16 bufferSize,const QObject &socket)=0;
};

#endif // ICOMMUNICATIONOBJECTEX_H
