/*!
\author zyb 2015.01.08
\section 通信基础接口类+管理类（主从切换+根据类型创建串口或以太通信）
*/
#ifndef COMMUNICATIONCOMPONENT_H
#define COMMUNICATIONCOMPONENT_H

#include <QtCore/qglobal.h>
#include <QString>
#include <QObject>
#include <QList>
#include "IEventCallback.h"
#include "../interface/Common/IMDDataType.h"

enum class CommunicationStatus
{
    DisConnected = 0,//断开连接
    DisConnecting = 1,//正在断开连接
    Connected = 2,   //已经建立连接
    Connecting = 3  //正在连接
};

enum class CommunicationType
{
    Serial = 0,     //串口通信
    TCP = 1,        //TCP通信
    UDP = 2,        //UDP通信
    OPCUA = 3,      //OPCUA通信
    USB = 4,	    //USB
    Board = 5,      //板卡

    Other = 100,    //其它
};
/*
class  ICommunicationComponent
{
public:
    virtual ~ICommunicationComponent(){}
public:
    //服务器端响应超时
    virtual qint16 getResponesTimeout()=0;
    virtual void setResponesTimeout(qint16 timeOut)=0;

    //通信类型
    virtual CommunicationType getCommType()=0;
    virtual void setCommType(CommunicationType commType)=0;

    //通信状态
    virtual CommunicationStatus getCommStatus()=0;
    virtual void setCommStatus(CommunicationStatus commStatus)=0;

    //通信参数（如串口：COM口，Baud等）
    virtual QString getCommParameter()=0;
    virtual void setCommParameter(QString commParameter)=0;

    // 主从通道切换
    // 通信不正常时:
    //  1.驱动模板多次重试后仍无法恢复通信时触发Switch
    //  2.通信组件主从切换时新通道也无法通信，自动触发Switch一次
    virtual qint16 switchChannel()=0;

    //启动通信连接
    virtual qint16 startConnect()=0;
    //停止通信连接
    virtual qint16 stopConnect()=0;

    //发送数据
    virtual qint16 sendData(quint8 *buffer, qint16 count)=0;
    //发送数据(tcp 服务器端)
    virtual qint16 sendData(quint8 *buffer, qint16 count,const QObject *socketObj)=0;


    //接收数据
    //buffer:接收数据缓冲
    //startIndex:接收数据缓冲区的开始索引
    //bufferSize:接收数据缓冲区的大小
    //返回值：收到的数据个数
    virtual qint16 getData(quint8 *buffer, qint16 startIndex, qint16 bufferSize)=0;

    //接收数据
    //buffer:接收数据缓冲
    //startIndex:接收数据缓冲区的开始索引
    //bufferSize:接收数据缓冲区的大小
    //socket:socket对象
    //返回值：收到的数据个数
    virtual qint16 getData(quint8 *buffer, qint16 startIndex, qint16 bufferSize,const QObject &socket)=0;

    //设置事件通知接口
    virtual qint16 setEventCallback( IEventCallback *onEvent)=0;

};

*/

typedef int (*subCallbackFunc)(QString entityPath, QVariant value, int status, void* contex);

class ICommunicationObject: public QObject
{
public:

    virtual CommunicationStatus getCommStatus()=0;
    virtual void setCommStatus(CommunicationStatus commStatus)=0;

    //!通信参数（如串口：COM口，Baud等）
    virtual QString getCommParameter()=0;
    virtual void setCommParameter(QString commParameter)=0;


    //!启动通信连接
    virtual bool connect(qint32 msecs)=0;
    //!断开通信连接
    virtual bool disConnect(qint32 msecs)=0;
    //!发送数据
    virtual bool writeData(quint8 *buffer, qint16 count,qint32 msecs)=0;
    /*!
    \section !接收数据
    \buffer:接收数据缓冲
    \startIndex:接收数据缓冲区的开始索引
    \bufferSize:接收数据缓冲区的大小
    \返回值：0：接收失败，大于0为收到的数据个数，-1为数据粘包
    */
    virtual qint64 readData(quint8 *buffer, qint16 startIndex, qint16 bufferSize,qint32 msecs)=0;

    virtual bool subscribes(const std::vector<std::string> &entityBrowseNames)=0;

    virtual bool registSubCallback(subCallbackFunc callback, void* contex)=0;

    virtual void asyncWriteVariable(QString address, MDDataType type, QVariant value)=0;

    virtual void asyncWriteVariables(QList<QString> addresses, QList<MDDataType> types, QList<QVariant> values){};
};

class  ICommunicationComponent:public ICommunicationObject
{
public:
    virtual ~ICommunicationComponent(){}
public:
    //!通信类型
    virtual CommunicationType getCommType()=0;
    virtual void setCommType(CommunicationType commType)=0;
    /*!
    \section 主从通道切换
    \通信不正常时:
        \1.驱动模板多次重试后仍无法恢复通信时触发Switch
        \2.通信组件主从切换时新通道也无法通信，自动触发Switch一次
    */
    virtual void switchChannel()=0;
    //!获取当前通信参数
    virtual QString getCurCommParam() =0;
};
#endif // COMMUNICATIONCOMPONENT_H
