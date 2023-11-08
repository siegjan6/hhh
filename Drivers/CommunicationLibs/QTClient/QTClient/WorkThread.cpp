#include "WorkThread.h"



Worker::Worker(qint16 id, QString sendmsg)
{
    ID=id;
    Msg.append(sendmsg);
    socket=new QTcpSocket(this);
}

Worker::~Worker()
{

}

int Worker::ConnectServer(QString ip, ushort port)
{
    try
    {
        if(socket!=NULL)
        {
          socket->connectToHost(ip,port);
        }
    }
    catch(...)
    {
       return 0;
    }
    return 1;
}

int Worker::SendMsg(QByteArray msg)
{
    socket->write(msg);
    socket->flush();
    return 1;
}
