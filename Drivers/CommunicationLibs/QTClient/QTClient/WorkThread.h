#ifndef WORKTHREAD_H
#define WORKTHREAD_H
#include <QThread>
#include <QString>
#include <QTcpSocket>

class Worker:public QThread
{
public:
    Worker(qint16 id,QString sendmsg);
    ~Worker();
    int ConnectServer(QString ip,ushort port);
    int SendMsg(QByteArray msg);
 private:
    int ID;
    QString Msg;
    QTcpSocket *socket;
};
#endif // WORKTHREAD_H
