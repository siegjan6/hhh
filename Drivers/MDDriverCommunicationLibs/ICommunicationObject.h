#ifndef ICOMMUNICATIONOBJECT_H
#define ICOMMUNICATIONOBJECT_H
#include "QString"
#include "IEventCallback.h"
#include "ICommunicationComponent.h"

/*
class ICommunicationObject
{
public:
    //服务器端响应超时
    virtual qint16 getResponesTimeout()=0;
    virtual void setResponesTimeout(qint16 timeOut)=0;
    //通信状态
    virtual CommunicationStatus getCommStatus()=0;
    virtual void setCommStatus(CommunicationStatus commStatus)=0;

    //通信参数（如串口：COM口，Baud等）
    virtual QString getCommParameter()=0;
    virtual void setCommParameter(QString commParameter)=0;

    //启动通信连接
    virtual qint16 startConnect()=0;
    //停止通信连接
    virtual qint16 stopConnect()=0;

    //发送数据
    virtual qint16 sendData(quint8 *buffer, qint16 count)=0;


    //接收数据
    //buffer:接收数据缓冲
    //startIndex:接收数据缓冲区的开始索引
    //bufferSize:接收数据缓冲区的大小
    //返回值：收到的数据个数
    virtual qint16 getData(quint8 *buffer, qint16 startIndex, qint16 bufferSize)=0;

    //设置事件通知接口
    virtual qint16 setEventCallback( IEventCallback *onEvent)=0;

};*/
#endif // ICOMMUNICATIONOBJECT_H
